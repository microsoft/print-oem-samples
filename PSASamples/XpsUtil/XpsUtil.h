/* Header file automatically generated from XpsUtil.idl */
/*
 * File built with Microsoft(R) MIDLRT Compiler Engine Version 10.00.0231 
 */

#pragma warning( disable: 4049 )  /* more than 64k source lines */

/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 500
#endif

/* verify that the <rpcsal.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCSAL_H_VERSION__
#define __REQUIRED_RPCSAL_H_VERSION__ 100
#endif

#include <rpc.h>
#include <rpcndr.h>

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif /* __RPCNDR_H_VERSION__ */

#ifndef COM_NO_WINDOWS_H
#include <windows.h>
#include <ole2.h>
#endif /*COM_NO_WINDOWS_H*/
#ifndef __XpsUtil_h__
#define __XpsUtil_h__
#ifndef __XpsUtil_p_h__
#define __XpsUtil_p_h__


#pragma once

// Ensure that the setting of the /ns_prefix command line switch is consistent for all headers.
// If you get an error from the compiler indicating "warning C4005: 'CHECK_NS_PREFIX_STATE': macro redefinition", this
// indicates that you have included two different headers with different settings for the /ns_prefix MIDL command line switch
#if !defined(DISABLE_NS_PREFIX_CHECKS)
#define CHECK_NS_PREFIX_STATE "always"
#endif // !defined(DISABLE_NS_PREFIX_CHECKS)


#pragma push_macro("MIDL_CONST_ID")
#undef MIDL_CONST_ID
#define MIDL_CONST_ID const __declspec(selectany)


// Header files for imported files
#include "winrtbase.h"
#include "C:\Program Files (x86)\Windows Kits\10\References\10.0.26100.0\Windows.AI.MachineLearning.MachineLearningContract\5.0.0.0\Windows.AI.MachineLearning.MachineLearningContract.h"
#include "C:\Program Files (x86)\Windows Kits\10\References\10.0.26100.0\Windows.AI.MachineLearning.Preview.MachineLearningPreviewContract\2.0.0.0\Windows.AI.MachineLearning.Preview.MachineLearningPreviewContract.h"
#include "C:\Program Files (x86)\Windows Kits\10\References\10.0.26100.0\Windows.ApplicationModel.Calls.Background.CallsBackgroundContract\4.0.0.0\Windows.ApplicationModel.Calls.Background.CallsBackgroundContract.h"
#include "C:\Program Files (x86)\Windows Kits\10\References\10.0.26100.0\Windows.ApplicationModel.Calls.CallsPhoneContract\7.0.0.0\Windows.ApplicationModel.Calls.CallsPhoneContract.h"
#include "C:\Program Files (x86)\Windows Kits\10\References\10.0.26100.0\Windows.ApplicationModel.Calls.CallsVoipContract\5.0.0.0\Windows.ApplicationModel.Calls.CallsVoipContract.h"
#include "C:\Program Files (x86)\Windows Kits\10\References\10.0.26100.0\Windows.ApplicationModel.CommunicationBlocking.CommunicationBlockingContract\2.0.0.0\Windows.ApplicationModel.CommunicationBlocking.CommunicationBlockingContract.h"
#include "C:\Program Files (x86)\Windows Kits\10\References\10.0.26100.0\Windows.ApplicationModel.SocialInfo.SocialInfoContract\2.0.0.0\Windows.ApplicationModel.SocialInfo.SocialInfoContract.h"
#include "C:\Program Files (x86)\Windows Kits\10\References\10.0.26100.0\Windows.ApplicationModel.StartupTaskContract\3.0.0.0\Windows.ApplicationModel.StartupTaskContract.h"
#include "C:\Program Files (x86)\Windows Kits\10\References\10.0.26100.0\Windows.Devices.Custom.CustomDeviceContract\1.0.0.0\Windows.Devices.Custom.CustomDeviceContract.h"
#include "C:\Program Files (x86)\Windows Kits\10\References\10.0.26100.0\Windows.Devices.DevicesLowLevelContract\3.0.0.0\Windows.Devices.DevicesLowLevelContract.h"
#include "C:\Program Files (x86)\Windows Kits\10\References\10.0.26100.0\Windows.Devices.Printers.PrintersContract\1.0.0.0\Windows.Devices.Printers.PrintersContract.h"
#include "C:\Program Files (x86)\Windows Kits\10\References\10.0.26100.0\Windows.Devices.SmartCards.SmartCardBackgroundTriggerContract\3.0.0.0\Windows.Devices.SmartCards.SmartCardBackgroundTriggerContract.h"
#include "C:\Program Files (x86)\Windows Kits\10\References\10.0.26100.0\Windows.Devices.SmartCards.SmartCardEmulatorContract\6.0.0.0\Windows.Devices.SmartCards.SmartCardEmulatorContract.h"
#include "C:\Program Files (x86)\Windows Kits\10\References\10.0.26100.0\Windows.Foundation.FoundationContract\4.0.0.0\Windows.Foundation.FoundationContract.h"
#include "C:\Program Files (x86)\Windows Kits\10\References\10.0.26100.0\Windows.Foundation.UniversalApiContract\19.0.0.0\Windows.Foundation.UniversalApiContract.h"
#include "C:\Program Files (x86)\Windows Kits\10\References\10.0.26100.0\Windows.Gaming.XboxLive.StorageApiContract\1.0.0.0\Windows.Gaming.XboxLive.StorageApiContract.h"
#include "C:\Program Files (x86)\Windows Kits\10\References\10.0.26100.0\Windows.Graphics.Printing3D.Printing3DContract\4.0.0.0\Windows.Graphics.Printing3D.Printing3DContract.h"
#include "C:\Program Files (x86)\Windows Kits\10\References\10.0.26100.0\Windows.Networking.Connectivity.WwanContract\3.0.0.0\Windows.Networking.Connectivity.WwanContract.h"
#include "C:\Program Files (x86)\Windows Kits\10\References\10.0.26100.0\Windows.Networking.Sockets.ControlChannelTriggerContract\3.0.0.0\Windows.Networking.Sockets.ControlChannelTriggerContract.h"
#include "C:\Program Files (x86)\Windows Kits\10\References\10.0.26100.0\Windows.Security.Isolation.IsolatedWindowsEnvironmentContract\5.0.0.0\Windows.Security.Isolation.Isolatedwindowsenvironmentcontract.h"
#include "C:\Program Files (x86)\Windows Kits\10\References\10.0.26100.0\Windows.Services.Maps.GuidanceContract\3.0.0.0\Windows.Services.Maps.GuidanceContract.h"
#include "C:\Program Files (x86)\Windows Kits\10\References\10.0.26100.0\Windows.Services.Maps.LocalSearchContract\4.0.0.0\Windows.Services.Maps.LocalSearchContract.h"
#include "C:\Program Files (x86)\Windows Kits\10\References\10.0.26100.0\Windows.Services.Store.StoreContract\4.0.0.0\Windows.Services.Store.StoreContract.h"
#include "C:\Program Files (x86)\Windows Kits\10\References\10.0.26100.0\Windows.Services.TargetedContent.TargetedContentContract\1.0.0.0\Windows.Services.TargetedContent.TargetedContentContract.h"
#include "C:\Program Files (x86)\Windows Kits\10\References\10.0.26100.0\Windows.Storage.Provider.CloudFilesContract\7.0.0.0\Windows.Storage.Provider.CloudFilesContract.h"
#include "C:\Program Files (x86)\Windows Kits\10\References\10.0.26100.0\Windows.System.Profile.ProfileHardwareTokenContract\1.0.0.0\Windows.System.Profile.ProfileHardwareTokenContract.h"
#include "C:\Program Files (x86)\Windows Kits\10\References\10.0.26100.0\Windows.System.Profile.ProfileRetailInfoContract\1.0.0.0\Windows.System.Profile.ProfileRetailInfoContract.h"
#include "C:\Program Files (x86)\Windows Kits\10\References\10.0.26100.0\Windows.System.Profile.ProfileSharedModeContract\2.0.0.0\Windows.System.Profile.ProfileSharedModeContract.h"
#include "C:\Program Files (x86)\Windows Kits\10\References\10.0.26100.0\Windows.System.Profile.SystemManufacturers.SystemManufacturersContract\3.0.0.0\Windows.System.Profile.SystemManufacturers.SystemManufacturersContract.h"
#include "C:\Program Files (x86)\Windows Kits\10\References\10.0.26100.0\Windows.System.SystemManagementContract\7.0.0.0\Windows.System.SystemManagementContract.h"
#include "C:\Program Files (x86)\Windows Kits\10\References\10.0.26100.0\Windows.UI.UIAutomation.UIAutomationContract\2.0.0.0\Windows.UI.UIAutomation.UIAutomationContract.h"
#include "C:\Program Files (x86)\Windows Kits\10\References\10.0.26100.0\Windows.UI.ViewManagement.ViewManagementViewScalingContract\1.0.0.0\Windows.UI.ViewManagement.ViewManagementViewScalingContract.h"
#include "C:\Program Files (x86)\Windows Kits\10\References\10.0.26100.0\Windows.UI.Xaml.Core.Direct.XamlDirectContract\5.0.0.0\Windows.UI.Xaml.Core.Direct.XamlDirectContract.h"
// Importing Collections header
#include <windows.foundation.collections.h>

