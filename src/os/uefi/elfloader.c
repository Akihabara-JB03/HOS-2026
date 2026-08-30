#include "include/elf.h"

EFI_STATUS load_elf(
    EFI_SYSTEM_TABLE *ST,
    VOID *KernelBuffer,
    UINTN KernelSize
) {
  if (KernelSize < sizeof(Elf64_Ehdr)) {
    return EFI_LOAD_ERROR;
  }
  Elf64_Ehdr *Header = (Elf64_Ehdr *)KernelBuffer;
  if (Header->e_ident[0] != 0x7F ||
    Header->e_ident[1] != 'E'  ||
    Header->e_ident[2] != 'L'  ||
    Header->e_ident[3] != 'F') {
    return EFI_LOAD_ERROR;
  }
  if (Header->e_ident[4] != ELFCLASS64) {
    return EFI_LOAD_ERROR;
  }
  if (Header->e_ident[5] != ELFDATA2LSB) {
    return EFI_LOAD_ERROR;
  }
  UINT64 Entry = Header->e_entry;
  if (Header->e_phnum == 0) {
    return EFI_LOAD_ERROR;
  }
  UINT64 PhdrSize =
    (UINT64)Header->e_phentsize * Header->e_phnum;
  if (Header->e_phentsize != sizeof(Elf64_Phdr)) {
    return EFI_LOAD_ERROR;
  }
  if (Header->e_phoff > KernelSize ||
    PhdrSize > KernelSize - Header->e_phoff) {
    return EFI_LOAD_ERROR;
  }
  Elf64_Phdr *Phdr =
    (Elf64_Phdr *)((UINT8 *)KernelBuffer + Header->e_phoff);
  for (UINT16 i = 0; i < Header->e_phnum; i++) {
    Elf64_Phdr *Current = &Phdr[i];

    if (Current->p_type == PT_LOAD) {
      if (Current->p_offset > KernelSize ||
        Current->p_filesz > KernelSize - Current->p_offset) {
        return EFI_LOAD_ERROR;
      }
      VOID *SegmentSource =
        (UINT8 *)KernelBuffer + Current->p_offset;
      if (Current->p_filesz > Current->p_memsz) {
        return EFI_LOAD_ERROR;
      }
      if (Current->p_memsz > MAX_UINT64 - Current->p_vaddr) {
        return EFI_LOAD_ERROR;
      }
      if (Current->p_memsz > MAX_UINTN - (EFI_PAGE_SIZE - 1)) {
        return EFI_LOAD_ERROR;
      }
      UINTN Pages = (Current->p_memsz + EFI_PAGE_SIZE - 1)
              / EFI_PAGE_SIZE;
      EFI_PHYSICAL_ADDRESS MemoryAddress = 0;
      EFI_STATUS status = ST->BootServices->AllocatePages(
        AllocateAnyPages,
        EfiLoaderData,
        Pages,
        &MemoryAddress
      );
      if (EFI_ERROR(status)) {
        return status;
      }
    }
  }
  return EFI_SUCCESS;
}
