/*++

File Name:

   dllentry.cpp

Abstract:

   Implementation of the qr code filter dllentry points. Dllmain only
   stores the instance handle. DllGetClassObject calls on to a generic
   get class factory template function.

--*/

#include "precomp.h"
#include "debug.h"
#include "globals.h"
#include "clasfact.h"
#include "qrcodeflt.h"
#include "xdexcept.h"

/*++

Routine Name:

    DllMain

Routine Description:

    Entry point to the watermark filter which is called when a new process is started

Arguments:

    hInst   - Handle to the DLL
    wReason - Specifies a flag indicating why the DLL entry-point function is being called

Return Value:

    TRUE

--*/
BOOL WINAPI
DllMain(
    _In_     HINSTANCE hInst,
    _In_     WORD      wReason,
    _In_opt_ LPVOID
    )
{
    switch (wReason)
    {
        case DLL_PROCESS_ATTACH:
        {
            g_hInstance = hInst;
        }
        break;
    }

    return TRUE;
}

/*++

Routine Name:

    DllCanUnloadNow

Routine Description:

    Method which reports whether the DLL is in use to allow the caller to unload
    the DLL safely

Arguments:

    None

Return Value:

    HRESULT
    S_OK    - Dll can unload
    S_FALSE - Dll can't unload

--*/
STDAPI
DllCanUnloadNow()
{
    if (g_cServerLocks == 0)
    {
        return S_OK ;
    }
    else
    {
        return S_FALSE;
    }
}

/*++

Routine Name:

    DllGetClassObject

Routine Description:

    Method to return the current class object

Arguments:

    rclsid - CLSID that will associate the correct data and code
    riid   - Reference to the identifier of the interface that the caller is to use to communicate with the class object
    ppv    - Address of pointer variable that receives the interface pointer requested in riid

Return Value:

    HRESULT
    S_OK                      - On success
    E_*                       - On error
    CLASS_E_CLASSNOTAVAILABLE - On unsupported class

--*/
STDAPI
DllGetClassObject(
    _In_        REFCLSID    rclsid,
    _In_        REFIID      riid,
    _Outptr_ LPVOID FAR* ppv
    )
{
    //
    // 5fb29be8-7326-4593-8228-49fdafec1a42
    //
    CLSID qrcodegenCLSID = {0x5fb29be8, 0x7326, 0x4593, {0x82, 0x28, 0x49, 0xfd, 0xaf, 0xec, 0x1a, 0x42}};

    return GetFilterClassFactory<CQrCodeFilter>(rclsid, riid, qrcodegenCLSID, ppv);
}

