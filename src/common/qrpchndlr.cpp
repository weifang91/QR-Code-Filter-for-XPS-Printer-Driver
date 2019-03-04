/*++

File Name:

qrcodepchndlr.cpp

Abstract:

Page QR Code PrintCapabilities handling implementation. The QR Code PC handler
is used to set Page QRCode settings in a PrintCapabilities.

--*/

#include "precomp.h"
#include "debug.h"
#include "globals.h"
#include "xdexcept.h"
#include "xdexcept.h"
#include "qrpchndlr.h"
#include "privatedefs.h"

using XDPrintSchema::PRINTCAPABILITIES_NAME;

using XDPrintSchema::PageQRCode::ECommonQRCodeProps;
using XDPrintSchema::PageQRCode::ECommonQRCodePropsMin;
using XDPrintSchema::PageQRCode::ECommonQRCodePropsMax;
using XDPrintSchema::PageQRCode::QRCODE_FEATURE;
using XDPrintSchema::PageQRCode::CMN_QRCODE_PROPS;
using XDPrintSchema::PageQRCode::QRCODE_OPTIONS;
using XDPrintSchema::PageQRCode::EQRCodeOption;
using XDPrintSchema::PageQRCode::DisableQRCode;
using XDPrintSchema::PageQRCode::EnableQRCode;


/*++

Routine Name:

CQRCodePCHandler::CQRCodePCHandler

Routine Description:

CQRCodePCHandler class constructor

Arguments:

pPrintCapabilities - Pointer to the DOM document representation of the PrintCapabilities

Return Value:

None

--*/
CQRCodePCHandler::CQRCodePCHandler(
	_In_ IXMLDOMDocument2* pPrintCapabilities
) :
	CPCHandler(pPrintCapabilities)
{
}

/*++

Routine Name:

CQRCodePCHandler::~CQRCodePCHandler

Routine Description:

CQRCodePCHandler class destructor

Arguments:

None

Return Value:

None

--*/
CQRCodePCHandler::~CQRCodePCHandler()
{
}

