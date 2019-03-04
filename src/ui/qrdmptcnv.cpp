/*++

File Name:

qrdmptcnv.cpp

Abstract:

PageQRCode devmode <-> PrintTicket conversion class implementation.
The class defines a common data representation between the DevMode (GPD) and PrintTicket
representations and implements the conversion and validation methods required
by CFeatureDMPTConvert.

--*/

#include "precomp.h"
#include "debug.h"
#include "globals.h"
#include "xdstring.h"
#include "qrdmptcnv.h"
#include "qrpchndlr.h"

using XDPrintSchema::PageQRCode::QRCodeData;
using XDPrintSchema::PageQRCode::EQRCodeOption;
using XDPrintSchema::PageQRCode::DisableQRCode;
using XDPrintSchema::PageQRCode::EnableQRCode;

using XDPrintSchema::PageQRCode::ECommonQRCodeProps;
using XDPrintSchema::PageQRCode::QRCode_Scale;
using XDPrintSchema::PageQRCode::QRCode_Offset_X;
using XDPrintSchema::PageQRCode::QRCode_Offset_Y;

//
// Look-up data converting GPD PageWatermarkType feature options to
// watermark type enumeration
//
PCSTR g_pszQRCodeTypeFeature = "PageQRCodeOption";
static GPDStringToOption<EQRCodeOption> g_QRCodeTypeOption[] = {
	{ "Disable",   DisableQRCode },
	{ "Enable",   EnableQRCode }
};
UINT g_cQRCodeTypeOption = sizeof(g_QRCodeTypeOption) / sizeof(GPDStringToOption<EQRCodeOption>);


/*++

Routine Name:

CQRCodeDMPTConv::CQRCodeDMPTConv

Routine Description:

CQRCodeDMPTConv class constructor

Arguments:

None

Return Value:

None

--*/
CQRCodeDMPTConv::CQRCodeDMPTConv()
{
}

/*++

Routine Name:

CQRCodeDMPTConv::~CQRCodeDMPTConv

Routine Description:

CQRCodeDMPTConv class destructor

Arguments:

None

Return Value:

None

--*/
CQRCodeDMPTConv::~CQRCodeDMPTConv()
{
}

/*++

Routine Name:

CQRCodeDMPTConv::GetPTDataSettingsFromDM

Routine Description:

Populates the QR Code data structure from the Devmode passed in.

Arguments:

pDevmode - pointer to input devmode buffer.
cbDevmode - size in bytes of full input devmode.
pPrivateDevmode - pointer to input private devmode buffer.
cbDrvPrivateSize - size in bytes of private devmode.
pDataSettings - Pointer to QR Code data structure to be updated.

Return Value:

HRESULT
S_OK - On success
E_*  - On error

--*/
HRESULT
CQRCodeDMPTConv::GetPTDataSettingsFromDM(
	_In_  PDEVMODE       pDevmode,
	_In_  ULONG          cbDevmode,
	_In_  PVOID          pPrivateDevmode,
	_In_  ULONG          cbDrvPrivateSize,
	_Out_ QRCodeData* pDataSettings
)
{
	HRESULT hr = S_OK;

	if (SUCCEEDED(hr = CHECK_POINTER(pDevmode, E_POINTER)) &&
		SUCCEEDED(hr = CHECK_POINTER(pPrivateDevmode, E_POINTER)) &&
		SUCCEEDED(hr = CHECK_POINTER(pDataSettings, E_POINTER)))
	{
		if (cbDevmode < sizeof(DEVMODE) ||
			cbDrvPrivateSize == 0)
		{
			hr = E_INVALIDARG;
		}
	}

	//
	// Retrieve the GPD and devmode controlled settings
	//
	CUIProperties uiProperties(static_cast<POEMDEV>(pPrivateDevmode));

	if (SUCCEEDED(hr) &&
		SUCCEEDED(hr = GetOptionFromGPDString<EQRCodeOption>(pDevmode,
															 cbDevmode,
															 g_pszQRCodeTypeFeature,
															 g_QRCodeTypeOption,
															 g_cQRCodeTypeOption,
															 pDataSettings->QRCodeOption)) &&
		SUCCEEDED(hr = uiProperties.GetItem(g_pszQRScale, &pDataSettings->scale, sizeof(pDataSettings->scale))) &&
		SUCCEEDED(hr = uiProperties.GetItem(g_pszQROffsetX, &pDataSettings->offset_x, sizeof(pDataSettings->offset_x))) &&
		SUCCEEDED(hr = uiProperties.GetItem(g_pszQROffsetY, &pDataSettings->offset_y, sizeof(pDataSettings->offset_y))))
	{
		
		//
		// Convert measurements from 100ths of an inch to microns
		//
		//pDataSettings->offset_x = HUNDREDTH_OFINCH_TO_MICRON(pDataSettings->offset_x);
		//pDataSettings->offset_y = HUNDREDTH_OFINCH_TO_MICRON(pDataSettings->offset_y);
	}

	ERR_ON_HR(hr);
	return hr;
}