#if defined(__cplusplus) && !defined(CINTERFACE)
#if defined(__MIDL_USE_C_ENUM)
#define MIDL_ENUM enum
#else
#define MIDL_ENUM enum class
#endif
/* Forward Declarations */
#ifndef ____x_ABI_CXpsUtil_CIXpsPageWrapper_FWD_DEFINED__
#define ____x_ABI_CXpsUtil_CIXpsPageWrapper_FWD_DEFINED__
namespace ABI {
    namespace XpsUtil {
        interface IXpsPageWrapper;
    } /* XpsUtil */
} /* ABI */
#define __x_ABI_CXpsUtil_CIXpsPageWrapper ABI::XpsUtil::IXpsPageWrapper

#endif // ____x_ABI_CXpsUtil_CIXpsPageWrapper_FWD_DEFINED__

#ifndef ____x_ABI_CXpsUtil_CIXpsPageWatermarker_FWD_DEFINED__
#define ____x_ABI_CXpsUtil_CIXpsPageWatermarker_FWD_DEFINED__
namespace ABI {
    namespace XpsUtil {
        interface IXpsPageWatermarker;
    } /* XpsUtil */
} /* ABI */
#define __x_ABI_CXpsUtil_CIXpsPageWatermarker ABI::XpsUtil::IXpsPageWatermarker

#endif // ____x_ABI_CXpsUtil_CIXpsPageWatermarker_FWD_DEFINED__

#ifndef ____x_ABI_CXpsUtil_CIXpsSequentialDocument_FWD_DEFINED__
#define ____x_ABI_CXpsUtil_CIXpsSequentialDocument_FWD_DEFINED__
namespace ABI {
    namespace XpsUtil {
        interface IXpsSequentialDocument;
    } /* XpsUtil */
} /* ABI */
#define __x_ABI_CXpsUtil_CIXpsSequentialDocument ABI::XpsUtil::IXpsSequentialDocument

#endif // ____x_ABI_CXpsUtil_CIXpsSequentialDocument_FWD_DEFINED__

#ifndef ____x_ABI_CXpsUtil_CIXpsSequentialDocumentFactory_FWD_DEFINED__
#define ____x_ABI_CXpsUtil_CIXpsSequentialDocumentFactory_FWD_DEFINED__
namespace ABI {
    namespace XpsUtil {
        interface IXpsSequentialDocumentFactory;
    } /* XpsUtil */
} /* ABI */
#define __x_ABI_CXpsUtil_CIXpsSequentialDocumentFactory ABI::XpsUtil::IXpsSequentialDocumentFactory

#endif // ____x_ABI_CXpsUtil_CIXpsSequentialDocumentFactory_FWD_DEFINED__

// Parameterized interface forward declarations (C++)

// Collection interface definitions
namespace ABI {
    namespace XpsUtil {
        class XpsSequentialDocument;
    } /* XpsUtil */
} /* ABI */



#ifndef DEF___FITypedEventHandler_2_XpsUtil__CXpsSequentialDocument_UINT32_USE
#define DEF___FITypedEventHandler_2_XpsUtil__CXpsSequentialDocument_UINT32_USE
#if !defined(RO_NO_TEMPLATE_NAME)
namespace ABI { namespace Windows { namespace Foundation {
template <>
struct __declspec(uuid("e02a02ec-d02e-5c71-a9f5-d7962e82cf22"))
ITypedEventHandler<ABI::XpsUtil::XpsSequentialDocument*,UINT32> : ITypedEventHandler_impl<ABI::Windows::Foundation::Internal::AggregateType<ABI::XpsUtil::XpsSequentialDocument*, ABI::XpsUtil::IXpsSequentialDocument*>,UINT32> 
{
    static const wchar_t* z_get_rc_name_impl() 
    {
        return L"Windows.Foundation.TypedEventHandler`2<XpsUtil.XpsSequentialDocument, UInt32>"; 
    }
};
// Define a typedef for the parameterized interface specialization's mangled name.
// This allows code which uses the mangled name for the parameterized interface to access the
// correct parameterized interface specialization.
typedef ITypedEventHandler<ABI::XpsUtil::XpsSequentialDocument*,UINT32> __FITypedEventHandler_2_XpsUtil__CXpsSequentialDocument_UINT32_t;
#define __FITypedEventHandler_2_XpsUtil__CXpsSequentialDocument_UINT32 ABI::Windows::Foundation::__FITypedEventHandler_2_XpsUtil__CXpsSequentialDocument_UINT32_t
/* Foundation */ } /* Windows */ } /* ABI */ } 

////  Define an alias for the C version of the interface for compatibility purposes.
//#define __FITypedEventHandler_2_XpsUtil__CXpsSequentialDocument_UINT32 ABI::Windows::Foundation::ITypedEventHandler<ABI::XpsUtil::IXpsSequentialDocument*,unsigned int>
//#define __FITypedEventHandler_2_XpsUtil__CXpsSequentialDocument_UINT32_t ABI::Windows::Foundation::ITypedEventHandler<ABI::XpsUtil::IXpsSequentialDocument*,unsigned int>
#endif // !defined(RO_NO_TEMPLATE_NAME)
#endif /* DEF___FITypedEventHandler_2_XpsUtil__CXpsSequentialDocument_UINT32_USE */





#ifndef DEF___FITypedEventHandler_2_XpsUtil__CXpsSequentialDocument_UINT64_USE
#define DEF___FITypedEventHandler_2_XpsUtil__CXpsSequentialDocument_UINT64_USE
#if !defined(RO_NO_TEMPLATE_NAME)
namespace ABI { namespace Windows { namespace Foundation {
template <>
struct __declspec(uuid("6ddf6c64-caa5-5489-bb1e-17cd09b09199"))
ITypedEventHandler<ABI::XpsUtil::XpsSequentialDocument*,UINT64> : ITypedEventHandler_impl<ABI::Windows::Foundation::Internal::AggregateType<ABI::XpsUtil::XpsSequentialDocument*, ABI::XpsUtil::IXpsSequentialDocument*>,UINT64> 
{
    static const wchar_t* z_get_rc_name_impl() 
    {
        return L"Windows.Foundation.TypedEventHandler`2<XpsUtil.XpsSequentialDocument, UInt64>"; 
    }
};
// Define a typedef for the parameterized interface specialization's mangled name.
// This allows code which uses the mangled name for the parameterized interface to access the
// correct parameterized interface specialization.
typedef ITypedEventHandler<ABI::XpsUtil::XpsSequentialDocument*,UINT64> __FITypedEventHandler_2_XpsUtil__CXpsSequentialDocument_UINT64_t;
#define __FITypedEventHandler_2_XpsUtil__CXpsSequentialDocument_UINT64 ABI::Windows::Foundation::__FITypedEventHandler_2_XpsUtil__CXpsSequentialDocument_UINT64_t
/* Foundation */ } /* Windows */ } /* ABI */ } 

////  Define an alias for the C version of the interface for compatibility purposes.
//#define __FITypedEventHandler_2_XpsUtil__CXpsSequentialDocument_UINT64 ABI::Windows::Foundation::ITypedEventHandler<ABI::XpsUtil::IXpsSequentialDocument*,unsigned __int64>
//#define __FITypedEventHandler_2_XpsUtil__CXpsSequentialDocument_UINT64_t ABI::Windows::Foundation::ITypedEventHandler<ABI::XpsUtil::IXpsSequentialDocument*,unsigned __int64>
#endif // !defined(RO_NO_TEMPLATE_NAME)
#endif /* DEF___FITypedEventHandler_2_XpsUtil__CXpsSequentialDocument_UINT64_USE */





