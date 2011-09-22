#include "StdAfx.h"
#include "PKIDtManager.h"
#include "base64util.h"
#define PROVIDER_NAME _T("HaiTai Cryptographic Service Provider")
#define MY_STRING_TYPE (CERT_X500_NAME_STR | CERT_NAME_STR_REVERSE_FLAG)

PKIDtManager::PKIDtManager(void)
{
}

PKIDtManager::~PKIDtManager(void)
{
}

BOOL PKIDtManager::GetContainers(BYTE *btFirstContainer)
{
	BOOL rt = false;
	HCRYPTPROV hCryptProv;
	BYTE pbData[1000];       // 1000 will hold the longest 
                         // key container name.
	DWORD cbData;
	WCHAR crContainer[1000];

	if(CryptAcquireContext(
		&hCryptProv, 
		NULL, 
		PROVIDER_NAME, 
		PROV_RSA_FULL, 
		CRYPT_VERIFYCONTEXT)) 
	{
		//InfoMsgBox(_T("CryptAcquireContext"));
	}
	else 
	{
		DWORD err = GetLastError();
		
		//InfoMsgBox(_T("CryptAcquireContext error"));
		MessageBox(GetForegroundWindow(),_T("CryptAcquireContext error"),_T("���Ź��Ľ���ƽ̨��ʾ"),MB_OK|MB_ICONINFORMATION);
		goto clean;
	}
	
	cbData = 0;
	memset(pbData, 0, sizeof(pbData));
	
	CryptGetProvParam(
		hCryptProv, 
		PP_ENUMCONTAINERS, 
		btFirstContainer, 
		&cbData, 
		CRYPT_FIRST);
	
	memset(pbData, 0, sizeof(pbData));
	if(CryptGetProvParam(
		hCryptProv, 
		PP_ENUMCONTAINERS, 
		btFirstContainer, 
		&cbData, 
		0))
	{
		//memset(crContainer, 0, sizeof(crContainer));
		//MultiByteToWideChar(CP_ACP,0,(char *)btFirstContainer,strlen((char *)btFirstContainer),crContainer,1024); 
		//wsprintf(crContainer, _T("count %s"), pbData);
		//InfoMsgBox(crContainer);
	}
	else
	{
		//InfoMsgBox(_T("CryptGetProvParam error"));
		MessageBox(GetForegroundWindow(),_T("ö��UKey����������ȷ��UKey�Ƿ��������ҹ�������������װ����ȷ����������"),_T("���Ź��Ľ���ƽ̨��ʾ"),MB_OK|MB_ICONINFORMATION);
		goto clean;
	}
	rt = true;
clean:
	if (hCryptProv) CryptReleaseContext(hCryptProv, 0);

	return rt;
}

