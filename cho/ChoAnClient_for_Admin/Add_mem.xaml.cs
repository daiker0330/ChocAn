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
            //CheckWindow win1 = new CheckWindow();
           //RecheckWindow win2 = new RecheckWindow();
            //win1.ShowDialog();
           // win2.ShowDialog();
            this.Close();
        }
    }
}
