using System;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using ChocAnClient;

namespace NetworkInterfaceUnitTest
{
    [TestClass]
    public class UnitTest1
    {
        RemoteProcessCall network = new RemoteProcessCall();
        [TestMethod]
        public void GetUserNameTest1()
        {
            network.init();
            String res = network.GetUserName("");
            Assert.AreEqual("Invalid", res);
        }
    }
}
