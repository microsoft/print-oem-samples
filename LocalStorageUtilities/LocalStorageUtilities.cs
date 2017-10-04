// Copyright(c) Microsoft Corporation.All rights reserved.
//
//   THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
//   ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
//   THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
//   PARTICULAR PURPOSE.
//
//   This is a sample implementation which saves parameters to LocalSettings in order to allow the Foreground Task to
//   pass properties to the Background Task or vice-versa. The SetStorageKeyPrefix(prefix) function allows these parameters to be
//   prefixed in a unique manner for each Print Workflow session; for example, you could use the SessionID from the Configuration
//   object, which is available to both the Foreground and Background Tasks, so that the Background Task can find the
//   parameters associated only with its corresponding Foreground Task, should that distinction be necessary
//
using System;
using System.Threading.Tasks;
using Windows.Graphics.Imaging;
using Windows.Storage;
using Windows.Storage.Streams;

namespace WorkflowAndWSDACombinedSample
{
    public class LocalStorageUtilities
    {
        #region Public member functions
        /// <summary>
        /// Constructor
        /// </summary>
        public LocalStorageUtilities()
        {
            localSettings = Windows.Storage.ApplicationData.Current.LocalSettings;
        }

        /// <summary>
        /// Initialize the local storage key with a disambiguation prefix if desired
        /// </summary>
        /// <param name="prefix"></param>
        public void SetStorageKeyPrefix(string prefix)
        {
            localStorageVariablePrefix = prefix;
        }

        /// <summary>
        /// Save the state of the check box to use standalone settings instead of displaying UI
        /// </summary>
        /// <param name="useStandaloneSettings"></param>
        public void SetUseStandaloneSettings(bool useStandaloneSettings)
        {
            localSettings.Values[UseStandaloneSettingsCanonicalName] = useStandaloneSettings;
        }

        /// <summary>
        /// Get the state of the check box to use standalone settings instead of displaying UI. Defaults to false if never set.
        /// </summary>
        /// <returns>A bool which if true means a value was found in local storage and the check box was set.</returns>
        public bool GetUseStandaloneSettings()
        {
            bool useStandaloneSettings = false;
            object useStandaloneSettingsAsOject = localSettings.Values[UseStandaloneSettingsCanonicalName];
            if (useStandaloneSettingsAsOject != null)
            {
                useStandaloneSettings = (bool)localSettings.Values[UseStandaloneSettingsCanonicalName];
            }

            return useStandaloneSettings;
        }

        /// <summary>
        /// Save the watermark text to local storage.
        /// </summary>
        /// <param name="watermarkText"></param>
        public void SaveWatermarkTextToLocalStorage(string watermarkText)
        {
            localSettings.Values[localStorageVariablePrefix + WatermarkCanonicalName] = watermarkText;
        }

        /// <summary>
        /// Retrieve the watermark text from local storage or return empty string if it isn't there, or is an empty string
        /// </summary>
        /// <returns>Watermark text as object, but it is a string or null</returns>
        public string GetWatermarkTextFromLocalStorage()
        {
            string watermarkText = "";
            object watermarkTextAsObject = localSettings.Values[localStorageVariablePrefix + WatermarkCanonicalName];
            if (watermarkTextAsObject != null)
            {
                watermarkText = (string)watermarkTextAsObject;
            }
            return watermarkText;
        }

        /// <summary>
        /// Retrieve the image path from local storage or return null if it isn't there
        /// </summary>
        /// <returns>Watermark text as string or empty string if null</returns>
        public string GetImagePathFromLocalStorage()
        {
            // Look for the image path text in local storage
            GetValueFromLocalStorage<string>(localStorageVariablePrefix + ImagePathCanonicalName, out string imagePath);
            return imagePath;
        }

        /// <summary>
        /// Delete the image path from local storage 
        /// </summary>
        public void DeleteImagePathFromLocalStorage()
        {
            localSettings.Values.Remove(localStorageVariablePrefix + ImagePathCanonicalName);
        }

        /// <summary>
        /// Save the printer name to local storage.
        /// </summary>
        /// <param name="printerName"></param>
        public void SavePrinterNameToLocalStorage(string printerName)
        {
            localSettings.Values[localStorageVariablePrefix + PrinterNameCanonicalName] = printerName;
        }

        /// <summary>
        /// Retrieve the printer name from local storage or return empty string if it isn't there, or is an empty string
        /// </summary>
        /// <returns>Printer name as string or empty string if null</returns>
        public string GetPrinterNameFromLocalStorage()
        {
            string printerName = "";
            object printerNameAsObject = localSettings.Values[localStorageVariablePrefix + PrinterNameCanonicalName];
            if (printerNameAsObject != null)
            {
                printerName = (string)printerNameAsObject;
            }
            return printerName;
        }

        /// <summary>
        /// Delete the watermark text from local storage.
        /// </summary>
        public void DeleteWatermarkTextFromLocalStorage()
        {
            localSettings.Values.Remove(localStorageVariablePrefix + WatermarkCanonicalName);
        }

        /// <summary>
        /// Delete the printer name from local storage.
        /// </summary>
        public void DeletePrinterNameFromLocalStorage()
        {
            localSettings.Values.Remove(localStorageVariablePrefix + PrinterNameCanonicalName);
        }

