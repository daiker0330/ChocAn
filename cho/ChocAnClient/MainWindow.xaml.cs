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

namespace ChocAnClient
{
    /// <summary>
    /// MainWindow.xaml 的交互逻辑
    /// </summary>
    /// 

    public partial class MainWindow : Window
    {
        
        RemoteProcessCall network = new RemoteProcessCall();
        /*****************UI*******************************/
        public bool full_small = true;
        public double w_left = 0;
        public double w_top = 0;
        public string suplier = "";
        public const int MemberID_length = 9;
        bool re;
        string result;
        string Ip;
        int Port;
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
           // WindowStartupLocation = WindowStartupLocation.CenterScreen;
            caozuo.Visibility = System.Windows.Visibility.Visible;
            deng.Visibility = System.Windows.Visibility.Hidden;
            JiZhang.Visibility = System.Windows.Visibility.Hidden;
            Date.Text = System.DateTime.Now.ToString();
            expander.IsEnabled= false ;
            w1.Height = SystemParameters.MaximizedPrimaryScreenHeight - 8d;
            w1.Width = SystemParameters.MaximizedPrimaryScreenWidth - 8d;
            WindowStartupLocation = WindowStartupLocation.Manual;
            w1.Left = 0; w1.Top = 0;
            HuiName.IsEnabled = false;
           re = false;

           FileStream fs = new FileStream("../../IpPort.ini", FileMode.Open, FileAccess.Read);

           StreamReader MyStreamReader = new StreamReader(fs);

           Ip = MyStreamReader.ReadLine();
           string P = MyStreamReader.ReadLine();
           Port=Convert.ToInt32(P);

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
            OleDbConnection con = new OleDbConnection("Provider=Microsoft.Jet.OLEDB.4.0;Data Source=../../QingDan.mdb");
            con.Open();

            string sql = "select * from FeeList where 提供者ID='";
            sql += suplier;
            sql += "'";
            OleDbDataAdapter ad = new OleDbDataAdapter(sql, con);

            DataSet ds = new DataSet();

            ad.Fill(ds, "Sum");
            data.ItemsSource = ds.Tables[0].DefaultView;

            DataRowView selectItem = data.Items[0] as DataRowView;
        }

