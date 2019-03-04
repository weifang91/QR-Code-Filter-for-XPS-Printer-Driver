/*++

File Name:

   qrcodegenflt.cpp

Modified from:

   wmflt.cpp

Abstract:

   QR code generator filter implementation. This class derives from the Xps filter
   class and implements the necessary part handlers to support QR code printing. 
   The QR code generator filter is responsible for 
   1. Creating a new page at the end of XPS document.
   2. Encode print data/text to QR code and display the QR code at new created page.
   3. Display the title of QR code which is set by user

   Two handlers are implemented in this filter:
   1. CDataSaxHandler	- Parse XPS fixedpage and store only the print data/text
   2. CQrCodeSaxHandler	- Encode print data/text to QR code and write this QR code with titel 
	  on the new created page
--*/

#include "precomp.h"
#include "debug.h"
#include "globals.h"
#include "xdstring.h"
#include "xdexcept.h"
#include "qrcodeflt.h"
#include "datasax.h"
#include "qrcodesax.h"
#include "qrpthndlr.h"

using XDPrintSchema::PageQRCode::QRCodeData;
using XDPrintSchema::PageQRCode::DisableQRCode;
using XDPrintSchema::PageQRCode::EnableQRCode;

/*++

Routine Name:

    CQrCodeFilter::CQrCodeFilter

Routine Description:

    Default constructor for the qr code filter which ensures GDI plus is correctly running

Arguments:

    None

Return Value:

    None

--*/
CQrCodeFilter::CQrCodeFilter() :
	m_startFixedPageTag(NULL),
	m_DataPerFixedPage(NULL),
	m_pQRCodeProp(NULL),
	m_QRCodeOption(DisableQRCode)
{
    ASSERTMSG(m_gdiPlus.GetGDIPlusStartStatus() == Ok, "GDI plus is not correctly initialized.\n");
}

/*++

Routine Name:

    CQrCodeFilter::~CQrCodeFilter

Routine Description:

    Default destructor for the qr code filter

Arguments:

    None

Return Value:

    None

--*/
CQrCodeFilter::~CQrCodeFilter()
{
}

/*++

Routine Name:

    CQrCodeFilter::ProcessPart

Routine Description:

    Method for processing each fixed page part in a container. After processing all the fixed page,
	print data and start fixed page element are obtained and stored in the cache using CDataSaxHandler
	handler

Arguments:

    pFP - Pointer to the fixed page to process

Return Value:

    HRESULT
    S_OK    - On success
    S_FALSE - When not enabled in the PT
    E_*     - On error

--*/
HRESULT
CQrCodeFilter::ProcessPart(
    _Inout_ IFixedPage* pFP
    )
{
    VERBOSE("Processing Fixed Page part with data handler\n");

    HRESULT hr = S_OK;

    if (SUCCEEDED(hr = CHECK_POINTER(pFP, E_POINTER)))
    {
		//
		// Retrieve the QR code settings from the PrintTicket
		// Not yet do the user interface
		IXMLDOMDocument2* pPT = NULL;
		if (SUCCEEDED(hr = m_ptManager.SetTicket(pFP)) &&
			SUCCEEDED(hr = m_ptManager.GetTicket(kPTPageScope, &pPT))&&
			SUCCEEDED(hr = GetQRCodeProp(pPT, &m_pQRCodeProp))&&
			SUCCEEDED(hr = m_pQRCodeProp->GetQROption(&m_QRCodeOption))&&
			(m_QRCodeOption== EnableQRCode))
		{
			//
			// Retrieve the writer from the fixed page
			//
			CComPtr<IPrintWriteStream>  pWriter(NULL);

			if (SUCCEEDED(hr = pFP->GetWriteStream(&pWriter)))
			{
				//
				// Set-up the SAX reader and begin parsing the mark-up
				//
				CComPtr<ISAXXMLReader>    pSaxRdr(NULL);
				CComPtr<IPrintReadStream> pReader(NULL);

				try
				{
					CDataSaxHandler dataSaxHndlr(pWriter);

					if (SUCCEEDED(hr = pSaxRdr.CoCreateInstance(CLSID_SAXXMLReader60)) &&
						SUCCEEDED(hr = pSaxRdr->putContentHandler(&dataSaxHndlr)) &&
						SUCCEEDED(hr = pFP->GetStream(&pReader)))
					{
						CComPtr<ISequentialStream> pReadStreamToSeq(NULL);

						pReadStreamToSeq.Attach(new(std::nothrow) pfp::PrintReadStreamToSeqStream(pReader));

						if (SUCCEEDED(hr = CHECK_POINTER(pReadStreamToSeq, E_OUTOFMEMORY)))
						{
							hr = pSaxRdr->parse(CComVariant(static_cast<ISequentialStream*>(pReadStreamToSeq)));
							hr = dataSaxHndlr.getFixedPageData(&m_startFixedPageTag, &m_DataPerFixedPage);
						}
					}
				}
				catch (CXDException& e)
				{
					hr = e;
				}

				pWriter->Close();
			}
		}
		else if (hr == E_ELEMENT_NOT_FOUND)
		{
			hr = S_FALSE;
		}

		if (SUCCEEDED(hr))
		{
			//
			// Form a cache to store important data/text so that it wouldn't lose after every loop
			//
			try
			{
				m_cacheFP.push_back(pFP);
				m_cacheAllPrintData.push_back(m_DataPerFixedPage);
			}
			catch (exception& DBG_ONLY(e))
			{
				ERR(e.what());
				hr = E_FAIL;
			}
		}

		//
		// We can send the fixed page
		//
		hr = m_pXDWriter->SendFixedPage(pFP);
    }

    ERR_ON_HR(hr);
    return hr;
}

