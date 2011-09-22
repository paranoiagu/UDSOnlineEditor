#pragma once


enum WD_CERT_INFO_PROP 
{
	serialNumber = 0,
	subject
};

class PKIDtManager
{
public:
	PKIDtManager(void);
public:
	~PKIDtManager(void);
public:
	BOOL GetContainers(BYTE *btFirstContainer);
	BOOL GetContainers(BYTE *btContainer/*��С�������1000*/, char *pCertName, char *pProvName);
	BOOL GetCertInfoProp(WD_CERT_INFO_PROP enProp, void *btCertProp/*��С�������1000 sid ����*/, int btCertPropLen/*����btSid ����*/, int &propLen/*sid ����*/, char *pCertName, char *pProvName);
};
