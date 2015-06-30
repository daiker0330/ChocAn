using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ChocAnClient
{

    public interface IRemoteProcessCall
    {
        //远程过程调用接口
		//网络模块会实现这些借口,对于客户端来说网络模块是透明的
		//客户端可以直接调用这些接口,使用其功能,并默认会返回正确的结果

		//程序运行一开始时候被调用
		//负责初始化网络模块
        void init(); 
	
		//提供者登陆终端
		//功能: 终端开机后服务提供者输入他的提供者编号
		//发送: 提供者编号
		//返回: 登陆(true) / 失败(false)
        bool SignIn(string id);

		//验证会员状态
		//功能 : 验证成员号码状态
		//发送 : 会员编号
		//回复 : 会员有效(1) / 无效(0) / 暂停(-1)
        int IsValid(string id);

		//获取服务名称
		//功能 : 根据输入的服务代号返回服务名称
		//发送 : 服务代号
		//回复 : 服务名称 / 不存在该服务(返回字符串"Invalid")
        string GetServerName(string id);

		//获取服务费用
		//功能 : 根据输入的服务代号返回服务费用
		//发送 : 服务代号
		//回复 : 服务费用 / 不存在服务(返回 - 1)
        double GetServerPrice(string id);

		//ChocAn记账
		//子过程 : 验证会员状态, 获取服务名称, 获取服务费用
		//功能 : 存储记账信息
		//发送 : 服务记录类
		//回复 : 成功(true) / 失败(false)
        bool SaveServerRecord(ServerRecord sr);

		//获取本周费用合计
		//功能: 到周末时提供者进行费用合计
		//发送 : 提供者编号
		//回复 : 合计费用 / 提供者编号错误(返回 - 1)
        double GetProviderSum(string id);
    }
}
