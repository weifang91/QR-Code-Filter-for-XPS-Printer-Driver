/*++

File Name:

qrpthndlr.cpp

Abstract:

PageQRCode PrintTicket handler implementation. Derived from CPTHandler,
this provides PageQRCode specific Get and Set methods acting on the
PrintTicket passed (as a DOM document).

--*/

#include "precomp.h"
#include "debug.h"
#include "globals.h"
#include "xdstring.h"
#include "xdexcept.h"
#include "qrpthndlr.h"

using XDPrintSchema::PRINTTICKET_NAME;
using XDPrintSchema::NAME_ATTRIBUTE_NAME;
using XDPrintSchema::SCHEMA_DECIMAL;
using XDPrintSchema::SCHEMA_INTEGER;
using XDPrintSchema::SCHEMA_STRING;

using XDPrintSchema::PageQRCode::QRCodeData;

using XDPrintSchema::PageQRCode::EQRCodeOption;
using XDPrintSchema::PageQRCode::EQRCodeOptionMin;
using XDPrintSchema::PageQRCode::DisableQRCode;
using XDPrintSchema::PageQRCode::EnableQRCode;
using XDPrintSchema::PageQRCode::EQRCodeOptionMax;

using XDPrintSchema::PageQRCode::ECommonQRCodeProps;
using XDPrintSchema::PageQRCode::ECommonQRCodePropsMin;
using XDPrintSchema::PageQRCode::QRCode_Scale;
using XDPrintSchema::PageQRCode::QRCode_Offset_X;
using XDPrintSchema::PageQRCode::QRCode_Offset_Y;
using XDPrintSchema::PageQRCode::ECommonQRCodePropsMax;

using XDPrintSchema::PageQRCode::QRCODE_FEATURE;
using XDPrintSchema::PageQRCode::QRCODE_OPTIONS;
using XDPrintSchema::PageQRCode::CMN_QRCODE_PROPS;



/*++

Routine Name:

CQRCodePTHandler::CQRCodePTHandler

Routine Description:

CQRCodePTHandler class constructor

Arguments:

pPrintTicket - Pointer to the DOM document representation of the PrintTicket

Return Value:

None

--*/
CQRCodePTHandler::CQRCodePTHandler(
	_In_ IXMLDOMDocument2* pPrintTicket
) :
	CPTHandler(pPrintTicket)
{
}

/*++

Routine Name:

CQRCodePTHandler::~CQRCodePTHandler

Routine Description:

CWMPTHandler class destructor

Arguments:

None

Return Value:

None

--*/
CQRCodePTHandler::~CQRCodePTHandler()
{
}

/*++

Routine Name:

CQRCodePTHandler::GetData

Routine Description:

The routine fills the data structure passed in with QR code data retrieved from
the PrintTicket passed to the class constructor.

Arguments:

pQRCodeData - Pointer to the QR code data structure to be filled in

Return Value:

HRESULT
S_OK                - On success
E_ELEMENT_NOT_FOUND - Feature not present in PrintTicket
E_*                 - On error

--*/
HRESULT
CQRCodePTHandler::GetData(
	_Out_ QRCodeData* pQRCodeData
)
{
	HRESULT hr = S_OK;

	if (SUCCEEDED(hr = CHECK_POINTER(pQRCodeData, E_POINTER)))
	{
		CComBSTR bstrQRCodeOption;

		if (SUCCEEDED(hr = GetFeatureOption(CComBSTR(QRCODE_FEATURE), &bstrQRCodeOption))&&
			SUCCEEDED(hr = GetScoredPropertyValue(CComBSTR(QRCODE_FEATURE),
												  CComBSTR(CMN_QRCODE_PROPS[QRCode_Scale]),
												  &pQRCodeData->scale)) &&
			SUCCEEDED(hr = GetScoredPropertyValue(CComBSTR(QRCODE_FEATURE),
												  CComBSTR(CMN_QRCODE_PROPS[QRCode_Offset_X]),
												  &pQRCodeData->offset_x))&&
			SUCCEEDED(hr = GetScoredPropertyValue(CComBSTR(QRCODE_FEATURE),
												  CComBSTR(CMN_QRCODE_PROPS[QRCode_Offset_Y]),
												  &pQRCodeData->offset_y)))
		{
			if (bstrQRCodeOption == QRCODE_OPTIONS[EnableQRCode])
			{
				pQRCodeData->QRCodeOption = EnableQRCode;

			}
			else if (bstrQRCodeOption == QRCODE_OPTIONS[DisableQRCode])
			{
				pQRCodeData->QRCodeOption = DisableQRCode;
			}
			else
			{
				hr = E_FAIL;
			}
		}

		if (hr == E_ELEMENT_NOT_FOUND)
		{
			pQRCodeData->QRCodeOption = DisableQRCode;
		}
	}

	//
	// Validate the data
	//
	if (SUCCEEDED(hr))
	{
		if (pQRCodeData->QRCodeOption <  EQRCodeOptionMin ||
			pQRCodeData->QRCodeOption >= EQRCodeOptionMax)
		{
			hr = E_FAIL;
		}
	}

	ERR_ON_HR_EXC(hr, E_ELEMENT_NOT_FOUND);
	return hr;
}

