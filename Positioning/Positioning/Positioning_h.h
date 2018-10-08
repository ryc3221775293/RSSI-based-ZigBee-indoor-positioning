

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 7.00.0555 */
/* at Mon Oct 08 21:46:55 2018
 */
/* Compiler settings for Positioning.idl:
    Oicf, W1, Zp8, env=Win32 (32b run), target_arch=X86 7.00.0555 
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */

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


#ifndef __Positioning_h_h__
#define __Positioning_h_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IPositioning_FWD_DEFINED__
#define __IPositioning_FWD_DEFINED__
typedef interface IPositioning IPositioning;
#endif 	/* __IPositioning_FWD_DEFINED__ */


#ifndef __Positioning_FWD_DEFINED__
#define __Positioning_FWD_DEFINED__

#ifdef __cplusplus
typedef class Positioning Positioning;
#else
typedef struct Positioning Positioning;
#endif /* __cplusplus */

#endif 	/* __Positioning_FWD_DEFINED__ */


#ifdef __cplusplus
extern "C"{
#endif 



#ifndef __Positioning_LIBRARY_DEFINED__
#define __Positioning_LIBRARY_DEFINED__

/* library Positioning */
/* [version][uuid] */ 


EXTERN_C const IID LIBID_Positioning;

#ifndef __IPositioning_DISPINTERFACE_DEFINED__
#define __IPositioning_DISPINTERFACE_DEFINED__

/* dispinterface IPositioning */
/* [uuid] */ 


EXTERN_C const IID DIID_IPositioning;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("4E815C12-68E2-4CAD-87AC-E323D6996D59")
    IPositioning : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct IPositioningVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IPositioning * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IPositioning * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IPositioning * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IPositioning * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IPositioning * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IPositioning * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IPositioning * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } IPositioningVtbl;

    interface IPositioning
    {
        CONST_VTBL struct IPositioningVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IPositioning_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IPositioning_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IPositioning_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IPositioning_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IPositioning_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IPositioning_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IPositioning_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* __IPositioning_DISPINTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_Positioning;

#ifdef __cplusplus

class DECLSPEC_UUID("C367FA16-669C-4D4C-BA6B-44157B76C329")
Positioning;
#endif
#endif /* __Positioning_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


