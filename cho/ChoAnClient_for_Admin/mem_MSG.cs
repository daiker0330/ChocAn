using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ChoAnClient_for_Admin
{
    public class mem_MSG
    {
        public string name;
	    //修改char id[9];
        public string id;
        public string addr;
        public string city;
        public string nation;
	    //修改char zip[5];
        public string zip;
        public string email;
        public string Serialization()
        {
            string tmp = "";
	        char[] buf=new char[51];
	        tmp += name;
	        tmp += "^";
	        tmp += id;
	        tmp += "^";
	        tmp += addr;
	        tmp += "^";
	        tmp += city;
	        tmp += "^";
	        tmp += nation;
	        tmp += "^";
	        tmp += zip;
	        tmp += "^";
	        tmp += email;
	        tmp += "^";
	        return tmp;
        }
    }
}
