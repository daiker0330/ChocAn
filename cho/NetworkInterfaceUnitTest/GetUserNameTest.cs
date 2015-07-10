using System;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using ChocAnClient;

namespace NetworkInterfaceUnitTest
{
    [TestClass]
    public class GetUserNameTest
    {
        RemoteProcessCall network = new RemoteProcessCall();
        [TestMethod]
        public void GetUserNameTest1()
        {
            network.init();
            String res = network.GetUserName("");
            Assert.AreEqual("Invalid", res);
        }
        [TestMethod]
        public void GetUserNameTest2()
        {
            network.init();
            String res = network.GetUserName("123456789");
            Assert.AreEqual("Invalid", res);
        }
    }
}
