#pragma once

#include "LocalStorageHelper.g.h"
#include <winrt/Windows.Devices.Printers.h>

namespace winrt::BackgroundTasks::implementation
{
    struct LocalStorageHelper : LocalStorageHelperT<LocalStorageHelper>
    {
        LocalStorageHelper() = default;

        static bool GetWatermarkTextPropertiesFromLocalStorage(hstring& watermarkText, int32_t& fontSize, double& xPos, double& yPos);
        static void SaveWatermarkTextPropertiesToLocalStorage(hstring const& Watermark, int32_t const& fontSize, double const& xPos, double const& yPos);
        static void ResetWatermarkTextAndImage();
        static void SaveImagePropertiesToLocalStorage(hstring const& imageFile, double const& dpiX, double const& dpiY, int32_t const& imageWidth, int32_t const& imageHeight);
        static bool GetImagePropertiesFromLocalStorage(hstring& imageFile, double& dpiX, double& dpiY, int32_t& width, int32_t& height);
        static bool GetDisableIppCompression(bool& disableIppCompression);
        static void SetDisableIppCompression(bool disableIppCompression);
        static bool GetEnablePsaCustomToast(bool& enablePsaCustomToast);
        static void SetEnablePsaCustomToast(bool enablePsaCustomToast);

        static void SaveCustomIppAttributesToLocalStorage(Windows::Foundation::Collections::IMap<hstring, Windows::Devices::Printers::IppAttributeValue> attributes);
        static Windows::Foundation::Collections::IMap<hstring, Windows::Devices::Printers::IppAttributeValue> GetCustomIppAttributesFromLocalStorage();
    private:
        static const hstring WatermarkTextSizeCanonicalName;
        static const hstring WatermartTextCanonicalName;
        static const hstring WatermarkXposCanonicalName;
        static const hstring WatermarkYposCanonicalName;

        static const hstring ImagePathCanonicalName;
        static const hstring ImageDpiXCanonicalName;
        static const hstring ImageDpiYCanonicalName;
        static const hstring ImageWidthCanonicalName;
        static const hstring ImageHeightCanonicalName;
        static const hstring ImageUsingCanonicalName;

        static const hstring IppAttributeValueKeysCanonicalName;
        static const hstring DisableIppCompression;
        static const hstring EnablePsaCustomToast;
    };
}

namespace winrt::BackgroundTasks::factory_implementation
{
    struct LocalStorageHelper : LocalStorageHelperT<LocalStorageHelper, implementation::LocalStorageHelper>
    {
    };
}
