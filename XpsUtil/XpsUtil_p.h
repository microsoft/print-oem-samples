

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 8.01.0628 */
/* at Mon Jan 18 19:14:07 2038
 */
/* Compiler settings for d:\os\out\Temp\amd64fre\XpsUtil.idl-5ff571ed:
    Oicf, W1, Zp8, env=Win64 (32b run), target_arch=AMD64 8.01.0628 
    protocol : all , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */



/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 500
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif /* __RPCNDR_H_VERSION__ */

#ifndef COM_NO_WINDOWS_H
#include "windows.h"
#include "ole2.h"
#endif /*COM_NO_WINDOWS_H*/

#ifndef __XpsUtil_p_h__
#define __XpsUtil_p_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef DECLSPEC_XFGVIRT
#if defined(_CONTROL_FLOW_GUARD_XFG)
#define DECLSPEC_XFGVIRT(base, func) __declspec(xfg_virtual(base, func))
#else
#define DECLSPEC_XFGVIRT(base, func)
#endif
#endif

#if defined(__cplusplus)
#if defined(__MIDL_USE_C_ENUM)
#define MIDL_ENUM enum
#else
#define MIDL_ENUM enum class
#endif
#endif


/* Forward Declarations */ 

#ifndef ____x_ABI_CXpsUtil_CIXpsPageWrapper_FWD_DEFINED__
#define ____x_ABI_CXpsUtil_CIXpsPageWrapper_FWD_DEFINED__
typedef interface __x_ABI_CXpsUtil_CIXpsPageWrapper __x_ABI_CXpsUtil_CIXpsPageWrapper;

#endif 	/* ____x_ABI_CXpsUtil_CIXpsPageWrapper_FWD_DEFINED__ */


#ifndef ____x_ABI_CXpsUtil_CIXpsPageWatermarker_FWD_DEFINED__
#define ____x_ABI_CXpsUtil_CIXpsPageWatermarker_FWD_DEFINED__
typedef interface __x_ABI_CXpsUtil_CIXpsPageWatermarker __x_ABI_CXpsUtil_CIXpsPageWatermarker;

#endif 	/* ____x_ABI_CXpsUtil_CIXpsPageWatermarker_FWD_DEFINED__ */


#ifndef ____FITypedEventHandler_2_XpsUtil__CXpsSequentialDocument_UINT32_FWD_DEFINED__
#define ____FITypedEventHandler_2_XpsUtil__CXpsSequentialDocument_UINT32_FWD_DEFINED__
typedef interface __FITypedEventHandler_2_XpsUtil__CXpsSequentialDocument_UINT32 __FITypedEventHandler_2_XpsUtil__CXpsSequentialDocument_UINT32;

#endif 	/* ____FITypedEventHandler_2_XpsUtil__CXpsSequentialDocument_UINT32_FWD_DEFINED__ */


#ifndef ____FITypedEventHandler_2_XpsUtil__CXpsSequentialDocument_UINT64_FWD_DEFINED__
#define ____FITypedEventHandler_2_XpsUtil__CXpsSequentialDocument_UINT64_FWD_DEFINED__
typedef interface __FITypedEventHandler_2_XpsUtil__CXpsSequentialDocument_UINT64 __FITypedEventHandler_2_XpsUtil__CXpsSequentialDocument_UINT64;

#endif 	/* ____FITypedEventHandler_2_XpsUtil__CXpsSequentialDocument_UINT64_FWD_DEFINED__ */


#ifndef ____x_ABI_CXpsUtil_CIXpsSequentialDocument_FWD_DEFINED__
#define ____x_ABI_CXpsUtil_CIXpsSequentialDocument_FWD_DEFINED__
typedef interface __x_ABI_CXpsUtil_CIXpsSequentialDocument __x_ABI_CXpsUtil_CIXpsSequentialDocument;

#endif 	/* ____x_ABI_CXpsUtil_CIXpsSequentialDocument_FWD_DEFINED__ */


#ifndef ____x_ABI_CXpsUtil_CIXpsSequentialDocumentFactory_FWD_DEFINED__
#define ____x_ABI_CXpsUtil_CIXpsSequentialDocumentFactory_FWD_DEFINED__
typedef interface __x_ABI_CXpsUtil_CIXpsSequentialDocumentFactory __x_ABI_CXpsUtil_CIXpsSequentialDocumentFactory;

#endif 	/* ____x_ABI_CXpsUtil_CIXpsSequentialDocumentFactory_FWD_DEFINED__ */


/* header files for imported files */
#include "Windows.Foundation.h"
#include "Windows.Storage.Streams.h"
#include "Windows.Graphics.Printing.Workflow.h"

