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

namespace ChocAn
{
    /// <summary>
    /// MainWindow.xaml 的交互逻辑
    /// </summary>
    public partial class MainWindow : Window
    {
        public bool full_small = true;
        public double w_left = 0;
        public double w_top = 0;
        public MainWindow()
        {
            InitializeComponent();
            WindowStartupLocation = WindowStartupLocation.CenterScreen;
            caozuo.Visibility = System.Windows.Visibility.Visible;
            deng.Visibility = System.Windows.Visibility.Hidden;
            JiZhang.Visibility = System.Windows.Visibility.Hidden;
          
        }
        private void button1_Click(object sender, RoutedEventArgs e)
        {
            caozuo.Visibility = System.Windows.Visibility.Hidden;
            deng.Visibility = System.Windows.Visibility.Visible;
            JiZhang.Visibility = System.Windows.Visibility.Hidden;
        }

        private void button2_Click(object sender, RoutedEventArgs e)
        {
            caozuo.Visibility = System.Windows.Visibility.Hidden;
            deng.Visibility = System.Windows.Visibility.Hidden;
            JiZhang.Visibility = System.Windows.Visibility.Visible;
        }

        private void Button_Click(object sender, RoutedEventArgs e)
        {
            caozuo.Visibility = System.Windows.Visibility.Visible;
            deng.Visibility = System.Windows.Visibility.Hidden;
            JiZhang.Visibility = System.Windows.Visibility.Hidden;
        }

        private void Back_Click(object sender, RoutedEventArgs e)
        {
            caozuo.Visibility = System.Windows.Visibility.Visible;
            deng.Visibility = System.Windows.Visibility.Hidden;
            JiZhang.Visibility = System.Windows.Visibility.Hidden;
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
                w1.Top = (SystemParameters.MaximizedPrimaryScreenHeight-8d) / 2 - Height / 2;
                w1.Left = (SystemParameters.MaximizedPrimaryScreenWidth-8d) / 2 - Width / 2;
                WindowStartupLocation = WindowStartupLocation.CenterScreen;
            }
            full_small = !full_small;
        }

        private void MuLu_Click(object sender, RoutedEventArgs e)
        {
            SuplierList sup = new SuplierList();
            sup.Owner = this;
            sup.ShowDialog();
        }

        private void Button_Click_1(object sender, RoutedEventArgs e)
        {
            MessageBox.Show("提交到服务器");
        }

        private void TiJiao_Click(object sender, RoutedEventArgs e)
        {
            MessageBox.Show("存盘");
        }

        private void button3_Click(object sender, RoutedEventArgs e)
        {
            MessageBox.Show("给服务器发消息发邮件");
        }
    }
}
