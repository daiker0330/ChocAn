﻿using System;
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

using MahApps.Metro.Controls;
using MahApps.Metro;
using MahApps.Metro.Controls.Dialogs;

namespace ChocAnClient
{
    /// <summary>
    /// Login.xaml 的交互逻辑
    /// </summary>
    public partial class Login:MetroWindow
    {

        public bool result = false;
        public const int ServerID_length = 9;//服务者ID长度
        public bool exitbutton = false;
        public RemoteProcessCall net = new RemoteProcessCall();
        public Login(RemoteProcessCall network,bool re)
        {
            InitializeComponent();
            WindowStartupLocation = WindowStartupLocation.CenterScreen;
            net = network;
            Deng.IsEnabled = false;
            result = re;
           
        }

        public void MessageD_Box(string message)
        {
            DialogManager.ShowMessageAsync(wlogin, "", message, MessageDialogStyle.Affirmative, null);
        }
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


            
            if (login.Text.Length== ServerID_length||!result)
            {
                Deng.IsEnabled = true;
            }
            else
            {
                Deng.IsEnabled = false;
            }
        }

        private void login_KeyDown(object sender, KeyEventArgs e)
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

        private void Button_Click(object sender, RoutedEventArgs e)
        {
            if (result)
            {
               
                if (net.SignIn(login.Text))
                //if(login.Text=="123456789")
                {
                    MessageD_Box("连接成功");
                    wlogin.Close();
                }
                else
                {
                    MessageD_Box("TnT没找到这个账号，请重新输入");
                    login.Text = "";

                }
                exitbutton = false;
            }
            else
            {
                result=net.init();
                if (result)
                {
                    Deng.Content = "登录";
                    MessageD_Box("连接成功");
                    login.IsEnabled = true;
                }
                else
                {
                    Deng.Content = "重新连接";
                    MessageD_Box("连接失败,请重新连接");
                    login.IsEnabled = false;
                }
            }
        }

        private void wlogin_Loaded(object sender, RoutedEventArgs e)
        {
            if (!result)
            {
                Deng.Content = "重新连接";
                Deng.IsEnabled = true;
                login.IsEnabled = false;
            }
            else
            {
                //MessageBox.Show("连接成功");
                MessageD_Box("连接成功");

            }
        }
    }
}
