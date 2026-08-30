#include <Uefi.h>

EFI_STATUS efi_main(EFI_HANDLE IH,EFI_SYSTEM_TABLE *ST) { /*ImageHandle,System Table*/
  ST->ConOut->OutputString(
    ST->ConOut,
    L"Hello, Hama OS 2026 [VERSION BETA 0.01]"
  )
  return EFI_SUCCESS;
}
