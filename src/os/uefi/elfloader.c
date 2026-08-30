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
  Elf64_Phdr *Phdr =
    (Elf64_Phdr *)((UINT8 *)KernelBuffer + Header->e_phoff);
  return EFI_SUCCESS;
}
