using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Net;
using System.Net.Sockets;
using System.Threading;
using ChoAnClient_for_Admin;

namespace ChoAnClient_for_Admin
{
    public class RemoteProcessCall : IRemoteProcessCall
    {
        public RemoteProcessCall() { }

        //message type
        private static int MSG_NULL = 0x00;
        private static int MSG_FAR_ADD_MEM_REQUEST = 0x90;
        private static int MSG_FAR_ADD_MEM_RETURN = 0x91;
        private static int MSG_FAR_DEL_MEM_REQUEST = 0x92;
        private static int MSG_FAR_DEL_MEM_RETURN = 0x93;
        private static int MSG_FAR_ADD_SPT_REQUEST = 0x94;
        private static int MSG_FAR_ADD_SPT_RETURN = 0x95;
        private static int MSG_FAR_DEL_SPT_REQUEST = 0x96;
        private static int MSG_FAR_DEL_SPT_RETURN = 0x97;
        private static int MSG_SEND_MEMBER_EMAIL_REQUEST = 0x98;
        private static int MSG_SEND_MEMBER_EMAIL_RETURN = 0x99;
        private static int MSG_SEND_SUPPORTER_EMAIL_REQUEST = 0x9A;
        private static int MSG_SEND_SUPPORTER_EMAIL_RETURN = 0x9B;
        private static int MSG_PRINT_REPORT_REQUEST = 0x9C;
        private static int MSG_PRINT_REPORT_RETURN = 0x9D;

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

        public bool far_add_mem(mem_MSG mem)
        {
            string msg;
            msg = MSG_FAR_ADD_MEM_REQUEST.ToString() + ":" + mem.Serialization();
            SendMessage(msg);

            String[] recv;
            recv = ReceiveMessage();

            Console.WriteLine("*data:" + recv[1]);

            return Convert.ToBoolean(recv[1]);
        }

        public bool far_del_mem(string id)
        {
            string msg;
            msg = MSG_FAR_DEL_MEM_REQUEST.ToString() + ":" + id;
            SendMessage(msg);

            String[] recv;
            recv = ReceiveMessage();

            Console.WriteLine("*data:" + recv[1]);

            return Convert.ToBoolean(recv[1]);
        }

        public bool far_add_spt(spt_MSG mem)
        {
            string msg;
            msg = MSG_FAR_ADD_SPT_REQUEST.ToString() + ":" + mem.Serialization();
            SendMessage(msg);

            String[] recv;
            recv = ReceiveMessage();

            Console.WriteLine("*data:" + recv[1]);

            return Convert.ToBoolean(recv[1]);
        }

        public bool far_del_spt(string id)
        {
            string msg;
            msg = MSG_FAR_DEL_SPT_REQUEST.ToString() + ":" + id;
            SendMessage(msg);

            String[] recv;
            recv = ReceiveMessage();

            Console.WriteLine("*data:" + recv[1]);

            return Convert.ToBoolean(recv[1]);
        }

        public void send_member_email()
        {
            string msg;
            msg = MSG_SEND_MEMBER_EMAIL_REQUEST.ToString();

            SendMessage(msg);

            ReceiveMessage();
        }

        public void send_supporter_email()
        {
            string msg;
            msg = MSG_SEND_SUPPORTER_EMAIL_REQUEST.ToString();

            SendMessage(msg);

            ReceiveMessage();
        }

        public bool print_report()
        {
            string msg;
            msg = MSG_PRINT_REPORT_REQUEST.ToString();
            SendMessage(msg);

            String[] recv;
            recv = ReceiveMessage();

            Console.WriteLine("*data:" + recv[1]);

            return Convert.ToBoolean(recv[1]);
        }
    }
}
