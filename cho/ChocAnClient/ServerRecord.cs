using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ChocAnClient
{
    public class ServerRecord
    {

        public int Y, M, D;  //提供者输入的那个时间，就是服务的时间
        public int yy, mm, dd, h, m, s;  //提交这个服务表单的时间
        //修改string mem_name;
        //修改string mem_id[9];
        //修改string server_id[6];
        public string spt_id;
        public string mem_id;
        public string server_id;
        public string other;
        public string Serialization()
        {
            string tmp = "";
            char[] buf = new char[26];

            //tmp += itoa(Y,buf,10);
            tmp += Convert.ToString(Y, 10);
            tmp += "^";
            //tmp += itoa(M, buf, 10);
            tmp += Convert.ToString(M, 10);
            tmp += "^";
            //tmp += itoa(D, buf, 10);
            tmp += Convert.ToString(D, 10);
            tmp += "^";
            //tmp += itoa(yy, buf, 10);
            tmp += Convert.ToString(yy, 10);
            tmp += "^";
            //tmp += itoa(mm, buf, 10);
            tmp += Convert.ToString(mm, 10);
            tmp += "^";
            //tmp += itoa(dd, buf, 10);
            tmp += Convert.ToString(dd, 10);
            tmp += "^";
            //tmp += itoa(h, buf, 10);
            tmp += Convert.ToString(h, 10);
            tmp += "^";
            //tmp += itoa(m, buf, 10);
            tmp += Convert.ToString(m, 10);
            tmp += "^";
            //tmp += itoa(s, buf, 10);
            tmp += Convert.ToString(s, 10);
            tmp += "^";

            tmp += spt_id;
            tmp += "^";
            tmp += mem_id;
            tmp += "^";
            tmp += server_id;
            tmp += "^";
            tmp += other;
            tmp += "^";
            return tmp;
        }
    }
}
