#include <iostream>
#include <windows.h>
#include <winhttp.h>
#include <comdef.h>
#include <Wbemidl.h>
#include <vector>
#include <sstream>
#include <string>
#include <map>

#pragma comment(lib, "wbemuuid.lib")
#pragma comment(lib, "winhttp.lib")

std::string GetBiosId() {
    std::string biosId = "";
    HRESULT hres;

    // Initialize COM
    hres = CoInitializeEx(0, COINIT_MULTITHREADED);
    if (FAILED(hres)) {
        std::cerr << "Failed to initialize COM library." << std::endl;
        return biosId;
    }

    // Initialize Security
    hres = CoInitializeSecurity(
        NULL,
        -1,
        NULL,
        NULL,
        RPC_C_AUTHN_LEVEL_DEFAULT,
        RPC_C_IMP_LEVEL_IMPERSONATE,
        NULL,
        EOAC_NONE,
        NULL);

    if (FAILED(hres)) {
        std::cerr << "Failed to initialize security." << std::endl;
        CoUninitialize();
        return biosId;
    }

    // Obtain the initial locator to WMI
    IWbemLocator* pLoc = NULL;
    hres = CoCreateInstance(
        CLSID_WbemLocator,
        0,
        CLSCTX_INPROC_SERVER,
        IID_IWbemLocator, (LPVOID*)&pLoc);

    if (FAILED(hres)) {
        std::cerr << "Failed to create IWbemLocator object." << std::endl;
        CoUninitialize();
        return biosId;
    }

    IWbemServices* pSvc = NULL;
    hres = pLoc->ConnectServer(
        _bstr_t(L"ROOT\\CIMV2"),
        NULL,
        NULL,
        0,
        NULL,
        0,
        0,
        &pSvc);

    if (FAILED(hres)) {
        std::cerr << "Could not connect to WMI namespace." << std::endl;
        pLoc->Release();
        CoUninitialize();
        return biosId;
    }

    // Set security levels on the proxy
    hres = CoSetProxyBlanket(
        pSvc,
        RPC_C_AUTHN_WINNT,
        RPC_C_AUTHZ_NONE,
        NULL,
        RPC_C_AUTHN_LEVEL_CALL,
        RPC_C_IMP_LEVEL_IMPERSONATE,
        NULL,
        EOAC_NONE);

    if (FAILED(hres)) {
        std::cerr << "Could not set proxy blanket." << std::endl;
        pSvc->Release();
        pLoc->Release();
        CoUninitialize();
        return biosId;
    }

    // Query BIOS Serial Number
    IEnumWbemClassObject* pEnumerator = NULL;
    hres = pSvc->ExecQuery(
        bstr_t("WQL"),
        bstr_t("SELECT SerialNumber FROM Win32_BIOS"),
        WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
        NULL,
        &pEnumerator);

    if (FAILED(hres)) {
        std::cerr << "Query for BIOS SerialNumber failed." << std::endl;
        pSvc->Release();
        pLoc->Release();
        CoUninitialize();
        return biosId;
    }

    IWbemClassObject* pclsObj = NULL;
    ULONG uReturn = 0;
    while (pEnumerator) {
        HRESULT hr = pEnumerator->Next(WBEM_INFINITE, 1, &pclsObj, &uReturn);

        if (0 == uReturn) {
            break;
        }

        VARIANT vtProp;
        hr = pclsObj->Get(L"SerialNumber", 0, &vtProp, 0, 0);
        biosId = _bstr_t(vtProp.bstrVal);
        VariantClear(&vtProp);

        pclsObj->Release();
    }

    // Cleanup
    pSvc->Release();
    pLoc->Release();
    pEnumerator->Release();
    CoUninitialize();

    return biosId;
}

void CheckBios()
{
    HINTERNET hSession = WinHttpOpen(L"Check", WINHTTP_ACCESS_TYPE_DEFAULT_PROXY, WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, 0);
    if (hSession)
    {
        HINTERNET hConnect = WinHttpConnect(hSession, L"pastebin.com", INTERNET_DEFAULT_HTTPS_PORT, 0);
        if (hConnect)
        {
            HINTERNET hRequest = WinHttpOpenRequest(hConnect, L"GET", L"/raw/mArn6HFb", NULL, WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES, WINHTTP_FLAG_SECURE);
            if (hRequest)
            {
                if (WinHttpSendRequest(hRequest, WINHTTP_NO_ADDITIONAL_HEADERS, 0, WINHTTP_NO_REQUEST_DATA, 0, 0, 0))
                {
                    if (WinHttpReceiveResponse(hRequest, NULL))
                    {
                        DWORD dwSize = 0;
                        DWORD dwDownloaded = 0;
                        std::string response;
                        do {
                            dwSize = 0;
                            WinHttpQueryDataAvailable(hRequest, &dwSize);
                            if (dwSize > 0)
                            {
                                char* pszOutBuffer = new char[dwSize + 1];
                                if (pszOutBuffer)
                                {
                                    if (WinHttpReadData(hRequest, (LPVOID)pszOutBuffer, dwSize, &dwDownloaded))
                                    {
                                        pszOutBuffer[dwDownloaded] = '\0';
                                        response += pszOutBuffer;
                                    }
                                    delete[] pszOutBuffer;
                                }
                            }
                        } while (dwSize > 0);


                        std::string biosSerial = GetBiosId();


                        if (response.find(biosSerial) != std::string::npos)
                        {
                            //MessageBoxA(NULL, "Login Success", NULL, NULL);

                        }
                        else
                        {
                            //SelfDestruct();
                            MessageBoxA(NULL, "Panel Error !!!", NULL, NULL);
                            exit(0);
                        }
                    }
                }
                WinHttpCloseHandle(hRequest);
            }

            WinHttpCloseHandle(hConnect);
        }
        WinHttpCloseHandle(hSession);
    }
}
