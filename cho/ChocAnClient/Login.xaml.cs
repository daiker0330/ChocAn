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
using System.Windows.Shapes;

namespace ChocAnClient
{
    /// <summary>
    /// Login.xaml 的交互逻辑
    /// </summary>
    public partial class Login : Window
    {
        public Login(RemoteProcessCall network)
        {
            InitializeComponent();
            WindowStartupLocation = WindowStartupLocation.CenterScreen;
            net = network;   
        }


        public const int ServerID_length = 8;//服务者ID长度
        public bool exitbutton = false;
        public RemoteProcessCall net = new RemoteProcessCall();
        private void submit_Click(object sender, RoutedEventArgs e)
        {
            //if (login.Text == "hahaha")
           
        }

        private void exit_Click(object sender, RoutedEventArgs e)
        {
            exitbutton = true;
            wlogin.Close();
        }

        private void login_TextChanged(object sender, TextChangedEventArgs e)
        {
            
            if (login.Text.Length== ServerID_length)
            {
                if (net.SignIn(login.Text))
                //if(login.Text=="123456789")
                {
                    MessageBox.Show("登陆成功");
                    wlogin.Close();
                }
                else
                {
                    MessageBox.Show("没这个账号，请重新输入");
                }
                exitbutton = false;
            }
        }
    }
}
