#include "pch.h"
#include "LocalStorageHelper.h"
#if __has_include("LocalStorageHelper.g.cpp")
#include "LocalStorageHelper.g.cpp"
#endif
#include <winrt/Windows.Storage.h>

using namespace winrt;
using namespace Windows::Devices::Printers;
using namespace Windows::Storage;

namespace winrt::BackgroundTasks::implementation
{
    const hstring LocalStorageHelper::WatermarkTextSizeCanonicalName = L"WatermarkTextSize";
    const hstring LocalStorageHelper::WatermartTextCanonicalName = L"WatermarkText";
    const hstring LocalStorageHelper::WatermarkXposCanonicalName = L"WatermarkXpos";
    const hstring LocalStorageHelper::WatermarkYposCanonicalName = L"WatermarkYpos";

    const hstring LocalStorageHelper::ImagePathCanonicalName = L"WatermarkImage";
    const hstring LocalStorageHelper::ImageDpiXCanonicalName = L"WatermarkImageDpiX";
    const hstring LocalStorageHelper::ImageDpiYCanonicalName = L"WatermarkImageDpiY";
    const hstring LocalStorageHelper::ImageWidthCanonicalName = L"WatermarkImageWidth";
    const hstring LocalStorageHelper::ImageHeightCanonicalName = L"WatermarkImageHeight";
    const hstring LocalStorageHelper::ImageUsingCanonicalName = L"WatermarkImageSet";

    const hstring LocalStorageHelper::IppAttributeValueKeysCanonicalName = L"IppAttributeValueKeys";
    const hstring LocalStorageHelper::DisableIppCompression = L"DisableIppCompression";
    const hstring LocalStorageHelper::EnablePsaCustomToast = L"EnablePsaCustomToast";

    template<typename T>
    bool GetValueFromLocalStorage(ApplicationDataContainer const& localSettings, winrt::hstring propertyName, T& requiredValue)
    {
        bool foundProperty = false;
        auto value = localSettings.Values().TryLookup(propertyName);
        if (value)
        {
            requiredValue = winrt::unbox_value<T>(value);
            foundProperty = true;
        }
        else
        {
            requiredValue = T{};
        }
        return foundProperty;
    }

    bool LocalStorageHelper::GetWatermarkTextPropertiesFromLocalStorage(hstring& watermarkText,
        int32_t& fontSize, double& xPos, double& yPos)
    {
        auto localSettings = ApplicationData::Current().LocalSettings();
        bool retval = true;
        if (!GetValueFromLocalStorage(localSettings, WatermartTextCanonicalName, watermarkText))
        {
            retval = false;
        }
        if (!GetValueFromLocalStorage(localSettings, WatermarkTextSizeCanonicalName, fontSize))
        {
            retval = false;
        }
        if (!GetValueFromLocalStorage(localSettings, WatermarkXposCanonicalName, xPos))
        {
            retval = false;
        }
        if (!GetValueFromLocalStorage(localSettings, WatermarkYposCanonicalName, yPos))
        {
            retval = false;
        }

        return retval && !watermarkText.empty();
    }

    void LocalStorageHelper::SaveWatermarkTextPropertiesToLocalStorage(hstring const& watermark,
        int32_t const& fontSize, double const& xPos, double const& yPos)
    {
        auto values = ApplicationData::Current().LocalSettings().Values();
        values.Insert(WatermartTextCanonicalName, winrt::box_value(watermark));
        values.Insert(WatermarkTextSizeCanonicalName, winrt::box_value(fontSize));
        values.Insert(WatermarkXposCanonicalName, winrt::box_value(xPos));
        values.Insert(WatermarkYposCanonicalName, winrt::box_value(yPos));
    }

    void LocalStorageHelper::ResetWatermarkTextAndImage()
    {
        SaveWatermarkTextPropertiesToLocalStorage(L"", 0, 0.0, 0.0);
        ApplicationData::Current().LocalSettings().Values().Insert(ImageUsingCanonicalName, winrt::box_value(false));
    }

    void LocalStorageHelper::SaveImagePropertiesToLocalStorage(hstring const& imageFile,
        double const& dpiX, double const& dpiY, int32_t const& imageWidth, int32_t const& imageHeight)
    {
        auto values = ApplicationData::Current().LocalSettings().Values();
        values.Insert(ImagePathCanonicalName, winrt::box_value(imageFile));
        values.Insert(ImageDpiXCanonicalName, winrt::box_value(dpiX));
        values.Insert(ImageDpiYCanonicalName, winrt::box_value(dpiY));
        values.Insert(ImageWidthCanonicalName, winrt::box_value(imageWidth));
        values.Insert(ImageHeightCanonicalName, winrt::box_value(imageHeight));
        values.Insert(ImageUsingCanonicalName, winrt::box_value(true));
    }

    bool LocalStorageHelper::GetImagePropertiesFromLocalStorage(hstring& imageFile,
        double& dpiX, double& dpiY, int32_t& width, int32_t& height)
    {
        auto localSettings = ApplicationData::Current().LocalSettings();
        bool retval = true;
        if (!GetValueFromLocalStorage(localSettings, ImagePathCanonicalName, imageFile))
        {
            retval = false;
        }
        if (!GetValueFromLocalStorage(localSettings, ImageDpiXCanonicalName, dpiX))
        {
            retval = false;
        }
        if (!GetValueFromLocalStorage(localSettings, ImageHeightCanonicalName, height))
        {
            retval = false;
        }
        if (!GetValueFromLocalStorage(localSettings, ImageDpiYCanonicalName, dpiY))
        {
            retval = false;
        }
        if (!GetValueFromLocalStorage(localSettings, ImageWidthCanonicalName, width))
        {
            retval = false;
        }
        bool isUsingWatermarkPicture;
        return retval && GetValueFromLocalStorage(localSettings, ImageUsingCanonicalName, isUsingWatermarkPicture) && isUsingWatermarkPicture;
    }

    bool LocalStorageHelper::GetDisableIppCompression(bool& disableIppCompression)
    {
        disableIppCompression = false;
        auto localSettings = ApplicationData::Current().LocalSettings();
        return GetValueFromLocalStorage(localSettings, DisableIppCompression, disableIppCompression);
    }

    void LocalStorageHelper::SetDisableIppCompression(bool disableIppCompression)
    {
        auto values = ApplicationData::Current().LocalSettings().Values();
        values.Insert(DisableIppCompression, winrt::box_value(disableIppCompression));
    }

    bool LocalStorageHelper::GetEnablePsaCustomToast(bool& enablePsaCustomToast)
    {
        enablePsaCustomToast = false;
        auto localSettings = ApplicationData::Current().LocalSettings();
        return GetValueFromLocalStorage(localSettings, EnablePsaCustomToast, enablePsaCustomToast);
    }

    void LocalStorageHelper::SetEnablePsaCustomToast(bool enablePsaCustomToast)
    {
        auto values = ApplicationData::Current().LocalSettings().Values();
        values.Insert(EnablePsaCustomToast, winrt::box_value(enablePsaCustomToast));
    }

    void LocalStorageHelper::SaveCustomIppAttributesToLocalStorage(Windows::Foundation::Collections::IMap<hstring, Windows::Devices::Printers::IppAttributeValue> /*attributes*/)
    {
        throw winrt::hresult_not_implemented();
    }

    Windows::Foundation::Collections::IMap<hstring, Windows::Devices::Printers::IppAttributeValue> LocalStorageHelper::GetCustomIppAttributesFromLocalStorage()
    {
        throw winrt::hresult_not_implemented();
    }
}
