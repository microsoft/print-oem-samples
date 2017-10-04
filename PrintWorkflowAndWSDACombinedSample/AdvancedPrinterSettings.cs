using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Windows.Devices.Printers.Extensions;

namespace WorkflowAndWSDACombinedSample
{
    class AdvancedPrinterSettings
    {
        public void LoadAdvancedPrintSettingsContext(PrintTaskSettingsActivatedEventArgs args)
        {
            Config = args.Configuration;
            Context = Config.PrinterExtensionContext;
            LoadScenario(typeof(DeviceAppForPrinters.Preferences));
        }
    }
}
