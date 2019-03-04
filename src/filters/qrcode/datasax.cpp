/*++

File Name:

   dataxas.cpp

Modified from:

   wmsax.cpp

Abstract:

	Data sax handler definition. The Data SAX handler is responsible for parsing the FixedPage
	mark-up for the page size and collecting the print data (used to generate QR code). This 
	Data SAX does not write or change the origianl print file but only collecting important data

--*/

#include "precomp.h"
#include "debug.h"
#include "globals.h"
#include "xdstring.h"
#include "datasax.h"

/*++

Routine Name:

    CDataSaxHandler::CDataSaxHandler

Routine Description:

    Contructor for the qr code filters SAX handler. 
    The constructor registers a writer and cache for Fixed Page Elememt (Needed so that the new create page
	has same width and height as original file) and Print Data (Used to be encoded as QR code)

Arguments:

    pWriter    - Pointer to a write stream which receives markup

Return Value:

    None
    Throws CXDException(HRESULT) on an error

--*/
CDataSaxHandler::CDataSaxHandler(
    _In_ IPrintWriteStream* pWriter
    ) :
    m_pWriter(pWriter),
    m_bOpenTag(FALSE),
	m_bstrFixedPageElem(L""),
	m_bstrPrintData(L"")
{
    ASSERTMSG(m_pWriter != NULL, "NULL writer passed to data SAX handler.\n");

    HRESULT hr = S_OK;
    if (FAILED(hr = CHECK_POINTER(m_pWriter, E_POINTER)))
    {
        throw CXDException(hr);
    }
}

/*++

Routine Name:

    CDataSaxHandler::~CDataSaxHandler

Routine Description:

    Default destructor for the qr code filter SAX handler

Arguments:

    None

Return Value:

    None

--*/
CDataSaxHandler::~CDataSaxHandler()
{
}

/*++

Routine Name:

    CDataSaxHandler::startElement

Routine Description:

    SAX handler method which handles each start element for the XML markup

Arguments:

    pwchQName   - Pointer to a string containing the element name
    cchQName    - Count of the number of characters in the element name
    pAttributes - Pointer to the attribute list for the supplied element

Return Value:

    HRESULT
    S_OK - On success
    E_*  - On error

--*/
HRESULT STDMETHODCALLTYPE
CDataSaxHandler::startElement(
    CONST wchar_t*,
    INT,
    CONST wchar_t*,
    INT,
    _In_reads_(cchQName) CONST wchar_t*  pwchQName,
    _In_                  INT             cchQName,
    _In_                  ISAXAttributes* pAttributes
    )
{
    HRESULT hr = S_OK;
    CStringXDW cstrOut;

    try
    {
        //
        // Check if we need to close an opened tag
        //
        if (m_bOpenTag)
        {
            cstrOut.Append(L">");
        }

        //
        // Store the opened element name so we can handle nested elements
        //
        m_bstrOpenElement = CComBSTR(cchQName, pwchQName);

        //
        // Write out element
        //
        cstrOut.Append(L"<");
        cstrOut.Append(m_bstrOpenElement);
    }
    catch (CXDException& e)
    {
        hr = e;
    }

    //
    // We opened a tag
    //
    m_bOpenTag = TRUE;

    //
    // If this is the fixed page we need the width and height retreived
    //
    BOOL bIsFixedPage = (m_bstrOpenElement == L"FixedPage");

    //
    // Find the number of attributes and enumerate over all of them
    //
    INT cAttributes = 0;
    if (SUCCEEDED(hr) &&
        SUCCEEDED(hr = pAttributes->getLength(&cAttributes)))
    {
        for (INT cIndex = 0; cIndex < cAttributes; cIndex++)
        {
            PCWSTR pszAttUri   = NULL;
            INT    cchAttUri   = 0;
            PCWSTR pszAttName  = NULL;
            INT    cchAttName  = 0;
            PCWSTR pszAttQName = NULL;
            INT    cchAttQName = 0;
            PCWSTR pszAttValue = NULL;
            INT    cchAttValue = 0;

            //
            // Get the attribute data ready to write out
            //
            if (SUCCEEDED(hr = pAttributes->getName(cIndex,
                                                    &pszAttUri,
                                                    &cchAttUri,
                                                    &pszAttName,
                                                    &cchAttName,
                                                    &pszAttQName,
                                                    &cchAttQName)))
            {
                if (SUCCEEDED(pAttributes->getValue(cIndex, &pszAttValue, &cchAttValue)))
                {
                    try
                    {
                        CComBSTR bstrAttName(cchAttQName, pszAttQName);
                        CComBSTR bstrAttValue(cchAttValue, pszAttValue);

                        //
                        // Delimit attributes with a space
                        //
                        cstrOut.Append(L" ");

                        //
                        // Reconstruct the attribute and write back to
                        // the fixed page
                        //
                        cstrOut.Append(bstrAttName);
                        cstrOut.Append(L"=\"");

                        //
                        // If this is a UnicodeString we may need to escape entities
                        //
                        if (bstrAttName == L"UnicodeString")
                        {
							//
							// !!Important : Collecting all the data/text in every Fixed Page
							//
							m_bstrPrintData.Append(bstrAttValue);
							//
							// Takes a BSTR and replaces all instances of these characters with
							// their escaped versions
							//
                            hr = EscapeEntity(&bstrAttValue);
                        }

                        cstrOut.Append(bstrAttValue);
                        cstrOut.Append(L"\"");
                    }
                    catch (CXDException& e)
                    {
                        hr = e;
                    }
                }
            }
        }
    }

    //
    // We output the mark-up here for underlaid watermarks
    //
    if (SUCCEEDED(hr) &&
        bIsFixedPage)
    {
        //
        // If this is the fixed page element make sure we close it in case the
        // page has no content.
        //
        try
        {
            cstrOut.Append(L">");
			m_bstrFixedPageElem.Append(cstrOut);
            m_bOpenTag = FALSE;
        }
        catch (CXDException& e)
        {
            hr = e;
        }
    }

    if (SUCCEEDED(hr))
    {
        hr = WriteToPrintStream(&cstrOut, m_pWriter);
    }

    ERR_ON_HR(hr);
    return hr;
}

