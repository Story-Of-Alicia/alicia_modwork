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

#else

#ifndef __IID_DEFINED__
#define __IID_DEFINED__

typedef struct _IID
{
    unsigned long x;
    unsigned short s1;
    unsigned short s2;
    unsigned char  c[8];
} IID;

#endif

#ifndef CLSID_DEFINED
#define CLSID_DEFINED
typedef IID CLSID;
#endif

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        EXTERN_C __declspec(selectany) const type name = {l,w1,w2,{b1,b2,b3,b4,b5,b6,b7,b8}}

#endif

MIDL_DEFINE_GUID(IID, LIBID_GTLxLib,0x4DD97E39,0xAC69,0x47C3,0xBF,0x57,0xE8,0x1A,0x77,0xEE,0x4F,0xA3);


MIDL_DEFINE_GUID(IID, DIID__IGTLEvents,0xF943E25C,0x00F9,0x408B,0x9D,0xAA,0xF4,0xA3,0xEF,0xA9,0xA5,0x52);


MIDL_DEFINE_GUID(IID, IID_IGTL,0xD32A8602,0x1F90,0x4453,0xA7,0x46,0xA8,0x16,0x71,0xF0,0x3F,0xB5);


MIDL_DEFINE_GUID(CLSID, CLSID_GTL,0x99846979,0x30FD,0x453E,0xA4,0x27,0x99,0x21,0x7E,0xA6,0x25,0x71);

#undef MIDL_DEFINE_GUID

#ifdef __cplusplus
}
#endif



