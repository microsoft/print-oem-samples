// Copyright(c) Microsoft Corporation.All rights reserved.
//
//   THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
//   ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
//   THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
//   PARTICULAR PURPOSE.
//
//   This is a sample implementation which provides some utility functions which can be shared by both background and foreground tasks
//
using System;
using System.Threading.Tasks;
using Windows.Graphics.Imaging;
using Windows.Storage;
using Windows.Storage.Streams;
using Windows.UI.Notifications;

namespace WorkflowAndWSDACombinedSample
{
    public static class UtilitiesLibrary
    {
        #region Public member functions
        /// <summary>
        /// Display a toast notifcation with optional image
        /// </summary>
        /// <param name="message">Test of message</param>
        /// <param name="imageName">Imahe path, e.g. @"Assets\mylogo.jpg" or empty string for no image</param>
        public static void SendToastNotification(string message, string imageName)
        {
            if (!suppressToast)
            {
                var notificationXml = ToastNotificationManager.GetTemplateContent(ToastTemplateType.ToastImageAndText01);
                // Find the "text" node and add the message, prefixed by the time
                var toastElements = notificationXml.GetElementsByTagName("text");
                toastElements[0].AppendChild(notificationXml.CreateTextNode(System.DateTime.Now + " " + message));
                // If an image ghas been specified use that else falls back to a default
                if (string.IsNullOrEmpty(imageName))
                {
                    imageName = @"Assets\contoso_logo.jpg";
                }

                // Fnd the image node and insert the required image path
                var imageElement = notificationXml.GetElementsByTagName("image");
                imageElement[0].Attributes[1].NodeValue = imageName;
                // Create and display the toast using the filled-out template
                var toastNotification = new ToastNotification(notificationXml);
                ToastNotificationManager.CreateToastNotifier().Show(toastNotification);
            }
        }
        #endregion

        #region Private member functions
        #endregion

        #region Private member variables
        private static bool suppressToast = false;
        #endregion
    }
}

