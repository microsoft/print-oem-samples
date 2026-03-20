using Windows.ApplicationModel;
using Microsoft.UI.Xaml.Controls;
using Microsoft.UI.Xaml.Navigation;

namespace PrintSupportApplication
{
    public sealed partial class AppInfo : Page
    {
        public AppInfo()
        {
            InitializeComponent();
        }

        protected override void OnNavigatedTo(NavigationEventArgs e)
        {
            PackageFamilyNameTextBox.Text = Package.Current.Id.FamilyName;
        }
    }
}