namespace ABI {
    namespace XpsUtil {
        class XpsPageWrapper;
    } /* XpsUtil */
} /* ABI */



/*
 *
 * Interface XpsUtil.IXpsPageWrapper
 *
 * Interface is a part of the implementation of type XpsUtil.XpsPageWrapper
 *
 *
 * The IID for this interface was automatically generated by MIDLRT.
 *
 * Interface IID generation seed: XpsUtil.IXpsPageWrapper:HRESULT Clone(XpsUtil.XpsPageWrapper**);HRESULT RenderPageToBMP(Windows.Storage.Streams.IRandomAccessStream**);
 *
 *
 */
#if !defined(____x_ABI_CXpsUtil_CIXpsPageWrapper_INTERFACE_DEFINED__)
#define ____x_ABI_CXpsUtil_CIXpsPageWrapper_INTERFACE_DEFINED__
extern const __declspec(selectany) _Null_terminated_ WCHAR InterfaceName_XpsUtil_IXpsPageWrapper[] = L"XpsUtil.IXpsPageWrapper";
namespace ABI {
    namespace XpsUtil {
        /* [uuid("74af329f-367e-5130-a0eb-3a82403cebf9"), version, object, exclusiveto] */
        MIDL_INTERFACE("74af329f-367e-5130-a0eb-3a82403cebf9")
        IXpsPageWrapper : public IInspectable
        {
        public:
            virtual HRESULT STDMETHODCALLTYPE Clone(
                /* [out, retval] */ABI::XpsUtil::IXpsPageWrapper * * result
                ) = 0;
            virtual HRESULT STDMETHODCALLTYPE RenderPageToBMP(
                /* [out, retval] */ABI::Windows::Storage::Streams::IRandomAccessStream * * result
                ) = 0;
            
        };

        MIDL_CONST_ID IID & IID_IXpsPageWrapper=__uuidof(IXpsPageWrapper);
        
    } /* XpsUtil */
} /* ABI */

EXTERN_C const IID IID___x_ABI_CXpsUtil_CIXpsPageWrapper;
#endif /* !defined(____x_ABI_CXpsUtil_CIXpsPageWrapper_INTERFACE_DEFINED__) */


/*
 *
 * Class XpsUtil.XpsPageWrapper
 *
 * Class implements the following interfaces:
 *    XpsUtil.IXpsPageWrapper ** Default Interface **
 *
 * Class Marshaling Behavior:  Agile - Class is agile
 *
 */

#ifndef RUNTIMECLASS_XpsUtil_XpsPageWrapper_DEFINED
#define RUNTIMECLASS_XpsUtil_XpsPageWrapper_DEFINED
extern const __declspec(selectany) _Null_terminated_ WCHAR RuntimeClass_XpsUtil_XpsPageWrapper[] = L"XpsUtil.XpsPageWrapper";
#endif

namespace ABI {
    namespace XpsUtil {
        class XpsPageWatermarker;
    } /* XpsUtil */
} /* ABI */



/*
 *
 * Interface XpsUtil.IXpsPageWatermarker
 *
 * Interface is a part of the implementation of type XpsUtil.XpsPageWatermarker
 *
 *
 * The IID for this interface was automatically generated by MIDLRT.
 *
 * Interface IID generation seed: XpsUtil.IXpsPageWatermarker:HRESULT SetWatermarkText(String,Double,Double,Double);HRESULT SetWatermarkImage(String,Double,Double,Int32,Int32);HRESULT SetWatermarkImageEnabled(Boolean);HRESULT ApplyWatermarks(XpsUtil.XpsPageWrapper*);
 *
 *
 */
#if !defined(____x_ABI_CXpsUtil_CIXpsPageWatermarker_INTERFACE_DEFINED__)
#define ____x_ABI_CXpsUtil_CIXpsPageWatermarker_INTERFACE_DEFINED__
extern const __declspec(selectany) _Null_terminated_ WCHAR InterfaceName_XpsUtil_IXpsPageWatermarker[] = L"XpsUtil.IXpsPageWatermarker";
namespace ABI {
    namespace XpsUtil {
        /* [uuid("6af08114-b0f1-5e16-8477-33e76c2d2a1e"), version, object, exclusiveto] */
        MIDL_INTERFACE("6af08114-b0f1-5e16-8477-33e76c2d2a1e")
        IXpsPageWatermarker : public IInspectable
        {
        public:
            virtual HRESULT STDMETHODCALLTYPE SetWatermarkText(
                /* [in] */HSTRING text,
                /* [in] */double fontSize,
                /* [in] */double xRelativeOffset,
                /* [in] */double yRelativeOffset
                ) = 0;
            virtual HRESULT STDMETHODCALLTYPE SetWatermarkImage(
                /* [in] */HSTRING imagePath,
                /* [in] */double dpiX,
                /* [in] */double dpiY,
                /* [in] */int width,
                /* [in] */int height
                ) = 0;
            virtual HRESULT STDMETHODCALLTYPE SetWatermarkImageEnabled(
                /* [in] */::boolean enabled
                ) = 0;
            virtual HRESULT STDMETHODCALLTYPE ApplyWatermarks(
                /* [in] */ABI::XpsUtil::IXpsPageWrapper * page
                ) = 0;
            
        };

        MIDL_CONST_ID IID & IID_IXpsPageWatermarker=__uuidof(IXpsPageWatermarker);
        
    } /* XpsUtil */
} /* ABI */

EXTERN_C const IID IID___x_ABI_CXpsUtil_CIXpsPageWatermarker;
#endif /* !defined(____x_ABI_CXpsUtil_CIXpsPageWatermarker_INTERFACE_DEFINED__) */


/*
 *
 * Class XpsUtil.XpsPageWatermarker
 *
 * RuntimeClass can be activated.
 *
 * Class implements the following interfaces:
 *    XpsUtil.IXpsPageWatermarker ** Default Interface **
 *
 * Class Threading Model:  Both Single and Multi Threaded Apartment
 *
 * Class Marshaling Behavior:  Agile - Class is agile
 *
 */

#ifndef RUNTIMECLASS_XpsUtil_XpsPageWatermarker_DEFINED
#define RUNTIMECLASS_XpsUtil_XpsPageWatermarker_DEFINED
extern const __declspec(selectany) _Null_terminated_ WCHAR RuntimeClass_XpsUtil_XpsPageWatermarker[] = L"XpsUtil.XpsPageWatermarker";
#endif



/*
 *
 * Interface XpsUtil.IXpsSequentialDocument
 *
 * Interface is a part of the implementation of type XpsUtil.XpsSequentialDocument
 *
 *
 * The IID for this interface was automatically generated by MIDLRT.
 *
 * Interface IID generation seed: XpsUtil.IXpsSequentialDocument:HRESULT StartXpsOMGeneration();HRESULT GetWatermarkedStream(XpsUtil.XpsPageWatermarker*,Windows.Storage.Streams.IInputStream**);HRESULT GetPage(UInt32,XpsUtil.XpsPageWrapper**);HRESULT PageCount(UInt32*);HRESULT add_PageAdded(Windows.Foundation.TypedEventHandler`2<XpsUtil.XpsSequentialDocument, UInt32>*,Windows.Foundation.EventRegistrationToken*);HRESULT remove_PageAdded(EventRegistrationToken);HRESULT add_DocumentClosed(Windows.Foundation.TypedEventHandler`2<XpsUtil.XpsSequentialDocument, UInt32>*,Windows.Foundation.EventRegistrationToken*);HRESULT remove_DocumentClosed(EventRegistrationToken);HRESULT add_XpsGenerationFailed(Windows.Foundation.TypedEventHandler`2<XpsUtil.XpsSequentialDocument, UInt64>*,Windows.Foundation.EventRegistrationToken*);HRESULT remove_XpsGenerationFailed(EventRegistrationToken);
 *
 *
 */