        /// <summary>
        /// Save the image properties to local storage
        /// </summary>
        /// <param name="imageFile">Image file</param>
        /// <param name="dpiX">Image dots per inch X</param>
        /// <param name="dpiY">Image dots per inch Y</param>
        /// <param name="imageWidth">Image width in pixels</param>
        /// <param name="imageHeight">Image height in pixels</param>
        public void SaveImagePropertiesToLocalStorage(string imageFile, double dpiX, double dpiY, int imageWidth, int imageHeight)
        {
            localSettings.Values[localStorageVariablePrefix + ImagePathCanonicalName] = imageFile;
            localSettings.Values[localStorageVariablePrefix + ImageDpiXCanonicalName] = dpiX;
            localSettings.Values[localStorageVariablePrefix + ImageDpiYCanonicalName] = dpiY;
            localSettings.Values[localStorageVariablePrefix + ImageWidthCanonicalName] = imageWidth;
            localSettings.Values[localStorageVariablePrefix + ImageHeightCanonicalName] = imageHeight;
        }

        /// <summary>
        /// Get the image properties from local storage
        /// </summary>
        /// <param name="imageFile">Image file, e.g. "Assets\\MyImage.jpg"</param>
        /// <param name="dpiX">Image dots per inch X</param>
        /// <param name="dpiY">Image dots per inch Y</param>
        /// <param name="width">Image width in pixels</param>
        /// <param name="height">Image height in pixels</param>
        public void GetImagePropertiesFromLocalStorage(out string imageFile, out double dpiX, out double dpiY, out int width, out int height)
        {
            imageFile = GetImagePathFromLocalStorage();
            GetValueFromLocalStorage(localStorageVariablePrefix + ImageDpiXCanonicalName, out dpiX);
            GetValueFromLocalStorage(localStorageVariablePrefix + ImageDpiYCanonicalName, out dpiY);
            GetValueFromLocalStorage(localStorageVariablePrefix + ImageWidthCanonicalName, out width);
            GetValueFromLocalStorage(localStorageVariablePrefix + ImageHeightCanonicalName, out height);
        }

        /// <summary>
        /// Delete the image properties from local storage
        /// </summary>
        public void DeleteImagePropertiesFromLocalStorage()
        {
            DeleteImagePathFromLocalStorage();
            localSettings.Values.Remove(localStorageVariablePrefix + ImageDpiXCanonicalName);
            localSettings.Values.Remove(localStorageVariablePrefix + ImageDpiYCanonicalName);
            localSettings.Values.Remove(localStorageVariablePrefix + ImageWidthCanonicalName);
            localSettings.Values.Remove(localStorageVariablePrefix + ImageHeightCanonicalName);
        }
        /// <summary>
        /// Get the metadata for the specified image so that its size and resolution can be determined
        /// Typical usage would be
        ///                         softwareBitmap = await GetSoftwareBitmapFromFileAsync(file);
        /// </summary>
        /// <param name="file">Image file</param>
        /// <returns>Task<SoftwareBitmap></returns>
        public async Task<SoftwareBitmap> GetSoftwareBitmapFromFileAsync(StorageFile file)
        {
            SoftwareBitmap softwareBitmap = null;

            if (file != null)
            {
                using (IRandomAccessStream fileStream = await file.OpenAsync(FileAccessMode.Read))
                {
                    BitmapDecoder decoder = await BitmapDecoder.CreateAsync(fileStream);

                    // Get the SoftwareBitmap representation of the file
                    softwareBitmap = await decoder.GetSoftwareBitmapAsync();
                }
            }

            return softwareBitmap;
        }
        #endregion

        #region Private member functions
        /// <summary>
        /// Retrieve required property from local storage
        /// </summary>
        /// <typeparam name="T"></typeparam>
        /// <param name="localSettings">Windows.Storage.ApplicationDataContainer in app local storage</param>
        /// <param name="propertyName">Name of proprty to retrieve</param>
        /// <param name="requiredValue">Property value returned</param>
        /// <returns>True if property was found, else false if not</returns>
        private bool GetValueFromLocalStorage<T>(string propertyName, out T requiredValue)
        {
            bool foundProperty = localSettings.Values.TryGetValue(propertyName, out object o);
            if (foundProperty)
            {
                requiredValue = (T)o;
            }
            else
            {
                requiredValue = default(T);
            }

            return foundProperty;
        }
        #endregion

        #region Private member variables
        // START: These are the canonical name you want for the properties you wish to pass through to the background task
        private const string ImagePathCanonicalName = "WatermarkImage";
        private const string ImageDpiXCanonicalName = "WatermarkImageDpiX";
        private const string ImageDpiYCanonicalName = "WatermarkImageDpiY";
        private const string ImageWidthCanonicalName = "WatermarkImageWidth";
        private const string ImageHeightCanonicalName = "WatermarkImageHeight";
        private const string PrinterNameCanonicalName = "PrinterName";
        private const string UseStandaloneSettingsCanonicalName = "UseStandaloneSettings";
        private const string WatermarkCanonicalName = "WatermarkText";
        // END: These are the canonical name you want for the properties you wish to pass through to the background task

        private Windows.Storage.ApplicationDataContainer localSettings;
        private string localStorageVariablePrefix = "";
        #endregion
    }
}
