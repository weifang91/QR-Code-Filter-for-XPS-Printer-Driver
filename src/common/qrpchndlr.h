/*++

File Name:

qrcodepchndlr.h

Abstract:

Page QR Code PrintCapabilities handling implementation. The QR Code PC handler
is used to set Page QRCode settings in a PrintCapabilities.

--*/
#pragma once

#include "pchndlr.h"
#include "qrdata.h"

class CQRCodePCHandler : public CPCHandler
{
public:
	CQRCodePCHandler(
		_In_ IXMLDOMDocument2* pPrintCapabilities
	);

	virtual ~CQRCodePCHandler();

	HRESULT
		SetCapabilities(
			VOID
		);
};
