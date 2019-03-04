/*++

File Name:

qrpthnlr.h

Abstract:

PageQRCode PrintTicket handling definition. The QR Code PT handler
is used to extract QR code settings from a PrintTicket and populate
the QR code properties class with the retrieved settings. The class also
defines a method for setting the feature in the PrintTicket given the
data structure.

--*/

#pragma once

#include "pthndlr.h"
#include "qrdata.h"

class CQRCodePTHandler : public CPTHandler
{
public:
	CQRCodePTHandler(
		_In_ IXMLDOMDocument2* pPrintTicket
	);

	virtual ~CQRCodePTHandler();

	HRESULT
	GetData(
		_Out_ XDPrintSchema::PageQRCode::QRCodeData* pQRCodeData
	);

	HRESULT
	SetData(
		_In_ CONST XDPrintSchema::PageQRCode::QRCodeData* pQRCodeData
	);

	HRESULT
	Delete(
		VOID
	);

private:
	HRESULT
	CreateCommonQRCodeElements(
		_In_        CONST XDPrintSchema::PageQRCode::QRCodeData*	   pQRCodeData,
		_Outptr_	IXMLDOMElement**                                   ppQRCodeDataElem,
		_Out_       PTDOMElementVector*                                pParamInitList
	);

	HRESULT
	GetCmnPropTypeAndValue(
		_In_        CONST XDPrintSchema::PageQRCode::QRCodeData*		   pQRCodeData,
		_In_        CONST XDPrintSchema::PageQRCode::ECommonQRCodeProps	   cmnProps,
		_Outptr_ BSTR*                                                     pbstrType,
		_Outptr_ BSTR*                                                     pbstrValue
	);
};


