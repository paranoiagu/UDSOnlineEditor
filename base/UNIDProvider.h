// UNIDProvider.h : UNIDProvider ������

#pragma once
#include "resource.h"       // ������

#include "base.h"


#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Windows CE ƽ̨(�粻�ṩ��ȫ DCOM ֧�ֵ� Windows Mobile ƽ̨)���޷���ȷ֧�ֵ��߳� COM ���󡣶��� _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA ��ǿ�� ATL ֧�ִ������߳� COM ����ʵ�ֲ�����ʹ���䵥�߳� COM ����ʵ�֡�rgs �ļ��е��߳�ģ���ѱ�����Ϊ��Free����ԭ���Ǹ�ģ���Ƿ� DCOM Windows CE ƽ̨֧�ֵ�Ψһ�߳�ģ�͡�"
#endif



// UNIDProvider

class ATL_NO_VTABLE UNIDProvider :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<UNIDProvider, &CLSID_UNIDProvider>,
	public IDispatchImpl<IUNIDProvider, &IID_IUNIDProvider, &LIBID_baseLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public IObjectSafetyImpl<UNIDProvider,INTERFACESAFE_FOR_UNTRUSTED_CALLER |INTERFACESAFE_FOR_UNTRUSTED_DATA>
{
public:
	UNIDProvider()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_UNIDPROVIDER)


BEGIN_COM_MAP(UNIDProvider)
	COM_INTERFACE_ENTRY(IUNIDProvider)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IObjectSafety)
END_COM_MAP()



	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease()
	{
	}

public:
	STDMETHOD(ProvideGUID)(BSTR* szGUID);
	STDMETHOD(ProvideUNID)(BSTR* szUNID);
};

OBJECT_ENTRY_AUTO(__uuidof(UNIDProvider), UNIDProvider)