/*++

Routine Name:

CQRCodeDMPTConv::MergePTDataSettingsWithPT

Routine Description:

This method updates the QR Code data structure from a PrintTicket description.

Arguments:

pPrintTicket  - Pointer to the input PrintTicket.
pDataSettings - Pointer to the watermark data structure

Return Value:

HRESULT
S_OK - On success
E_*  - On error

--*/
HRESULT
CQRCodeDMPTConv::MergePTDataSettingsWithPT(
	_In_    IXMLDOMDocument2* pPrintTicket,
	_Inout_ QRCodeData*    pDataSettings
)
{
	HRESULT hr = S_OK;
	
	if (SUCCEEDED(hr) &&
		SUCCEEDED(hr = CHECK_POINTER(pPrintTicket, E_POINTER)) &&
		SUCCEEDED(hr = CHECK_POINTER(pDataSettings, E_POINTER)))
	{
		try
		{
			//
			// Get the QR Code settings from the PrintTicket and set the options in
			// the input QR Code data structure
			//
			QRCodeData qrcodeData;
			CQRCodePTHandler  qrcodePTHndlr(pPrintTicket);

			if (SUCCEEDED(hr = qrcodePTHndlr.GetData(&qrcodeData)))
			{
				//
				// Only update settings relevant to the feature so that we do not unset
				// other QR Code settings in the devmode
				//
				pDataSettings->QRCodeOption = DisableQRCode;

				switch (qrcodeData.QRCodeOption)
				{
				case EnableQRCode:
				{
					pDataSettings->QRCodeOption = EnableQRCode;
					pDataSettings->scale = qrcodeData.scale;
					pDataSettings->offset_x = qrcodeData.offset_x;
					pDataSettings->offset_y = qrcodeData.offset_y;
				}
				break;

				case DisableQRCode:
					break;

				default:
				{
					WARNING("Unrecognized QR Code feature - setting to default\n");
				}
				break;
				}
			}
			else if (hr == E_ELEMENT_NOT_FOUND)
			{
				//
				// QR Code setting not in the PT - this is not an error. Just
				// leave the type as DisableQRCode and reset the HRESULT to S_OK
				//
				hr = S_OK;
			}
		}
		catch (CXDException& e)
		{
			hr = e;
		}
	}

	ERR_ON_HR(hr);
	return hr;
}