BOOL PKIDtManager::GetContainers(BYTE *btContainer/*��С�������1000*/, char *pCertName, char *pProvName)
{
	BOOL rt = false;
	HCERTSTORE hCertStore;					//֤��洢�����
	PCCERT_CONTEXT pCertContext = NULL;		//֤����
	TCHAR pszNameString[1000];				//����֤�����Ƶ�����
	PCRYPT_KEY_PROV_INFO pKeyProvInfo=NULL;	//������������ṩ����Ϣ�Ľṹ��ָ��
	DWORD dwLen;
	char ptmp[1000];	
	
	//��MY֤��⣨����֤��洢����
	hCertStore = CertOpenStore(CERT_STORE_PROV_SYSTEM_W, 0, 0,
		CERT_STORE_OPEN_EXISTING_FLAG |
		CERT_SYSTEM_STORE_CURRENT_USER,
		L"MY");
	if(hCertStore == NULL)
	{
		//HandleError("���� CertOpenStore ʧ�ܡ�");
	}
	else
	{
		//printf("��ǰ֤���MY����֤���У�\n\n");
		//ö��֤�����֤��
		while(pCertContext= CertEnumCertificatesInStore(
			hCertStore,
			pCertContext)) 
		{
			//int serlen = pCertContext->pCertInfo->SerialNumber.cbData;
			//char *crOutByteChar = new char[serlen * 2 + 1];
			//ZeroMemory(crOutByteChar, serlen*2+1);
			//crOutByteChar[serlen*2] = '\0';
			//base64util::ToHexString(crOutByteChar, pCertContext->pCertInfo->SerialNumber.pbData, serlen);
			//delete[] crOutByteChar;

			//���֤�������
			CertGetNameString(pCertContext,CERT_NAME_SIMPLE_DISPLAY_TYPE,0,NULL,pszNameString,500);	
			
			WideCharToMultiByte (CP_ACP, 0,pszNameString, -1, ptmp, 1000, NULL,NULL);
			int r = strcmp(ptmp,pCertName);

			if (r != 0)
				continue;
			//��ӡ��Ϣ
			//printf("\n֤�����ƣ�%s\n",pszNameString);
			//���֤��䷢�ߵ����ƣ�����������ΪCERT_NAME_ISSUER_FLAG
			CertGetNameString(pCertContext,CERT_NAME_SIMPLE_DISPLAY_TYPE,CERT_NAME_ISSUER_FLAG,NULL,pszNameString,500);	
			//��ӡ��Ϣ
			//printf("\n֤��䷢�����ƣ�%s\n",pszNameString);
			
			//���֤�������ԣ�CERT_KEY_PROV_INFO_PROP_ID�����֤���Ӧ��������ṩ����Ϣ
			//��һ�ε��û��������ݳ���
			
			if(!CertGetCertificateContextProperty(pCertContext,CERT_KEY_PROV_INFO_PROP_ID,NULL,&dwLen))
			{
				//HandleError("���� CertGetCertificateContextProperty1 ʧ�ܡ�");
				continue;
			}
			//ΪpKeyProvInfo�����ڴ�
			pKeyProvInfo = (PCRYPT_KEY_PROV_INFO)malloc(dwLen);
			//�ڶ��ε��ã��������
			if(!CertGetCertificateContextProperty(pCertContext,CERT_KEY_PROV_INFO_PROP_ID,pKeyProvInfo,&dwLen))
			{
				//HandleError("���� CertGetCertificateContextProperty ʧ�ܡ�");
			}
			else
			{				
				WideCharToMultiByte(CP_ACP,0,pKeyProvInfo->pwszProvName,-1,ptmp,1000,NULL,NULL); 
				int r = strcmp(ptmp,pProvName);
				if (r == 0)
					rt = true;

				//printf("\tCSP����:%s\n",ptmp);
				WideCharToMultiByte(CP_ACP,0,pKeyProvInfo->pwszContainerName,-1,(char*)btContainer,1000,NULL,NULL); 
				
				//printf("\t������:%s\n",ptmp);
				//if(pKeyProvInfo->dwKeySpec == AT_SIGNATURE)
				//	printf("\t��Կ��;��AT_SIGNATURE(ǩ��)\n");
				//else
				//	printf("\t��Կ��;��AT_KEYEXCHANGE(����)\n");
			}
			free(pKeyProvInfo);
			pKeyProvInfo = NULL;
			if (rt)
				break;
		}
	}
	CertCloseStore(hCertStore,0);
	return rt;
}

