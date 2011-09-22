// UKEYSignature.h : UKEYSignature ������

#pragma once
#include "resource.h"       // ������

#include "util.h"


#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Windows CE ƽ̨(�粻�ṩ��ȫ DCOM ֧�ֵ� Windows Mobile ƽ̨)���޷���ȷ֧�ֵ��߳� COM ���󡣶��� _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA ��ǿ�� ATL ֧�ִ������߳� COM ����ʵ�ֲ�����ʹ���䵥�߳� COM ����ʵ�֡�rgs �ļ��е��߳�ģ���ѱ�����Ϊ��Free����ԭ���Ǹ�ģ���Ƿ� DCOM Windows CE ƽ̨֧�ֵ�Ψһ�߳�ģ�͡�"
#endif



// UKEYSignature

class ATL_NO_VTABLE UKEYSignature :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<UKEYSignature, &CLSID_UKEYSignature>,
	public IObjectWithSiteImpl<UKEYSignature>,
	public IDispatchImpl<IUKEYSignature, &IID_IUKEYSignature, &LIBID_utilLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public IObjectSafetyImpl<UKEYSignature,INTERFACESAFE_FOR_UNTRUSTED_CALLER |INTERFACESAFE_FOR_UNTRUSTED_DATA>
{
public:
	UKEYSignature()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_UKEYSIGNATURE)


BEGIN_COM_MAP(UKEYSignature)
	COM_INTERFACE_ENTRY(IUKEYSignature)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IObjectWithSite)
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
	STDMETHOD(Sign)(BSTR szMessage,BSTR* szSignature);
	STDMETHOD(VerifySignature)(BSTR szSignature,BSTR szMessage,VARIANT_BOOL* blRet);
	STDMETHOD(getCertXML)(BSTR* szXml);
private:
	void ErrorMsgBox(DWORD dwErrorCode,TCHAR* szTemplet);	//��ȡ�����루dwErrorCode����Ӧ�Ĵ�����Ϣ��������Ϣ�ã�szTemplet�����ı�ģ�����������ı�ģ�岿�ṩ����ֻ���dwErrorCode��Ӧ������Ϣ����������ʾ�Ի���
	void InfoMsgBox(TCHAR* szTip);	//���szTipΪ��Ϣ���ݵ���ʾ��Ϣ�Ի���
	void __Sign(BSTR provider_name, BSTR container_name, BSTR szMessage, BSTR szSignature_dat, BSTR cuser, BSTR* szSignature);
	STDMETHOD(NewSign)(BSTR provider_name, BSTR container_name, BSTR szMessage, BSTR szSignature_dat, BSTR* szSignature);
	STDMETHOD(HAND_Sign)(BSTR provider_name, BSTR container_name, BSTR szMessage, BSTR szSignature_dat, BSTR cuser, BSTR* szSignature);
	STDMETHOD(SignByUser)(BSTR szMessage, BSTR szUser, BSTR* szSignature);
	STDMETHOD(GetCertSId)(BSTR inCertName, BSTR* outCertSid);
	STDMETHOD(GetCertSubject)(BSTR inCertName, BSTR* outSubject);
};

OBJECT_ENTRY_AUTO(__uuidof(UKEYSignature), UKEYSignature)
