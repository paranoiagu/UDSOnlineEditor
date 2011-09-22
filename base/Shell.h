// Shell.h : Shell ������

#pragma once
#include "resource.h"       // ������

#include "base.h"


#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Windows CE ƽ̨(�粻�ṩ��ȫ DCOM ֧�ֵ� Windows Mobile ƽ̨)���޷���ȷ֧�ֵ��߳� COM ���󡣶��� _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA ��ǿ�� ATL ֧�ִ������߳� COM ����ʵ�ֲ�����ʹ���䵥�߳� COM ����ʵ�֡�rgs �ļ��е��߳�ģ���ѱ�����Ϊ��Free����ԭ���Ǹ�ģ���Ƿ� DCOM Windows CE ƽ̨֧�ֵ�Ψһ�߳�ģ�͡�"
#endif



// Shell

class ATL_NO_VTABLE Shell :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<Shell, &CLSID_Shell>,
	public IObjectWithSiteImpl<Shell>,
	public IDispatchImpl<IShell, &IID_IShell, &LIBID_baseLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public IObjectSafetyImpl<Shell,INTERFACESAFE_FOR_UNTRUSTED_CALLER |INTERFACESAFE_FOR_UNTRUSTED_DATA>
{
public:
	Shell(){}

DECLARE_REGISTRY_RESOURCEID(IDR_SHELL)

BEGIN_COM_MAP(Shell)
	COM_INTERFACE_ENTRY(IShell)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IObjectWithSite)
	COM_INTERFACE_ENTRY(IObjectSafety)
END_COM_MAP()

	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease(){}

public:
	STDMETHOD(GetWindowsVersionString)(BSTR* szVer);
	STDMETHOD(GetFileVersionString)(BSTR szFile, BSTR* szVer);
	STDMETHOD(GetWindowsUserName)(BSTR* szUser);
	STDMETHOD(GetSystemFolder)(BSTR* szFolder);
	STDMETHOD(GetPersonalFolder)(BSTR* szFolder);
	STDMETHOD(GetWorkFolder)(BSTR* szFolder);
	STDMETHOD(GetWorkFolderTemp)(BSTR* szFolder);
	STDMETHOD(GetFileLength)(BSTR szFilePath, ULONG* ulFileLength);
	STDMETHOD(GetInstallPath)(BSTR* szPath);
	STDMETHOD(ClearTempWorkFolder)(void);
	STDMETHOD(DeleteTempFile)(BSTR szFileName, VARIANT_BOOL* bSuccess);
	STDMETHOD(FileExists)(BSTR szFilePath, VARIANT_BOOL* bSuccess);
	STDMETHOD(LaunchFile)(BSTR szFilePath,BSTR szVerb);
	STDMETHOD(FindAssocProgram)(BSTR szFilePath,BSTR szVerb,BSTR* szProgram);
	STDMETHOD(CheckFileOpened)(BSTR szFilePath,VARIANT_BOOL* bSuccess);
};

OBJECT_ENTRY_AUTO(__uuidof(Shell), Shell)
