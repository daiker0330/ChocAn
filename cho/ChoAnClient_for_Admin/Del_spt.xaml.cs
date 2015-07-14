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


namespace ChoAnClient_for_Admin
{
    /// <summary>
    /// Del_spt.xaml 的交互逻辑
    /// </summary>
    public partial class Del_spt : Window
    {
        public Del_spt()
        {
            InitializeComponent();
        }

        private void Button_Click(object sender, RoutedEventArgs e)
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

            //MessageD_Box(ID_box.Text);

            bool flag = network.far_del_spt(ID_box.Text);

            //Console.WriteLine(flag);
            //MessageBox.Show(flag.ToString());

            if (flag == true)
            {
                CheckWindow win2 = new CheckWindow();
                win2.ShowDialog();
                this.Close();
            }
            else
            {
                RecheckWindow win2 = new RecheckWindow();
                win2.ShowDialog();
            }
        }
    }
}
