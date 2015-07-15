using System;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using ChoAnClient_for_Admin;

namespace ChoAnClient_for_Admin_Test
{
    [TestClass]
    public class NetworkTest
    {
        ChoAnClient_for_Admin.IRemoteProcessCall network = new ChoAnClient_for_Admin.RemoteProcessCall();

        [TestMethod]
        public void RPCTest()
        {
            string id = "123";
            network.init();
            network.far_add_mem(new mem_MSG());
            network.far_add_spt(new spt_MSG());
            network.far_del_mem(id);
            network.far_del_spt(id);
            network.send_supporter_email();
            network.send_member_email();
            network.print_report();
            Assert.AreEqual(1, 1);
        }
    }
}
