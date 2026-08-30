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
}
