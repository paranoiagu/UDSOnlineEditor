// FileWriter.cpp : FileWriter ��ʵ��

#include "stdafx.h"
#include "FileWriter.h"

#import "..\base\Release\base.dll" rename_namespace("Base") raw_interfaces_only 
using namespace Base;

// FileWriter

STDMETHODIMP FileWriter::CreateFile(BSTR szFileName){
	ATLASSERT(szFileName);
	if (_tcslen(szFileName)<=0) {
		MessageBox(GetForegroundWindow(),_T("û���ṩ��Ч�ļ�����"),_T("���Ź��Ľ���ƽ̨��ʾ"),MB_OK|MB_ICONINFORMATION);
		return S_OK;
	}
	this->m_blClosed=FALSE;
	HRESULT hr=S_OK;
	CComPtr<Base::IShell> shl=NULL;
	BSTR bstrTempPath=NULL;
	CoInitialize(NULL);
	{
		hr=CoCreateInstance(__uuidof(Base::Shell),NULL,CLSCTX_INPROC_SERVER,__uuidof(Base::IShell),(void**)&shl);
		if (FAILED(hr)){
			MessageBox(GetForegroundWindow(),_T("�޷�������ǹ�����������ܿͻ������û�а�װ��ע��������"),_T("���Ź��Ľ���ƽ̨��ʾ"),MB_OK|MB_ICONINFORMATION);
			goto clean;
		}
		hr=shl->GetWorkFolderTemp(&bstrTempPath);
		if (FAILED(hr)){
			MessageBox(GetForegroundWindow(),_T("�޷���ȡ��ʱĿ¼�����ܿͻ������û�а�װ��ע��������"),_T("���Ź��Ľ���ƽ̨��ʾ"),MB_OK|MB_ICONINFORMATION);
			goto clean;
		}
		_stprintf_s(this->m_szFilePath,_T("%s%s"),bstrTempPath,szFileName);
	}
clean:
	CoUninitialize();
	return S_OK;
}

STDMETHODIMP FileWriter::AppendFileContent(BSTR szContent){
	if(!szContent || _tcslen(szContent)==0) return S_OK;
	CComBSTR str(_T(""));
	if(this->m_szContent){
		str.Append(this->m_szContent);
		delete this->m_szContent;
		m_szContent=NULL;
	}
	str.AppendBSTR(szContent);
	size_t newLen=str.Length()+1;
	this->m_szContent=new TCHAR[newLen];
	ZeroMemory(this->m_szContent,newLen*sizeof(TCHAR));
	_tcscpy_s(this->m_szContent,newLen,str.Detach());
	return S_OK;
}

STDMETHODIMP FileWriter::WriteFile(VARIANT_BOOL* blSuccess){
	*blSuccess=FALSE;
	if (this->m_blClosed) {
		MessageBox(GetForegroundWindow(),_T("�ļ������Ѿ��رգ�����д�룡"),_T("���Ź��Ľ���ƽ̨��ʾ"),MB_OK|MB_ICONINFORMATION);
		return S_OK;
	}

	if (_tcslen(this->m_szFilePath)==0){
		MessageBox(GetForegroundWindow(),_T("�ļ���û��ָ����"),_T("���Ź��Ľ���ƽ̨��ʾ"),MB_OK|MB_ICONINFORMATION);
		return S_OK;
	}
	
	//����XMLԤ���������ļ�
	HANDLE hFile=INVALID_HANDLE_VALUE; 
	hFile = ::CreateFile(this->m_szFilePath,GENERIC_WRITE,0,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
	if (hFile == INVALID_HANDLE_VALUE) {
		MessageBox(GetForegroundWindow(),_T("�޷�����Ŀ���ļ���"),_T("���Ź��Ľ���ƽ̨��ʾ"),MB_OK|MB_ICONINFORMATION);
		return S_OK;
	}
	//д���ļ�
	DWORD dwBytesWritten;
	USES_CONVERSION;
	char* szBuffer=T2A(this->m_szContent);
	DWORD dwBytesToWrite=(DWORD)(strlen(szBuffer));
	if (!::WriteFile(hFile, szBuffer, dwBytesToWrite,&dwBytesWritten, NULL)){
		MessageBox(GetForegroundWindow(),_T("�޷�д�����ݵ�Ŀ���ļ���"),_T("���Ź��Ľ���ƽ̨��ʾ"),MB_OK|MB_ICONINFORMATION);
		goto clean;
	}
	*blSuccess=TRUE;
clean:
	CloseHandle(hFile);
	if (this->m_szContent){
		delete this->m_szContent;
		m_szContent=NULL;
	}
	this->m_blClosed=TRUE;
	return S_OK;
}