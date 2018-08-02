

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 7.00.0500 */
/* at Thu Aug 02 22:25:26 2018
 */
/* Compiler settings for .\DataAnalysisTool.idl:
    Oicf, W1, Zp8, env=Win32 (32b run)
    protocol : dce , ms_ext, c_ext, robust
    error checks: stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
//@@MIDL_FILE_HEADING(  )

#pragma warning( disable: 4049 )  /* more than 64k source lines */


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 475
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__


#ifndef __DataAnalysisTool_h_h__
#define __DataAnalysisTool_h_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IDataAnalysisTool_FWD_DEFINED__
#define __IDataAnalysisTool_FWD_DEFINED__
typedef interface IDataAnalysisTool IDataAnalysisTool;
#endif 	/* __IDataAnalysisTool_FWD_DEFINED__ */


#ifndef __DataAnalysisTool_FWD_DEFINED__
#define __DataAnalysisTool_FWD_DEFINED__

#ifdef __cplusplus
typedef class DataAnalysisTool DataAnalysisTool;
#else
typedef struct DataAnalysisTool DataAnalysisTool;
#endif /* __cplusplus */

#endif 	/* __DataAnalysisTool_FWD_DEFINED__ */


#ifdef __cplusplus
extern "C"{
#endif 



#ifndef __DataAnalysisTool_LIBRARY_DEFINED__
#define __DataAnalysisTool_LIBRARY_DEFINED__

/* library DataAnalysisTool */
/* [version][uuid] */ 


EXTERN_C const IID LIBID_DataAnalysisTool;

#ifndef __IDataAnalysisTool_DISPINTERFACE_DEFINED__
#define __IDataAnalysisTool_DISPINTERFACE_DEFINED__

/* dispinterface IDataAnalysisTool */
/* [uuid] */ 


EXTERN_C const IID DIID_IDataAnalysisTool;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("966CD290-A1A0-4BDF-B684-80D49F934D70")
    IDataAnalysisTool : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct IDataAnalysisToolVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IDataAnalysisTool * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IDataAnalysisTool * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IDataAnalysisTool * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IDataAnalysisTool * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IDataAnalysisTool * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IDataAnalysisTool * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IDataAnalysisTool * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } IDataAnalysisToolVtbl;

    interface IDataAnalysisTool
    {
        CONST_VTBL struct IDataAnalysisToolVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDataAnalysisTool_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IDataAnalysisTool_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IDataAnalysisTool_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IDataAnalysisTool_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IDataAnalysisTool_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IDataAnalysisTool_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IDataAnalysisTool_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* __IDataAnalysisTool_DISPINTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_DataAnalysisTool;

#ifdef __cplusplus

class DECLSPEC_UUID("F4F88504-9F02-484F-94CF-F94B75F1C899")
DataAnalysisTool;
#endif
#endif /* __DataAnalysisTool_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


