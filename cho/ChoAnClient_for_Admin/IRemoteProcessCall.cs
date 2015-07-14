using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using ChoAnClient_for_Admin;

namespace ChoAnClient_for_Admin
{

    public interface IRemoteProcessCall
    {
        //远程过程调用接口
		//网络模块会实现这些借口,对于客户端来说网络模块是透明的
		//客户端可以直接调用这些接口,使用其功能,并默认会返回正确的结果

		//程序运行一开始时候被调用
		//负责初始化网络模块
        bool init();

        bool far_add_mem(mem_MSG mem);
        bool far_del_mem(string id);
        bool far_add_spt(spt_MSG mem);
        bool far_del_spt(string id);
        void send_member_email();
        void send_supporter_email();
        bool print_report();

        //设置IP地址
        void SetIp(string _ip);

        //设置端口号
        void SetPort(int _port);
    }
}
