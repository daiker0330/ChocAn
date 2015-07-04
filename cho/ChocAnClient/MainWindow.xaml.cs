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
using System.IO;

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
        /*****************UI*******************************/
        public bool full_small = true;
        public double w_left = 0;
        public double w_top = 0;
        public string suplier = "";
        public const int MemberID_length = 8;
        /*****************UI******************************/
        public MainWindow()
        {
            InitializeComponent();
            InitWindowLocalInCenter();
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

        /**************************以下是UI的代码******************************************************************/
        public void InitWindowLocalInCenter()
        {
            WindowStartupLocation = WindowStartupLocation.CenterScreen;
            caozuo.Visibility = System.Windows.Visibility.Visible;
            deng.Visibility = System.Windows.Visibility.Hidden;
            JiZhang.Visibility = System.Windows.Visibility.Hidden;
            Date.Text = System.DateTime.Now.ToString();
            ToDate.Text = System.DateTime.Now.ToString();
            FWDate.Text = System.DateTime.Now.ToString();
            expander.IsEnabled= false ;
        }
        private void SignIn_Button(object sender, RoutedEventArgs e)
        {
            caozuo.Visibility = System.Windows.Visibility.Hidden;
            deng.Visibility = System.Windows.Visibility.Visible;
            JiZhang.Visibility = System.Windows.Visibility.Hidden;
        }

        private void JiZhang_Button(object sender, RoutedEventArgs e)
        {
            caozuo.Visibility = System.Windows.Visibility.Hidden;
            deng.Visibility = System.Windows.Visibility.Hidden;
            JiZhang.Visibility = System.Windows.Visibility.Visible;
        }

        private void BackHome_Button(object sender, RoutedEventArgs e)
        {
            caozuo.Visibility = System.Windows.Visibility.Visible;
            deng.Visibility = System.Windows.Visibility.Hidden;
            JiZhang.Visibility = System.Windows.Visibility.Hidden;

            Hui.Text = "";
            Date.Text = System.DateTime.Now.ToString();
            fuWuText.Text = "";
            NameFu.Content = "";
            Fee.Content = "0";
            Zhu.Text = "";
        }

        private void Back_Click(object sender, RoutedEventArgs e)
        {
            caozuo.Visibility = System.Windows.Visibility.Visible;
            deng.Visibility = System.Windows.Visibility.Hidden;
            JiZhang.Visibility = System.Windows.Visibility.Hidden;

            ToDate.Text = System.DateTime.Now.ToString();
            FWDate.Text = System.DateTime.Now.ToString();
            HYM.Text = "";
            HYH.Text = "";
            FWDM.Text = "";
            Fei.Text = "";
        }

        private void exit_MouseDown(object sender, MouseButtonEventArgs e)
        {
            Application.Current.Shutdown();
        }

        private void fullScreen_MouseDown(object sender, MouseButtonEventArgs e)
        {
            if (full_small)
            {
                w1.Height = SystemParameters.MaximizedPrimaryScreenHeight - 8d;
                w1.Width = SystemParameters.MaximizedPrimaryScreenWidth - 8d;
                WindowStartupLocation = WindowStartupLocation.Manual;
                w1.Left = 0; w1.Top = 0;

            }
            else
            {
                w1.Height = 350;
                w1.Width = 525;
                w1.Top = (SystemParameters.MaximizedPrimaryScreenHeight - 8d) / 2 - Height / 2;
                w1.Left = (SystemParameters.MaximizedPrimaryScreenWidth - 8d) / 2 - Width / 2;
                WindowStartupLocation = WindowStartupLocation.CenterScreen;
            }
            full_small = !full_small;
        }

        private void MuLu_Click(object sender, RoutedEventArgs e)
        {
            SuplierList sup = new SuplierList();
            sup.Owner = this;
            sup.ShowDialog();
            fuWuText.Text = sup.tNumber;
        }

        private void Button_Click_Submit(object sender, RoutedEventArgs e)
        {
            MessageBox.Show("提交到服务器");
            ServerRecord server = new ServerRecord();
            DateTime time = new DateTime();
            time = Convert.ToDateTime(Date.Text);
            server.Y = time.Year;
            server.M = time.Month;
            server.D = time.Day;
            time = System.DateTime.Now;
            server.yy = time.Year;
            server.mm = time.Month;
            server.dd = time.Day;
            server.h = time.Hour;
            server.m = time.Minute;
            server.s = time.Second;
            server.spt_id = suplier;
            server.mem_id = Hui.Text;
            server.server_id = fuWuText.Text;
            server.other = Zhu.Text;
          // MessageBox.Show(server.Serialization());
         if(network.SaveServerRecord(server))
         {
             MessageBox.Show("记账成功");
         }
         else
         {
             MessageBox.Show("记账失败");
         }
        }

        private void TiJiao_Click(object sender, RoutedEventArgs e)
        {
            MessageBox.Show("存盘");
            FileStream fs = new FileStream("../../tip.txt", FileMode.Open, FileAccess.Write);
            StreamWriter MyStreamWite = new StreamWriter(fs);
            string str = "";
            str+= ToDate.Text;
            str += "^";
            str += FWDate.Text;
            str += "^";
            str += HYM.Text;
            str += "^";
            str += HYH.Text;
            str += "^";
            str += FWDM.Text;
            str += "^";
            str += Fei.Text;
            str += "^";
            MyStreamWite.WriteLine(str);
            MyStreamWite.Close();
           
        }

        private void button3_Click(object sender, RoutedEventArgs e)
        {
           // MessageBox.Show("给服务器发消息发邮件");
            if(network.GetProviderSum(suplier)==-1)
            {
                MessageBox.Show("发送失败");
            }
            else
            {
                MessageBox.Show("发送成功");
            }
        }

        private void w1_Loaded(object sender, RoutedEventArgs e)
        {
            network.init();
            Login logo = new Login(network); w1.Visibility = System.Windows.Visibility.Hidden;

            logo.ShowDialog();
           // MessageBox.Show(logo.login.Text);
            suplier = logo.login.Text;
            if (logo.exitbutton == true)
            {
                Application.Current.Shutdown();
            }
            w1.Visibility = System.Windows.Visibility.Visible;
        }

        private void Hui_TextChanged(object sender, TextChangedEventArgs e)
        {
            if(Hui.Text.Length==MemberID_length)
            {
                int state=-10;
                state=network.IsValid(Hui.Text);
                if (state == 1)
                {
                    expander.IsEnabled = true;
                    Result.Content = "Validated";
                }
                else if (state == 0)
                {
                    expander.IsEnabled = false;
                    Result.Content = "Invalid number";
                }
                else if(state==-1)
                {
                    expander.IsEnabled = false;
                    Result.Content = "Member suspended";
                }
                else
                {
                    expander.IsEnabled = false;
                    Result.Content = "传回的数不对";
                }
            }
        }

        private void fuWuText_TextChanged(object sender, TextChangedEventArgs e)
        {
             if(fuWuText.Text.Length==6)
             {
                 NameFu.Content = network.GetServerName(fuWuText.Text);
                 Fee.Content = network.GetServerPrice(fuWuText.Text);
             }
        }
        /**************************UI的代码******************************************************************/
    }
}