/*++

Routine Name:

CQRCodePTHandler::SetData

Routine Description:

This routine sets the QR Code data in the PrintTicket passed to the
class constructor.

Arguments:

pQRCOdeData - Pointer to the QR code data to be set in the PrintTicket

Return Value:

HRESULT
S_OK - On success
E_*  - On error

--*/
HRESULT
CQRCodePTHandler::SetData(
	_In_ CONST QRCodeData* pQRCodeData
)
{
	HRESULT hr = S_OK;

	if (SUCCEEDED(hr = CHECK_POINTER(pQRCodeData, E_POINTER)))
	{
		try
		{
			//
			// Remove any existing QR code feature node
			//
			if (SUCCEEDED(hr = Delete()) &&
				pQRCodeData->QRCodeOption == EnableQRCode)
			{
				//
				// Construct the appropriate QR code element
				//
				CComPtr<IXMLDOMElement> pQRCodeDataElem(NULL);
				PTDOMElementVector      paramInitList;

				hr = CreateCommonQRCodeElements(pQRCodeData, &pQRCodeDataElem, &paramInitList);
				
				//
				// Insert the QR code and paramater init nodes
				//
				CComPtr<IXMLDOMNode> pPTRoot(NULL);

				CComBSTR bstrPTQuery(m_bstrFrameworkPrefix);
				bstrPTQuery += PRINTTICKET_NAME;

				if (SUCCEEDED(hr) &&
					SUCCEEDED(hr = GetNode(bstrPTQuery, &pPTRoot)))
				{
					hr = pPTRoot->appendChild(pQRCodeDataElem, NULL);

					PTDOMElementVector::iterator iterParamInit = paramInitList.begin();

					for (; iterParamInit != paramInitList.end() && SUCCEEDED(hr); iterParamInit++)
					{
						hr = pPTRoot->appendChild(*iterParamInit, NULL);
					}
				}
			}
		}
		catch (exception& DBG_ONLY(e))
		{
			ERR(e.what());
			hr = E_FAIL;
		}
	}

	ERR_ON_HR(hr);
	return hr;
}

/*++

Routine Name:

CQRCodePTHandler::Delete

Routine Description:

This routine deletes the QR Code feature from the PrintTicket passed to the
class constructor

Arguments:

None

Return Value:

HRESULT
S_OK - On success
E_*  - On error

--*/
HRESULT
CQRCodePTHandler::Delete(
	VOID
)
{
	//
	// Remove any existing watermark feature node
	//
	HRESULT hr = DeleteFeature(CComBSTR(QRCODE_FEATURE));

	ERR_ON_HR(hr);
	return hr;
}

