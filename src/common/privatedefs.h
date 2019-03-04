/*++

Copyright (c) 2005 Microsoft Corporation

All rights reserved.

THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
PARTICULAR PURPOSE.

File Name:

   privatedefs.h

Abstract:

   Definitions for private PrintTicket/PrintCapabilities properties for watermark and page scaling features.

--*/

#pragma once

//
// Macro to return the number of elements in an array
//
#define numof(n) (sizeof(n) / sizeof(n[0]))

//
// Structure definition for the Private Parameter Defs (Integer Type)
//
typedef struct _PRIVATE_DEF_INTS
{
    PCSTR property_name;
    BOOL  is_public;
    PCSTR display_name;
    INT   default_value;
    INT   min_length;
    INT   max_length;
    INT   multiple;
    PCSTR unit_type;
}PRIVATE_DEF_INTEGERS;

//
// Structure definition for the Private Parameter Defs (String Type)
//
typedef struct _PRIVATE_DEF_STRS
{
    PCSTR property_name;
    PCSTR display_name;
    PCSTR default_value;
    INT   min_length;
    INT   max_length;
    PCSTR unit_type;
}PRIVATE_DEF_STRINGS;

//
// Structure definition for the Private Parameter Defs (Decimal Type)
//
typedef struct _PRIVATE_DEF_DECS
{
    PCSTR property_name;
    PCSTR display_name;
    REAL  default_value;
    REAL  min_length;
    REAL  max_length;
    REAL  multiple;
    PCSTR unit_type;
}PRIVATE_DEF_DECIMALS;


//--------------------------------------------------------------------------------------------------
// Table of values for private integer QR code properties.
//--------------------------------------------------------------------------------------------------
CONST PRIVATE_DEF_INTEGERS qrcodeParamDefIntegers[] =
{
	{
		"PageQRCodeScale",
		TRUE,
		NULL,
		4,
		1,
		10,
		1,
		"Integer"
	},
	{
		"PageQRCodeOffsetX",
		TRUE,
		NULL,
		80,
		0,
		500,
		1,
		"Integer"
	},
	{
		"PageQRCodeOffsetY",
		TRUE,
		NULL,
		150,
		150,
		700,
		1,
		"Integer"
	}
};

//
// enum lookup into above table
//
enum eQRCodeParamDefIntegers
{
	ePageQRCodeScale = 0,
	ePageQRCodeOffsetX,
	ePageQRCodeOffsetY,
};
//--------------------------------------------------------------------------------------------------
// Table of values for private string watermark properties.
//--------------------------------------------------------------------------------------------------
CONST PRIVATE_DEF_STRINGS qrcodeParamDefStrings[] =
{
	{
		"PageQRCodeMerchantID",
		NULL,
		"Teaspoon",
		0,
		24,
		"characters"
	},
	{
		"PageQRCodeSecurityKey",
		NULL,
		"123456789",
		//"xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx",
		0,
		36,
		"characters"
	},
	{
		"PageQRCodeRedirectPrinterModel",
		NULL,
		"Enter Printer Model",
		0,
		24,
		"characters"
	}
};

//
// enum lookup into above table
//
enum eQRCodeParamDefStrings
{
	ePageQRCodeMerchantID = 0,
	ePageQRCodeSecurityKey,
	ePageQRCodeRedirectPrinterModel,
};
//--------------------------------------------------------------------------------------------------
// End QR code properties
//--------------------------------------------------------------------------------------------------