#if !defined(____x_ABI_CXpsUtil_CIXpsSequentialDocument_INTERFACE_DEFINED__)
#define ____x_ABI_CXpsUtil_CIXpsSequentialDocument_INTERFACE_DEFINED__
extern const __declspec(selectany) _Null_terminated_ WCHAR InterfaceName_XpsUtil_IXpsSequentialDocument[] = L"XpsUtil.IXpsSequentialDocument";
namespace ABI {
    namespace XpsUtil {
        /* [uuid("3dbdceab-938b-50aa-a51b-876f0f25cd22"), version, object, exclusiveto] */
        MIDL_INTERFACE("3dbdceab-938b-50aa-a51b-876f0f25cd22")
        IXpsSequentialDocument : public IInspectable
        {
        public:
            virtual HRESULT STDMETHODCALLTYPE StartXpsOMGeneration(void) = 0;
            virtual HRESULT STDMETHODCALLTYPE GetWatermarkedStream(
                /* [in] */ABI::XpsUtil::IXpsPageWatermarker * watermarker,
                /* [out, retval] */ABI::Windows::Storage::Streams::IInputStream * * result
                ) = 0;
            virtual HRESULT STDMETHODCALLTYPE GetPage(
                /* [in] */unsigned int pageNumber,
                /* [out, retval] */ABI::XpsUtil::IXpsPageWrapper * * result
                ) = 0;
            virtual HRESULT STDMETHODCALLTYPE PageCount(
                /* [out, retval] */unsigned int * result
                ) = 0;
            /* [eventadd] */virtual HRESULT STDMETHODCALLTYPE add_PageAdded(
                /* [in] */__FITypedEventHandler_2_XpsUtil__CXpsSequentialDocument_UINT32 * handler,
                /* [retval, out] */EventRegistrationToken * token
                ) = 0;
            /* [eventremove] */virtual HRESULT STDMETHODCALLTYPE remove_PageAdded(
                /* [in] */EventRegistrationToken token
                ) = 0;
            /* [eventadd] */virtual HRESULT STDMETHODCALLTYPE add_DocumentClosed(
                /* [in] */__FITypedEventHandler_2_XpsUtil__CXpsSequentialDocument_UINT32 * handler,
                /* [retval, out] */EventRegistrationToken * token
                ) = 0;
            /* [eventremove] */virtual HRESULT STDMETHODCALLTYPE remove_DocumentClosed(
                /* [in] */EventRegistrationToken token
                ) = 0;
            /* [eventadd] */virtual HRESULT STDMETHODCALLTYPE add_XpsGenerationFailed(
                /* [in] */__FITypedEventHandler_2_XpsUtil__CXpsSequentialDocument_UINT64 * handler,
                /* [retval, out] */EventRegistrationToken * token
                ) = 0;
            /* [eventremove] */virtual HRESULT STDMETHODCALLTYPE remove_XpsGenerationFailed(
                /* [in] */EventRegistrationToken token
                ) = 0;
            
        };

        MIDL_CONST_ID IID & IID_IXpsSequentialDocument=__uuidof(IXpsSequentialDocument);
        
    } /* XpsUtil */
} /* ABI */

EXTERN_C const IID IID___x_ABI_CXpsUtil_CIXpsSequentialDocument;
#endif /* !defined(____x_ABI_CXpsUtil_CIXpsSequentialDocument_INTERFACE_DEFINED__) */


/*
 *
 * Interface XpsUtil.IXpsSequentialDocumentFactory
 *
 * Interface is a part of the implementation of type XpsUtil.XpsSequentialDocument
 *
 *
 * The IID for this interface was automatically generated by MIDLRT.
 *
 * Interface IID generation seed: XpsUtil.IXpsSequentialDocumentFactory:HRESULT CreateInstance(Windows.Graphics.Printing.Workflow.PrintWorkflowObjectModelSourceFileContent*,XpsUtil.XpsSequentialDocument**);
 *
 *
 */
#if !defined(____x_ABI_CXpsUtil_CIXpsSequentialDocumentFactory_INTERFACE_DEFINED__)
#define ____x_ABI_CXpsUtil_CIXpsSequentialDocumentFactory_INTERFACE_DEFINED__
extern const __declspec(selectany) _Null_terminated_ WCHAR InterfaceName_XpsUtil_IXpsSequentialDocumentFactory[] = L"XpsUtil.IXpsSequentialDocumentFactory";
namespace ABI {
    namespace XpsUtil {
        /* [uuid("b2b480dd-91de-5ab7-be85-e11783c5236d"), version, object, exclusiveto] */
        MIDL_INTERFACE("b2b480dd-91de-5ab7-be85-e11783c5236d")
        IXpsSequentialDocumentFactory : public IInspectable
        {
        public:
            virtual HRESULT STDMETHODCALLTYPE CreateInstance(
                /* [in] */ABI::Windows::Graphics::Printing::Workflow::IPrintWorkflowObjectModelSourceFileContent * sourceFileContent,
                /* [retval, out] */ABI::XpsUtil::IXpsSequentialDocument * * value
                ) = 0;
            
        };

        MIDL_CONST_ID IID & IID_IXpsSequentialDocumentFactory=__uuidof(IXpsSequentialDocumentFactory);
        
    } /* XpsUtil */
} /* ABI */

EXTERN_C const IID IID___x_ABI_CXpsUtil_CIXpsSequentialDocumentFactory;
#endif /* !defined(____x_ABI_CXpsUtil_CIXpsSequentialDocumentFactory_INTERFACE_DEFINED__) */


/*
 *
 * Class XpsUtil.XpsSequentialDocument
 *
 * RuntimeClass can be activated.
 *
 * Class implements the following interfaces:
 *    XpsUtil.IXpsSequentialDocument ** Default Interface **
 *
 * Class Threading Model:  Both Single and Multi Threaded Apartment
 *
 * Class Marshaling Behavior:  Agile - Class is agile
 *
 */

#ifndef RUNTIMECLASS_XpsUtil_XpsSequentialDocument_DEFINED
#define RUNTIMECLASS_XpsUtil_XpsSequentialDocument_DEFINED
extern const __declspec(selectany) _Null_terminated_ WCHAR RuntimeClass_XpsUtil_XpsSequentialDocument[] = L"XpsUtil.XpsSequentialDocument";
#endif


#else // !defined(__cplusplus)
/* Forward Declarations */
#ifndef ____x_ABI_CXpsUtil_CIXpsPageWrapper_FWD_DEFINED__
#define ____x_ABI_CXpsUtil_CIXpsPageWrapper_FWD_DEFINED__
typedef interface __x_ABI_CXpsUtil_CIXpsPageWrapper __x_ABI_CXpsUtil_CIXpsPageWrapper;

#endif // ____x_ABI_CXpsUtil_CIXpsPageWrapper_FWD_DEFINED__

#ifndef ____x_ABI_CXpsUtil_CIXpsPageWatermarker_FWD_DEFINED__
#define ____x_ABI_CXpsUtil_CIXpsPageWatermarker_FWD_DEFINED__
typedef interface __x_ABI_CXpsUtil_CIXpsPageWatermarker __x_ABI_CXpsUtil_CIXpsPageWatermarker;

#endif // ____x_ABI_CXpsUtil_CIXpsPageWatermarker_FWD_DEFINED__

#ifndef ____x_ABI_CXpsUtil_CIXpsSequentialDocument_FWD_DEFINED__
#define ____x_ABI_CXpsUtil_CIXpsSequentialDocument_FWD_DEFINED__
typedef interface __x_ABI_CXpsUtil_CIXpsSequentialDocument __x_ABI_CXpsUtil_CIXpsSequentialDocument;

#endif // ____x_ABI_CXpsUtil_CIXpsSequentialDocument_FWD_DEFINED__

#ifndef ____x_ABI_CXpsUtil_CIXpsSequentialDocumentFactory_FWD_DEFINED__
#define ____x_ABI_CXpsUtil_CIXpsSequentialDocumentFactory_FWD_DEFINED__
typedef interface __x_ABI_CXpsUtil_CIXpsSequentialDocumentFactory __x_ABI_CXpsUtil_CIXpsSequentialDocumentFactory;

#endif // ____x_ABI_CXpsUtil_CIXpsSequentialDocumentFactory_FWD_DEFINED__

// Parameterized interface forward declarations (C)

// Collection interface definitions

#if !defined(____FITypedEventHandler_2_XpsUtil__CXpsSequentialDocument_UINT32_INTERFACE_DEFINED__)
#define ____FITypedEventHandler_2_XpsUtil__CXpsSequentialDocument_UINT32_INTERFACE_DEFINED__

