using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using ChocAnClient;

namespace testClient
{
    /// <summary>
    /// MainWindow.xaml 的交互逻辑
    /// </summary>
    public partial class MainWindow : Window
    {
        string result;
        RemoteProcessCall network = new RemoteProcessCall();
        public MainWindow()
        {
            InitializeComponent();
        }

        private void Button_Click(object sender, RoutedEventArgs e)
        {
            bool recv = network.SignIn("123456789");
            if (recv == false)
            {
                result = "SignIn(false)";
                ((Button)sender).Content = result;
            }
            else
            {
                result = "SignIn(true)";
                ((Button)sender).Content = result;
            }

        }

        private void Button_Click_1(object sender, RoutedEventArgs e)
        {
            network.init();
        }

        private void Button_Click_2(object sender, RoutedEventArgs e)
        {
            int recv = network.IsValid("123456789");
            ((Button)sender).Content = "IsValid(" + recv.ToString() + ")";
        }

        private void Button_Click_3(object sender, RoutedEventArgs e)
        {
            string recv = network.GetServerName("123456789");
            ((Button)sender).Content = "GetServerName(" + recv.ToString() + ")";

        }

        private void Button_Click_4(object sender, RoutedEventArgs e)
        {
            double recv = network.GetServerPrice("123456789");
            ((Button)sender).Content = "GetServerPrice(" + recv.ToString() + ")";
        }

        private void Button_Click_5(object sender, RoutedEventArgs e)
        {
            ServerRecord sr = new ServerRecord();
            bool recv = network.SaveServerRecord(sr);
            ((Button)sender).Content = "SaveServerRecord(" + recv.ToString() + ")";
        }

        private void Button_Click_6(object sender, RoutedEventArgs e)
        {
            double recv = network.GetProviderSum("123456789");
            ((Button)sender).Content = "GetProviderSum(" + recv.ToString() + ")";
        }
    }
}