/*++

Routine Name:

    CWMSaxHandler::endElement

Routine Description:

    SAX handler method which handles each end element for the XML markup

Arguments:

    pwchQName - Pointer to a string containing the element name
    cchQName  - Count of the number of characters in the element name

Return Value:

    HRESULT
    S_OK - On success
    E_*  - On error

--*/
HRESULT STDMETHODCALLTYPE
CDataSaxHandler::endElement(
    CONST wchar_t*,
    INT,
    CONST wchar_t*,
    INT,
    _In_reads_(cchQName) CONST wchar_t* pwchQName,
    _In_                  INT            cchQName
    )
{
    HRESULT hr = S_OK;
    CStringXDW cstrClose;

    try
    {
        CComBSTR bstrElement(cchQName, pwchQName);

        //
        // If this element matches the current open element, use shorthand to close
        // the tag
        //
        if (bstrElement == m_bstrOpenElement &&
            m_bOpenTag)
        {
            cstrClose.Append(L"/>");
        }
        else
        {
            //
            // Close the element
            //
            cstrClose.Append(L"</");
            cstrClose.Append(bstrElement);
            cstrClose.Append(L">");
        }
    }
    catch (CXDException& e)
    {
        hr = e;
    }

    if (SUCCEEDED(hr))
    {
        hr = WriteToPrintStream(&cstrClose, m_pWriter);
    }

    m_bOpenTag = FALSE;

    ERR_ON_HR(hr);
    return hr;
}


/*++

Routine Name:

    CWMSaxHandler::startDocument

Routine Description:

    SAX handler method which handles the start document call to ensure
    the xml version is correctly set

Arguments:

    None

Return Value:

    HRESULT
    S_OK - On success
    E_*  - On error

--*/
HRESULT STDMETHODCALLTYPE
CDataSaxHandler::startDocument(
    void
    )
{
    HRESULT hr = S_OK;

    try
    {
        if (SUCCEEDED(hr = CHECK_POINTER(m_pWriter, E_FAIL)))
        {
            CStringXDW cstrOut(L"<?xml version=\"1.0\" encoding=\"utf-8\"?>");
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

CWMSaxHandler::getFixedPageData

Routine Description:

Method to get all the related data after parsing. The data inlcude the Fixed Page Tag and all the text.

Arguments:

None

Return Value:

HRESULT
S_OK - On success
E_*  - On error

--*/
HRESULT STDMETHODCALLTYPE
CDataSaxHandler::getFixedPageData(
	_Outptr_ BSTR*			pstartFixedPageTag,
	_Outptr_ BSTR*			pDataPerFixedPage
)
{
	HRESULT hr = S_OK;

	try
	{
		hr = m_bstrFixedPageElem.CopyTo(pstartFixedPageTag);
		hr = m_bstrPrintData.CopyTo(pDataPerFixedPage);
	}
	catch (CXDException& e)
	{
		hr = e;
	}

	ERR_ON_HR(hr);
	return hr;
}