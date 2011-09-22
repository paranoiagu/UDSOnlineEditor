// TestSample.h : CTestSample ������

#pragma once
#include "resource.h"       // ������

#include "util.h"


#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Windows CE ƽ̨(�粻�ṩ��ȫ DCOM ֧�ֵ� Windows Mobile ƽ̨)���޷���ȷ֧�ֵ��߳� COM ���󡣶��� _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA ��ǿ�� ATL ֧�ִ������߳� COM ����ʵ�ֲ�����ʹ���䵥�߳� COM ����ʵ�֡�rgs �ļ��е��߳�ģ���ѱ�����Ϊ��Free����ԭ���Ǹ�ģ���Ƿ� DCOM Windows CE ƽ̨֧�ֵ�Ψһ�߳�ģ�͡�"
#endif



// CTestSample

class ATL_NO_VTABLE CTestSample :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CTestSample, &CLSID_TestSample>,
	public IDispatchImpl<ITestSample, &IID_ITestSample, &LIBID_utilLib, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
public:
	CTestSample()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_TESTSAMPLE)


BEGIN_COM_MAP(CTestSample)
	COM_INTERFACE_ENTRY(ITestSample)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()



	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		//InfoMsgBox(_T("construct"));
		num = 0;
		return S_OK;
	}

	void FinalRelease()
	{
		//InfoMsgBox(_T("release"));
	}

public:
	void InfoMsgBox(TCHAR* szTip);	//���szTipΪ��Ϣ���ݵ���ʾ��Ϣ�Ի���

public:
	STDMETHOD(FindCerts)(void);
private:
	int num;
	STDMETHOD(AddNums)(BSTR* rtnum);
	STDMETHOD(FindCertByUser)(BSTR cuser,BSTR* ccontainer);
};

OBJECT_ENTRY_AUTO(__uuidof(TestSample), CTestSample)
