// FileEncrypter.cpp : FileEncrypter ��ʵ��

#include "stdafx.h"
#include "FileEncrypter.h"
#include <wincrypt.h>
#include "cryptcls.h"

#define STRLEN_DEFAULT 1024

#define MY_ENCODING_TYPE  (PKCS_7_ASN_ENCODING | X509_ASN_ENCODING)
#define KEYLENGTH  0x00//0x00800000

#define ALG_SID_SCB2    0xf0
#define CALG_SCB2   (ALG_CLASS_DATA_ENCRYPT|ALG_TYPE_BLOCK|ALG_SID_SCB2)

//#define CALG_SF33	(ALG_CLASS_DATA_ENCRYPT|ALG_TYPE_BLOCK|(ALG_SID_EXAMPLE+1))

//#define CALG_SCB2	(ALG_CLASS_DATA_ENCRYPT|ALG_TYPE_BLOCK|(ALG_SID_EXAMPLE+2))

#define ENCRYPT_ALGORITHM CALG_SCB2
#define ENCRYPT_BLOCK_SIZE 16 

#define PROVIDER_NAME _T("HaiTai Cryptographic Service Provider")
// FileEncrypter

STDMETHODIMP FileEncrypter::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* arr[] = 
	{
		&IID_IFileEncrypter
	};

	for (int i=0; i < sizeof(arr) / sizeof(arr[0]); i++)
	{
		if (InlineIsEqualGUID(*arr[i],riid))
			return S_OK;
	}
	return S_FALSE;
}
STDMETHODIMP FileEncrypter::EncryptFile(BSTR szInFN,BSTR szOutFN,BSTR szPwd){
	if (!szInFN || _tcslen(szInFN)==0) {
		InfoMsgBox(_T("�Բ����������ṩ�����ܵ��ļ�����"));
		return S_OK;
	}
	if (!szOutFN || _tcslen(szOutFN)==0) {
		InfoMsgBox(_T("�Բ����������ṩ���ܺ���ļ�����"));
		return S_OK;
	}
	FILE *hSource=0; 
	FILE *hDestination=0; 

	HCRYPTPROV hCryptProv; 
	HCRYPTKEY hKey; 
	HCRYPTKEY hXchgKey; 
	HCRYPTHASH hHash; 
	
	PBYTE pbKeyBlob; 
	DWORD dwKeyBlobLen; 
	
	PBYTE pbBuffer; 
	DWORD dwBlockLen; 
	DWORD dwBufferLen; 
	DWORD dwCount; 

	char* utf8Pwd=NULL;

	TCHAR msgTip[STRLEN_DEFAULT]={0};
	_tfopen_s(&hSource,szInFN,_T("rb"));
	if(!hSource){
		_stprintf_s(msgTip,_T("�Բ����޷����ļ���%s����"),szInFN);
		InfoMsgBox(msgTip);
		goto clean;
	}
	_tfopen_s(&hDestination,szOutFN,_T("wb"));
	if(!hDestination){
		_stprintf_s(msgTip,_T("�Բ����޷����ļ���%s����"),szOutFN);
		InfoMsgBox(msgTip);
		goto clean;
	}

	TCHAR szContainerName[]=_T("f0d74c20-4a68-47d2-a1d2-7c622399ad48");	//TODO:��������Ϊ��test��?
	//��ʼ��CSP������
	if(CryptAcquireContext(&hCryptProv,szContainerName,PROVIDER_NAME,PROV_RSA_FULL,0))
	{
		//��ȷ��ʼ��CSP
	}
	else
	{
		if(!CryptAcquireContext(&hCryptProv,szContainerName,PROVIDER_NAME,PROV_RSA_FULL,CRYPT_NEWKEYSET)){
			InfoMsgBox(_T("�Բ����޷���ʼ��CSP�����ģ���ȷ��UKey�Ƿ��������ҹ�������������װ����ȷ����������"));
			goto clean;
		}
	}
	//���û���ṩ����
	if(!szPwd || _tcslen(szPwd)==0){
		if(!CryptGenKey(hCryptProv,ENCRYPT_ALGORITHM,CRYPT_EXPORTABLE, /*KEYLENGTH | CRYPT_EXPORTABLE, */&hKey))
		{
			InfoMsgBox(_T("�Բ����޷�������Կ����ȷ��UKey�Ƿ��������ҹ�������������װ����ȷ����������"));
			goto clean;
		}
		if(CryptGetUserKey(hCryptProv,AT_KEYEXCHANGE,&hXchgKey)){
			//��Կ���ɹ���ȡ
		}
		else{
			CryptGenKey(hCryptProv,AT_KEYEXCHANGE,0,&hXchgKey);
		}
		if(!CryptExportKey(hKey,hXchgKey,SIMPLEBLOB, 0,NULL, &dwKeyBlobLen)){
			InfoMsgBox(_T("�Բ����޷���ȡ��Կ�飬��ȷ��UKey�Ƿ��������ҹ�������������װ����ȷ����������"));
			goto clean;
		}
		if(!(pbKeyBlob =(BYTE *)malloc(dwKeyBlobLen)))
		{ 
			InfoMsgBox(_T("�Բ����޷�Ϊ��Կ������ڴ棡"));
			goto clean;
		}
		if(!CryptExportKey(hKey,hXchgKey,SIMPLEBLOB,0, pbKeyBlob, &dwKeyBlobLen)){
			InfoMsgBox(_T("�Բ����޷�Ϊ������Կ��"));
			goto clean;
		}
		CryptDestroyKey(hXchgKey); 
		hXchgKey = 0;
		fwrite(&dwKeyBlobLen, sizeof(DWORD), 1, hDestination); 
		if(ferror(hDestination))
		{
			InfoMsgBox(_T("�޷�д������ļ���"));
			goto clean;
		}
		fwrite(pbKeyBlob, 1, dwKeyBlobLen, hDestination); 
		if(ferror(hDestination))
		{ 
			InfoMsgBox(_T("�޷�д������ļ���"));
			goto clean;
		}
	}
	else{	//�ṩ������
		if(!CryptCreateHash(hCryptProv,CALG_MD5, 0,0,&hHash))
		{
			InfoMsgBox(_T("�޷�������ϣ��"));
			goto clean;
		}
		int keyLen=FromUTF16(szPwd,NULL,0);
		if (keyLen>0) keyLen+=1;
		utf8Pwd=new char[keyLen];
		FromUTF16(szPwd,utf8Pwd,keyLen);
		if(!CryptHashData( hHash, (BYTE *)utf8Pwd, keyLen-1, 0))
		{
			InfoMsgBox(_T("�޷����������ϣֵ��"));
			goto clean;
		}
		if(!CryptDeriveKey( hCryptProv,ENCRYPT_ALGORITHM, hHash, KEYLENGTH, &hKey))
		{
			InfoMsgBox(_T("�޷��ӹ�ϣֵ������Կ��"));
			goto clean;
		}
		CryptDestroyHash(hHash); 
		hHash = 0; 
	}
	dwBlockLen = 1024 - 1024 % ENCRYPT_BLOCK_SIZE; 
	if(ENCRYPT_BLOCK_SIZE > 1) 
		dwBufferLen = dwBlockLen + ENCRYPT_BLOCK_SIZE; 
	else 
		dwBufferLen = dwBlockLen; 

	if(!(pbBuffer = (BYTE *)malloc(dwBufferLen)))
	{
		InfoMsgBox(_T("�Բ����޷�Ϊ�����������ڴ棡"));
		goto clean;
	}
	//ѭ����ȡԴ�ļ�������Ȼ��������ݲ��ѽ��д��Ŀ���ļ�
	do
	{ 
		dwCount = fread(pbBuffer, 1, dwBlockLen, hSource); 
		if(ferror(hSource))
		{ 
			InfoMsgBox(_T("�Բ����޷���ȡ�������ļ����ݣ�"));
			goto clean;
		}
		
		if(!CryptEncrypt(hKey,0,feof(hSource),0,pbBuffer,&dwCount,dwBufferLen))
		{ 
			InfoMsgBox(_T("���ܹ��̳��ִ���"));
			goto clean;
		} 
		
		fwrite(pbBuffer, 1, dwCount, hDestination); 
		if(ferror(hDestination))
		{ 
			InfoMsgBox(_T("�Բ����޷�д����ܺ�����ݵ�Ŀ���ļ���"));
			goto clean;
		}
	} 
	while(!feof(hSource)); 
	InfoMsgBox(_T("���ܳɹ���"));
	//��Դ�ͷ�
clean:
	if(utf8Pwd) delete[] utf8Pwd;
	if(hSource) fclose(hSource); 
	if(hDestination) fclose(hDestination);
	if(pbBuffer) free(pbBuffer); 

	if(hKey) CryptDestroyKey(hKey); 

	if(hXchgKey) CryptDestroyKey(hXchgKey); 

	if(hHash) CryptDestroyHash(hHash); 

	if(hCryptProv) CryptReleaseContext(hCryptProv, 0);
	return S_OK;
}

