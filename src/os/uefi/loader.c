#include "include/loader.h"

EFI_STATUS load_kernel(EFI_HANDLE IH,EFI_SYSTEM_TABLE *ST) {
  EFI_LOADED_IMAGE_PROTOCOL *LoadedImage;
  ST->BootServices->OpenProtocol(IH,&gEfiLoadedImageProtocolGuid,&LoadedImage,IH,NULL,EFI_OPEN_PROTOCOL_GET_PROTOCOL);
}
