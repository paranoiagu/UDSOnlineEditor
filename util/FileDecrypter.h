// FileDecrypter.h : FileDecrypter ������

#pragma once
#include "resource.h"       // ������

#include "util.h"
#include "testp11.h"


#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Windows CE ƽ̨(�粻�ṩ��ȫ DCOM ֧�ֵ� Windows Mobile ƽ̨)���޷���ȷ֧�ֵ��߳� COM ���󡣶��� _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA ��ǿ�� ATL ֧�ִ������߳� COM ����ʵ�ֲ�����ʹ���䵥�߳� COM ����ʵ�֡�rgs �ļ��е��߳�ģ���ѱ�����Ϊ��Free����ԭ���Ǹ�ģ���Ƿ� DCOM Windows CE ƽ̨֧�ֵ�Ψһ�߳�ģ�͡�"
#endif



// FileDecrypter

class ATL_NO_VTABLE FileDecrypter :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<FileDecrypter, &CLSID_FileDecrypter>,
	public IObjectWithSiteImpl<FileDecrypter>,
	public IDispatchImpl<IFileDecrypter, &IID_IFileDecrypter, &LIBID_utilLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public IObjectSafetyImpl<FileDecrypter,INTERFACESAFE_FOR_UNTRUSTED_CALLER |INTERFACESAFE_FOR_UNTRUSTED_DATA>
{
public:
	FileDecrypter()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_FILEDECRYPTER)


BEGIN_COM_MAP(FileDecrypter)
	COM_INTERFACE_ENTRY(IFileDecrypter)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IObjectWithSite)
	COM_INTERFACE_ENTRY(IObjectSafety)
END_COM_MAP()



	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		hSession=0;
		loadDllPkcs11_Lib();
		return S_OK;
	}

	void FinalRelease()
	{
		if (hSession) WT11_LogOutFinalize(hSession);
		freeDllPkcs11_Lib();
	}
private:
	CK_SESSION_HANDLE hSession;
public:
	STDMETHOD(DecryptFile)(BSTR mailer, BSTR out_mailer, BSTR* rtinfo);
private:
	void ErrorMsgBox(DWORD dwErrorCode,TCHAR* szTemplet);	//��ȡ�����루dwErrorCode����Ӧ�Ĵ�����Ϣ��������Ϣ�ã�szTemplet�����ı�ģ�����������ı�ģ�岿�ṩ����ֻ���dwErrorCode��Ӧ������Ϣ����������ʾ�Ի���
	void InfoMsgBox(TCHAR* szTip);	//���szTipΪ��Ϣ���ݵ���ʾ��Ϣ�Ի���
	void loadDllPkcs11_Lib();//ת��p11dll
	void freeDllPkcs11_Lib();//ж��p11dll
	bool GetPinDlg(unsigned char *pin);
	void __DecryptFile(BSTR provider_name, BSTR container_name, BSTR bsPin, BSTR mailer, BSTR out_mailer, BSTR* rtinfo);
	STDMETHOD(NewDecryptFile)(BSTR provider_name, BSTR container_name, BSTR mailer, BSTR out_mailer, BSTR* rtinfo);
	STDMETHOD(PIN_DecryptFile)(BSTR pin, BSTR mailer, BSTR out_mailer, BSTR* rtinfo);
	STDMETHOD(HAND_DecryptFile)(BSTR provider_name, BSTR container_name, BSTR bsPin, BSTR mailer, BSTR out_mailer, BSTR* rtinfo);
	STDMETHOD(P11LogOutFinalize)(void);//�ͷ�P11
	STDMETHOD(FindCertByUser)(BSTR provider_name, BSTR cuser, BSTR* ccontainer);
};

OBJECT_ENTRY_AUTO(__uuidof(FileDecrypter), FileDecrypter)