/*++

Routine Name:

CQRCodePCHandler::SetCapabilities

Routine Description:

This routine sets QR Code capabilities in the PrintCapabilities passed to the
class constructor.

Arguments:

None

Return Value:

HRESULT
S_OK - On success
E_*  - On error

--*/
HRESULT
CQRCodePCHandler::SetCapabilities(
	VOID
)
{
	HRESULT hr = S_OK;

	try
	{
		//
		// Retrieve the PrintTicket root
		//
		CComPtr<IXMLDOMNode> pPTRoot(NULL);

		CComBSTR bstrPTQuery(m_bstrFrameworkPrefix);
		bstrPTQuery += PRINTCAPABILITIES_NAME;

		if (SUCCEEDED(hr = GetNode(bstrPTQuery, &pPTRoot)))
		{
			CComPtr<IXMLDOMElement> pFeatureElement(NULL);

			if (SUCCEEDED(hr = CreateFeatureSelection(CComBSTR(QRCODE_FEATURE), NULL, &pFeatureElement)))
			{
				CComPtr<IXMLDOMElement> pQRCodeOption(NULL);

				//
				// Create the QR Code Options
				//
				if (SUCCEEDED(hr = CreateOption(CComBSTR(QRCODE_OPTIONS[EnableQRCode]), NULL, &pQRCodeOption)))
				{
					PTDOMElementVector propertList;

					//
					// Create the common scored property list
					//
					for (ECommonQRCodeProps cmnProps = ECommonQRCodePropsMin;
						cmnProps < ECommonQRCodePropsMax && SUCCEEDED(hr);
						cmnProps = static_cast<ECommonQRCodeProps>(cmnProps + 1))
					{
						CComPtr<IXMLDOMElement> pScoredProperty(NULL);

						if (SUCCEEDED(hr = CreateScoredProperty(CComBSTR(CMN_QRCODE_PROPS[cmnProps]), &pScoredProperty)))
						{
							CComPtr<IXMLDOMElement> pParamRef(NULL);

							//
							// Construct the parameter reference elements
							//
							CComBSTR bstrPRefName(QRCODE_FEATURE);
							bstrPRefName += CMN_QRCODE_PROPS[cmnProps];

							if (SUCCEEDED(hr) &&
								SUCCEEDED(hr = CreateParameterRef(bstrPRefName, &pParamRef)))
							{
								hr = pScoredProperty->appendChild(pParamRef, NULL);
							}

							propertList.push_back(pScoredProperty);
						}
					}
					/*
					// Can be used for QR Code's TITLE
					// Create the text specific scored property list
					//
					for (ETextWatermarkProps txtProps = ETextWatermarkPropsMin;
						txtProps < ETextWatermarkPropsMax && SUCCEEDED(hr);
						txtProps = static_cast<ETextWatermarkProps>(txtProps + 1))
					{
						CComPtr<IXMLDOMElement> pScoredProperty(NULL);

						if (SUCCEEDED(hr = CreateScoredProperty(CComBSTR(TXT_WATERMARK_PROPS[txtProps]), &pScoredProperty)))
						{
							CComPtr<IXMLDOMElement> pParamRef(NULL);

							//
							// Construct the parameter reference elements
							//
							CComBSTR bstrPRefName(WATERMARK_FEATURE);
							bstrPRefName += TXT_WATERMARK_PROPS[txtProps];

							if (SUCCEEDED(hr = CreateParameterRef(bstrPRefName, &pParamRef)))
							{
								hr = pScoredProperty->appendChild(pParamRef, NULL);
							}

							propertList.push_back(pScoredProperty);
						}
					}
					*/

					//
					// Add the properties to the text option
					//
					PTDOMElementVector::iterator iterPropertList = propertList.begin();

					for (; iterPropertList != propertList.end() && SUCCEEDED(hr); iterPropertList++)
					{
						hr = pQRCodeOption->appendChild(*iterPropertList, NULL);
					}

					if (SUCCEEDED(hr))
					{
						hr = pFeatureElement->appendChild(pQRCodeOption, NULL);
					}
				}

				if (SUCCEEDED(hr))
				{
					hr = pPTRoot->appendChild(pFeatureElement, NULL);
				}
			}

			//
			// Create the integer parameter defs
			//
			for (UINT cIndex = 0; SUCCEEDED(hr) && cIndex < numof(qrcodeParamDefIntegers); cIndex++)
			{
				CComPtr<IXMLDOMElement> pParameterDef(NULL);

				//
				// PageWatermarkTextColor has no associated multiple value
				//
				INT multiple;
				
				multiple = qrcodeParamDefIntegers[cIndex].multiple;

				hr = CreateIntParameterDef(CComBSTR(qrcodeParamDefIntegers[cIndex].property_name),  // Paramater Name
										   qrcodeParamDefIntegers[cIndex].is_public,                // Is Print Schema keyword?
										   CComBSTR(qrcodeParamDefIntegers[cIndex].display_name),   // Display Text
										   qrcodeParamDefIntegers[cIndex].default_value,            // Default
										   qrcodeParamDefIntegers[cIndex].min_length,               // Min value
										   qrcodeParamDefIntegers[cIndex].max_length,               // Max value
										   multiple,                                            // Multiple
										   CComBSTR(qrcodeParamDefIntegers[cIndex].unit_type),      // Unit Type
										   &pParameterDef);                                     // Parameter Def

				if (SUCCEEDED(hr))
				{
					hr = pPTRoot->appendChild(pParameterDef, NULL);
				}
			}

			//
			// Create the string parameter defs (previous is integer)
			//
			for (UINT cIndex = 0; SUCCEEDED(hr) && cIndex < numof(qrcodeParamDefStrings); cIndex++)
			{
				CComPtr<IXMLDOMElement> pParameterDef(NULL);

				if (SUCCEEDED(hr = CreateStringParameterDef(CComBSTR(qrcodeParamDefStrings[cIndex].property_name),  // Paramater Name
															TRUE,                                               // Is Print Schema keyword?
															CComBSTR(qrcodeParamDefStrings[cIndex].display_name),   // Display Text
															CComBSTR(qrcodeParamDefStrings[cIndex].default_value),  // Default
															qrcodeParamDefStrings[cIndex].min_length,               // Min Length
															qrcodeParamDefStrings[cIndex].max_length,               // Max Length
															CComBSTR(qrcodeParamDefStrings[cIndex].unit_type),      // Unit Type
															&pParameterDef)))                                   // Parameter Def
				{
					hr = pPTRoot->appendChild(pParameterDef, NULL);
				}
			}
		}
	}
	catch (exception& DBG_ONLY(e))
	{
		ERR(e.what());
		hr = E_FAIL;
	}

	ERR_ON_HR(hr);
	return hr;
}

