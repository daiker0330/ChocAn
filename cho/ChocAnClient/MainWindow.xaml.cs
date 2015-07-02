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

namespace ChocAnClient
{
    /// <summary>
    /// MainWindow.xaml 的交互逻辑
    /// </summary>
    /// 

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
            if(recv == false)
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
            if (recv == 1)
            {
                ((Button)sender).Content = "IsValid(" + recv.ToString() + ")";
            }
            else if (recv == 0)
            {
                ((Button)sender).Content = "IsValid(" + recv.ToString() + ")";
            }
            else if (recv == -1)
            {
                ((Button)sender).Content = "IsValid(" + recv.ToString() + ")";
            }
        }
    }
}