/*++

Routine Name:

CQrCodeFilter::Finalize

Routine Description:

Method to create new page by using CreateNewQRPage() method. After that, write the font resource
which is used to write TITLE to generated QR code. At last the new created fixed page is send to 
the stream

Arguments:

None

Return Value:

HRESULT
S_OK - On success
E_*  - On error

--*/
HRESULT
CQrCodeFilter::Finalize(
	VOID
)
{
	HRESULT hr = S_OK;

	if (m_QRCodeOption == EnableQRCode)
	{
		if (m_pXDWriter == NULL)
		{
			hr = E_PENDING;
		}

		size_t cMaxPagesIndex = m_cacheFP.size() - 1;

		if (SUCCEEDED(hr) &&
			SUCCEEDED(hr = CreateNewQRPage(&m_pNewFP)))
		{
			//
			// Copy resources part for new page
			//
			if (SUCCEEDED(hr) &&
				SUCCEEDED(hr = CHECK_POINTER(m_pResCopier, E_PENDING)) &&
				//
				//just copy resource from original page
				//
				SUCCEEDED(hr = m_pResCopier->CopyPageResources(m_cacheFP[cMaxPagesIndex], m_pNewFP)) &&
				//
				// Write font resource (Arial) to the new page (used to write TITLE of generated QR code)
				// If the resource is not set, the new created page cannot be displayed or showing errror
				//
				SUCCEEDED(hr = m_resCache.WriteResource<IPartFont>(m_pXDWriter, m_pNewFP, &m_QrTitleFont)))
			{
				//
				// We can send the fixed page
				//
				hr = m_pXDWriter->SendFixedPage(m_pNewFP);

			}
			else if (hr == HRESULT_FROM_WIN32(ERROR_FILE_NOT_FOUND))
			{
				//
				// Could not find resource file - fail gracefully so we continue
				// to process the document
				//
				ERR("Specified resource file does not exist\n");
				hr = S_FALSE;
			}
			else if (hr == HRESULT_FROM_WIN32(ERROR_ACCESS_DENIED))
			{
				//
				// Insufficient rights to open file - fail gracefully so we continue
				// to process the document
				//
				ERR("Insufficient rights to open resource file\n");
				hr = S_FALSE;
			}
			else if (hr == HRESULT_FROM_WIN32(ERROR_NOT_SUPPORTED))
			{
				//
				// We do not support this URI as a resource - fail gracefully so we continue
				// to process the document
				//
				ERR("Unsupported URI to resource resource\n");
				hr = S_FALSE;
			}

		}
	}
	ERR_ON_HR(hr);
	return hr;
}

