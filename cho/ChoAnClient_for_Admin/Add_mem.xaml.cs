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
    /// Add_mem.xaml 的交互逻辑
    /// </summary>
    public partial class Add_mem : Window
    {
        public Add_mem()
        {
            InitializeComponent();
        }

        private void Bsubmit_Click(object sender, RoutedEventArgs e)
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

            mem_MSG mem = new mem_MSG();
            mem.name = Bname.Text;
            mem.id = Bid.Text;
            mem.addr = Baddr.Text;
            mem.city = Bcity.Text;
            mem.nation = Bnation.Text;
            mem.zip = Bzip.Text;
            mem.email = Bemail.Text;


            bool flag = network.far_add_mem(mem);

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
            


            //this.Close();
        }
    }
}
