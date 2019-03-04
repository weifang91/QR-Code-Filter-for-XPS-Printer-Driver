/*++
File Name:

qrsax.cpp

Abstract:

QR code sax handler implementation. The QR code SAX handler is responsible to WRITE 
the new created fixed page so that it has same size as printed file. Besides that, 
this handler encode the collected data/text from printed file to QR code. At the end 
of this process, a QR code with TITLE is written in the new created fixed page

--*/

#include "precomp.h"
#include "debug.h"
#include "globals.h"
#include "xdstring.h"
#include "qrcodesax.h"
#include <string>
#include <vector>
#include "QrCode.hpp"
using namespace qrcodegen;


/*++

Routine Name:

CQrCodeSaxHandler::CQrCodeSaxHandler

Routine Description:

Contructor for the qr code filters SAX handler.
The constructor registers a writer for streaming out any markup and a watermark
handler object for generating any new watermark markup and handling related resources

Arguments:

pWriter				- Pointer to a write stream which receives markup
startFixedPageTag	- String for Fixed Page Tag
cacheAllPrintData	- String Vector for all collected print data/text

Return Value:

None
Throws CXDException(HRESULT) on an error

--*/

CQrCodeSaxHandler::CQrCodeSaxHandler(
	_In_ IPrintWriteStream* pWriter,
	_In_ CQRCodePTProperties* pQRCodeProp,
	_In_ CComBSTR			startFixedPageTag,
	_In_ vector<CComBSTR>	cacheAllPrintData
):
	m_pWriter(pWriter),
	m_bOpenTag(FALSE),
	m_pQRCodeProp(pQRCodeProp),
	m_startFixedPageTag(startFixedPageTag),
	m_cacheAllPrintData(cacheAllPrintData),
	m_combinePrintData(L"Decoded Data:")
{
	ASSERTMSG(m_pWriter != NULL, "NULL writer passed to QR code SAX handler.\n");

	HRESULT hr = S_OK;
	if (FAILED(hr = CHECK_POINTER(m_pWriter, E_POINTER)))
	{
		throw CXDException(hr);
	}
}

/*++

Routine Name:

CQRSaxHandler::~CQRSaxHandler

Routine Description:

Default destructor for the QR code filter SAX handler

Arguments:

None

Return Value:

None

--*/
CQrCodeSaxHandler::~CQrCodeSaxHandler()
{
}

/*++

Routine Name:

CQRSaxHandler::startDocument

Routine Description:

SAX handler method which handles the start document call to ensure
the xml version is correctly set. This method also create "Glyphs"
element to display generated QR code's TITLE

Arguments:

None

Return Value:

HRESULT
S_OK - On success
E_*  - On error

--*/
HRESULT STDMETHODCALLTYPE
CQrCodeSaxHandler::startDocument(
	void
)
{
	HRESULT hr = S_OK;

	try
	{
		if (SUCCEEDED(hr = CHECK_POINTER(m_pWriter, E_FAIL)))
		{
			CStringXDW cstrOut(L"<?xml version=\"1.0\" encoding=\"utf-8\"?>");
			cstrOut.Append(m_startFixedPageTag);

			//
			// Create <Glyphs> element to write TITLE for generated QR code
			//
			CComPtr<IXMLDOMElement>   m_pQRElem;
			CComPtr<IXMLDOMDocument2> m_pQRDOMDoc(NULL);
			hr = m_pQRDOMDoc.CoCreateInstance(CLSID_DOMDocument60);

			if (FAILED(hr))
			{
				ERR("Failed to create QR code DOM document.\n");
				throw CXDException(hr);
			}

			if (SUCCEEDED(hr) &&
				SUCCEEDED(hr = m_pQRDOMDoc->createElement(CComBSTR(L"Glyphs"), &m_pQRElem)) &&
				SUCCEEDED(hr = m_pQRElem->setAttribute(CComBSTR(L"Fill"), CComVariant(L"#000000"))) &&
				SUCCEEDED(hr = m_pQRElem->setAttribute(CComBSTR(L"Opacity"), CComVariant(L"1.00"))) &&
				SUCCEEDED(hr = m_pQRElem->setAttribute(CComBSTR(L"RenderTransform"), CComVariant(L"1.00,0.00,0.00,1.00,0.00,100.00"))) &&
				SUCCEEDED(hr = m_pQRElem->setAttribute(CComBSTR(L"FontUri"), CComVariant(L"/Resources/Fonts/78F47176-ADD7-0E49-AB3A-C59F137240AC.odttf"))) &&
				SUCCEEDED(hr = m_pQRElem->setAttribute(CComBSTR(L"FontRenderingEmSize"), CComVariant(L"16"))) &&
				SUCCEEDED(hr = m_pQRElem->setAttribute(CComBSTR(L"OriginX"), CComVariant(L"20"))) &&
				SUCCEEDED(hr = m_pQRElem->setAttribute(CComBSTR(L"OriginY"), CComVariant(L"20"))))
			{
				hr = m_pQRElem->setAttribute(CComBSTR(L"UnicodeString"), CComVariant("Teaspoon QR code :"));
			}

			CComBSTR bstrQRcode;

			hr = m_pQRElem->get_xml(&bstrQRcode);

			if (SUCCEEDED(hr))
			{
				_Analysis_assume_nullterminated_(bstrQRcode);
			}

			cstrOut.Append(bstrQRcode);
			// end 

			hr = WriteToPrintStream(&cstrOut, m_pWriter);
		}


	}
	catch (CXDException& e)
	{
		hr = e;
	}

	ERR_ON_HR(hr);
	return hr;
}