/*++

Routine Name:

CDataSaxHandler::CreateNewQRPage

Routine Description:

Method to create a new QR page at the end of original document. Pass all the obtained 
data (m_startFixedPageTag, m_cacheAllPrintData) from first time parsing of XPS to 
QRSaxHndlr (second time parsing - generate QR code)

Arguments:

pNewPage - Pointer to the newly created fixed page

Return Value:

HRESULT
S_OK - On success
E_*  - On error

--*/
HRESULT
CQrCodeFilter::CreateNewQRPage(
	_Outptr_ IFixedPage** ppNewPage
)
{
	HRESULT hr = S_OK;

	//
	// Validate parameters and members before proceeding
	//
	if (SUCCEEDED(hr = CHECK_POINTER(ppNewPage, E_POINTER)) &&
		SUCCEEDED(hr = CHECK_POINTER(m_pXDWriter, E_PENDING)))
	{
		*ppNewPage = NULL;

		try
		{
			//
			// Create a unique name for the new QR code page for this print session
			CComBSTR bstrPageURI;
			CStringXDW cstrPageURI;
			cstrPageURI.Format(L"/QR_Code_Page_%u.xml", GetUniqueNumber());
			bstrPageURI.Empty();
			bstrPageURI.Attach(cstrPageURI.AllocSysString());

			//
			// Create a new empty page and retrieve a writer. Also get a
			// reader from the first printed page so we can copy the FixedPage root
			// element. This ensures the page sizes match.
			//
			CComPtr<IPrintWriteStream>  pWriter(NULL);
			CComPtr<ISAXXMLReader>      pSaxRdr(NULL);

			if (SUCCEEDED(hr) &&
				SUCCEEDED(hr = m_pXDWriter->GetNewEmptyPart(bstrPageURI,
															__uuidof(IFixedPage),
															reinterpret_cast<VOID**>(ppNewPage),
															&pWriter)) &&
				SUCCEEDED(hr = pSaxRdr.CoCreateInstance(CLSID_SAXXMLReader60)))
			{
				//
				// Use a simple SAX handler the set new created page as the page to write. This SAX will
				// encode the print data/text to QR code and write a title to QR code. For more detail,
				// read CQrCodeSaxHandler class
				//
				CQrCodeSaxHandler QRSaxHndlr(pWriter, m_pQRCodeProp, m_startFixedPageTag, m_cacheAllPrintData);
				CComPtr<IPrintReadStream> pReader(NULL);
				//
				// Set first page of printed file as the file to read. (Not neccessary use first page)
				// CQrCodeSaxHandler only write the new page, so the content of the reading file is not
				// important. (**Maybe can use other method to write this process)
				//
				IFixedPage* pFP = NULL;
				pFP = m_cacheFP[0];

				if (SUCCEEDED(hr) &&
					SUCCEEDED(hr = pSaxRdr->putContentHandler(&QRSaxHndlr)) &&
					SUCCEEDED(hr = pFP->GetStream(&pReader)))
				{
					CComPtr<ISequentialStream> pReadStreamToSeq(NULL);

					pReadStreamToSeq.Attach(new(std::nothrow) pfp::PrintReadStreamToSeqStream(pReader));

					if (SUCCEEDED(hr = CHECK_POINTER(pReadStreamToSeq, E_OUTOFMEMORY)))
					{
						hr = pSaxRdr->parse(CComVariant(static_cast<ISequentialStream*>(pReadStreamToSeq)));
					}
				}

				pWriter->Close();
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

CQrCodeFilter::GetQRCodeProp

Routine Description:

Method for obtaining the QR Code properties from Print Ticket

Arguments:

pPrintTicket - DOM document containing the PrintTicket
pQRCodeProp  - Pointer to QR Code Properties Obtained from
Print Ticket

Return Value:

HRESULT
S_OK                - On success
E_ELEMENT_NOT_FOUND - When feature not present in the PT
E_*                 - On error

--*/

HRESULT
CQrCodeFilter::GetQRCodeProp(
	_In_      IXMLDOMDocument2*  pPrintTicket,
	_Outptr_ CQRCodePTProperties** ppQRCodeProp
)
{
	HRESULT hr = S_OK;

	if (SUCCEEDED(hr = CHECK_POINTER(ppQRCodeProp, E_POINTER)))
	{

		*ppQRCodeProp = NULL;

		try
		{

			CQRCodePTHandler  qrcodePTHandler(pPrintTicket);
			QRCodeData qrcodeData;

			if (SUCCEEDED(hr = qrcodePTHandler.GetData(&qrcodeData)))
			{
				*ppQRCodeProp = new(std::nothrow) CQRCodePTProperties(qrcodeData);
			}
		}
		catch (CXDException& e)
		{
			hr = e;
		}
	}

	ERR_ON_HR_EXC(hr, E_ELEMENT_NOT_FOUND);
	return hr;
}