BOOL PKIDtManager::GetCertInfoProp(WD_CERT_INFO_PROP enProp, void *btCertProp/*��С�������1000 sid ����*/, int btCertPropLen/*����btCertPropLen ����*/, int &propLen/*propLen ����*/, char *pCertName, char *pProvName)
{
	BOOL rt = false;
	HCERTSTORE hCertStore;					//֤��洢�����
	PCCERT_CONTEXT pCertContext = NULL;		//֤����
	TCHAR pszNameString[1000];				//����֤�����Ƶ�����
	PCRYPT_KEY_PROV_INFO pKeyProvInfo=NULL;	//������������ṩ����Ϣ�Ľṹ��ָ��
	DWORD dwLen;
	char ptmp[1000];	
	
	//��MY֤��⣨����֤��洢����
	hCertStore = CertOpenStore(CERT_STORE_PROV_SYSTEM_W, 0, 0,
		CERT_STORE_OPEN_EXISTING_FLAG |
		CERT_SYSTEM_STORE_CURRENT_USER,
		L"MY");
	if(hCertStore == NULL)
	{
		//HandleError("���� CertOpenStore ʧ�ܡ�");
	}
	else
	{
		//printf("��ǰ֤���MY����֤���У�\n\n");
		//ö��֤�����֤��
		while(pCertContext= CertEnumCertificatesInStore(
			hCertStore,
			pCertContext)) 
		{
			

			//���֤�������
			CertGetNameString(pCertContext,CERT_NAME_SIMPLE_DISPLAY_TYPE,0,NULL,pszNameString,500);	
			
			WideCharToMultiByte (CP_ACP, 0,pszNameString, -1, ptmp, 1000, NULL,NULL);
			int r = strcmp(ptmp,pCertName);

			//if (r != 0)
			//	continue;
			//��ӡ��Ϣ
			//printf("\n֤�����ƣ�%s\n",pszNameString);
			//���֤��䷢�ߵ����ƣ�����������ΪCERT_NAME_ISSUER_FLAG
			CertGetNameString(pCertContext,CERT_NAME_SIMPLE_DISPLAY_TYPE,CERT_NAME_ISSUER_FLAG,NULL,pszNameString,500);	
			//��ӡ��Ϣ
			//printf("\n֤��䷢�����ƣ�%s\n",pszNameString);
			
			//���֤�������ԣ�CERT_KEY_PROV_INFO_PROP_ID�����֤���Ӧ��������ṩ����Ϣ
			//��һ�ε��û��������ݳ���
			
			if(!CertGetCertificateContextProperty(pCertContext,CERT_KEY_PROV_INFO_PROP_ID,NULL,&dwLen))
			{
				//HandleError("���� CertGetCertificateContextProperty1 ʧ�ܡ�");
				continue;
			}
			//ΪpKeyProvInfo�����ڴ�
			pKeyProvInfo = (PCRYPT_KEY_PROV_INFO)malloc(dwLen);
			//�ڶ��ε��ã��������
			if(!CertGetCertificateContextProperty(pCertContext,CERT_KEY_PROV_INFO_PROP_ID,pKeyProvInfo,&dwLen))
			{
				//HandleError("���� CertGetCertificateContextProperty ʧ�ܡ�");
			}
			else
			{				
				WideCharToMultiByte(CP_ACP,0,pKeyProvInfo->pwszProvName,-1,ptmp,1000,NULL,NULL); 
				int r = strcmp(ptmp,pProvName);
				if (r == 0)
					rt = true;

				//printf("\tCSP����:%s\n",ptmp);
				//WideCharToMultiByte(CP_ACP,0,pKeyProvInfo->pwszContainerName,-1,(char*)btContainer,1000,NULL,NULL); 
				
				//printf("\t������:%s\n",ptmp);
				//if(pKeyProvInfo->dwKeySpec == AT_SIGNATURE)
				//	printf("\t��Կ��;��AT_SIGNATURE(ǩ��)\n");
				//else
				//	printf("\t��Կ��;��AT_KEYEXCHANGE(����)\n");
			}
			if (rt)
			{
				//int serlen = pCertContext->pCertInfo->SerialNumber.cbData;
				if (enProp == serialNumber)//��֤�����к�
				{
					propLen = pCertContext->pCertInfo->SerialNumber.cbData;
					if (btCertPropLen < propLen)
					{
						rt = false;
					}
					else
					{
						strcpy((char*)btCertProp, (char*)pCertContext->pCertInfo->SerialNumber.pbData);
					}
				}
				else if (enProp == subject)//������
				{
					DWORD cbSize;
					LPTSTR pszString;
					cbSize = CertNameToStr(
						pCertContext->dwCertEncodingType,
						&(pCertContext->pCertInfo->Subject),
						MY_STRING_TYPE,
						NULL,
						0);
					propLen = cbSize;
					if (1 == cbSize || cbSize > btCertPropLen)
					{
						//MyHandleError(TEXT("Subject name is an empty string."));
						rt = false;
					}
					else
					{
						if((pszString = (LPTSTR)malloc(cbSize * sizeof(TCHAR))))
						{
							
							cbSize = CertNameToStr(
								pCertContext->dwCertEncodingType,
								&(pCertContext->pCertInfo->Subject),
								MY_STRING_TYPE,
								pszString,
								cbSize);

							if (1 == cbSize)
							{
								rt = false;
							}
							else
							{
								USES_CONVERSION;
							
								strcpy((char*)btCertProp, (char*)T2A(pszString));
							}
							free(pszString);
						}
					}
					
				}
			}
			//char *crOutByteChar = new char[serlen * 2 + 1];
			//ZeroMemory(crOutByteChar, serlen*2+1);
			//crOutByteChar[serlen*2] = '\0';
			//base64util::ToHexString(crOutByteChar, pCertContext->pCertInfo->SerialNumber.pbData, serlen);
			//delete[] crOutByteChar;

			free(pKeyProvInfo);
			pKeyProvInfo = NULL;
			if (rt)
				break;
		}
	}
	CertCloseStore(hCertStore,0);
	return rt;
}