/*++

Routine Name:

CQRSaxHandler::endDocument

Routine Description:

SAX handler method which handles the end document call. This method encode all the 
collected print data/ text to QR code. The source code of QR code generator is 
obtained from https://www.nayuki.io/page/qr-code-generator-library#cpp

Arguments:

None

Return Value:

HRESULT
S_OK - On success
E_*  - On error

--*/
HRESULT STDMETHODCALLTYPE
CQrCodeSaxHandler::endDocument(
	void
)
{
	HRESULT hr = S_OK;

	try
	{
		if (SUCCEEDED(hr = CHECK_POINTER(m_pWriter, E_FAIL)))
		{
			//
			// Get scale, offset X and Offset Y
			//
			CComBSTR bstrQRScale;		m_pQRCodeProp->GetQRScale(&bstrQRScale);
			CComBSTR bstrQROffsetX;		m_pQRCodeProp->GetQROffsetX(&bstrQROffsetX);
			CComBSTR bstrQROffsetY;		m_pQRCodeProp->GetQROffsetY(&bstrQROffsetY);

			//
			//Combine all the print data/text
			//
			size_t cMaxPageData = m_cacheAllPrintData.size();
			for (size_t dataIndex = 0; dataIndex < cMaxPageData; dataIndex++)
			{
				m_combinePrintData.Append(m_cacheAllPrintData[dataIndex]);
			}

			//-----------------------------------------------------------------------------------
			//							START QR CODE
			//-----------------------------------------------------------------------------------
			// Convert string from CComBSTR to const_char
			//
			USES_CONVERSION;
			std::string qrCodeProp[3] = { W2CA(bstrQRScale) ,
										 W2CA(bstrQROffsetX) ,
										 W2CA(bstrQROffsetY) };
			const char *const_char_PrintData = W2CA(m_combinePrintData);

			QrCode qr0 = QrCode::encodeText(const_char_PrintData, QrCode::Ecc::MEDIUM);
			std::string qrPathData = qr0.toXpsPathString(4, qrCodeProp);  // set the module value

			//
			// Convert std::string to CComBSTR
			//
			CComBSTR QRcodeStringInXPS(qrPathData.c_str());

			CStringXDW	cstrEnd;
			cstrEnd.Append(QRcodeStringInXPS);
			//-----------------------------------------------------------------------------------
			//							END QR CODE
			//-----------------------------------------------------------------------------------


			//
			//Close Fixed Page elements
			//
			cstrEnd.Append(L"</FixedPage>");

			hr = WriteToPrintStream(&cstrEnd, m_pWriter);
		}
	}
	catch (CXDException& e)
	{
		hr = e;
	}

	ERR_ON_HR(hr);
	return hr;
}

