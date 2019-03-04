/*++
File Name:

qrsax.h

Abstract:

QR code sax handler definition. The QR code SAX handler is responsible for
parsing the FixedPage mark-up for the page size and inserting the QR code mark-up
in appropriate place.

--*/

#pragma once

#include "saxhndlr.h"
#include "qrptprop.h"

class CQrCodeSaxHandler : public CSaxHandler
{
public:
	
	CQrCodeSaxHandler(
		_In_ IPrintWriteStream* pWriter,
		_In_ CQRCodePTProperties* pQRCodeProp,
		_In_ CComBSTR			startFixedPageTag,
		_In_ vector<CComBSTR>	cacheAllPrintData
	);

	virtual ~CQrCodeSaxHandler();

	virtual HRESULT STDMETHODCALLTYPE
	startDocument(
		void
	);

	virtual HRESULT STDMETHODCALLTYPE
	endDocument(
		void
	);

private:
	CComPtr<IPrintWriteStream>	m_pWriter;

	CComBSTR					m_bstrOpenElement;

	BOOL						m_bOpenTag;

	CQRCodePTProperties*		m_pQRCodeProp;

	CComBSTR					m_startFixedPageTag;

	vector<CComBSTR>			m_cacheAllPrintData;

	CComBSTR					m_combinePrintData;
};

