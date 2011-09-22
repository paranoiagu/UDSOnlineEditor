// FileDecrypter.cpp : FileDecrypter ��ʵ��

#include "stdafx.h"
#include "FileDecrypter.h"
#include <wincrypt.h>
#include "cryptcls.h"
#include "comutil.h"
#include "P11PinInputDlg.h"
#include <atlstr.h>
#include "PKIDtManager.h"

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

// FileDecrypter

STDMETHODIMP FileDecrypter::DecryptFile(BSTR mailer, BSTR out_mailer, BSTR* rtinfo)
{
	__DecryptFile(PROVIDER_NAME, L"dtcertcontainer", L"1111", mailer, out_mailer, rtinfo);
	
	return S_OK;
}

void FileDecrypter::ErrorMsgBox(DWORD dwErrorCode,TCHAR* szTemplet){
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

void FileDecrypter::InfoMsgBox(TCHAR* szTip){
	MessageBox(GetForegroundWindow(),szTip,_T("���Ź��Ľ���ƽ̨��ʾ"),MB_OK|MB_ICONINFORMATION);
}
void FileDecrypter::loadDllPkcs11_Lib()
{
	Load_Pkcs11_Lib( "HtPkcs11.dll" );
}
void FileDecrypter::freeDllPkcs11_Lib() 
{
	Free_Pkcs11_Lib();
}
STDMETHODIMP FileDecrypter::NewDecryptFile(BSTR provider_name, BSTR container_name, BSTR mailer, BSTR out_mailer, BSTR* rtinfo)
{
	__DecryptFile(provider_name, container_name, L"1111", mailer, out_mailer, rtinfo);
	return S_OK;
}

void FileDecrypter::__DecryptFile(BSTR provider_name, BSTR container_name, BSTR bsPin, BSTR mailer, BSTR out_mailer, BSTR* rtinfo)
{
	// TODO: �ڴ����ʵ�ִ���
	if (!mailer || _tcslen(mailer)==0) {
		InfoMsgBox(_T("�Բ����������ṩ�����ܵ��ļ�����"));
		*rtinfo = _T("");
		return ;
	}

	unsigned int myDefDWORDLong = 8;
	//CK_SESSION_HANDLE hSession=0;
	unsigned long hSymKey = 0;
	char* outfile = _com_util::ConvertBSTRToString(out_mailer);
	FILE *hSource=0; 
	FILE *hDestination=0; 
	HCRYPTPROV hCryptProv = 0;
	HCRYPTHASH hHash = 0; 
	BYTE *pbKeyBlob = NULL; 
//	unsigned char cDWORDBuffer[8];//���Ȼ���
	DWORDLONG  dwKeyBlobLen = 0;
	HCRYPTKEY hKey = NULL;
	BYTE *pbBuffer=NULL;
	BYTE *pbDeBuffer = NULL;
	int bRead = 0;
	FILE *fSpeedLog = 0;
	unsigned char * pin = (unsigned char*) _com_util::ConvertBSTRToString(bsPin);
	unsigned char * pcontainer = (unsigned char*) _com_util::ConvertBSTRToString(container_name);

	char *pmailer = _com_util::ConvertBSTRToString(mailer);
	hSource = fopen(pmailer,"rb");
	if(hSource == NULL)
	{
		InfoMsgBox(_T("�޷�����������ļ���"));
		*rtinfo = _T("");
		goto clean;
	}
	hDestination = fopen(outfile,"wb");
	if(hDestination == NULL)
	{
		InfoMsgBox(_T("�޷��������Դ�ļ���"));
		*rtinfo = _T("");
		goto clean;
	}
	/*
	fSpeedLog = fopen("c:\\temp\\speedTime.log", "w+b");
	if (fSpeedLog == NULL)
	{
		InfoMsgBox(_T("�޷���ʱ����־�ļ���"));
		*rtinfo = _T("");
		goto clean;
	}
	*/
	bRead = fread(&dwKeyBlobLen,sizeof(DWORDLONG),1,hSource);//��ȡ���ĻỰ��Կ�ĳ���
	if (bRead == 0)
	{
		InfoMsgBox(_T("���ĻỰ��Կ�ĳ���ʧ��1��"));
		*rtinfo = _T("");
		goto clean;
	}

	if((dwKeyBlobLen<=0)||(dwKeyBlobLen >256))
	{
		InfoMsgBox(_T("���ĻỰ��Կ�ĳ���ʧ�ܣ�"));
		*rtinfo = _T("");
		goto clean;
	}
	if(!(pbKeyBlob = (BYTE *)malloc(dwKeyBlobLen)))
	{
		InfoMsgBox(_T("�޷�������Կ���ڴ棡"));
		goto clean;
	}
	//BYTE b;
	//bRead = fread(&b,1,1,hSource);
	bRead = fread(pbKeyBlob,1,dwKeyBlobLen,hSource);//��ȡ���ĻỰ��Կ
	if (bRead == 0)
	{
		InfoMsgBox(_T("��ȡ���ĻỰ��Կʧ�ܣ�"));
		*rtinfo = _T("");
		goto clean;
	}
	/*
	FILE *streamTmpKey;
	if( (streamTmpKey = fopen( "E:\\work\\yirong\\dtproject\\testfile\\fread.out", "w+t" )) != NULL )
	{
		//for (int i = 0; i < dwKeyBlobLen; i++ )
		//{
      /* Write 25 characters to stream */
	//		int numwritten = fwrite( pbKeyBlob, 1, dwKeyBlobLen, streamTmpKey );
	//		printf( "Wrote %d items\n", numwritten );
		//}
	/*	fclose( streamTmpKey );

	}
	else
		printf( "Problem opening the file\n" );
	*/

	//��ȡ16�ֽ������
	char cRandomBytes[16];
	bRead = fread(&cRandomBytes, sizeof(char), 16, hSource);
	
	//��ȡԭʼ�ļ���С
	DWORDLONG dwSrcFileSize;
	bRead = fread(&dwSrcFileSize, sizeof(DWORDLONG), 1, hSource);
	if (bRead == 0)
	{
		InfoMsgBox(_T("��ȡ�ļ���Сʧ�ܣ�"));
		*rtinfo = _T("");
		goto clean;
	}
	//��ȡ������ֽ�
	DWORDLONG dwBBit;
	bRead = fread(&dwBBit, sizeof(DWORDLONG), 1, hSource);
	if (bRead == 0)
	{
		InfoMsgBox(_T("��ȡ�������ʧ�ܣ�"));
		*rtinfo = _T("");
		goto clean;
	}


	TCHAR str[100];
	memset(str, 0, sizeof(str));	

	//��ȡ��չ�ֽ�1024
	char cbExtend[1024];
	fread(&cbExtend, 1, 1024, hSource);
	unsigned char wrappedKey[ 256 ];
	unsigned long wrappedKeyLen = sizeof(wrappedKey);

	if (hSession == 0)
	{
		//InfoMsgBox(_T("login"));
		/*
		unsigned char n_pin[64];
		memset(n_pin, 0, sizeof(n_pin));	
		
		bool dlg_rt = GetPinDlg(n_pin);
		if (! dlg_rt)
		{
			*rtinfo = _T("");
			goto clean;
		}*/
		unsigned char n_pin[] = {"popuppin\0"};
		hSession = WT11_LoginInit(n_pin, pcontainer);
	}
	else
	{
		//InfoMsgBox(_T("init"));
		//WT11_Initialize();
	}

	if (hSession == 0)
	{
		InfoMsgBox(_T("��ʼ��USBKeyʧ�ܣ�"));
		*rtinfo = _T("");
		goto clean;
	}

	SYSTEMTIME st;

	GetSystemTime(&st);

	wsprintf(str, _T("begin: hour %d, min %d, sec %d, milliseconds %d \n\a"), st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
	
	//fwrite( str, 1, 100, fSpeedLog );

	hSymKey = WT11_SessionKeyRsa(hSession, (unsigned char *) pbKeyBlob, dwKeyBlobLen);
	if (hSymKey == 0)
	{
		InfoMsgBox(_T("USBKey����ʧ�ܣ�ȷ��USBKey�Ƿ�����PIN�����Ƿ���ȷkey��"));
		*rtinfo = _T("");
		if (hSession) WT11_LogOutFinalize(hSession);
		hSession = 0;
		goto clean;
	}
	
	DWORD dwCount;
	//DWORDLONG dwBlockLen = 1000 - 1000 % ENCRYPT_BLOCK_SIZE; 
	DWORDLONG dwBlockLen = 1024*1024*100;//1000 - 1000 % ENCRYPT_BLOCK_SIZE; 
	DWORDLONG dwBufferLen = dwBlockLen; 
	
	pbBuffer = (BYTE *)malloc(dwBufferLen);
	pbDeBuffer = (BYTE *)malloc(dwBufferLen);
	DWORD delen = 0;
	DWORDLONG dwReadFileNum = 0;
	int rt_decrypt = 0;
	// ����ԭ�ģ�ѭ����ȡԭ�Ĳ�����
	do { 
	 	// ������
		dwCount = fread(
			pbBuffer, 
			1, 
			dwBlockLen, 
			hSource); 
		if(ferror(hSource))
		{
			InfoMsgBox(_T("��ȡԭ��ʧ�ܣ�"));
			*rtinfo = _T("");
			goto clean;
		}

		rt_decrypt = WT11_SCB2_Decrypt(hSession, hSymKey, (unsigned char*) pbBuffer, dwCount, pbDeBuffer, &delen);
		
		if (rt_decrypt != 0)
		{
			InfoMsgBox(_T("����ʧ�ܣ�ȷ��USBKey�Ƿ�����PIN�����Ƿ���ȷde��"));
			*rtinfo = _T("");
			if (hSession) WT11_LogOutFinalize(hSession);
			hSession = 0;
			goto clean;
		}

		dwReadFileNum += dwCount;
		if (dwSrcFileSize < dwReadFileNum)
		{
			//InfoMsgBox(_T("111"));
			// д�������ݵ��ļ�
			fwrite(
				pbDeBuffer, 
				1, 
				dwCount - dwBBit, 
				hDestination); 
		} else
		{

			int writeLen = fwrite(
				pbDeBuffer, 
				1, 
				dwCount, 
				hDestination);
			   
		}
		if(ferror(hDestination))
		{
			InfoMsgBox(_T("д�������ݵ��ļ�ʧ�ܣ�"));
			*rtinfo = _T("");
			goto clean;
		}
	} 
	while(!feof(hSource)); 
	
	GetSystemTime(&st);

	wsprintf(str, _T(" end: hour %d, min %d, sec %d, milliseconds %d \n\a"), st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
	
	//fwrite( str, 1, 100, fSpeedLog );

	*rtinfo = _T("right");

	clean:
		if (hSource) fclose(hSource); 
		if (hDestination) fclose(hDestination); 
		if (hKey) CryptDestroyKey(hKey); 
		if (hCryptProv) CryptReleaseContext(hCryptProv, 0);
		if (pbBuffer) free(pbBuffer);
		if (pbDeBuffer) free(pbDeBuffer);
		if(pbKeyBlob) free(pbKeyBlob);
		if (hSymKey) WT11_DestroyObject(hSession, hSymKey);
		//if (hSession) WT11_LogOutFinalize(hSession);
		if (fSpeedLog) fclose(fSpeedLog);
		if (pmailer) delete(pmailer);
		if (outfile) delete(outfile);
		if (pin) delete(pin);
		if (pcontainer) delete(pcontainer);
}


STDMETHODIMP FileDecrypter::PIN_DecryptFile(BSTR pin, BSTR mailer, BSTR out_mailer, BSTR* rtinfo)
{
	// TODO: �ڴ����ʵ�ִ���
	__DecryptFile(PROVIDER_NAME, L"dtcertcontainer", pin, mailer, out_mailer, rtinfo);
	return S_OK;
}

STDMETHODIMP FileDecrypter::HAND_DecryptFile(BSTR provider_name, BSTR container_name, BSTR bsPin, BSTR mailer, BSTR out_mailer, BSTR* rtinfo)
{
	// TODO: �ڴ����ʵ�ִ���
	__DecryptFile(provider_name, container_name, bsPin, mailer, out_mailer, rtinfo);
	return S_OK;
}

STDMETHODIMP FileDecrypter::P11LogOutFinalize(void)
{
	// TODO: �ڴ����ʵ�ִ���
	if (hSession) WT11_LogOutFinalize(hSession);
	return S_OK;
}

bool FileDecrypter::GetPinDlg(unsigned char *pin)
{
	bool rt = false;
	CP11PinInputDlg pinDlg;
	if (pinDlg.DoModal() == IDOK)
	{

		CString spin;
		//BSTR pin;
		//pinDlg.GetDlgControl();
		//pinDlg.GetDlgItem(IDC_EDIT_PIN);
		//CT2A(pin.AllocSysString());
		//InfoMsgBox(pin.AllocSysString());
		spin.Append(pinDlg.m_sz);		
		//unsigned char *t_pin = T2CA(pinDlg.m_sz);
		//LPCWSTR pszW;
		BSTR bstrText = spin.AllocSysString();
		char* pszA = _com_util::ConvertBSTRToString(bstrText); 
		memcpy(pin, pszA, pinDlg.pin_len + 1);
		SysFreeString(bstrText); // �����ͷ� 
		delete(pszA); 
		rt = true;
		//InfoMsgBox(pinDlg.m_sz);
	}
	return rt;
}

STDMETHODIMP FileDecrypter::FindCertByUser(BSTR provider_name, BSTR cuser, BSTR* ccontainer)
{
	// TODO: �ڴ����ʵ�ִ���
	// TODO: �ڴ����ʵ�ִ���
	PKIDtManager pkiDtManager;
	char btContainer[1000];
	char *pCertName = (char*)_com_util::ConvertBSTRToString(cuser);
	char *pProvName = (char*)_com_util::ConvertBSTRToString(provider_name);
	if (pkiDtManager.GetContainers((BYTE*)btContainer, pCertName, pProvName))
	{
		//char containerName[1000];
		//MultiByteToWideChar(CP_ACP, 0, (char*)btContainer, 1000, containerName, 500);
		//InfoMsgBox(containerName);
		*ccontainer = A2BSTR(btContainer);
	}
	delete(pCertName);
	delete(pProvName);
	return S_OK;
}
