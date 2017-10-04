// Copyright(c) Microsoft Corporation.All rights reserved.
//
//   THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
//   ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
//   THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
//   PARTICULAR PURPOSE.
//
using System;
using System.ComponentModel;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using Windows.UI.Xaml.Media.Imaging;

// The Blank Page item template is documented at http://go.microsoft.com/fwlink/?LinkId=402352&clcid=0x409

namespace WorkflowAndWSDACombinedSample
{
    /// <summary>
    /// An empty page that can be used on its own or navigated to within a Frame.
    /// </summary>
    public sealed partial class MainPage : Page, INotifyPropertyChanged
    {
        #region Public properties
        public event PropertyChangedEventHandler PropertyChanged;
        public static string MainTitleLabel { get => LocalizedStrings.MainTitleLabel;}
        public static string InkLevelLabel { get => LocalizedStrings.InkLevelLabel;}
        #endregion

        #region Public member functions
        public MainPage()
        {
            this.InitializeComponent();
            DataContext = this;
        }

        /// <summary>
        /// Called when the BiDi query gets a response, e.g.for the Ink Level query
        /// </summary>
        /// <param name="sender">Sender</param>
        /// <param name="e">string</param>
        public void OnQueryResultReceived(object sender, string e)
        {
            throw new NotImplementedException();
        }
        #endregion

        #region Internal member functions
        /// <summary>
        /// Return the named resource string
        /// </summary>
        /// <param name="resourceName"></param>
        /// <returns>A string containing the requested resource string value</returns>
        internal static string GetResourceString(string resourceName)
        {
            string theResourceString = "##Failed to get resource string##";
            var resourceLoader = Windows.ApplicationModel.Resources.ResourceLoader.GetForCurrentView();
            theResourceString = resourceLoader.GetString(resourceName);
            return theResourceString;
        }

        /// <summary>
        /// Load the bitmap and immediately cache it so that the file isn't left locked.
        /// That way it can be updated on disk at any time
        /// </summary>
        /// <param name="resourceLocator">Uri pointing to the next heartbeat image file</param>
        /// <returns></returns>
        internal static BitmapImage LoadAndCacheBitmapFromUri(Uri resourceLocator)
        {
            // Define a BitmapImage.
            var bi = new BitmapImage()
            {
                // Use UriSource to take advantage of automatic caching
                UriSource = resourceLocator
            };

            return bi;
        }
        #endregion

        #region Private member functions
        private void Page_Loaded(object sender, RoutedEventArgs e)
        {
            ((Page)sender).Focus(FocusState.Programmatic);
            ((Page)sender).Loaded -= Page_Loaded;
        }

        /// <summary>
        /// Implement the INotifyPropertyChanged method, OnPropertyChanged
        /// </summary>
        /// <param name="name">Property name of the property which changed</param>
        private void OnPropertyChanged(string name)
        {
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(name));
        }
        #endregion
    }
}
