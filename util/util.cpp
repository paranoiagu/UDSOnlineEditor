// util.cpp : DLL ������ʵ�֡�


#include "stdafx.h"
#include "resource.h"
#include "util.h"


class CutilModule : public CAtlDllModuleT< CutilModule >
{
public :
	DECLARE_LIBID(LIBID_utilLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_UTIL, "{7BC2AACB-B1D8-4E22-85B9-2CF73A5CDF1F}")
};

CutilModule _AtlModule;


#ifdef _MANAGED
#pragma managed(push, off)
#endif

// DLL ��ڵ�
extern "C" BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	hInstance;
    return _AtlModule.DllMain(dwReason, lpReserved); 
}

#ifdef _MANAGED
#pragma managed(pop)
#endif




// ����ȷ�� DLL �Ƿ���� OLE ж��
STDAPI DllCanUnloadNow(void)
{
    return _AtlModule.DllCanUnloadNow();
}


// ����һ���๤���Դ������������͵Ķ���
STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv)
{
    return _AtlModule.DllGetClassObject(rclsid, riid, ppv);
}


// DllRegisterServer - ������ӵ�ϵͳע���
STDAPI DllRegisterServer(void)
{
    // ע��������Ϳ�����Ϳ��е����нӿ�
    HRESULT hr = _AtlModule.DllRegisterServer();
	return hr;
}


// DllUnregisterServer - �����ϵͳע������Ƴ�
STDAPI DllUnregisterServer(void)
{
	HRESULT hr = _AtlModule.DllUnregisterServer();
	return hr;
}

