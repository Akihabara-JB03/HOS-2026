#include "include/loader.h"

EFI_STATUS load_kernel(EFI_HANDLE IH,EFI_SYSTEM_TABLE *ST) {
  EFI_LOADED_IMAGE_PROTOCOL *LoadedImage;
    EFI_STATUS status = ST->BootServices->OpenProtocol(IH,
    &gEfiLoadedImageProtocolGuid,
    &LoadedImage,
    IH,
    NULL,
    EFI_OPEN_PROTOCOL_GET_PROTOCOL
  );
  if (EFI_ERROR(status)) {
    return status;
  }
  EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *SFSP;
  status = ST->BootServices->OpenProtocol(LoadedImage->DeviceHandle,
    &gEfiSimpleFileSystemProtocolGuid,
    &SFSP,
    IH,
    NULL,
    EFI_OPEN_PROTOCOL_GET_PROTOCOL
  );
  if (EFI_ERROR(status)) {
    return status;
  }
  EFI_FILE_PROTOCOL *Root;
  status = SFSP->OpenVolume(SFSP,&Root);
  if (EFI_ERROR(status)) {
    return status;
  }
  EFI_FILE_PROTOCOL *KernelFile;
  status = Root->Open(&KernelFile,
    L"kernel.elf",
    EFI_FILE_MODE_READ,
    0
  );
  if (EFI_ERROR(status)) {
    return status;
  }
  UINTN InfoSize = 0;
  EFI_FILE_INFO *FileInfo = NULL;
  status = KernelFile->GetInfo(
    KernelFile,
    &gEfiFileInfoGuid,
    &InfoSize,
    NULL
  );
  if (status != EFI_BUFFER_TOO_SMALL) {
    return status;
  }
  status = ST->BootServices->AllocatePool(
    EfiLoaderData,
    InfoSize,
    (VOID **)&FileInfo
  );
  if (EFI_ERROR(status)) {
    return status;
  }
  status = KernelFile->GetInfo(
    KernelFile,
    &gEfiFileInfoGuid,
    &InfoSize,
    FileInfo
  );

  if (EFI_ERROR(status)) {
    return status;
  }
  UINT64 KernelSize=FileInfo->FileSize;
  status = ST->BootServices->FreePool(FileInfo);

  if (EFI_ERROR(status)) {
    return status;
  }
  VOID *KernelBuffer = NULL;
  status = ST->BootServices->AllocatePool(
    EfiLoaderData,
    KernelSize,
    &KernelBuffer
  );
  if (EFI_ERROR(status)) {
    return status;
  }
  return EFI_SUCCESS;
}
