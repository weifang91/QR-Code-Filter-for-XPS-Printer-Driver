/*++

File Name:

   datasax.h

Modified from:

   wmsax.h

Abstract:

   Data sax handler definition. The Data SAX handler is responsible for parsing the FixedPage 
   mark-up for the page size and collecting the print data (used to generate QR code)

--*/

#pragma once

#include "saxhndlr.h"

class CDataSaxHandler : public CSaxHandler
{
public:
	CDataSaxHandler(
        _In_ IPrintWriteStream* pWriter
        );

    virtual ~CDataSaxHandler();

    virtual HRESULT STDMETHODCALLTYPE
    startElement(
        CONST wchar_t*,
        INT,
        CONST wchar_t*,
        INT,
        _In_reads_(cchQName) CONST wchar_t*  pwchQName,
        _In_                  INT             cchQName,
        _In_                  ISAXAttributes* pAttributes
        );

    virtual HRESULT STDMETHODCALLTYPE
    endElement(
        CONST wchar_t*,
        INT,
        CONST wchar_t*,
        INT,
        _In_reads_(cchQName) CONST wchar_t* pwchQName,
        _In_                  INT            cchQName
        );

    HRESULT STDMETHODCALLTYPE
    startDocument(
        void
        );

	HRESULT STDMETHODCALLTYPE
	getFixedPageData(
		_Outptr_ BSTR*			pstartFixedPageTag,
		_Outptr_ BSTR*			pDataPerFixedPage
		);

private:
    CComPtr<IPrintWriteStream> m_pWriter;

    CComBSTR                   m_bstrOpenElement;

    BOOL                       m_bOpenTag;

	CComBSTR					m_bstrFixedPageElem;

	CComBSTR					m_bstrPrintData;
};

