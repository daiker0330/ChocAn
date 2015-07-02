using System;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using ChocAnClient;


namespace NetworkInterfaceUnitTest
{
    [TestClass]
    public class Validtest
    {
        RemoteProcessCall network = new RemoteProcessCall();
        [TestMethod]
        public void ValidTest1()
        {
            network.init();
            int res = network.IsValid("12070405");
            Assert.AreEqual(1, res);
        }

        [TestMethod]
        public void ValidTest2()
        {
            network.init();
            int res = network.IsValid("12070403");
            Assert.AreEqual(-1, res);
        }

        [TestMethod]
        public void ValidTest3()
        {
            network.init();
            int res = network.IsValid("12070000");
            Assert.AreEqual(0, res);
        }

        [TestMethod]
        public void ValidTest4()
        {
            network.init();
            int res = network.IsValid("");
            Assert.AreEqual(0, res);
        }
    }
}
