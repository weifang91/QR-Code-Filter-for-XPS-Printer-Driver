/*++

File Name:

qrcodedata.h

Abstract:

PageQRCode data structure definition. This provides a convenient
description of the PrintSchema PageQRCode feature.

--*/
#pragma once

#include "qrschema.h"

namespace XDPrintSchema
{
	namespace PageQRCode
	{

		struct QRCodeData
		{
			QRCodeData() :
				scale(400),
				offset_x(80),
				offset_y(150),
				QRCodeOption(DisableQRCode)
			{
			}

			INT						  scale;
			INT                       offset_x;
			INT                       offset_y;
			EQRCodeOption		  QRCodeOption;

		};
	}
}