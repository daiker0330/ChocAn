﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Net;
using System.Net.Sockets;
using System.Threading;

namespace ChocAnClient
{
    public class RemoteProcessCall : IRemoteProcessCall
    {
        public RemoteProcessCall(){}

        //message type
        private static int MSG_NULL = 0x00;
        private static int MSG_SIGNIN_REQUEST = 0x10;
        private static int MSG_SIGNIN_SUCCESS = 0x11;
        private static int MSG_SIGNIN_FAILED = 0x12;
        private static int MSG_ISVALID_REQUEST = 0x20;
        private static int MSG_ISVALID_VALID = 0x21;
        private static int MSG_ISVALID_INVALID = 0x22;
        private static int MSG_ISVALID_SUSPEND = 0x23;
        private static int MSG_SEVRNAME_REQUEST = 0x30;
        private static int MSG_SEVRNAME_RETURN = 0x31;
        private static int MSG_SEVRPRICE_REQUEST = 0x40;
        private static int MSG_SEVRPRICE_RETURN = 0x41;
        private static int MSG_SERVRECORD_REQUEST = 0x50;
        private static int MSG_SERVRECORD_SUCCESS = 0x51;
        private static int MSG_SERVRECORD_FAILED = 0x52;
        private static int MSG_PRODSUM_REQUEST = 0x60;
        private static int MSG_PRODSUM_RETURN = 0x61;
        private static int MSG_USERNAME_REQUEST = 0x70;
        private static int MSG_USERNAME_RETURN	= 0x71;
        private static int MSG_SERVEEMAIL_REQUEST = 0x80;
        private static int MSG_SERVEEMAIL_RETURN = 0x81;

        //setting
        private int port = 12345;
        private IPAddress ip = IPAddress.Parse("127.0.0.1");
        private Socket clientSocket;
        private bool isInit = false;

        //message
        private static byte[] result = new byte[1024];

        public void SetIp(string _ip)
        {
            ip = IPAddress.Parse(_ip);
        }
        
        public void SetPort(int _port)
        {
            port = _port;
        }

        public void SendMessage(string _msg)
        {
            try
            {
                //if(!isInit)
                //{
                //    init();
                //}
                clientSocket.Send(Encoding.ASCII.GetBytes(_msg));
                Console.WriteLine("向服务器发送消息：{0}" + _msg);
            }
            catch
            {
                clientSocket.Shutdown(SocketShutdown.Both);
                clientSocket.Close();
            }
        }

        public String[] ReceiveMessage()
        {
            string msg;
            String[] recv = new String[2];

            int receiveLength = clientSocket.Receive(result);

            msg = Encoding.ASCII.GetString(result, 0, receiveLength);

            char[] separator = { ':' };
            String[] splitStrings = new String[5];
            splitStrings = msg.Split(separator);
            recv[0] = splitStrings[0];
            recv[1] = splitStrings[1];

            Console.WriteLine("type:" + recv[0]);
            Console.WriteLine("data:" + recv[1]);

            return recv;
        }

        //interface

        //程序运行一开始时候被调用
        //负责初始化网络模块
        public bool init()
        {
            Console.WriteLine("开始连接服务器");


            clientSocket = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
            try
            {
                Console.WriteLine(ip);
                clientSocket.Connect(new IPEndPoint(ip, port)); //配置服务器IP与端口
                if (clientSocket.Connected)
                {
                    SendMessage("connect");
                    ReceiveMessage();
                    isInit = true;
                    Console.WriteLine("连接服务器成功");
                    return true;
                }
                else
                {
                    Console.WriteLine("连接服务器失败！");
                    return false;
                }
            }
            catch
            {
                Console.WriteLine("连接服务器异常！");
                return false;
            }
        }

