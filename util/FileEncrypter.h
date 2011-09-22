// FileEncrypter.h : FileEncrypter ������

#pragma once
#include "resource.h"       // ������

#include "util.h"


#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Windows CE ƽ̨(�粻�ṩ��ȫ DCOM ֧�ֵ� Windows Mobile ƽ̨)���޷���ȷ֧�ֵ��߳� COM ���󡣶��� _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA ��ǿ�� ATL ֧�ִ������߳� COM ����ʵ�ֲ�����ʹ���䵥�߳� COM ����ʵ�֡�rgs �ļ��е��߳�ģ���ѱ�����Ϊ��Free����ԭ���Ǹ�ģ���Ƿ� DCOM Windows CE ƽ̨֧�ֵ�Ψһ�߳�ģ�͡�"
#endif



// FileEncrypter

class ATL_NO_VTABLE FileEncrypter :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<FileEncrypter, &CLSID_FileEncrypter>,
	public ISupportErrorInfo,
	public IObjectWithSiteImpl<FileEncrypter>,
	public IDispatchImpl<IFileEncrypter, &IID_IFileEncrypter, &LIBID_utilLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public IObjectSafetyImpl<FileEncrypter,INTERFACESAFE_FOR_UNTRUSTED_CALLER |INTERFACESAFE_FOR_UNTRUSTED_DATA>
{
public:
	FileEncrypter()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_FILEENCRYPTER)


BEGIN_COM_MAP(FileEncrypter)
	COM_INTERFACE_ENTRY(IFileEncrypter)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY(IObjectWithSite)
	COM_INTERFACE_ENTRY(IObjectSafety)
END_COM_MAP()

// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);


	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease()
	{
	}

public:
	STDMETHOD(EncryptFile)(BSTR szInFN,BSTR szOutFN,BSTR szPwd);
private:
	void ErrorMsgBox(DWORD dwErrorCode,TCHAR* szTemplet);	//��ȡ�����루dwErrorCode����Ӧ�Ĵ�����Ϣ��������Ϣ�ã�szTemplet�����ı�ģ�����������ı�ģ�岿�ṩ����ֻ���dwErrorCode��Ӧ������Ϣ����������ʾ�Ի���
	void InfoMsgBox(TCHAR* szTip);	//���szTipΪ��Ϣ���ݵ���ʾ��Ϣ�Ի���
};

OBJECT_ENTRY_AUTO(__uuidof(FileEncrypter), FileEncrypter)
