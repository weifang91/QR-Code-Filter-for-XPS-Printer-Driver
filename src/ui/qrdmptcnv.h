/*++

File Name:

qrdmptcnv.h

Abstract:

PageQRCode devmode <-> PrintTicket conversion class definition.
The class defines a common data representation between the DevMode (GPD) and PrintTicket
representations and implements the conversion and validation methods required
by CFeatureDMPTConvert.

--*/

#pragma once

#include "ftrdmptcnv.h"
#include "qrpthndlr.h"
#include "uiproperties.h"

class CQRCodeDMPTConv : public CFeatureDMPTConvert<XDPrintSchema::PageQRCode::QRCodeData>
{
public:
	CQRCodeDMPTConv();

	~CQRCodeDMPTConv();

private:
	HRESULT
		GetPTDataSettingsFromDM(
			_In_  PDEVMODE pDevmode,
			_In_  ULONG    cbDevmode,
			_In_  PVOID    pPrivateDevmode,
			_In_  ULONG    cbDrvPrivateSize,
			_Out_ XDPrintSchema::PageQRCode::QRCodeData* pDataSettings
		);

	HRESULT
		MergePTDataSettingsWithPT(
			_In_    IXMLDOMDocument2* pPrintTicket,
			_Inout_ XDPrintSchema::PageQRCode::QRCodeData*    pDrvSettings
		);

	HRESULT
		SetPTDataInDM(
			_In_    CONST   XDPrintSchema::PageQRCode::QRCodeData& drvSettings,
			_Inout_ PDEVMODE pDevmode,
			_In_    ULONG    cbDevmode,
			_Inout_ PVOID    pPrivateDevmode,
			_In_    ULONG    cbDrvPrivateSize
		);

	HRESULT
		SetPTDataInPT(
			_In_    CONST XDPrintSchema::PageQRCode::QRCodeData&    drvSettings,
			_Inout_ IXMLDOMDocument2* pPrintTicket
		);

	HRESULT STDMETHODCALLTYPE
		CompletePrintCapabilities(
			_In_opt_ IXMLDOMDocument2*,
			_Inout_  IXMLDOMDocument2* pPrintCapabilities
		);
};


