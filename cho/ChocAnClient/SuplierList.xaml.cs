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
using System.IO;

using MahApps.Metro.Controls;
using MahApps.Metro;
using MahApps.Metro.Controls.Dialogs;

namespace ChocAnClient
{
    /// <summary>
    /// SuplierList.xaml 的交互逻辑
    /// </summary>
    public partial class SuplierList :MetroWindow
    {
        public class Suppler
        {

            private string _Service;
            private string _number;
            public Suppler()
            {
            }
            public Suppler(string S, string n)
            {
                _Service = S;
                _number = n;
            }

            public string Service
            {
                get { return _Service; }
                set { _Service = value; }
            }
            public string Number
            {
                get { return _number; }
                set { _number = value; }
            }
            public void SetSupper(string S, string n)
            {
                _Service = S;
                _number = n;
            }
        }
        public string tNumber;
        public List<Suppler> list = new List<Suppler>();
        public List<Suppler> resultList = new List<Suppler>();
        public SuplierList()
        {
            InitializeComponent();
            ReadSuplierLsit();
        }

        public void ReadSuplierLsit()
        {

            FileStream fs = new FileStream("../../SuplierList.txt", FileMode.Open, FileAccess.Read);

            StreamReader MyStreamReader = new StreamReader(fs);

            string strline = MyStreamReader.ReadLine();
            int count = 1;
            string Ser = "";
            string N = "";
            
            Suppler supe;
            // Ser = strline;
            while (strline != null)
            {
                if (count == 1)
                {
                    Ser = strline;
                }
                if (count == 2)
                {
                    N = strline;
                    supe = new Suppler(Ser, N);
                    list.Add(supe);
                    count = 0;
                }
                strline = MyStreamReader.ReadLine();
                count++;
            }
            MyStreamReader.Close();
            l1.ItemsSource = list;
        }

        private void l1_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            Suppler test = l1.SelectedItem as Suppler;
            if (test != null && test is Suppler)
            {
       //         MessageBox.Show("Service:" + test.Service + "\n\n" + "Number:" + test.Number);
                tNumber = test.Number;
                fuH.Content = test.Number;
            }
        }

       

        private void Button_Click(object sender, RoutedEventArgs e)
        {
           if(fuH.Content.ToString()!="")
           {
               tNumber = fuH.Content.ToString();
               
           }
           l.Close();
        }

        private void FM_TextChanged(object sender, TextChangedEventArgs e)
        {
            for (int i = 0; i < list.Count; i++)
            {
                if (FM.Text == list[i].Service)
                {
                    fuH.Content = list[i].Number;
                    break;
                }
                else
                {
                    fuH.Content = "";
                }
            }
            bool ct = false;
            resultList.Clear();
            for (int i = 0; i < list.Count; i++)
            {
                if (list[i].Service.Contains(FM.Text))
                {
                    resultList.Add(list[i]);
                    ct = true;
                }

            }
            if (!ct)
            {
                l1.ItemsSource = null;
            }
            else
            {
                l1.ItemsSource = resultList;
            }

            if (FM.Text == "")
            {
                l1.ItemsSource = list;
            }
            else
            {
                ;
            }
        }

        private void FM_KeyDown(object sender, KeyEventArgs e)
        {
           
        }
    }
}
