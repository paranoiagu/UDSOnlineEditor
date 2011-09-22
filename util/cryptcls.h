//�ӽ����ı��Ķ���
#ifndef __MYCRYPT_H__
#define __MYCRYPT_H__
#pragma once

const int IN_BUFFER_SIZE    = 1024;
// OUT_BUFFER_SIZE is 8 bytes larger than IN_BUFFER_SIZE
// When CALG_RC2 algorithm is used, encrypted data 
// will be 8 bytes larger than IN_BUFFER_SIZE
const int OUT_BUFFER_SIZE   = IN_BUFFER_SIZE + 8; // extra padding

//����ԭʼ�ַ�������base64���뷽ʽ�������������Ĭ��Ϊ��11235813
BOOL Encrypt(const TCHAR* szRaw,TCHAR* szOut,int iOutSize,TCHAR* szKey=L"11235813");
//������base64��ʽ����ļӹ��ܵ��ַ��������ԭʼ�ַ�������������Ĭ��Ϊ��11235813
BOOL Decrypt(TCHAR* szRaw,TCHAR* szOut,int iOutSize,TCHAR* szKey=L"11235813");
BOOL GetSystemDriveSerialNumber(TCHAR* szSerialNumber,UINT iSize);
//��utf8ָ����utf-8��ʽ�ַ���ת��Ϊunicode�ַ��������������dst�У�dst�Ĵ�С��dstSizeָ�������dstSizeΪ0���򷵻���Ҫ��dst��С����
BOOL ToUTF16(IN const char * utf8 ,wchar_t * dst,size_t dstSize);
//��utf16ָ����unicode�ַ���ת��Ϊutf8�ַ��������������dst�У�dst�Ĵ�С��dstSizeָ�������dstSizeΪ0���򷵻���Ҫ��dst��С����
BOOL FromUTF16( IN const wchar_t * utf16,char* dst,size_t dstSize);
#endif // __MYCRYPT_H__