#ifdef __cplusplus
extern "C"{
#endif 


/* interface __MIDL_itf_XpsUtil_0000_0000 */
/* [local] */ 











extern RPC_IF_HANDLE __MIDL_itf_XpsUtil_0000_0000_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_XpsUtil_0000_0000_v0_0_s_ifspec;

#ifndef ____x_ABI_CXpsUtil_CIXpsPageWrapper_INTERFACE_DEFINED__
#define ____x_ABI_CXpsUtil_CIXpsPageWrapper_INTERFACE_DEFINED__

/* interface __x_ABI_CXpsUtil_CIXpsPageWrapper */
/* [object][uuid] */ 


EXTERN_C const IID IID___x_ABI_CXpsUtil_CIXpsPageWrapper;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("74af329f-367e-5130-a0eb-3a82403cebf9")
    __x_ABI_CXpsUtil_CIXpsPageWrapper : public IInspectable
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE Clone( 
            /* [retval][out] */ __x_ABI_CXpsUtil_CIXpsPageWrapper **result) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE RenderPageToBMP( 
            /* [retval][out] */ __x_ABI_CWindows_CStorage_CStreams_CIRandomAccessStream **result) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct __x_ABI_CXpsUtil_CIXpsPageWrapperVtbl
    {
        BEGIN_INTERFACE
        
        DECLSPEC_XFGVIRT(IUnknown, QueryInterface)
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            __x_ABI_CXpsUtil_CIXpsPageWrapper * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        DECLSPEC_XFGVIRT(IUnknown, AddRef)
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            __x_ABI_CXpsUtil_CIXpsPageWrapper * This);
        
        DECLSPEC_XFGVIRT(IUnknown, Release)
        ULONG ( STDMETHODCALLTYPE *Release )( 
            __x_ABI_CXpsUtil_CIXpsPageWrapper * This);
        
        DECLSPEC_XFGVIRT(IInspectable, GetIids)
        HRESULT ( STDMETHODCALLTYPE *GetIids )( 
            __x_ABI_CXpsUtil_CIXpsPageWrapper * This,
            /* [out] */ ULONG *iidCount,
            /* [size_is][size_is][out] */ IID **iids);
        
        DECLSPEC_XFGVIRT(IInspectable, GetRuntimeClassName)
        HRESULT ( STDMETHODCALLTYPE *GetRuntimeClassName )( 
            __x_ABI_CXpsUtil_CIXpsPageWrapper * This,
            /* [out] */ HSTRING *className);
        
        DECLSPEC_XFGVIRT(IInspectable, GetTrustLevel)
        HRESULT ( STDMETHODCALLTYPE *GetTrustLevel )( 
            __x_ABI_CXpsUtil_CIXpsPageWrapper * This,
            /* [out] */ TrustLevel *trustLevel);
        
        DECLSPEC_XFGVIRT(__x_ABI_CXpsUtil_CIXpsPageWrapper, Clone)
        HRESULT ( STDMETHODCALLTYPE *Clone )( 
            __x_ABI_CXpsUtil_CIXpsPageWrapper * This,
            /* [retval][out] */ __x_ABI_CXpsUtil_CIXpsPageWrapper **result);
        
        DECLSPEC_XFGVIRT(__x_ABI_CXpsUtil_CIXpsPageWrapper, RenderPageToBMP)
        HRESULT ( STDMETHODCALLTYPE *RenderPageToBMP )( 
            __x_ABI_CXpsUtil_CIXpsPageWrapper * This,
            /* [retval][out] */ __x_ABI_CWindows_CStorage_CStreams_CIRandomAccessStream **result);
        
        END_INTERFACE
    } __x_ABI_CXpsUtil_CIXpsPageWrapperVtbl;

    interface __x_ABI_CXpsUtil_CIXpsPageWrapper
    {
        CONST_VTBL struct __x_ABI_CXpsUtil_CIXpsPageWrapperVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define __x_ABI_CXpsUtil_CIXpsPageWrapper_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define __x_ABI_CXpsUtil_CIXpsPageWrapper_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define __x_ABI_CXpsUtil_CIXpsPageWrapper_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define __x_ABI_CXpsUtil_CIXpsPageWrapper_GetIids(This,iidCount,iids)	\
    ( (This)->lpVtbl -> GetIids(This,iidCount,iids) ) 

#define __x_ABI_CXpsUtil_CIXpsPageWrapper_GetRuntimeClassName(This,className)	\
    ( (This)->lpVtbl -> GetRuntimeClassName(This,className) ) 

#define __x_ABI_CXpsUtil_CIXpsPageWrapper_GetTrustLevel(This,trustLevel)	\
    ( (This)->lpVtbl -> GetTrustLevel(This,trustLevel) ) 


#define __x_ABI_CXpsUtil_CIXpsPageWrapper_Clone(This,result)	\
    ( (This)->lpVtbl -> Clone(This,result) ) 

#define __x_ABI_CXpsUtil_CIXpsPageWrapper_RenderPageToBMP(This,result)	\
    ( (This)->lpVtbl -> RenderPageToBMP(This,result) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* ____x_ABI_CXpsUtil_CIXpsPageWrapper_INTERFACE_DEFINED__ */


/* interface __MIDL_itf_XpsUtil_0000_0001 */
/* [local] */ 




extern RPC_IF_HANDLE __MIDL_itf_XpsUtil_0000_0001_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_XpsUtil_0000_0001_v0_0_s_ifspec;

#ifndef ____x_ABI_CXpsUtil_CIXpsPageWatermarker_INTERFACE_DEFINED__
#define ____x_ABI_CXpsUtil_CIXpsPageWatermarker_INTERFACE_DEFINED__

/* interface __x_ABI_CXpsUtil_CIXpsPageWatermarker */
/* [object][uuid] */ 


EXTERN_C const IID IID___x_ABI_CXpsUtil_CIXpsPageWatermarker;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("6af08114-b0f1-5e16-8477-33e76c2d2a1e")
    __x_ABI_CXpsUtil_CIXpsPageWatermarker : public IInspectable
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE SetWatermarkText( 
            /* [in] */ HSTRING text,
            /* [in] */ double fontSize,
            /* [in] */ double xRelativeOffset,
            /* [in] */ double yRelativeOffset) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetWatermarkImage( 
            /* [in] */ HSTRING imagePath,
            /* [in] */ double dpiX,
            /* [in] */ double dpiY,
            /* [in] */ int width,
            /* [in] */ int height) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetWatermarkImageEnabled( 
            /* [in] */ boolean enabled) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE ApplyWatermarks( 
            /* [in] */ __x_ABI_CXpsUtil_CIXpsPageWrapper *page) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct __x_ABI_CXpsUtil_CIXpsPageWatermarkerVtbl
    {
        BEGIN_INTERFACE
        
        DECLSPEC_XFGVIRT(IUnknown, QueryInterface)
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            __x_ABI_CXpsUtil_CIXpsPageWatermarker * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        DECLSPEC_XFGVIRT(IUnknown, AddRef)
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            __x_ABI_CXpsUtil_CIXpsPageWatermarker * This);
        
        DECLSPEC_XFGVIRT(IUnknown, Release)
        ULONG ( STDMETHODCALLTYPE *Release )( 
            __x_ABI_CXpsUtil_CIXpsPageWatermarker * This);
        
        DECLSPEC_XFGVIRT(IInspectable, GetIids)
        HRESULT ( STDMETHODCALLTYPE *GetIids )( 
            __x_ABI_CXpsUtil_CIXpsPageWatermarker * This,
            /* [out] */ ULONG *iidCount,
            /* [size_is][size_is][out] */ IID **iids);
        
        DECLSPEC_XFGVIRT(IInspectable, GetRuntimeClassName)
        HRESULT ( STDMETHODCALLTYPE *GetRuntimeClassName )( 
            __x_ABI_CXpsUtil_CIXpsPageWatermarker * This,
            /* [out] */ HSTRING *className);
        
        DECLSPEC_XFGVIRT(IInspectable, GetTrustLevel)
        HRESULT ( STDMETHODCALLTYPE *GetTrustLevel )( 
            __x_ABI_CXpsUtil_CIXpsPageWatermarker * This,
            /* [out] */ TrustLevel *trustLevel);
        
        DECLSPEC_XFGVIRT(__x_ABI_CXpsUtil_CIXpsPageWatermarker, SetWatermarkText)
        HRESULT ( STDMETHODCALLTYPE *SetWatermarkText )( 
            __x_ABI_CXpsUtil_CIXpsPageWatermarker * This,
            /* [in] */ HSTRING text,
            /* [in] */ double fontSize,
            /* [in] */ double xRelativeOffset,
            /* [in] */ double yRelativeOffset);
        
        DECLSPEC_XFGVIRT(__x_ABI_CXpsUtil_CIXpsPageWatermarker, SetWatermarkImage)
        HRESULT ( STDMETHODCALLTYPE *SetWatermarkImage )( 
            __x_ABI_CXpsUtil_CIXpsPageWatermarker * This,
            /* [in] */ HSTRING imagePath,
            /* [in] */ double dpiX,
            /* [in] */ double dpiY,
            /* [in] */ int width,
            /* [in] */ int height);
        
        DECLSPEC_XFGVIRT(__x_ABI_CXpsUtil_CIXpsPageWatermarker, SetWatermarkImageEnabled)
        HRESULT ( STDMETHODCALLTYPE *SetWatermarkImageEnabled )( 
            __x_ABI_CXpsUtil_CIXpsPageWatermarker * This,
            /* [in] */ boolean enabled);
        
        DECLSPEC_XFGVIRT(__x_ABI_CXpsUtil_CIXpsPageWatermarker, ApplyWatermarks)
        HRESULT ( STDMETHODCALLTYPE *ApplyWatermarks )( 
            __x_ABI_CXpsUtil_CIXpsPageWatermarker * This,
            /* [in] */ __x_ABI_CXpsUtil_CIXpsPageWrapper *page);
        
        END_INTERFACE
    } __x_ABI_CXpsUtil_CIXpsPageWatermarkerVtbl;

    interface __x_ABI_CXpsUtil_CIXpsPageWatermarker
    {
        CONST_VTBL struct __x_ABI_CXpsUtil_CIXpsPageWatermarkerVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define __x_ABI_CXpsUtil_CIXpsPageWatermarker_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define __x_ABI_CXpsUtil_CIXpsPageWatermarker_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define __x_ABI_CXpsUtil_CIXpsPageWatermarker_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define __x_ABI_CXpsUtil_CIXpsPageWatermarker_GetIids(This,iidCount,iids)	\
    ( (This)->lpVtbl -> GetIids(This,iidCount,iids) ) 

#define __x_ABI_CXpsUtil_CIXpsPageWatermarker_GetRuntimeClassName(This,className)	\
    ( (This)->lpVtbl -> GetRuntimeClassName(This,className) ) 

#define __x_ABI_CXpsUtil_CIXpsPageWatermarker_GetTrustLevel(This,trustLevel)	\
    ( (This)->lpVtbl -> GetTrustLevel(This,trustLevel) ) 


#define __x_ABI_CXpsUtil_CIXpsPageWatermarker_SetWatermarkText(This,text,fontSize,xRelativeOffset,yRelativeOffset)	\
    ( (This)->lpVtbl -> SetWatermarkText(This,text,fontSize,xRelativeOffset,yRelativeOffset) ) 

#define __x_ABI_CXpsUtil_CIXpsPageWatermarker_SetWatermarkImage(This,imagePath,dpiX,dpiY,width,height)	\
    ( (This)->lpVtbl -> SetWatermarkImage(This,imagePath,dpiX,dpiY,width,height) ) 

#define __x_ABI_CXpsUtil_CIXpsPageWatermarker_SetWatermarkImageEnabled(This,enabled)	\
    ( (This)->lpVtbl -> SetWatermarkImageEnabled(This,enabled) ) 

#define __x_ABI_CXpsUtil_CIXpsPageWatermarker_ApplyWatermarks(This,page)	\
    ( (This)->lpVtbl -> ApplyWatermarks(This,page) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* ____x_ABI_CXpsUtil_CIXpsPageWatermarker_INTERFACE_DEFINED__ */


/* interface __MIDL_itf_XpsUtil2Eidl_0000_2475 */




extern RPC_IF_HANDLE __MIDL_itf_XpsUtil2Eidl_0000_2475_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_XpsUtil2Eidl_0000_2475_v0_0_s_ifspec;

/* interface __MIDL_itf_XpsUtil_0000_0003 */
/* [local] */ 

#ifndef DEF___FITypedEventHandler_2_XpsUtil__CXpsSequentialDocument_UINT32
#define DEF___FITypedEventHandler_2_XpsUtil__CXpsSequentialDocument_UINT32
#if !defined(__cplusplus) || defined(RO_NO_TEMPLATE_NAME)


extern RPC_IF_HANDLE __MIDL_itf_XpsUtil_0000_0003_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_XpsUtil_0000_0003_v0_0_s_ifspec;

#ifndef ____FITypedEventHandler_2_XpsUtil__CXpsSequentialDocument_UINT32_INTERFACE_DEFINED__
#define ____FITypedEventHandler_2_XpsUtil__CXpsSequentialDocument_UINT32_INTERFACE_DEFINED__

/* interface __FITypedEventHandler_2_XpsUtil__CXpsSequentialDocument_UINT32 */
/* [unique][uuid][object] */ 


EXTERN_C const IID IID___FITypedEventHandler_2_XpsUtil__CXpsSequentialDocument_UINT32;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("e02a02ec-d02e-5c71-a9f5-d7962e82cf22")
    __FITypedEventHandler_2_XpsUtil__CXpsSequentialDocument_UINT32 : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE Invoke( 
            /* [in] */ __x_ABI_CXpsUtil_CIXpsSequentialDocument *sender,
            /* [in] */ UINT32 e) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct __FITypedEventHandler_2_XpsUtil__CXpsSequentialDocument_UINT32Vtbl
    {
        BEGIN_INTERFACE
        
        DECLSPEC_XFGVIRT(IUnknown, QueryInterface)
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            __FITypedEventHandler_2_XpsUtil__CXpsSequentialDocument_UINT32 * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        DECLSPEC_XFGVIRT(IUnknown, AddRef)
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            __FITypedEventHandler_2_XpsUtil__CXpsSequentialDocument_UINT32 * This);
        
        DECLSPEC_XFGVIRT(IUnknown, Release)
        ULONG ( STDMETHODCALLTYPE *Release )( 
            __FITypedEventHandler_2_XpsUtil__CXpsSequentialDocument_UINT32 * This);
        
        DECLSPEC_XFGVIRT(__FITypedEventHandler_2_XpsUtil__CXpsSequentialDocument_UINT32, Invoke)
        HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            __FITypedEventHandler_2_XpsUtil__CXpsSequentialDocument_UINT32 * This,
            /* [in] */ __x_ABI_CXpsUtil_CIXpsSequentialDocument *sender,
            /* [in] */ UINT32 e);
        
        END_INTERFACE
    } __FITypedEventHandler_2_XpsUtil__CXpsSequentialDocument_UINT32Vtbl;

    interface __FITypedEventHandler_2_XpsUtil__CXpsSequentialDocument_UINT32
    {
        CONST_VTBL struct __FITypedEventHandler_2_XpsUtil__CXpsSequentialDocument_UINT32Vtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define __FITypedEventHandler_2_XpsUtil__CXpsSequentialDocument_UINT32_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define __FITypedEventHandler_2_XpsUtil__CXpsSequentialDocument_UINT32_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define __FITypedEventHandler_2_XpsUtil__CXpsSequentialDocument_UINT32_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define __FITypedEventHandler_2_XpsUtil__CXpsSequentialDocument_UINT32_Invoke(This,sender,e)	\
    ( (This)->lpVtbl -> Invoke(This,sender,e) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* ____FITypedEventHandler_2_XpsUtil__CXpsSequentialDocument_UINT32_INTERFACE_DEFINED__ */


/* interface __MIDL_itf_XpsUtil_0000_0004 */
/* [local] */ 

#endif /* pinterface */
#endif /* DEF___FITypedEventHandler_2_XpsUtil__CXpsSequentialDocument_UINT32 */


extern RPC_IF_HANDLE __MIDL_itf_XpsUtil_0000_0004_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_XpsUtil_0000_0004_v0_0_s_ifspec;

/* interface __MIDL_itf_XpsUtil2Eidl_0000_2476 */




extern RPC_IF_HANDLE __MIDL_itf_XpsUtil2Eidl_0000_2476_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_XpsUtil2Eidl_0000_2476_v0_0_s_ifspec;

/* interface __MIDL_itf_XpsUtil_0000_0005 */
/* [local] */ 

#ifndef DEF___FITypedEventHandler_2_XpsUtil__CXpsSequentialDocument_UINT64
#define DEF___FITypedEventHandler_2_XpsUtil__CXpsSequentialDocument_UINT64
#if !defined(__cplusplus) || defined(RO_NO_TEMPLATE_NAME)


extern RPC_IF_HANDLE __MIDL_itf_XpsUtil_0000_0005_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_XpsUtil_0000_0005_v0_0_s_ifspec;

#ifndef ____FITypedEventHandler_2_XpsUtil__CXpsSequentialDocument_UINT64_INTERFACE_DEFINED__
#define ____FITypedEventHandler_2_XpsUtil__CXpsSequentialDocument_UINT64_INTERFACE_DEFINED__

/* interface __FITypedEventHandler_2_XpsUtil__CXpsSequentialDocument_UINT64 */
/* [unique][uuid][object] */ 


EXTERN_C const IID IID___FITypedEventHandler_2_XpsUtil__CXpsSequentialDocument_UINT64;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("6ddf6c64-caa5-5489-bb1e-17cd09b09199")
    __FITypedEventHandler_2_XpsUtil__CXpsSequentialDocument_UINT64 : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE Invoke( 
            /* [in] */ __x_ABI_CXpsUtil_CIXpsSequentialDocument *sender,
            /* [in] */ UINT64 e) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct __FITypedEventHandler_2_XpsUtil__CXpsSequentialDocument_UINT64Vtbl
    {
        BEGIN_INTERFACE
        
        DECLSPEC_XFGVIRT(IUnknown, QueryInterface)
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            __FITypedEventHandler_2_XpsUtil__CXpsSequentialDocument_UINT64 * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        DECLSPEC_XFGVIRT(IUnknown, AddRef)
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            __FITypedEventHandler_2_XpsUtil__CXpsSequentialDocument_UINT64 * This);
        
        DECLSPEC_XFGVIRT(IUnknown, Release)
        ULONG ( STDMETHODCALLTYPE *Release )( 
            __FITypedEventHandler_2_XpsUtil__CXpsSequentialDocument_UINT64 * This);
        
        DECLSPEC_XFGVIRT(__FITypedEventHandler_2_XpsUtil__CXpsSequentialDocument_UINT64, Invoke)
        HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            __FITypedEventHandler_2_XpsUtil__CXpsSequentialDocument_UINT64 * This,
            /* [in] */ __x_ABI_CXpsUtil_CIXpsSequentialDocument *sender,
            /* [in] */ UINT64 e);
        
        END_INTERFACE
    } __FITypedEventHandler_2_XpsUtil__CXpsSequentialDocument_UINT64Vtbl;

    interface __FITypedEventHandler_2_XpsUtil__CXpsSequentialDocument_UINT64
    {
        CONST_VTBL struct __FITypedEventHandler_2_XpsUtil__CXpsSequentialDocument_UINT64Vtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define __FITypedEventHandler_2_XpsUtil__CXpsSequentialDocument_UINT64_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define __FITypedEventHandler_2_XpsUtil__CXpsSequentialDocument_UINT64_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define __FITypedEventHandler_2_XpsUtil__CXpsSequentialDocument_UINT64_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define __FITypedEventHandler_2_XpsUtil__CXpsSequentialDocument_UINT64_Invoke(This,sender,e)	\
    ( (This)->lpVtbl -> Invoke(This,sender,e) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* ____FITypedEventHandler_2_XpsUtil__CXpsSequentialDocument_UINT64_INTERFACE_DEFINED__ */


/* interface __MIDL_itf_XpsUtil_0000_0006 */
/* [local] */ 

#endif /* pinterface */
#endif /* DEF___FITypedEventHandler_2_XpsUtil__CXpsSequentialDocument_UINT64 */


extern RPC_IF_HANDLE __MIDL_itf_XpsUtil_0000_0006_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_XpsUtil_0000_0006_v0_0_s_ifspec;

#ifndef ____x_ABI_CXpsUtil_CIXpsSequentialDocument_INTERFACE_DEFINED__
#define ____x_ABI_CXpsUtil_CIXpsSequentialDocument_INTERFACE_DEFINED__

/* interface __x_ABI_CXpsUtil_CIXpsSequentialDocument */
/* [object][uuid] */ 


EXTERN_C const IID IID___x_ABI_CXpsUtil_CIXpsSequentialDocument;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("3dbdceab-938b-50aa-a51b-876f0f25cd22")
    __x_ABI_CXpsUtil_CIXpsSequentialDocument : public IInspectable
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE StartXpsOMGeneration( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetWatermarkedStream( 
            /* [in] */ __x_ABI_CXpsUtil_CIXpsPageWatermarker *watermarker,
            /* [retval][out] */ __x_ABI_CWindows_CStorage_CStreams_CIInputStream **result) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetPage( 
            /* [in] */ unsigned int pageNumber,
            /* [retval][out] */ __x_ABI_CXpsUtil_CIXpsPageWrapper **result) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE PageCount( 
            /* [retval][out] */ unsigned int *result) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE add_PageAdded( 
            /* [in] */ __FITypedEventHandler_2_XpsUtil__CXpsSequentialDocument_UINT32 *handler,
            /* [out][retval] */ EventRegistrationToken *token) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE remove_PageAdded( 
            /* [in] */ EventRegistrationToken token) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE add_DocumentClosed( 
            /* [in] */ __FITypedEventHandler_2_XpsUtil__CXpsSequentialDocument_UINT32 *handler,
            /* [out][retval] */ EventRegistrationToken *token) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE remove_DocumentClosed( 
            /* [in] */ EventRegistrationToken token) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE add_XpsGenerationFailed( 
            /* [in] */ __FITypedEventHandler_2_XpsUtil__CXpsSequentialDocument_UINT64 *handler,
            /* [out][retval] */ EventRegistrationToken *token) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE remove_XpsGenerationFailed( 
            /* [in] */ EventRegistrationToken token) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct __x_ABI_CXpsUtil_CIXpsSequentialDocumentVtbl
    {
        BEGIN_INTERFACE
        
        DECLSPEC_XFGVIRT(IUnknown, QueryInterface)
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            __x_ABI_CXpsUtil_CIXpsSequentialDocument * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        DECLSPEC_XFGVIRT(IUnknown, AddRef)
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            __x_ABI_CXpsUtil_CIXpsSequentialDocument * This);
        
        DECLSPEC_XFGVIRT(IUnknown, Release)
        ULONG ( STDMETHODCALLTYPE *Release )( 
            __x_ABI_CXpsUtil_CIXpsSequentialDocument * This);
        
        DECLSPEC_XFGVIRT(IInspectable, GetIids)
        HRESULT ( STDMETHODCALLTYPE *GetIids )( 
            __x_ABI_CXpsUtil_CIXpsSequentialDocument * This,
            /* [out] */ ULONG *iidCount,
            /* [size_is][size_is][out] */ IID **iids);
        
        DECLSPEC_XFGVIRT(IInspectable, GetRuntimeClassName)
        HRESULT ( STDMETHODCALLTYPE *GetRuntimeClassName )( 
            __x_ABI_CXpsUtil_CIXpsSequentialDocument * This,
            /* [out] */ HSTRING *className);
        
        DECLSPEC_XFGVIRT(IInspectable, GetTrustLevel)
        HRESULT ( STDMETHODCALLTYPE *GetTrustLevel )( 
            __x_ABI_CXpsUtil_CIXpsSequentialDocument * This,
            /* [out] */ TrustLevel *trustLevel);
        
        DECLSPEC_XFGVIRT(__x_ABI_CXpsUtil_CIXpsSequentialDocument, StartXpsOMGeneration)
        HRESULT ( STDMETHODCALLTYPE *StartXpsOMGeneration )( 
            __x_ABI_CXpsUtil_CIXpsSequentialDocument * This);
        
        DECLSPEC_XFGVIRT(__x_ABI_CXpsUtil_CIXpsSequentialDocument, GetWatermarkedStream)
        HRESULT ( STDMETHODCALLTYPE *GetWatermarkedStream )( 
            __x_ABI_CXpsUtil_CIXpsSequentialDocument * This,
            /* [in] */ __x_ABI_CXpsUtil_CIXpsPageWatermarker *watermarker,
            /* [retval][out] */ __x_ABI_CWindows_CStorage_CStreams_CIInputStream **result);
        
        DECLSPEC_XFGVIRT(__x_ABI_CXpsUtil_CIXpsSequentialDocument, GetPage)
        HRESULT ( STDMETHODCALLTYPE *GetPage )( 
            __x_ABI_CXpsUtil_CIXpsSequentialDocument * This,
            /* [in] */ unsigned int pageNumber,
            /* [retval][out] */ __x_ABI_CXpsUtil_CIXpsPageWrapper **result);
        
        DECLSPEC_XFGVIRT(__x_ABI_CXpsUtil_CIXpsSequentialDocument, PageCount)
        HRESULT ( STDMETHODCALLTYPE *PageCount )( 
            __x_ABI_CXpsUtil_CIXpsSequentialDocument * This,
            /* [retval][out] */ unsigned int *result);
        
        DECLSPEC_XFGVIRT(__x_ABI_CXpsUtil_CIXpsSequentialDocument, add_PageAdded)
        HRESULT ( STDMETHODCALLTYPE *add_PageAdded )( 
            __x_ABI_CXpsUtil_CIXpsSequentialDocument * This,
            /* [in] */ __FITypedEventHandler_2_XpsUtil__CXpsSequentialDocument_UINT32 *handler,
            /* [out][retval] */ EventRegistrationToken *token);
        
        DECLSPEC_XFGVIRT(__x_ABI_CXpsUtil_CIXpsSequentialDocument, remove_PageAdded)
        HRESULT ( STDMETHODCALLTYPE *remove_PageAdded )( 
            __x_ABI_CXpsUtil_CIXpsSequentialDocument * This,
            /* [in] */ EventRegistrationToken token);
        
        DECLSPEC_XFGVIRT(__x_ABI_CXpsUtil_CIXpsSequentialDocument, add_DocumentClosed)
        HRESULT ( STDMETHODCALLTYPE *add_DocumentClosed )( 
            __x_ABI_CXpsUtil_CIXpsSequentialDocument * This,
            /* [in] */ __FITypedEventHandler_2_XpsUtil__CXpsSequentialDocument_UINT32 *handler,
            /* [out][retval] */ EventRegistrationToken *token);
        
        DECLSPEC_XFGVIRT(__x_ABI_CXpsUtil_CIXpsSequentialDocument, remove_DocumentClosed)
        HRESULT ( STDMETHODCALLTYPE *remove_DocumentClosed )( 
            __x_ABI_CXpsUtil_CIXpsSequentialDocument * This,
            /* [in] */ EventRegistrationToken token);
        
        DECLSPEC_XFGVIRT(__x_ABI_CXpsUtil_CIXpsSequentialDocument, add_XpsGenerationFailed)
        HRESULT ( STDMETHODCALLTYPE *add_XpsGenerationFailed )( 
            __x_ABI_CXpsUtil_CIXpsSequentialDocument * This,
            /* [in] */ __FITypedEventHandler_2_XpsUtil__CXpsSequentialDocument_UINT64 *handler,
            /* [out][retval] */ EventRegistrationToken *token);
        
        DECLSPEC_XFGVIRT(__x_ABI_CXpsUtil_CIXpsSequentialDocument, remove_XpsGenerationFailed)
        HRESULT ( STDMETHODCALLTYPE *remove_XpsGenerationFailed )( 
            __x_ABI_CXpsUtil_CIXpsSequentialDocument * This,
            /* [in] */ EventRegistrationToken token);
        
        END_INTERFACE
    } __x_ABI_CXpsUtil_CIXpsSequentialDocumentVtbl;

    interface __x_ABI_CXpsUtil_CIXpsSequentialDocument
    {
        CONST_VTBL struct __x_ABI_CXpsUtil_CIXpsSequentialDocumentVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define __x_ABI_CXpsUtil_CIXpsSequentialDocument_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define __x_ABI_CXpsUtil_CIXpsSequentialDocument_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define __x_ABI_CXpsUtil_CIXpsSequentialDocument_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define __x_ABI_CXpsUtil_CIXpsSequentialDocument_GetIids(This,iidCount,iids)	\
    ( (This)->lpVtbl -> GetIids(This,iidCount,iids) ) 

#define __x_ABI_CXpsUtil_CIXpsSequentialDocument_GetRuntimeClassName(This,className)	\
    ( (This)->lpVtbl -> GetRuntimeClassName(This,className) ) 

#define __x_ABI_CXpsUtil_CIXpsSequentialDocument_GetTrustLevel(This,trustLevel)	\
    ( (This)->lpVtbl -> GetTrustLevel(This,trustLevel) ) 


#define __x_ABI_CXpsUtil_CIXpsSequentialDocument_StartXpsOMGeneration(This)	\
    ( (This)->lpVtbl -> StartXpsOMGeneration(This) ) 

#define __x_ABI_CXpsUtil_CIXpsSequentialDocument_GetWatermarkedStream(This,watermarker,result)	\
    ( (This)->lpVtbl -> GetWatermarkedStream(This,watermarker,result) ) 

#define __x_ABI_CXpsUtil_CIXpsSequentialDocument_GetPage(This,pageNumber,result)	\
    ( (This)->lpVtbl -> GetPage(This,pageNumber,result) ) 

#define __x_ABI_CXpsUtil_CIXpsSequentialDocument_PageCount(This,result)	\
    ( (This)->lpVtbl -> PageCount(This,result) ) 

#define __x_ABI_CXpsUtil_CIXpsSequentialDocument_add_PageAdded(This,handler,token)	\
    ( (This)->lpVtbl -> add_PageAdded(This,handler,token) ) 

#define __x_ABI_CXpsUtil_CIXpsSequentialDocument_remove_PageAdded(This,token)	\
    ( (This)->lpVtbl -> remove_PageAdded(This,token) ) 

#define __x_ABI_CXpsUtil_CIXpsSequentialDocument_add_DocumentClosed(This,handler,token)	\
    ( (This)->lpVtbl -> add_DocumentClosed(This,handler,token) ) 

#define __x_ABI_CXpsUtil_CIXpsSequentialDocument_remove_DocumentClosed(This,token)	\
    ( (This)->lpVtbl -> remove_DocumentClosed(This,token) ) 

#define __x_ABI_CXpsUtil_CIXpsSequentialDocument_add_XpsGenerationFailed(This,handler,token)	\
    ( (This)->lpVtbl -> add_XpsGenerationFailed(This,handler,token) ) 

#define __x_ABI_CXpsUtil_CIXpsSequentialDocument_remove_XpsGenerationFailed(This,token)	\
    ( (This)->lpVtbl -> remove_XpsGenerationFailed(This,token) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* ____x_ABI_CXpsUtil_CIXpsSequentialDocument_INTERFACE_DEFINED__ */


#ifndef ____x_ABI_CXpsUtil_CIXpsSequentialDocumentFactory_INTERFACE_DEFINED__
#define ____x_ABI_CXpsUtil_CIXpsSequentialDocumentFactory_INTERFACE_DEFINED__

/* interface __x_ABI_CXpsUtil_CIXpsSequentialDocumentFactory */
/* [object][uuid] */ 


EXTERN_C const IID IID___x_ABI_CXpsUtil_CIXpsSequentialDocumentFactory;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("b2b480dd-91de-5ab7-be85-e11783c5236d")
    __x_ABI_CXpsUtil_CIXpsSequentialDocumentFactory : public IInspectable
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE CreateInstance( 
            /* [in] */ __x_ABI_CWindows_CGraphics_CPrinting_CWorkflow_CIPrintWorkflowObjectModelSourceFileContent *sourceFileContent,
            /* [out][retval] */ __x_ABI_CXpsUtil_CIXpsSequentialDocument **value) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct __x_ABI_CXpsUtil_CIXpsSequentialDocumentFactoryVtbl
    {
        BEGIN_INTERFACE
        
        DECLSPEC_XFGVIRT(IUnknown, QueryInterface)
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            __x_ABI_CXpsUtil_CIXpsSequentialDocumentFactory * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        DECLSPEC_XFGVIRT(IUnknown, AddRef)
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            __x_ABI_CXpsUtil_CIXpsSequentialDocumentFactory * This);
        
        DECLSPEC_XFGVIRT(IUnknown, Release)
        ULONG ( STDMETHODCALLTYPE *Release )( 
            __x_ABI_CXpsUtil_CIXpsSequentialDocumentFactory * This);
        
        DECLSPEC_XFGVIRT(IInspectable, GetIids)
        HRESULT ( STDMETHODCALLTYPE *GetIids )( 
            __x_ABI_CXpsUtil_CIXpsSequentialDocumentFactory * This,
            /* [out] */ ULONG *iidCount,
            /* [size_is][size_is][out] */ IID **iids);
        
        DECLSPEC_XFGVIRT(IInspectable, GetRuntimeClassName)
        HRESULT ( STDMETHODCALLTYPE *GetRuntimeClassName )( 
            __x_ABI_CXpsUtil_CIXpsSequentialDocumentFactory * This,
            /* [out] */ HSTRING *className);
        
        DECLSPEC_XFGVIRT(IInspectable, GetTrustLevel)
        HRESULT ( STDMETHODCALLTYPE *GetTrustLevel )( 
            __x_ABI_CXpsUtil_CIXpsSequentialDocumentFactory * This,
            /* [out] */ TrustLevel *trustLevel);
        
        DECLSPEC_XFGVIRT(__x_ABI_CXpsUtil_CIXpsSequentialDocumentFactory, CreateInstance)
        HRESULT ( STDMETHODCALLTYPE *CreateInstance )( 
            __x_ABI_CXpsUtil_CIXpsSequentialDocumentFactory * This,
            /* [in] */ __x_ABI_CWindows_CGraphics_CPrinting_CWorkflow_CIPrintWorkflowObjectModelSourceFileContent *sourceFileContent,
            /* [out][retval] */ __x_ABI_CXpsUtil_CIXpsSequentialDocument **value);
        
        END_INTERFACE
    } __x_ABI_CXpsUtil_CIXpsSequentialDocumentFactoryVtbl;

    interface __x_ABI_CXpsUtil_CIXpsSequentialDocumentFactory
    {
        CONST_VTBL struct __x_ABI_CXpsUtil_CIXpsSequentialDocumentFactoryVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define __x_ABI_CXpsUtil_CIXpsSequentialDocumentFactory_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define __x_ABI_CXpsUtil_CIXpsSequentialDocumentFactory_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define __x_ABI_CXpsUtil_CIXpsSequentialDocumentFactory_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define __x_ABI_CXpsUtil_CIXpsSequentialDocumentFactory_GetIids(This,iidCount,iids)	\
    ( (This)->lpVtbl -> GetIids(This,iidCount,iids) ) 

#define __x_ABI_CXpsUtil_CIXpsSequentialDocumentFactory_GetRuntimeClassName(This,className)	\
    ( (This)->lpVtbl -> GetRuntimeClassName(This,className) ) 

#define __x_ABI_CXpsUtil_CIXpsSequentialDocumentFactory_GetTrustLevel(This,trustLevel)	\
    ( (This)->lpVtbl -> GetTrustLevel(This,trustLevel) ) 


#define __x_ABI_CXpsUtil_CIXpsSequentialDocumentFactory_CreateInstance(This,sourceFileContent,value)	\
    ( (This)->lpVtbl -> CreateInstance(This,sourceFileContent,value) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* ____x_ABI_CXpsUtil_CIXpsSequentialDocumentFactory_INTERFACE_DEFINED__ */


/* Additional Prototypes for ALL interfaces */

unsigned long             __RPC_USER  HSTRING_UserSize(     unsigned long *, unsigned long            , HSTRING * ); 
unsigned char * __RPC_USER  HSTRING_UserMarshal(  unsigned long *, unsigned char *, HSTRING * ); 
unsigned char * __RPC_USER  HSTRING_UserUnmarshal(unsigned long *, unsigned char *, HSTRING * ); 
void                      __RPC_USER  HSTRING_UserFree(     unsigned long *, HSTRING * ); 

unsigned long             __RPC_USER  HSTRING_UserSize64(     unsigned long *, unsigned long            , HSTRING * ); 
unsigned char * __RPC_USER  HSTRING_UserMarshal64(  unsigned long *, unsigned char *, HSTRING * ); 
unsigned char * __RPC_USER  HSTRING_UserUnmarshal64(unsigned long *, unsigned char *, HSTRING * ); 
void                      __RPC_USER  HSTRING_UserFree64(     unsigned long *, HSTRING * ); 

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