//
// Table of values for private integer watermark properties.
//
CONST PRIVATE_DEF_INTEGERS wmParamDefIntegers[] =
{
    {
        "PageWatermarkTextAngle",
        TRUE,
        NULL,
        0,
        0,
        360,
        1,
        "Degrees"
    },
    {
        "PageWatermarkOriginWidth",
        TRUE,
        NULL,
        0,
        -INT_MAX,
        INT_MAX,
        1,
        "Microns"
    },
    {
        "PageWatermarkOriginHeight",
        TRUE,
        NULL,
        HUNDREDTH_OFINCH_TO_MICRON(100),
        -INT_MAX,
        INT_MAX,
        1,
        "Microns"
    },
    {
        "PageWatermarkSizeWidth",
        FALSE,
        NULL,
        HUNDREDTH_OFINCH_TO_MICRON(100),
        0,
        INT_MAX,
        1,
        "Microns"
    },
    {
        "PageWatermarkSizeHeight",
        FALSE,
        NULL,
        HUNDREDTH_OFINCH_TO_MICRON(100),
        0,
        INT_MAX,
        1,
        "Microns"
    },
    {
        "PageWatermarkTextFontSize",
        TRUE,
        NULL,
        72,
        1,
        INT_MAX,
        1,
        "Points Per Inch"
    },
    {
        "PageWatermarkTextColor",
        TRUE,
        NULL,
        (INT) 0xFFFF0000,
        0,
        (INT) 0xFFFFFFFF,
        1,
        "sRGB"
    },
    {
        "PageWatermarkTransparency",
        TRUE,
        NULL,
        50,
        0,
        100,
        1,
        "Transparency"
    },
};

//
// enum lookup into above table
//
enum eWMParamDefIntegers
{
    ePageWatermarkAngle = 0,
    ePageWatermarkOriginWidth,
    ePageWatermarkOriginHeight,
    ePageWatermarkSizeWidth,
    ePageWatermarkSizeHeight,
    ePageWatermarkTextFontSize,
    ePageWatermarkTextColor,
    ePageWatermarkTransparency,
};

//
// Table of values for private string watermark properties.
//
CONST PRIVATE_DEF_STRINGS wmParamDefStrings[] =
{
    {
        "PageWatermarkTextText",
        NULL,
        "CONFIDENTIAL",
        0,
        20,
        "characters"
    }
};

//
// enum lookup into above table
//
enum eWMParamDefStrings
{
    ePageWatermarkTextText = 0
};

//
// Table of values for private integer page scaling properties.
//
CONST PRIVATE_DEF_INTEGERS pgscParamDefIntegers[] =
{
    {
        "PageScalingScaleWidth",
        TRUE,
        NULL,
        100,
        1,
        1000,
        1,
        "Percent"
    },
    {
        "PageScalingScaleHeight",
        TRUE,
        NULL,
        100,
        1,
        1000,
        1,
        "Percent"
    },
    {
        "PageScalingScale",
        TRUE,
        NULL,
        100,
        1,
        1000,
        1,
        "Percent"
    },
    {
        "PageScalingOffsetWidth",
        TRUE,
        NULL,
        0,
        -INT_MAX,
        INT_MAX,
        1,
        "Microns"
    },
    {
        "PageScalingOffsetHeight",
        TRUE,
        NULL,
        0,
        -INT_MAX,
        INT_MAX,
        1,
        "Microns"
    },
};

//
// enum lookup into above table
//
enum ePGSCParamDefIntegers
{
    ePageScalingScaleWidth = 0,
    ePageScalingScaleHeight,
    ePageScalingScale,
    ePageScalingOffsetWidth,
    ePageScalingOffsetHeight,
};

//
// Table of values for private integer booklet gutter properties.
//
CONST PRIVATE_DEF_INTEGERS bkParamDefIntegers[] =
{
    {
        "JobBindAllDocumentsGutter",
        TRUE,
        NULL,
        0,
        -INT_MAX,
        INT_MAX,
        0,
        "Microns"
    },
    {
        "DocumentBindingGutter",
        TRUE,
        NULL,
        0,
        -INT_MAX,
        INT_MAX,
        0,
        "Microns"
    }
};

//
// enum lookup into above table
//
enum eBindingParamDefIntegers
{
    eJobBindAllDocumentsGutter = 0,
    eDocumentBindingGutter,
};