/*++

Routine Name:

CQRCodeDMPTConv::SetPTDataInDM

Routine Description:

This method updates the QR code options in the devmode from the UI Settings.

Arguments:

dataSettings - Reference to QR Code data settings to be updated.
pDevmode - pointer to devmode to be updated.
cbDevmode - size in bytes of full devmode.
pPrivateDevmode - pointer to input private devmode buffer.
cbDrvPrivateSize - size in bytes of private devmode.

Return Value:

HRESULT
S_OK - On success
E_*  - On error

--*/
HRESULT
CQRCodeDMPTConv::SetPTDataInDM(
	_In_    CONST	QRCodeData&	 dataSettings,
	_Inout_	PDEVMODE             pDevmode,
	_In_    ULONG                cbDevmode,
	_Inout_ PVOID                pPrivateDevmode,
	_In_    ULONG                cbDrvPrivateSize
)
{
	HRESULT hr = S_OK;

	if (SUCCEEDED(hr = CHECK_POINTER(pDevmode, E_POINTER)) &&
		SUCCEEDED(hr = CHECK_POINTER(pPrivateDevmode, E_POINTER)))
	{
		if (cbDevmode < sizeof(DEVMODE) ||
			cbDrvPrivateSize == 0)
		{
			hr = E_INVALIDARG;
		}
	}

	//
	// Convert from microns to 100ths of an inch before writing to the DevMode
	//
	if (SUCCEEDED(hr))
	{
		// offset_x = MICRON_TO_HUNDREDTH_OFINCH(dataSettings.offset_x);
		//INT offset_y = MICRON_TO_HUNDREDTH_OFINCH(dataSettings.offset_y);

		//
		// Set the GPD and devmode controlled settings
		//
		CUIProperties uiProperties(static_cast<POEMDEV>(pPrivateDevmode));
		if (SUCCEEDED(hr = SetGPDStringFromOption<EQRCodeOption>(pDevmode,
																 cbDevmode,
																 g_pszQRCodeTypeFeature,
																 g_QRCodeTypeOption,
																 g_cQRCodeTypeOption,
																 dataSettings.QRCodeOption)) &&
			SUCCEEDED(hr = uiProperties.SetItem(g_pszQRScale, &dataSettings.scale, sizeof(dataSettings.scale))) &&
			SUCCEEDED(hr = uiProperties.SetItem(g_pszQROffsetX, &dataSettings.offset_x, sizeof(dataSettings.offset_x))))
		{
			hr = uiProperties.SetItem(g_pszQROffsetY, &dataSettings.offset_y, sizeof(dataSettings.offset_y));
		}
	}
	
	ERR_ON_HR(hr);
	return hr;
}

/*++

Routine Name:

CQRCodeDMPTConv::SetPTDataInPT

Routine Description:

This method updates the QR code PrintTicket description from QR code data structure.

Arguments:

drvSettings  - Reference to QR Code data structure to update from.
pPrintTicket - Pointer to the PrintTicket to be updated.

Return Value:

HRESULT
S_OK - On success
E_*  - On error

--*/
HRESULT
CQRCodeDMPTConv::SetPTDataInPT(
	_In_    CONST QRCodeData&	 dataSettings,
	_Inout_ IXMLDOMDocument2*    pPrintTicket
)
{
	HRESULT hr = S_OK;

	if (SUCCEEDED(hr = CHECK_POINTER(pPrintTicket, E_POINTER)))
	{
		try
		{
			CQRCodePTHandler  qrcodePTHndlr(pPrintTicket);
			hr = qrcodePTHndlr.SetData(&dataSettings);
		}
		catch (CXDException& e)
		{
			hr = e;
		}
	}
	else
	{
		hr = E_POINTER;
	}

	ERR_ON_HR(hr);
	return hr;
}

/*++

Routine Name:

CQRCodeDMPTConv::CompletePrintCapabilities

Routine Description:

Unidrv calls this routine with an input Device Capabilities Document
that is partially populated with Device capabilities information
filled in by Unidrv for features that it understands. The plug-in
needs to read any private features in the input PrintTicket, delete
them and add them back under Printschema namespace so that higher
level applications can understand them and make use of them.

Arguments:

pPrintTicket - pointer to input PrintTicket
pCapabilities - pointer to Device Capabilities Document.

Return Value:

HRESULT
S_OK - Always

--*/
HRESULT STDMETHODCALLTYPE
CQRCodeDMPTConv::CompletePrintCapabilities(
	_In_opt_ IXMLDOMDocument2*,
	_Inout_  IXMLDOMDocument2* pPrintCapabilities
)
{
	HRESULT hr = S_OK;

	if (SUCCEEDED(hr = CHECK_POINTER(pPrintCapabilities, E_POINTER)))
	{
		try
		{
			CQRCodePCHandler qrcodepcHandler(pPrintCapabilities);
			qrcodepcHandler.SetCapabilities();
		}
		catch (CXDException& e)
		{
			hr = e;
		}
	}

	ERR_ON_HR(hr);
	return hr;
}

