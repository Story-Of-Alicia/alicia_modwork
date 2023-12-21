#ifndef GTLx_H
#define GTLx_H

#include "rpc.h"
#include "rpcndr.h"

#ifndef IGTLEvents_FWD_DEFINED
#define IGTLEvents_FWD_DEFINED

//! IGTLEvents
typedef interface _IGTLEvents _IGTLEvents;

#ifndef IGTL_FWD_DEFINED
#define IGTL_FWD_DEFINED

//! IGTL
typedef interface IGTL IGTL;

#endif // IGTL_FWD_DEFINED

#ifndef GTL_FWD_DEFINED
#define GTL_FWD_DEFINED

#ifdef __cplusplus
typedef class GTL GTL;
#else
typedef struct GTL GTL;
#endif 

#endif // GTL_FWD_DEFINED

#ifdef __cplusplus
extern "C" {
#endif 

#ifndef __GTLxLib_LIBRARY_DEFINED__
#define __GTLxLib_LIBRARY_DEFINED__

EXTERN_C const IID LIBID_GTLxLib;

#ifndef ___IGTLEvents_DISPINTERFACE_DEFINED__
#define ___IGTLEvents_DISPINTERFACE_DEFINED__


EXTERN_C const IID DIID__IGTLEvents;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("F943E25C-00F9-408B-9DAA-F4A3EFA9A552")
    _IGTLEvents : public IDispatch
    {
    };
    
#else 

    typedef struct _IGTLEventsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( _IGTLEvents * This,REFIID riid,_COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _IGTLEvents * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _IGTLEvents * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            _IGTLEvents * This,
             UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            _IGTLEvents * This,
             UINT iTInfo,
             LCID lcid,
             ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            _IGTLEvents * This,
            REFIID riid,
             LPOLESTR *rgszNames,
             UINT cNames,
             LCID lcid,
             DISPID *rgDispId);
        
        HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            _IGTLEvents * This,
             
            _In_  DI 
            _In_  REFIID riid,
    
            _In_  LCID lcid,
             
            _In_  WORD wFlags,
             
            _In_  DISPPARAMS *pDispParams,
           
            _Out_opt_  VARIANT *pVarResult,
            
            _Out_opt_  EXCEPINFO *pExcepInfo,
             
            _Out_opt_  UINT *puArgErr);
        
        END_INTERFACE
    } _IGTLEventsVtbl;

    interface _IGTLEvents
    {
        CONST_VTBL struct _IGTLEventsVtbl *lpVtbl;
    };


#ifdef COBJMACROS

#define _IGTLEvents_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define _IGTLEvents_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define _IGTLEvents_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define _IGTLEvents_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define _IGTLEvents_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define _IGTLEvents_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define _IGTLEvents_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif // COBJMACROS
#endif
#endif

#ifndef __IGTL_INTERFACE_DEFINED__
#define __IGTL_INTERFACE_DEFINED__

EXTERN_C const IID IID_IGTL;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("D32A8602-1F90-4453-A746-A81671F03FB5")
    IGTL : public IDispatch
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE Run(SAFEARRAY * arguments) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE RunExt(SAFEARRAY * arguments) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Runs_Variable(SAFEARRAY * arguments) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetInfo(BSTR name, BSTR *info) = 0;
        
        virtual  HRESULT STDMETHODCALLTYPE GetSystemInfo( 
             BSTR name,
             BSTR *info) = 0;
        
        virtual  HRESULT STDMETHODCALLTYPE ProcessCheck( 
             BSTR processName,
             BSTR *result) = 0;
        
    };

#else

    typedef struct IGTLVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IGTL * This, REFIID riid, _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IGTL * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IGTL * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IGTL * This,
             UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IGTL * This,
             UINT iTInfo,
             LCID lcid,
             ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IGTL * This,
             REFIID riid,
             LPOLESTR *rgszNames,
            UINT cNames,
             LCID lcid,
            DISPID *rgDispId);
        
         HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IGTL * This,
            
            _In_  DISPID dispIdMember,
            
            _In_  REFIID riid,
             
            _In_  LCID lcid,
             
            _In_  WORD wFlags,
            
            _In_  DISPPARAMS *pDispParams,
            
            _Out_opt_  VARIANT *pVarResult,
            
            _Out_opt_  EXCEPINFO *pExcepInfo,
             
            _Out_opt_  UINT *puArgErr);
        
         HRESULT ( STDMETHODCALLTYPE *Run )( 
            IGTL * This,
            SAFEARRAY * arguments);
        
         HRESULT ( STDMETHODCALLTYPE *RunExt )( 
            IGTL * This,
            SAFEARRAY * arguments);
        
         HRESULT ( STDMETHODCALLTYPE *Runs_Variable )( 
            IGTL * This,
            SAFEARRAY * arguments);
        
         HRESULT ( STDMETHODCALLTYPE *GetInfo )( 
            IGTL * This,
             BSTR name,
             BSTR *info);
        
         HRESULT ( STDMETHODCALLTYPE *GetSystemInfo )( 
            IGTL * This,
             BSTR name,
            BSTR *info);
        
        HRESULT ( STDMETHODCALLTYPE *ProcessCheck )( 
            IGTL * This,
             BSTR processName,
             BSTR *result);
        
        END_INTERFACE
    } IGTLVtbl;

    interface IGTL
    {
        CONST_VTBL struct IGTLVtbl *lpVtbl;
    };

#ifdef COBJMACROS

#define IGTL_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IGTL_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IGTL_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IGTL_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IGTL_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IGTL_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IGTL_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#define IGTL_Run(This,arguments)	\
    ( (This)->lpVtbl -> Run(This,arguments) ) 

#define IGTL_RunExt(This,arguments)	\
    ( (This)->lpVtbl -> RunExt(This,arguments) ) 

#define IGTL_Runs_Variable(This,arguments)	\
    ( (This)->lpVtbl -> Runs_Variable(This,arguments) ) 

#define IGTL_GetInfo(This,name,info)	\
    ( (This)->lpVtbl -> GetInfo(This,name,info) ) 

#define IGTL_GetSystemInfo(This,name,info)	\
    ( (This)->lpVtbl -> GetSystemInfo(This,name,info) ) 

#define IGTL_ProcessCheck(This,processName,result)	\
    ( (This)->lpVtbl -> ProcessCheck(This,processName,result) ) 

#endif
#endif
#endif 

EXTERN_C const CLSID CLSID_GTL;

#ifdef __cplusplus

class DECLSPEC_UUID("99846979-30FD-453E-A427-99217EA62571")
GTL;
#endif
#endif 

#ifdef __cplusplus
}
#endif

#endif // IGTLEvents_FWD_DEFINED
#endif // GTLx_H