/*++

Routine Name:

CQRCodePTHandler::CreateCommonQRCodeElements

Routine Description:

This routine creates all the DOM elements common to QR code. Note that
this routine also returns the option element seperately (so the caller can specify the
QR code type) and a list of the ParameterInit elements (so the caller can append them
to the root PrintTicket element)

Arguments:

pQRCodeData        - Pointer to the watermark data structure
ppQRCodeDataElem   - Pointer to an IXMLDOMElement pointer that recieves feature element
ppOptionElem   - Pointer to an IXMLDOMElement pointer that recieves option element
pParamInitList - Pointer to a vector of DOM element pointers that recieves the parameter init elements

Return Value:

HRESULT
S_OK - On success
E_*  - On error

--*/
HRESULT
CQRCodePTHandler::CreateCommonQRCodeElements(
	_In_        CONST QRCodeData* pQRCodeData,
	_Outptr_ IXMLDOMElement**     ppQRCodeDataElem,
	_Out_       PTDOMElementVector*  pParamInitList
)
{
	HRESULT hr = S_OK;

	CComPtr<IXMLDOMElement> pOptionElem(NULL);

	if (SUCCEEDED(hr = CHECK_POINTER(pQRCodeData, E_POINTER)) &&
		SUCCEEDED(hr = CHECK_POINTER(ppQRCodeDataElem, E_POINTER)) &&
		SUCCEEDED(hr = CHECK_POINTER(pParamInitList, E_POINTER)))
	{
		*ppQRCodeDataElem = NULL;
	
		//
		// Create the feature option pair - CreateFeatureOptionPair does not
		// set the option name, this is done in the calling function
		//
		if (SUCCEEDED(hr = CreateFeatureOptionPair(CComBSTR(QRCODE_FEATURE), ppQRCodeDataElem, &pOptionElem)))
		{
			//
			// Over all common properties, create and insert the element
			//
			for (ECommonQRCodeProps cmnProps = ECommonQRCodePropsMin;
				cmnProps < ECommonQRCodePropsMax && SUCCEEDED(hr);
				cmnProps = static_cast<ECommonQRCodeProps>(cmnProps + 1))
			{
				//
				// Create the scored property element
				//
				CComPtr<IXMLDOMElement> pScoredProperty(NULL);

				if (SUCCEEDED(hr = CreateScoredProperty(CComBSTR(CMN_QRCODE_PROPS[cmnProps]), &pScoredProperty)))
				{
					//
					// Construct the param ref and param init elements
					//
					CComBSTR bstrPRefName(QRCODE_FEATURE);

					bstrPRefName += CMN_QRCODE_PROPS[cmnProps];

					CComPtr<IXMLDOMElement> pParamRef(NULL);
					CComPtr<IXMLDOMElement> pParamInit(NULL);

					CComBSTR bstrType;
					CComBSTR bstrValue;

					if (SUCCEEDED(hr = GetCmnPropTypeAndValue(pQRCodeData, cmnProps, &bstrType, &bstrValue)) &&
						SUCCEEDED(hr = CreateParamRefInitPair(bstrPRefName, bstrType, bstrValue, &pParamRef, &pParamInit)))
					{
						//
						// Append the parameter ref element to the scored property, append the
						// scored property to the option element and add the parameter init
						// element to the vector
						//
						CComPtr<IXMLDOMNode> pPRInserted(NULL);
						CComPtr<IXMLDOMNode> pSPInserted(NULL);

						if (SUCCEEDED(hr = pScoredProperty->appendChild(pParamRef, &pPRInserted)) &&
							SUCCEEDED(hr = (pOptionElem)->appendChild(pScoredProperty, &pSPInserted)))
						{
							try
							{
								pParamInitList->push_back(pParamInit);
							}
							catch (exception& DBG_ONLY(e))
							{
								ERR(e.what());
								hr = E_FAIL;
							}
						}
					}
				}
			}
		}

	}
	
	if (SUCCEEDED(hr))
	{
		//
		// Set the option node name attribute
		//
		CComBSTR bstrAttribName(m_bstrKeywordsPrefix);
		bstrAttribName += QRCODE_OPTIONS[EnableQRCode];
		hr = CreateXMLAttribute(pOptionElem, NAME_ATTRIBUTE_NAME, NULL, bstrAttribName);
	}
	
	ERR_ON_HR(hr);
	return hr;
}

/*++

Routine Name:

CQRCodePTHandler::GetCmnPropTypeAndValue

Routine Description:

This routine initalises the type and value strings for a given common QR Code property

Arguments:

pQRCodeData    - Pointer to a QR Code data structure with the watermark settings
cmnProps   - The QR Code setting to retrieve the type and value for
pbstrType  - Pointer to a BSTR that recieves the type of the value
pbstrValue - Pointer to a BSTR that recieves the value as a string

Return Value:

HRESULT
S_OK - On success
E_*  - On error

--*/
HRESULT
CQRCodePTHandler::GetCmnPropTypeAndValue(
	_In_        CONST QRCodeData*        pQRCodeData,
	_In_        CONST ECommonQRCodeProps cmnProps,
	_Outptr_ BSTR*                       pbstrType,
	_Outptr_ BSTR*                       pbstrValue
)
{
	HRESULT hr = S_OK;

	if (SUCCEEDED(hr = CHECK_POINTER(pQRCodeData, E_POINTER)) &&
		SUCCEEDED(hr = CHECK_POINTER(pbstrType, E_POINTER)) &&
		SUCCEEDED(hr = CHECK_POINTER(pbstrValue, E_POINTER)))
	{
		if (cmnProps < ECommonQRCodePropsMin ||
			cmnProps >= ECommonQRCodePropsMax)
		{
			hr = E_INVALIDARG;
		}
	}

	if (SUCCEEDED(hr))
	{
		try
		{
			CStringXDW cstrType;
			CStringXDW cstrValue;

			switch (cmnProps)
			{
			case QRCode_Scale:
			{
				cstrType = SCHEMA_INTEGER;
				cstrValue.Format(L"%i", pQRCodeData->scale);
			}
			break;

			case QRCode_Offset_X:
			{
				cstrType = SCHEMA_INTEGER;
				cstrValue.Format(L"%i", pQRCodeData->offset_x);
			}
			break;

			case QRCode_Offset_Y:
			{
				cstrType = SCHEMA_INTEGER;
				cstrValue.Format(L"%i", pQRCodeData->offset_y);
			}
			break;

			default:
			{
				hr = E_FAIL;
				ERR("Unknown common QR Code property\n");
			}
			break;
			}

			if (SUCCEEDED(hr))
			{
				*pbstrType = cstrType.AllocSysString();
				*pbstrValue = cstrValue.AllocSysString();
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