typedef interface __FITypedEventHandler_2_XpsUtil__CXpsSequentialDocument_UINT32 __FITypedEventHandler_2_XpsUtil__CXpsSequentialDocument_UINT32;

//  Declare the parameterized interface IID.
EXTERN_C const IID IID___FITypedEventHandler_2_XpsUtil__CXpsSequentialDocument_UINT32;

typedef struct __FITypedEventHandler_2_XpsUtil__CXpsSequentialDocument_UINT32Vtbl
{
    BEGIN_INTERFACE
    HRESULT ( STDMETHODCALLTYPE *QueryInterface )(__RPC__in __FITypedEventHandler_2_XpsUtil__CXpsSequentialDocument_UINT32 * This,
        /* [in] */ __RPC__in REFIID riid,
        /* [annotation][iid_is][out] */ 
        _COM_Outptr_  void **ppvObject);
    ULONG ( STDMETHODCALLTYPE *AddRef )(__RPC__in __FITypedEventHandler_2_XpsUtil__CXpsSequentialDocument_UINT32 * This);
    ULONG ( STDMETHODCALLTYPE *Release )(__RPC__in __FITypedEventHandler_2_XpsUtil__CXpsSequentialDocument_UINT32 * This);

    HRESULT ( STDMETHODCALLTYPE *Invoke )(__RPC__in __FITypedEventHandler_2_XpsUtil__CXpsSequentialDocument_UINT32 * This,/* [in] */ __RPC__in_opt __x_ABI_CXpsUtil_CIXpsSequentialDocument * sender,/* [in] */ __RPC__in_opt unsigned int e);
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



#endif // ____FITypedEventHandler_2_XpsUtil__CXpsSequentialDocument_UINT32_INTERFACE_DEFINED__



#if !defined(____FITypedEventHandler_2_XpsUtil__CXpsSequentialDocument_UINT64_INTERFACE_DEFINED__)
#define ____FITypedEventHandler_2_XpsUtil__CXpsSequentialDocument_UINT64_INTERFACE_DEFINED__

typedef interface __FITypedEventHandler_2_XpsUtil__CXpsSequentialDocument_UINT64 __FITypedEventHandler_2_XpsUtil__CXpsSequentialDocument_UINT64;

//  Declare the parameterized interface IID.
EXTERN_C const IID IID___FITypedEventHandler_2_XpsUtil__CXpsSequentialDocument_UINT64;

typedef struct __FITypedEventHandler_2_XpsUtil__CXpsSequentialDocument_UINT64Vtbl
{
    BEGIN_INTERFACE
    HRESULT ( STDMETHODCALLTYPE *QueryInterface )(__RPC__in __FITypedEventHandler_2_XpsUtil__CXpsSequentialDocument_UINT64 * This,
        /* [in] */ __RPC__in REFIID riid,
        /* [annotation][iid_is][out] */ 
        _COM_Outptr_  void **ppvObject);
    ULONG ( STDMETHODCALLTYPE *AddRef )(__RPC__in __FITypedEventHandler_2_XpsUtil__CXpsSequentialDocument_UINT64 * This);
    ULONG ( STDMETHODCALLTYPE *Release )(__RPC__in __FITypedEventHandler_2_XpsUtil__CXpsSequentialDocument_UINT64 * This);

    HRESULT ( STDMETHODCALLTYPE *Invoke )(__RPC__in __FITypedEventHandler_2_XpsUtil__CXpsSequentialDocument_UINT64 * This,/* [in] */ __RPC__in_opt __x_ABI_CXpsUtil_CIXpsSequentialDocument * sender,/* [in] */ __RPC__in_opt unsigned __int64 e);
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



#endif // ____FITypedEventHandler_2_XpsUtil__CXpsSequentialDocument_UINT64_INTERFACE_DEFINED__





/*
 *
 * Interface XpsUtil.IXpsPageWrapper
 *
 * Interface is a part of the implementation of type XpsUtil.XpsPageWrapper
 *
 *
 * The IID for this interface was automatically generated by MIDLRT.
 *
 * Interface IID generation seed: XpsUtil.IXpsPageWrapper:HRESULT Clone(XpsUtil.XpsPageWrapper**);HRESULT RenderPageToBMP(Windows.Storage.Streams.IRandomAccessStream**);
 *
 *
 */
#if !defined(____x_ABI_CXpsUtil_CIXpsPageWrapper_INTERFACE_DEFINED__)
#define ____x_ABI_CXpsUtil_CIXpsPageWrapper_INTERFACE_DEFINED__
extern const __declspec(selectany) _Null_terminated_ WCHAR InterfaceName_XpsUtil_IXpsPageWrapper[] = L"XpsUtil.IXpsPageWrapper";
/* [uuid("74af329f-367e-5130-a0eb-3a82403cebf9"), version, object, exclusiveto] */
typedef struct __x_ABI_CXpsUtil_CIXpsPageWrapperVtbl
{
    BEGIN_INTERFACE
    HRESULT ( STDMETHODCALLTYPE *QueryInterface)(
    __RPC__in __x_ABI_CXpsUtil_CIXpsPageWrapper * This,
    /* [in] */ __RPC__in REFIID riid,
    /* [annotation][iid_is][out] */
    _COM_Outptr_  void **ppvObject
    );

ULONG ( STDMETHODCALLTYPE *AddRef )(
    __RPC__in __x_ABI_CXpsUtil_CIXpsPageWrapper * This
    );

ULONG ( STDMETHODCALLTYPE *Release )(
    __RPC__in __x_ABI_CXpsUtil_CIXpsPageWrapper * This
    );

HRESULT ( STDMETHODCALLTYPE *GetIids )(
    __RPC__in __x_ABI_CXpsUtil_CIXpsPageWrapper * This,
    /* [out] */ __RPC__out ULONG *iidCount,
    /* [size_is][size_is][out] */ __RPC__deref_out_ecount_full_opt(*iidCount) IID **iids
    );

HRESULT ( STDMETHODCALLTYPE *GetRuntimeClassName )(
    __RPC__in __x_ABI_CXpsUtil_CIXpsPageWrapper * This,
    /* [out] */ __RPC__deref_out_opt HSTRING *className
    );

HRESULT ( STDMETHODCALLTYPE *GetTrustLevel )(
    __RPC__in __x_ABI_CXpsUtil_CIXpsPageWrapper * This,
    /* [OUT ] */ __RPC__out TrustLevel *trustLevel
    );
HRESULT ( STDMETHODCALLTYPE *Clone )(
        __x_ABI_CXpsUtil_CIXpsPageWrapper * This,
        /* [out, retval] */__x_ABI_CXpsUtil_CIXpsPageWrapper * * result
        );
    HRESULT ( STDMETHODCALLTYPE *RenderPageToBMP )(
        __x_ABI_CXpsUtil_CIXpsPageWrapper * This,
        /* [out, retval] */__x_ABI_CWindows_CStorage_CStreams_CIRandomAccessStream * * result
        );
    END_INTERFACE
    
} __x_ABI_CXpsUtil_CIXpsPageWrapperVtbl;

interface __x_ABI_CXpsUtil_CIXpsPageWrapper
{
    CONST_VTBL struct __x_ABI_CXpsUtil_CIXpsPageWrapperVtbl *lpVtbl;
};

#ifdef COBJMACROS
#define __x_ABI_CXpsUtil_CIXpsPageWrapper_QueryInterface(This,riid,ppvObject) \
( (This)->lpVtbl->QueryInterface(This,riid,ppvObject) )

#define __x_ABI_CXpsUtil_CIXpsPageWrapper_AddRef(This) \
        ( (This)->lpVtbl->AddRef(This) )

#define __x_ABI_CXpsUtil_CIXpsPageWrapper_Release(This) \
        ( (This)->lpVtbl->Release(This) )

#define __x_ABI_CXpsUtil_CIXpsPageWrapper_GetIids(This,iidCount,iids) \
        ( (This)->lpVtbl->GetIids(This,iidCount,iids) )

#define __x_ABI_CXpsUtil_CIXpsPageWrapper_GetRuntimeClassName(This,className) \
        ( (This)->lpVtbl->GetRuntimeClassName(This,className) )

#define __x_ABI_CXpsUtil_CIXpsPageWrapper_GetTrustLevel(This,trustLevel) \
        ( (This)->lpVtbl->GetTrustLevel(This,trustLevel) )

#define __x_ABI_CXpsUtil_CIXpsPageWrapper_Clone(This,result) \
    ( (This)->lpVtbl->Clone(This,result) )

#define __x_ABI_CXpsUtil_CIXpsPageWrapper_RenderPageToBMP(This,result) \
    ( (This)->lpVtbl->RenderPageToBMP(This,result) )


#endif /* COBJMACROS */


EXTERN_C const IID IID___x_ABI_CXpsUtil_CIXpsPageWrapper;
#endif /* !defined(____x_ABI_CXpsUtil_CIXpsPageWrapper_INTERFACE_DEFINED__) */


/*
 *
 * Class XpsUtil.XpsPageWrapper
 *
 * Class implements the following interfaces:
 *    XpsUtil.IXpsPageWrapper ** Default Interface **
 *
 * Class Marshaling Behavior:  Agile - Class is agile
 *
 */

#ifndef RUNTIMECLASS_XpsUtil_XpsPageWrapper_DEFINED
#define RUNTIMECLASS_XpsUtil_XpsPageWrapper_DEFINED
extern const __declspec(selectany) _Null_terminated_ WCHAR RuntimeClass_XpsUtil_XpsPageWrapper[] = L"XpsUtil.XpsPageWrapper";
#endif



/*
 *
 * Interface XpsUtil.IXpsPageWatermarker
 *
 * Interface is a part of the implementation of type XpsUtil.XpsPageWatermarker
 *
 *
 * The IID for this interface was automatically generated by MIDLRT.
 *
 * Interface IID generation seed: XpsUtil.IXpsPageWatermarker:HRESULT SetWatermarkText(String,Double,Double,Double);HRESULT SetWatermarkImage(String,Double,Double,Int32,Int32);HRESULT SetWatermarkImageEnabled(Boolean);HRESULT ApplyWatermarks(XpsUtil.XpsPageWrapper*);
 *
 *
 */
#if !defined(____x_ABI_CXpsUtil_CIXpsPageWatermarker_INTERFACE_DEFINED__)
#define ____x_ABI_CXpsUtil_CIXpsPageWatermarker_INTERFACE_DEFINED__
extern const __declspec(selectany) _Null_terminated_ WCHAR InterfaceName_XpsUtil_IXpsPageWatermarker[] = L"XpsUtil.IXpsPageWatermarker";
/* [uuid("6af08114-b0f1-5e16-8477-33e76c2d2a1e"), version, object, exclusiveto] */
typedef struct __x_ABI_CXpsUtil_CIXpsPageWatermarkerVtbl
{
    BEGIN_INTERFACE
    HRESULT ( STDMETHODCALLTYPE *QueryInterface)(
    __RPC__in __x_ABI_CXpsUtil_CIXpsPageWatermarker * This,
    /* [in] */ __RPC__in REFIID riid,
    /* [annotation][iid_is][out] */
    _COM_Outptr_  void **ppvObject
    );

ULONG ( STDMETHODCALLTYPE *AddRef )(
    __RPC__in __x_ABI_CXpsUtil_CIXpsPageWatermarker * This
    );

ULONG ( STDMETHODCALLTYPE *Release )(
    __RPC__in __x_ABI_CXpsUtil_CIXpsPageWatermarker * This
    );

HRESULT ( STDMETHODCALLTYPE *GetIids )(
    __RPC__in __x_ABI_CXpsUtil_CIXpsPageWatermarker * This,
    /* [out] */ __RPC__out ULONG *iidCount,
    /* [size_is][size_is][out] */ __RPC__deref_out_ecount_full_opt(*iidCount) IID **iids
    );

HRESULT ( STDMETHODCALLTYPE *GetRuntimeClassName )(
    __RPC__in __x_ABI_CXpsUtil_CIXpsPageWatermarker * This,
    /* [out] */ __RPC__deref_out_opt HSTRING *className
    );

HRESULT ( STDMETHODCALLTYPE *GetTrustLevel )(
    __RPC__in __x_ABI_CXpsUtil_CIXpsPageWatermarker * This,
    /* [OUT ] */ __RPC__out TrustLevel *trustLevel
    );
HRESULT ( STDMETHODCALLTYPE *SetWatermarkText )(
        __x_ABI_CXpsUtil_CIXpsPageWatermarker * This,
        /* [in] */HSTRING text,
        /* [in] */double fontSize,
        /* [in] */double xRelativeOffset,
        /* [in] */double yRelativeOffset
        );
    HRESULT ( STDMETHODCALLTYPE *SetWatermarkImage )(
        __x_ABI_CXpsUtil_CIXpsPageWatermarker * This,
        /* [in] */HSTRING imagePath,
        /* [in] */double dpiX,
        /* [in] */double dpiY,
        /* [in] */int width,
        /* [in] */int height
        );
    HRESULT ( STDMETHODCALLTYPE *SetWatermarkImageEnabled )(
        __x_ABI_CXpsUtil_CIXpsPageWatermarker * This,
        /* [in] */boolean enabled
        );
    HRESULT ( STDMETHODCALLTYPE *ApplyWatermarks )(
        __x_ABI_CXpsUtil_CIXpsPageWatermarker * This,
        /* [in] */__x_ABI_CXpsUtil_CIXpsPageWrapper * page
        );
    END_INTERFACE
    
} __x_ABI_CXpsUtil_CIXpsPageWatermarkerVtbl;

interface __x_ABI_CXpsUtil_CIXpsPageWatermarker
{
    CONST_VTBL struct __x_ABI_CXpsUtil_CIXpsPageWatermarkerVtbl *lpVtbl;
};

#ifdef COBJMACROS
#define __x_ABI_CXpsUtil_CIXpsPageWatermarker_QueryInterface(This,riid,ppvObject) \
( (This)->lpVtbl->QueryInterface(This,riid,ppvObject) )

#define __x_ABI_CXpsUtil_CIXpsPageWatermarker_AddRef(This) \
        ( (This)->lpVtbl->AddRef(This) )

#define __x_ABI_CXpsUtil_CIXpsPageWatermarker_Release(This) \
        ( (This)->lpVtbl->Release(This) )

#define __x_ABI_CXpsUtil_CIXpsPageWatermarker_GetIids(This,iidCount,iids) \
        ( (This)->lpVtbl->GetIids(This,iidCount,iids) )

#define __x_ABI_CXpsUtil_CIXpsPageWatermarker_GetRuntimeClassName(This,className) \
        ( (This)->lpVtbl->GetRuntimeClassName(This,className) )

#define __x_ABI_CXpsUtil_CIXpsPageWatermarker_GetTrustLevel(This,trustLevel) \
        ( (This)->lpVtbl->GetTrustLevel(This,trustLevel) )

#define __x_ABI_CXpsUtil_CIXpsPageWatermarker_SetWatermarkText(This,text,fontSize,xRelativeOffset,yRelativeOffset) \
    ( (This)->lpVtbl->SetWatermarkText(This,text,fontSize,xRelativeOffset,yRelativeOffset) )

#define __x_ABI_CXpsUtil_CIXpsPageWatermarker_SetWatermarkImage(This,imagePath,dpiX,dpiY,width,height) \
    ( (This)->lpVtbl->SetWatermarkImage(This,imagePath,dpiX,dpiY,width,height) )

#define __x_ABI_CXpsUtil_CIXpsPageWatermarker_SetWatermarkImageEnabled(This,enabled) \
    ( (This)->lpVtbl->SetWatermarkImageEnabled(This,enabled) )

#define __x_ABI_CXpsUtil_CIXpsPageWatermarker_ApplyWatermarks(This,page) \
    ( (This)->lpVtbl->ApplyWatermarks(This,page) )


#endif /* COBJMACROS */


EXTERN_C const IID IID___x_ABI_CXpsUtil_CIXpsPageWatermarker;
#endif /* !defined(____x_ABI_CXpsUtil_CIXpsPageWatermarker_INTERFACE_DEFINED__) */


/*
 *
 * Class XpsUtil.XpsPageWatermarker
 *
 * RuntimeClass can be activated.
 *
 * Class implements the following interfaces:
 *    XpsUtil.IXpsPageWatermarker ** Default Interface **
 *
 * Class Threading Model:  Both Single and Multi Threaded Apartment
 *
 * Class Marshaling Behavior:  Agile - Class is agile
 *
 */

#ifndef RUNTIMECLASS_XpsUtil_XpsPageWatermarker_DEFINED
#define RUNTIMECLASS_XpsUtil_XpsPageWatermarker_DEFINED
extern const __declspec(selectany) _Null_terminated_ WCHAR RuntimeClass_XpsUtil_XpsPageWatermarker[] = L"XpsUtil.XpsPageWatermarker";
#endif



/*
 *
 * Interface XpsUtil.IXpsSequentialDocument
 *
 * Interface is a part of the implementation of type XpsUtil.XpsSequentialDocument
 *
 *
 * The IID for this interface was automatically generated by MIDLRT.
 *
 * Interface IID generation seed: XpsUtil.IXpsSequentialDocument:HRESULT StartXpsOMGeneration();HRESULT GetWatermarkedStream(XpsUtil.XpsPageWatermarker*,Windows.Storage.Streams.IInputStream**);HRESULT GetPage(UInt32,XpsUtil.XpsPageWrapper**);HRESULT PageCount(UInt32*);HRESULT add_PageAdded(Windows.Foundation.TypedEventHandler`2<XpsUtil.XpsSequentialDocument, UInt32>*,Windows.Foundation.EventRegistrationToken*);HRESULT remove_PageAdded(EventRegistrationToken);HRESULT add_DocumentClosed(Windows.Foundation.TypedEventHandler`2<XpsUtil.XpsSequentialDocument, UInt32>*,Windows.Foundation.EventRegistrationToken*);HRESULT remove_DocumentClosed(EventRegistrationToken);HRESULT add_XpsGenerationFailed(Windows.Foundation.TypedEventHandler`2<XpsUtil.XpsSequentialDocument, UInt64>*,Windows.Foundation.EventRegistrationToken*);HRESULT remove_XpsGenerationFailed(EventRegistrationToken);
 *
 *
 */
#if !defined(____x_ABI_CXpsUtil_CIXpsSequentialDocument_INTERFACE_DEFINED__)
#define ____x_ABI_CXpsUtil_CIXpsSequentialDocument_INTERFACE_DEFINED__
extern const __declspec(selectany) _Null_terminated_ WCHAR InterfaceName_XpsUtil_IXpsSequentialDocument[] = L"XpsUtil.IXpsSequentialDocument";
/* [uuid("3dbdceab-938b-50aa-a51b-876f0f25cd22"), version, object, exclusiveto] */
typedef struct __x_ABI_CXpsUtil_CIXpsSequentialDocumentVtbl
{
    BEGIN_INTERFACE
    HRESULT ( STDMETHODCALLTYPE *QueryInterface)(
    __RPC__in __x_ABI_CXpsUtil_CIXpsSequentialDocument * This,
    /* [in] */ __RPC__in REFIID riid,
    /* [annotation][iid_is][out] */
    _COM_Outptr_  void **ppvObject
    );

ULONG ( STDMETHODCALLTYPE *AddRef )(
    __RPC__in __x_ABI_CXpsUtil_CIXpsSequentialDocument * This
    );

ULONG ( STDMETHODCALLTYPE *Release )(
    __RPC__in __x_ABI_CXpsUtil_CIXpsSequentialDocument * This
    );

HRESULT ( STDMETHODCALLTYPE *GetIids )(
    __RPC__in __x_ABI_CXpsUtil_CIXpsSequentialDocument * This,
    /* [out] */ __RPC__out ULONG *iidCount,
    /* [size_is][size_is][out] */ __RPC__deref_out_ecount_full_opt(*iidCount) IID **iids
    );

HRESULT ( STDMETHODCALLTYPE *GetRuntimeClassName )(
    __RPC__in __x_ABI_CXpsUtil_CIXpsSequentialDocument * This,
    /* [out] */ __RPC__deref_out_opt HSTRING *className
    );

HRESULT ( STDMETHODCALLTYPE *GetTrustLevel )(
    __RPC__in __x_ABI_CXpsUtil_CIXpsSequentialDocument * This,
    /* [OUT ] */ __RPC__out TrustLevel *trustLevel
    );
HRESULT ( STDMETHODCALLTYPE *StartXpsOMGeneration )(
        __x_ABI_CXpsUtil_CIXpsSequentialDocument * This
        );
    HRESULT ( STDMETHODCALLTYPE *GetWatermarkedStream )(
        __x_ABI_CXpsUtil_CIXpsSequentialDocument * This,
        /* [in] */__x_ABI_CXpsUtil_CIXpsPageWatermarker * watermarker,
        /* [out, retval] */__x_ABI_CWindows_CStorage_CStreams_CIInputStream * * result
        );
    HRESULT ( STDMETHODCALLTYPE *GetPage )(
        __x_ABI_CXpsUtil_CIXpsSequentialDocument * This,
        /* [in] */unsigned int pageNumber,
        /* [out, retval] */__x_ABI_CXpsUtil_CIXpsPageWrapper * * result
        );
    HRESULT ( STDMETHODCALLTYPE *PageCount )(
        __x_ABI_CXpsUtil_CIXpsSequentialDocument * This,
        /* [out, retval] */unsigned int * result
        );
    /* [eventadd] */HRESULT ( STDMETHODCALLTYPE *add_PageAdded )(
        __x_ABI_CXpsUtil_CIXpsSequentialDocument * This,
        /* [in] */__FITypedEventHandler_2_XpsUtil__CXpsSequentialDocument_UINT32 * handler,
        /* [retval, out] */EventRegistrationToken * token
        );
    /* [eventremove] */HRESULT ( STDMETHODCALLTYPE *remove_PageAdded )(
        __x_ABI_CXpsUtil_CIXpsSequentialDocument * This,
        /* [in] */EventRegistrationToken token
        );
    /* [eventadd] */HRESULT ( STDMETHODCALLTYPE *add_DocumentClosed )(
        __x_ABI_CXpsUtil_CIXpsSequentialDocument * This,
        /* [in] */__FITypedEventHandler_2_XpsUtil__CXpsSequentialDocument_UINT32 * handler,
        /* [retval, out] */EventRegistrationToken * token
        );
    /* [eventremove] */HRESULT ( STDMETHODCALLTYPE *remove_DocumentClosed )(
        __x_ABI_CXpsUtil_CIXpsSequentialDocument * This,
        /* [in] */EventRegistrationToken token
        );
    /* [eventadd] */HRESULT ( STDMETHODCALLTYPE *add_XpsGenerationFailed )(
        __x_ABI_CXpsUtil_CIXpsSequentialDocument * This,
        /* [in] */__FITypedEventHandler_2_XpsUtil__CXpsSequentialDocument_UINT64 * handler,
        /* [retval, out] */EventRegistrationToken * token
        );
    /* [eventremove] */HRESULT ( STDMETHODCALLTYPE *remove_XpsGenerationFailed )(
        __x_ABI_CXpsUtil_CIXpsSequentialDocument * This,
        /* [in] */EventRegistrationToken token
        );
    END_INTERFACE
    
} __x_ABI_CXpsUtil_CIXpsSequentialDocumentVtbl;

interface __x_ABI_CXpsUtil_CIXpsSequentialDocument
{
    CONST_VTBL struct __x_ABI_CXpsUtil_CIXpsSequentialDocumentVtbl *lpVtbl;
};

#ifdef COBJMACROS
#define __x_ABI_CXpsUtil_CIXpsSequentialDocument_QueryInterface(This,riid,ppvObject) \
( (This)->lpVtbl->QueryInterface(This,riid,ppvObject) )

#define __x_ABI_CXpsUtil_CIXpsSequentialDocument_AddRef(This) \
        ( (This)->lpVtbl->AddRef(This) )

#define __x_ABI_CXpsUtil_CIXpsSequentialDocument_Release(This) \
        ( (This)->lpVtbl->Release(This) )

#define __x_ABI_CXpsUtil_CIXpsSequentialDocument_GetIids(This,iidCount,iids) \
        ( (This)->lpVtbl->GetIids(This,iidCount,iids) )

#define __x_ABI_CXpsUtil_CIXpsSequentialDocument_GetRuntimeClassName(This,className) \
        ( (This)->lpVtbl->GetRuntimeClassName(This,className) )

#define __x_ABI_CXpsUtil_CIXpsSequentialDocument_GetTrustLevel(This,trustLevel) \
        ( (This)->lpVtbl->GetTrustLevel(This,trustLevel) )

#define __x_ABI_CXpsUtil_CIXpsSequentialDocument_StartXpsOMGeneration(This) \
    ( (This)->lpVtbl->StartXpsOMGeneration(This) )

#define __x_ABI_CXpsUtil_CIXpsSequentialDocument_GetWatermarkedStream(This,watermarker,result) \
    ( (This)->lpVtbl->GetWatermarkedStream(This,watermarker,result) )

#define __x_ABI_CXpsUtil_CIXpsSequentialDocument_GetPage(This,pageNumber,result) \
    ( (This)->lpVtbl->GetPage(This,pageNumber,result) )

#define __x_ABI_CXpsUtil_CIXpsSequentialDocument_PageCount(This,result) \
    ( (This)->lpVtbl->PageCount(This,result) )

#define __x_ABI_CXpsUtil_CIXpsSequentialDocument_add_PageAdded(This,handler,token) \
    ( (This)->lpVtbl->add_PageAdded(This,handler,token) )

#define __x_ABI_CXpsUtil_CIXpsSequentialDocument_remove_PageAdded(This,token) \
    ( (This)->lpVtbl->remove_PageAdded(This,token) )

#define __x_ABI_CXpsUtil_CIXpsSequentialDocument_add_DocumentClosed(This,handler,token) \
    ( (This)->lpVtbl->add_DocumentClosed(This,handler,token) )

#define __x_ABI_CXpsUtil_CIXpsSequentialDocument_remove_DocumentClosed(This,token) \
    ( (This)->lpVtbl->remove_DocumentClosed(This,token) )

#define __x_ABI_CXpsUtil_CIXpsSequentialDocument_add_XpsGenerationFailed(This,handler,token) \
    ( (This)->lpVtbl->add_XpsGenerationFailed(This,handler,token) )

#define __x_ABI_CXpsUtil_CIXpsSequentialDocument_remove_XpsGenerationFailed(This,token) \
    ( (This)->lpVtbl->remove_XpsGenerationFailed(This,token) )


#endif /* COBJMACROS */


EXTERN_C const IID IID___x_ABI_CXpsUtil_CIXpsSequentialDocument;
#endif /* !defined(____x_ABI_CXpsUtil_CIXpsSequentialDocument_INTERFACE_DEFINED__) */


/*
 *
 * Interface XpsUtil.IXpsSequentialDocumentFactory
 *
 * Interface is a part of the implementation of type XpsUtil.XpsSequentialDocument
 *
 *
 * The IID for this interface was automatically generated by MIDLRT.
 *
 * Interface IID generation seed: XpsUtil.IXpsSequentialDocumentFactory:HRESULT CreateInstance(Windows.Graphics.Printing.Workflow.PrintWorkflowObjectModelSourceFileContent*,XpsUtil.XpsSequentialDocument**);
 *
 *
 */
#if !defined(____x_ABI_CXpsUtil_CIXpsSequentialDocumentFactory_INTERFACE_DEFINED__)
#define ____x_ABI_CXpsUtil_CIXpsSequentialDocumentFactory_INTERFACE_DEFINED__
extern const __declspec(selectany) _Null_terminated_ WCHAR InterfaceName_XpsUtil_IXpsSequentialDocumentFactory[] = L"XpsUtil.IXpsSequentialDocumentFactory";
/* [uuid("b2b480dd-91de-5ab7-be85-e11783c5236d"), version, object, exclusiveto] */
typedef struct __x_ABI_CXpsUtil_CIXpsSequentialDocumentFactoryVtbl
{
    BEGIN_INTERFACE
    HRESULT ( STDMETHODCALLTYPE *QueryInterface)(
    __RPC__in __x_ABI_CXpsUtil_CIXpsSequentialDocumentFactory * This,
    /* [in] */ __RPC__in REFIID riid,
    /* [annotation][iid_is][out] */
    _COM_Outptr_  void **ppvObject
    );

ULONG ( STDMETHODCALLTYPE *AddRef )(
    __RPC__in __x_ABI_CXpsUtil_CIXpsSequentialDocumentFactory * This
    );

ULONG ( STDMETHODCALLTYPE *Release )(
    __RPC__in __x_ABI_CXpsUtil_CIXpsSequentialDocumentFactory * This
    );

HRESULT ( STDMETHODCALLTYPE *GetIids )(
    __RPC__in __x_ABI_CXpsUtil_CIXpsSequentialDocumentFactory * This,
    /* [out] */ __RPC__out ULONG *iidCount,
    /* [size_is][size_is][out] */ __RPC__deref_out_ecount_full_opt(*iidCount) IID **iids
    );

HRESULT ( STDMETHODCALLTYPE *GetRuntimeClassName )(
    __RPC__in __x_ABI_CXpsUtil_CIXpsSequentialDocumentFactory * This,
    /* [out] */ __RPC__deref_out_opt HSTRING *className
    );

HRESULT ( STDMETHODCALLTYPE *GetTrustLevel )(
    __RPC__in __x_ABI_CXpsUtil_CIXpsSequentialDocumentFactory * This,
    /* [OUT ] */ __RPC__out TrustLevel *trustLevel
    );
HRESULT ( STDMETHODCALLTYPE *CreateInstance )(
        __x_ABI_CXpsUtil_CIXpsSequentialDocumentFactory * This,
        /* [in] */__x_ABI_CWindows_CGraphics_CPrinting_CWorkflow_CIPrintWorkflowObjectModelSourceFileContent * sourceFileContent,
        /* [retval, out] */__x_ABI_CXpsUtil_CIXpsSequentialDocument * * value
        );
    END_INTERFACE
    
} __x_ABI_CXpsUtil_CIXpsSequentialDocumentFactoryVtbl;

interface __x_ABI_CXpsUtil_CIXpsSequentialDocumentFactory
{
    CONST_VTBL struct __x_ABI_CXpsUtil_CIXpsSequentialDocumentFactoryVtbl *lpVtbl;
};

#ifdef COBJMACROS
#define __x_ABI_CXpsUtil_CIXpsSequentialDocumentFactory_QueryInterface(This,riid,ppvObject) \
( (This)->lpVtbl->QueryInterface(This,riid,ppvObject) )

#define __x_ABI_CXpsUtil_CIXpsSequentialDocumentFactory_AddRef(This) \
        ( (This)->lpVtbl->AddRef(This) )

#define __x_ABI_CXpsUtil_CIXpsSequentialDocumentFactory_Release(This) \
        ( (This)->lpVtbl->Release(This) )

#define __x_ABI_CXpsUtil_CIXpsSequentialDocumentFactory_GetIids(This,iidCount,iids) \
        ( (This)->lpVtbl->GetIids(This,iidCount,iids) )

#define __x_ABI_CXpsUtil_CIXpsSequentialDocumentFactory_GetRuntimeClassName(This,className) \
        ( (This)->lpVtbl->GetRuntimeClassName(This,className) )

#define __x_ABI_CXpsUtil_CIXpsSequentialDocumentFactory_GetTrustLevel(This,trustLevel) \
        ( (This)->lpVtbl->GetTrustLevel(This,trustLevel) )

#define __x_ABI_CXpsUtil_CIXpsSequentialDocumentFactory_CreateInstance(This,sourceFileContent,value) \
    ( (This)->lpVtbl->CreateInstance(This,sourceFileContent,value) )


#endif /* COBJMACROS */


EXTERN_C const IID IID___x_ABI_CXpsUtil_CIXpsSequentialDocumentFactory;
#endif /* !defined(____x_ABI_CXpsUtil_CIXpsSequentialDocumentFactory_INTERFACE_DEFINED__) */


/*
 *
 * Class XpsUtil.XpsSequentialDocument
 *
 * RuntimeClass can be activated.
 *
 * Class implements the following interfaces:
 *    XpsUtil.IXpsSequentialDocument ** Default Interface **
 *
 * Class Threading Model:  Both Single and Multi Threaded Apartment
 *
 * Class Marshaling Behavior:  Agile - Class is agile
 *
 */

#ifndef RUNTIMECLASS_XpsUtil_XpsSequentialDocument_DEFINED
#define RUNTIMECLASS_XpsUtil_XpsSequentialDocument_DEFINED
extern const __declspec(selectany) _Null_terminated_ WCHAR RuntimeClass_XpsUtil_XpsSequentialDocument[] = L"XpsUtil.XpsSequentialDocument";
#endif


#endif // defined(__cplusplus)
#pragma pop_macro("MIDL_CONST_ID")
#endif // __XpsUtil_p_h__

#endif // __XpsUtil_h__
