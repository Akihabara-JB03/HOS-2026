#ifndef LOADER_H
#define LOADER_H

#include <efi.h>
#include <efilib.h>

EFI_STATUS load_kernel(
    EFI_HANDLE IH,
    EFI_SYSTEM_TABLE *ST
);

#endif
