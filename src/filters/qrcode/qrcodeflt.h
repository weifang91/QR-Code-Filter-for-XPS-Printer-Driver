/*++

File Name:

   qrcodeflt.h

Modified from:

   wmflt.h

Abstract:

   QR Code filter defnition. This class derives from the Xps filter
   class and implements the necessary part handlers to support Qr Code
   printing. The QR Code filter is responsible for adding resources to
   the XPS document and putting the appropriate mark-up onto pages with a
   QR code

--*/

#pragma once

#include "xdrchflt.h"
#include "gdip.h"
#include "rescpy.h"
#include "QrTitleFont.h"
#include "qrptprop.h"

using XDPrintSchema::PageQRCode::EQRCodeOption;

class CQrCodeFilter : public CXDXpsFilter
{
public:
	CQrCodeFilter();

    virtual ~CQrCodeFilter();

private:
    virtual HRESULT
    ProcessPart(
        _Inout_ IFixedPage* pFP
        );

	virtual HRESULT
	Finalize(
			VOID
		);

	HRESULT
	CreateNewQRPage(
		_Outptr_ IFixedPage** pNewPage
		);

	HRESULT
		GetQRCodeProp(
		_In_            IXMLDOMDocument2*  pPrintTicket,
		_Outptr_ CQRCodePTProperties** ppQRCodeProp
		);

private:
    GDIPlus						m_gdiPlus;

	CResourceCopier*			m_pResCopier;

	vector<CComPtr<IFixedPage> >   m_cacheFP;

	vector<CComBSTR>			m_cacheAllPrintData;

	CComBSTR					m_startFixedPageTag;

	CComBSTR					m_DataPerFixedPage;

	CComPtr<IFixedPage>			m_pNewFP;				//New fixed page

	CQrTitleFont				m_QrTitleFont;

	CQRCodePTProperties*		m_pQRCodeProp;

	EQRCodeOption				m_QRCodeOption;
};



