using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Net;
using System.Net.Sockets;
using System.Threading;

namespace ChocAnClient
{
    class RemoteProcessCall : IRemoteProcessCall
    {
        //message type
        private static short MSG_NULL				= 0x00;
        private static short MSG_SIGNIN_REQUEST		= 0x10;
        private static short MSG_SIGNIN_SUCCESS		= 0x11;
        private static short MSG_SIGNIN_FAILED		= 0x12;
        private static short MSG_ISVALID_REQUEST	= 0x20;
        private static short MSG_ISVALID_VALID		= 0x21;
        private static short MSG_ISVALID_INVALID	= 0x22;
        private static short MSG_ISVALID_SUSPEND	= 0x23;
        private static short MSG_SEVRNAME_REQUEST	= 0x30;
        private static short MSG_SEVRNAME_RETURN	= 0x31;
        private static short MSG_SEVRPRICE_REQUEST	= 0x40;
        private static short MSG_SEVRPRICE_RETURN	= 0x41;
        private static short MSG_SERVRECORD_REQUEST	= 0x50;
        private static short MSG_SERVRECORD_SUCCESS	= 0x51;
        private static short MSG_SERVRECORD_FAILED	= 0x52;
        private static short MSG_PRODSUM_REQUEST	= 0x60;
        private static short MSG_PRODSUM_RETURN		= 0x61;

        //setting
        private int port = 12345;
        private IPAddress ip = IPAddress.Parse("127.0.0.1");
        private Socket clientSocket;

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
                clientSocket.Send(Encoding.ASCII.GetBytes(_msg));
                Console.WriteLine("向服务器发送消息：{0}" + _msg);
            }
            catch
            {
                clientSocket.Shutdown(SocketShutdown.Both);
                clientSocket.Close();
            }
        }

        //interface

        //程序运行一开始时候被调用
        //负责初始化网络模块
        public void init()
        {
            clientSocket = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
            try
            {
                clientSocket.Connect(new IPEndPoint(ip, port)); //配置服务器IP与端口
                Console.WriteLine("连接服务器成功");
            }
            catch
            {
                Console.WriteLine("连接服务器失败，请按回车键退出！");
                return;
            }
        }

        //提供者登陆终端
        //功能: 终端开机后服务提供者输入他的提供者编号
        //发送: 提供者编号
        //返回: 登陆(true) / 失败(false)
        bool SignIn(string id)
        {
            return true;
        }

        //验证会员状态
        //功能 : 验证成员号码状态
        //发送 : 会员编号
        //回复 : 会员有效(1) / 无效(0) / 暂停(-1)
        int IsValid(string id)
        {
            return 0;
        }

        //获取服务名称
        //功能 : 根据输入的服务代号返回服务名称
        //发送 : 服务代号
        //回复 : 服务名称 / 不存在该服务(返回字符串"Invalid")
        string GetServerName(string id)
        {
            return "";
        }

        //获取服务费用
        //功能 : 根据输入的服务代号返回服务费用
        //发送 : 服务代号
        //回复 : 服务费用 / 不存在服务(返回 - 1)
        double GetServerPrice(string id)
        {
            return 0;
        }

        //ChocAn记账
        //子过程 : 验证会员状态, 获取服务名称, 获取服务费用
        //功能 : 存储记账信息
        //发送 : 服务记录类
        //回复 : 成功(true) / 失败(false)
        bool SaveServerRecord(ServerRecord sr)
        {
            return true;
        }

        //获取本周费用合计
        //功能: 到周末时提供者进行费用合计
        //发送 : 提供者编号
        //回复 : 合计费用 / 提供者编号错误(返回 - 1)
        double GetProviderSum(string id)
        {
            return 0;
        }

        //电子邮件发送
        //功能: 发送指定内容的邮件到指定邮箱
        //发送 : 收件人邮箱, 邮件标题, 邮件内容, 邮件附件
        //回复 : 发送成功(true) / 发送失败(false)
        bool send_email(string email_addr, string email_title, string email_content, string file_addr)
        {
            return true;
        }

    }
}