        //提供者登陆终端
        //功能: 终端开机后服务提供者输入他的提供者编号
        //发送: 提供者编号
        //返回: 登陆(true) / 失败(false)
        public bool SignIn(string id)
        {
            string msg;
            msg = MSG_SIGNIN_REQUEST.ToString() + ":" + id;
            SendMessage(msg);

            String[] recv;
            recv = ReceiveMessage();

            if (recv[0].Equals(MSG_SIGNIN_SUCCESS.ToString()))
                return true;
            else if (recv[0].Equals(MSG_SIGNIN_FAILED.ToString()))
                return false;
            else
                return false;
        }

        //验证会员状态
        //功能 : 验证成员号码状态
        //发送 : 会员编号
        //回复 : 会员有效(1) / 无效(0) / 暂停(-1)
        public int IsValid(string id)
        {
            string msg;
            msg = MSG_ISVALID_REQUEST.ToString() + ":" + id;
            SendMessage(msg);

            String[] recv;
            recv = ReceiveMessage();

            if (recv[0].Equals(MSG_ISVALID_VALID.ToString()))
                return 1;
            else if (recv[0].Equals(MSG_ISVALID_INVALID.ToString()))
                return 0;
            else if (recv[0].Equals(MSG_ISVALID_SUSPEND.ToString()))
                return -1;
            else
                return 0;
        }

        //获取会员名称
        //功能 : 根据输入的会员号码返回会员名称
        //发送 : 会员号码
        //回复 : 会员名称 / 不存在该服务(返回字符串"Invalid")
        public string GetUserName(string id)
        {
            string msg;
            msg = MSG_USERNAME_REQUEST.ToString() + ":" + id;
            SendMessage(msg);

            String[] recv;
            recv = ReceiveMessage();

            Console.WriteLine("*data:" + recv[1]);

            return recv[1];
        }

        //获取服务名称
        //功能 : 根据输入的服务代号返回服务名称
        //发送 : 服务代号
        //回复 : 服务名称 / 不存在该服务(返回字符串"Invalid")
        public string GetServerName(string id)
        {
            string msg;
            msg = MSG_SEVRNAME_REQUEST.ToString() + ":" + id;
            SendMessage(msg);

            String[] recv;
            recv = ReceiveMessage();

            Console.WriteLine("*data:" + recv[1]);

            return recv[1];
        }

        //获取服务费用
        //功能 : 根据输入的服务代号返回服务费用
        //发送 : 服务代号
        //回复 : 服务费用 / 不存在服务(返回 - 1)
        public double GetServerPrice(string id)
        {
            string msg;
            msg = MSG_SEVRPRICE_REQUEST.ToString() + ":" + id;
            SendMessage(msg);

            String[] recv;
            recv = ReceiveMessage();

            return Convert.ToDouble(recv[1]);
        }

        //ChocAn记账
        //子过程 : 验证会员状态, 获取服务名称, 获取服务费用
        //功能 : 存储记账信息
        //发送 : 服务记录类
        //回复 : 成功(true) / 失败(false)
        public bool SaveServerRecord(ServerRecord sr)
        {
            string msg;
            msg = MSG_SERVRECORD_REQUEST.ToString() + ":" + sr.Serialization();
            SendMessage(msg);

            String[] recv;
            recv = ReceiveMessage();

            if (recv[0].Equals(MSG_SERVRECORD_SUCCESS.ToString()))
                return true;
            else if (recv[0].Equals(MSG_SERVRECORD_FAILED.ToString()))
                return false;
            else
                return false;
        }

        //获取本周费用合计
        //功能: 到周末时提供者进行费用合计
        //发送 : 提供者编号
        //回复 : 合计费用 / 提供者编号错误(返回 - 1)
        public double GetProviderSum(string id)
        {
            string msg;
            msg = MSG_PRODSUM_REQUEST.ToString() + ":" + id;
            SendMessage(msg);

            String[] recv;
            recv = ReceiveMessage();

            return Convert.ToDouble(recv[1]);
        }

        //发送提供者目录
        public void SendProviderServes(string id)
        {
            string msg;
            msg = MSG_SERVEEMAIL_REQUEST.ToString() + ":" + id;
            //Console.WriteLine("!!!!!!!!!!!");
            SendMessage(msg);

            String[] recv;
            recv = ReceiveMessage();
        }
    }
}
