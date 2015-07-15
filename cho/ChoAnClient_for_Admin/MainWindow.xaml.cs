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
using System.Windows.Media.Animation;

using System.Data.OleDb;
using System.Data.SqlClient;
using System.Data;

/*
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
*/
namespace ChoAnClient_for_Admin
{
    /// <summary>
    /// MainWindow.xaml 的交互逻辑
    /// </summary>
    
    public partial class MainWindow : Window
    {
        //RemoteProcessCall network = new RemoteProcessCall();
        
        public MainWindow()
        {
            InitializeComponent();
        }


        private void add_member_Click(object sender, RoutedEventArgs e)
        {
            Add_mem win=new Add_mem();
            win.ShowDialog();


        }

        private void del_member_Click(object sender, RoutedEventArgs e)
        {
            //Console.WriteLine("(*&^%$#");
            //MessageBox.Show("*&^%$");
            Del_mem win = new Del_mem();
            win.ShowDialog();
        }

        private void add_supporter_Click(object sender, RoutedEventArgs e)
        {
            Add_spt win = new Add_spt();
            win.ShowDialog();
        }

        private void del_supporter_Click(object sender, RoutedEventArgs e)
        {
            Del_spt win = new Del_spt();
            win.ShowDialog();
        }
        private void send_mem_email_Click(object sender, RoutedEventArgs e)
        {
            FileStream fs = new FileStream("../../IpPort.ini", FileMode.Open, FileAccess.Read);

            StreamReader MyStreamReader = new StreamReader(fs);
            string Ip;
            int Port;
            Ip = MyStreamReader.ReadLine();
            string P = MyStreamReader.ReadLine();
            Port = Convert.ToInt32(P);
            
            RemoteProcessCall network = new RemoteProcessCall();
            network.SetIp(Ip);
            network.SetPort(Port);
            //   network.init();
            network.init();


            network.send_member_email();

            CheckWindow win = new CheckWindow();
            win.ShowDialog();

        }

        private void send_spt_email_Click(object sender, RoutedEventArgs e)
        {
            FileStream fs = new FileStream("../../IpPort.ini", FileMode.Open, FileAccess.Read);

            StreamReader MyStreamReader = new StreamReader(fs);
            string Ip;
            int Port;
            Ip = MyStreamReader.ReadLine();
            string P = MyStreamReader.ReadLine();
            Port = Convert.ToInt32(P);

            RemoteProcessCall network = new RemoteProcessCall();
            network.SetIp(Ip);
            network.SetPort(Port);
            //   network.init();
            network.init();


            network.send_supporter_email();

            CheckWindow win = new CheckWindow();
            win.ShowDialog();

        }

        private void print_report_Click(object sender, RoutedEventArgs e)
        {
            FileStream fs = new FileStream("../../IpPort.ini", FileMode.Open, FileAccess.Read);

            StreamReader MyStreamReader = new StreamReader(fs);
            string Ip;
            int Port;
            Ip = MyStreamReader.ReadLine();
            string P = MyStreamReader.ReadLine();
            Port = Convert.ToInt32(P);

            RemoteProcessCall network = new RemoteProcessCall();
            network.SetIp(Ip);
            network.SetPort(Port);
            //   network.init();
            network.init();


            network.print_report();

            CheckWindow win = new CheckWindow();
            win.ShowDialog();

        }

        private void exit_Click(object sender, RoutedEventArgs e)
        {
            Application.Current.Shutdown();
        }
    }
}
