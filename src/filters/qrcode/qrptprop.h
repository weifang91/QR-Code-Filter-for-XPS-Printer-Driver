/*++

File Name:

qrptprop.h

Abstract:

QR Code properties class definition. The QR Code properties class
is responsible for holding and controling QR Code properties.

--*/

#pragma once

#include "qrdata.h"

class CQRCodePTProperties
{
public:
	CQRCodePTProperties(
		_In_ CONST XDPrintSchema::PageQRCode::QRCodeData& qrcodeData
		);

	virtual ~CQRCodePTProperties();

	HRESULT
	GetQROption(
		_Out_ XDPrintSchema::PageQRCode::EQRCodeOption* pQROption
		);

	HRESULT
	GetQRScale(
		_Out_ INT* pQRScale
		);

	HRESULT
	GetQRScale(
		_Outptr_ BSTR* pbstrQRScale
		);

	HRESULT
	GetQROffsetX(
		_Out_ INT* pQROffsetX
		);

	HRESULT
	GetQROffsetX(
		_Outptr_ BSTR* pbstrQROffsetX
		);

	HRESULT
		GetQROffsetY(
			_Out_ INT* pQROffsetY
		);

	HRESULT
		GetQROffsetY(
			_Outptr_ BSTR* pbstrQROffsetY
		);

protected:
	XDPrintSchema::PageQRCode::QRCodeData m_qrcodeData;
};


