/*++

File Name:

qrcodeschema.h

Abstract:

PageQRCode PrintSchema definition. This defines the features, options
and enumerations that describe the PrintSchema PageQRCode feature within
a XDPrintSchema::PageQRCode namespace.

--*/
#pragma once

#include "schema.h"

namespace XDPrintSchema
{
	//
	// PageQRCode elements described as Printschema keywords
	//
	namespace PageQRCode
	{
		//
		// Feature name
		//
		extern LPCWSTR QRCODE_FEATURE;

		//
		// Option names
		//
		enum EQRCodeOption
		{
			DisableQRCode = 0, EQRCodeOptionMin = 0,
			EnableQRCode,
			EQRCodeOptionMax
		};

		extern LPCWSTR QRCODE_OPTIONS[EQRCodeOptionMax];
		//
		// Common qr code properties
		//
		enum ECommonQRCodeProps
		{
			QRCode_Scale = 0, ECommonQRCodePropsMin = 0,
			QRCode_Offset_X,
			QRCode_Offset_Y,
			ECommonQRCodePropsMax
		};

		extern LPCWSTR CMN_QRCODE_PROPS[ECommonQRCodePropsMax];
	}
}