using System;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using ChocAnClient;

namespace NetworkInterfaceUnitTest
{
    [TestClass]
    public class GetServerNameTest : TestBase
    {
        [TestMethod]
        public void ServerNameTest1()
        {
            network.init();
            String res = network.GetServerName("000001");
            Assert.AreEqual("Practice", res);
        }

        [TestMethod]
        public void ServerNameTest2()
        {
            network.init();
            String res = network.GetServerName("");
            Assert.AreEqual("Invalid", res);
        }
    }
}