        private void BackHome_Button(object sender, RoutedEventArgs e)
        {
            caozuo.Visibility = System.Windows.Visibility.Visible;
            deng.Visibility = System.Windows.Visibility.Hidden;
            JiZhang.Visibility = System.Windows.Visibility.Hidden;


            Hui.Text = "";
            HuiName.Text = "";
            Date.Text = System.DateTime.Now.ToString();
            fuWuText.Text = "";
            NameFu.Content = "";
            Fee.Content = "0";
            Zhu.Text = "";
            Result.Content = "";
            expander.IsEnabled = false;
            expander.IsExpanded = false;
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
            if (!full_small)
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
        public void Write_to_Access(ServerRecord server)
        {
            OleDbConnection con = new OleDbConnection("Provider=Microsoft.Jet.OLEDB.4.0;Data Source=../../QingDan.mdb");
            con.Open();

            OleDbDataAdapter adapter = new OleDbDataAdapter();
            OleDbCommand command;
            DateTime d = System.DateTime.Now;
            DateTime p = System.DateTime.Now;
            command = new OleDbCommand(
                "INSERT INTO tt(记账日期,服务日期,会员ID,服务ID,费用,会员名,注释,提供者ID)" +
                "VALUES(?,?,?,?,?,?,?,?)", con);
            command.Parameters.Add(
                "记账日期", OleDbType.DBTimeStamp, 20, "记账日期");
            command.Parameters.Add(
                "服务日期", OleDbType.DBTimeStamp, 20, "服务日期");
            command.Parameters.Add(
                "会员ID", OleDbType.VarChar, 9, "会员ID");
            command.Parameters.Add(
                "服务ID", OleDbType.VarChar, 6, "服务ID");
            command.Parameters.Add(
                "费用", OleDbType.Double, 5, "费用");
            command.Parameters.Add(
                "会员名", OleDbType.VarChar, 15, "会员名");
            command.Parameters.Add(
                "注释",OleDbType.VarChar,100,"注释");
            adapter.InsertCommand = command;
            command.Parameters.Add(
                "提供者ID", OleDbType.VarChar, 9, "提供者ID");
          
            string Str = "INSERT INTO FeeList(记账日期,服务日期,会员ID,服务ID,费用,会员名,注释,提供者ID) VALUES(";

            Str += "'";
            Str += server.yy.ToString();
            Str += "-";
            Str += server.mm.ToString();
            Str += "-";
            Str += server.dd.ToString();
            Str += " ";
            Str += server.h.ToString();
            Str += ":";
            Str += server.m.ToString();
            Str += ":";
            Str += server.s.ToString();
            Str += "'";

            Str += ",";

            Str += "'";
            Str += server.Y.ToString();
            Str += "-";
            Str += server.M.ToString();
            Str += "-";
            Str += server.D.ToString();
            Str += "'";

            Str += ",";

            Str += "'";
            Str += server.mem_id;
            Str += "'";

            Str += ",";

            Str += "'";
            Str += server.server_id;
            Str += "'";

            Str += ",";

            Str += Fee.Content;

            Str += ",";

            Str += "'";
            Str += HuiName.Text;
            Str += "'";

            Str += ",";

            Str += "'";
            Str += server.other;
            Str += "'";

            Str += ",";

            Str += "'";
            Str += server.spt_id;
            Str += "'";

            Str += ")";

           // MessageBox.Show(Str);

            adapter.InsertCommand.CommandText = Str;
            adapter.InsertCommand.Connection = con;
            adapter.InsertCommand.ExecuteNonQuery();
        }
        
        public bool List_Right()
        {
            if (HuiName.Text == "")
                return false;
            if (fuWuText.Text == "")
                return false;
            if (Fee.Content.ToString() == "-1" || Fee.Content.ToString() == "无效的服务代码" || Fee.Content.ToString() == "")
                return false;
            return true;
        }
        private void Button_Click_Submit(object sender, RoutedEventArgs e)
        {
         //   MessageBox.Show("提交到服务器");
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
            if (List_Right())
            {
                if (network.SaveServerRecord(server))
                {
                    Write_to_Access(server);
                    MessageBox.Show("记账成功");
                    HuiName.Text = "";
                    Hui.Text = "";
                    Date.Text = System.DateTime.Now.ToString();
                    fuWuText.Text = "";
                    NameFu.Content = "";
                    Fee.Content = "0";
                    Zhu.Text = "";
                    Result.Content = "";
                    expander.IsEnabled = false;
                    expander.IsExpanded = false;


                }
                else
                {
                    MessageBox.Show("记账失败");
                }

            }
            else
            {
                MessageBox.Show("账单不完整,请填写完整");
            }
        }

        private void TiJiao_Click(object sender, RoutedEventArgs e)
        {
            MessageBox.Show("存盘");
          
           
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
           
         //   network.SetIp(Ip);
         //   network.SetPort(Port); 
         //   network.init();
           re= network.init();
            Login logo = new Login(network,re); w1.Visibility = System.Windows.Visibility.Hidden;

            logo.ShowDialog();
           // MessageBox.Show(logo.login.Text);
            suplier = logo.login.Text;
            D.Content = suplier;
            D.Content += ":注销";
            if (logo.exitbutton == true)
            {
                Application.Current.Shutdown();
            }
            w1.Visibility = System.Windows.Visibility.Visible;
        }

        private void Hui_TextChanged(object sender, TextChangedEventArgs e)
        {
            TextBox textBox = sender as TextBox;
            TextChange[] change = new TextChange[e.Changes.Count];
            e.Changes.CopyTo(change, 0);

            int offset = change[0].Offset;
            if (change[0].AddedLength > 0)
            {
                double num = 0;
                if (!Double.TryParse(textBox.Text, out num))
                {
                    textBox.Text = textBox.Text.Remove(offset, change[0].AddedLength);
                    textBox.Select(offset, 0);
                }
            }




            if(Hui.Text.Length==MemberID_length)
            {
                int state=-10;
                state=network.IsValid(Hui.Text);
                if (state == 1)
                {
                    expander.IsEnabled = true;
                    Result.Content = "Validated";
                    HuiName.Text = network.GetUserName(Hui.Text);
                    HuiName.IsEnabled = false;
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
            else
            {
                expander.IsEnabled = false;
                Result.Content = "";
                expander.IsExpanded = false;
            }
        }

        private void fuWuText_TextChanged(object sender, TextChangedEventArgs e)
        {
            TextBox textBox = sender as TextBox;
            TextChange[] change = new TextChange[e.Changes.Count];
            e.Changes.CopyTo(change, 0);

            int offset = change[0].Offset;
            if (change[0].AddedLength > 0)
            {
                double num = 0;
                if (!Double.TryParse(textBox.Text, out num))
                {
                    textBox.Text = textBox.Text.Remove(offset, change[0].AddedLength);
                    textBox.Select(offset, 0);
                }
            }
            
            
            
            if(fuWuText.Text.Length==6)
             {
                 NameFu.Content = network.GetServerName(fuWuText.Text);
                 Fee.Content = network.GetServerPrice(fuWuText.Text);
                if(Fee.Content.ToString()=="-1")
                {
                    Fee.Content ="无效的服务代码";
                }
             }
        }

        private void D_MouseDown(object sender, MouseButtonEventArgs e)
        {
            suplier = ""; Hui.Text = ""; fuWuText.Text = ""; NameFu.Content = ""; Zhu.Text = ""; Fee.Content = "0"; Result.Content = ""; HuiName.Text = "";
            caozuo.Visibility = System.Windows.Visibility.Visible;
            deng.Visibility = System.Windows.Visibility.Hidden;
            JiZhang.Visibility = System.Windows.Visibility.Hidden;
           
            Date.Text = System.DateTime.Now.ToString();
           
           
           
            
            expander.IsEnabled = false;
            expander.IsExpanded = false;

            Login logo = new Login(network,re); w1.Visibility = System.Windows.Visibility.Hidden;

            logo.ShowDialog();
            // MessageBox.Show(logo.login.Text);
            suplier = logo.login.Text;
            D.Content = suplier;
            D.Content += ":注销";
            if (logo.exitbutton == true)
            {
                Application.Current.Shutdown();
            }
            w1.Visibility = System.Windows.Visibility.Visible;
        }

        private void Hui_KeyDown(object sender, KeyEventArgs e)
        {
            TextBox txt = sender as TextBox;

            //屏蔽非法按键
            if ((e.Key >= Key.NumPad0 && e.Key <= Key.NumPad9) || e.Key == Key.Decimal)
            {
                if (txt.Text.Contains(".") && e.Key == Key.Decimal)
                {
                    e.Handled = true;
                    return;
                }
                e.Handled = false;
            }
            else if (((e.Key >= Key.D0 && e.Key <= Key.D9) || e.Key == Key.OemPeriod) && e.KeyboardDevice.Modifiers != ModifierKeys.Shift)
            {
                if (txt.Text.Contains(".") && e.Key == Key.OemPeriod)
                {
                    e.Handled = true;
                    return;
                }
                e.Handled = false;
            }
            else
            {
                e.Handled = true;
            }
        }

        private void fuWuText_KeyDown(object sender, KeyEventArgs e)
        {
            TextBox txt = sender as TextBox;

            //屏蔽非法按键
            if ((e.Key >= Key.NumPad0 && e.Key <= Key.NumPad9) || e.Key == Key.Decimal)
            {
                if (txt.Text.Contains(".") && e.Key == Key.Decimal)
                {
                    e.Handled = true;
                    return;
                }
                e.Handled = false;
            }
            else if (((e.Key >= Key.D0 && e.Key <= Key.D9) || e.Key == Key.OemPeriod) && e.KeyboardDevice.Modifiers != ModifierKeys.Shift)
            {
                if (txt.Text.Contains(".") && e.Key == Key.OemPeriod)
                {
                    e.Handled = true;
                    return;
                }
                e.Handled = false;
            }
            else
            {
                e.Handled = true;
            }
        }
        /**************************UI的代码******************************************************************/
    }
}
