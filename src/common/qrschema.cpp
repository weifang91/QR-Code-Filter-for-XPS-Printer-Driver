/*++

File Name:

qrcodeschema.cpp

Abstract:

PageQRCode PrintSchema implementation. This implements the features,
options and enumerations that describe the PrintSchema PageQRCode feature.

--*/

#include "precomp.h"
#include "qrschema.h"

LPCWSTR XDPrintSchema::PageQRCode::QRCODE_FEATURE = L"PageQRCode";

LPCWSTR XDPrintSchema::PageQRCode::QRCODE_OPTIONS[] = {
	L"Disable",
	L"Enable"
};

// Must be same as string stated in uiproperties.cpp, eg. PageQRCodeScale
LPCWSTR XDPrintSchema::PageQRCode::CMN_QRCODE_PROPS[] = {
	L"Scale",
	L"OffsetX",
	L"OffsetY"
};