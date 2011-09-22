// Dialog.h : Dialog ������

#pragma once
#include "resource.h"       // ������

#include "util.h"
#include "_IDialogEvents_CP.h"


#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Windows CE ƽ̨(�粻�ṩ��ȫ DCOM ֧�ֵ� Windows Mobile ƽ̨)���޷���ȷ֧�ֵ��߳� COM ���󡣶��� _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA ��ǿ�� ATL ֧�ִ������߳� COM ����ʵ�ֲ�����ʹ���䵥�߳� COM ����ʵ�֡�rgs �ļ��е��߳�ģ���ѱ�����Ϊ��Free����ԭ���Ǹ�ģ���Ƿ� DCOM Windows CE ƽ̨֧�ֵ�Ψһ�߳�ģ�͡�"
#endif



// Dialog

class ATL_NO_VTABLE Dialog :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<Dialog, &CLSID_Dialog>,
	public IConnectionPointContainerImpl<Dialog>,
	public CProxy_IDialogEvents<Dialog>,
	public IObjectWithSiteImpl<Dialog>,
	public IDispatchImpl<IDialog, &IID_IDialog, &LIBID_utilLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public IObjectSafetyImpl<Dialog,INTERFACESAFE_FOR_UNTRUSTED_CALLER |INTERFACESAFE_FOR_UNTRUSTED_DATA>
{
public:
	Dialog()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_DIALOG)


BEGIN_COM_MAP(Dialog)
	COM_INTERFACE_ENTRY(IDialog)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IConnectionPointContainer)
	COM_INTERFACE_ENTRY(IObjectWithSite)
	COM_INTERFACE_ENTRY(IObjectSafety)
END_COM_MAP()

BEGIN_CONNECTION_POINT_MAP(Dialog)
	CONNECTION_POINT_ENTRY(__uuidof(_IDialogEvents))
END_CONNECTION_POINT_MAP()


	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease()
	{
	}

public:
	STDMETHOD(SelectFolder)(BSTR* szFolderResult);
	STDMETHOD(SelectFiles)(BSTR szDefaultFileName,BSTR szTitle,BSTR szFilter,VARIANT_BOOL blAllowMultiSelect,BSTR* szFile);
	STDMETHOD(SaveFile)(BSTR szDefaultFileName,BSTR szTitle,BSTR szFilter,BSTR* szFile);
};

OBJECT_ENTRY_AUTO(__uuidof(Dialog), Dialog)
