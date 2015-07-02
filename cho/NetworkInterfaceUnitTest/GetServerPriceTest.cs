using System;
using Microsoft.VisualStudio.TestTools.UnitTesting;

namespace NetworkInterfaceUnitTest
{
    [TestClass]
    public class GetServerPriceTest : TestBase
    {
        [TestMethod]
        public void GetServerPriceTest1()
        {
            network.init();
            double res = network.GetServerPrice("000002");
            Assert.AreEqual(60.0, res);
        }

        [TestMethod]
        public void GetServerPriceTest2()
        {
            network.init();
            double res = network.GetServerPrice("0");
            Assert.AreEqual(-1, res);
        }

    }
}