void FileEncrypter::ErrorMsgBox(DWORD dwErrorCode,TCHAR* szTemplet){
	BOOL hasTemplet=TRUE;

	if (!szTemplet || _tcslen(szTemplet)==0) hasTemplet=FALSE;

	if(dwErrorCode==0 && hasTemplet){
		MessageBox(GetForegroundWindow(),szTemplet,_T("���Ź��Ľ���ƽ̨����"),MB_OK|MB_ICONERROR);
		return ;
	}

	TCHAR* szPos=NULL;
	if (hasTemplet) szPos=_tcsstr(szTemplet,_T("%s"));
	LPVOID lpMsgBuf;
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER |FORMAT_MESSAGE_FROM_SYSTEM,NULL,dwErrorCode,MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),(LPTSTR) &lpMsgBuf,0, NULL );
	TCHAR szTip[STRLEN_DEFAULT]={0};
	_stprintf_s(szTip,(szPos?szTemplet:_T("%s")),(lpMsgBuf==NULL?_T("δ֪����"):lpMsgBuf));
	LocalFree(lpMsgBuf);
	MessageBox(GetForegroundWindow(),szTip,_T("���Ź��Ľ���ƽ̨����"),MB_OK|MB_ICONERROR);
}

void FileEncrypter::InfoMsgBox(TCHAR* szTip){
	MessageBox(GetForegroundWindow(),szTip,_T("���Ź��Ľ���ƽ̨��ʾ"),MB_OK|MB_ICONINFORMATION);
}