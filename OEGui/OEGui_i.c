

/* this ALWAYS GENERATED file contains the IIDs and CLSIDs */

/* link this file in with the server and any clients */


 /* File created by MIDL compiler version 7.00.0500 */
/* at Thu Sep 22 20:15:49 2011
 */
/* Compiler settings for .\OEGui.idl:
    Oicf, W1, Zp8, env=Win32 (32b run)
    protocol : dce , ms_ext, c_ext, robust
    error checks: stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
//@@MIDL_FILE_HEADING(  )

#pragma warning( disable: 4049 )  /* more than 64k source lines */


#ifdef __cplusplus
extern "C"{
#endif 


#include <rpc.h>
#include <rpcndr.h>

#ifdef _MIDL_USE_GUIDDEF_

#ifndef INITGUID
#define INITGUID
#include <guiddef.h>
#undef INITGUID
#else
#include <guiddef.h>
#endif

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        DEFINE_GUID(name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8)

#else // !_MIDL_USE_GUIDDEF_

#ifndef __IID_DEFINED__
#define __IID_DEFINED__

typedef struct _IID
{
    unsigned long x;
    unsigned short s1;
    unsigned short s2;
    unsigned char  c[8];
} IID;

#endif // __IID_DEFINED__

#ifndef CLSID_DEFINED
#define CLSID_DEFINED
typedef IID CLSID;
#endif // CLSID_DEFINED

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        const type name = {l,w1,w2,{b1,b2,b3,b4,b5,b6,b7,b8}}

#endif !_MIDL_USE_GUIDDEF_

MIDL_DEFINE_GUID(IID, IID_IOEGOffice,0xE043E43A,0x3241,0x46D4,0xBB,0xCB,0xF7,0xB3,0x43,0xCF,0xAB,0xFC);


MIDL_DEFINE_GUID(IID, LIBID_OEGuiLib,0x5B6ABA30,0xC26D,0x40C8,0xBB,0xCB,0x71,0x98,0xD8,0xDC,0xF4,0x44);


MIDL_DEFINE_GUID(IID, DIID__IOEGOfficeEvents,0xB6676F8B,0x0FFA,0x4BE8,0xA5,0xC5,0x1A,0xDB,0xB4,0x8C,0xF7,0xB8);


MIDL_DEFINE_GUID(CLSID, CLSID_OEGOffice,0xBDBC281B,0x0180,0x41AF,0xAA,0x9D,0xAF,0x61,0xA2,0xD4,0x6F,0xDC);

#undef MIDL_DEFINE_GUID

#ifdef __cplusplus
}
#endif



