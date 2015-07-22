using System;
using Microsoft.VisualStudio.TestTools.UnitTesting;

namespace NetworkInterfaceUnitTest
{
    [TestClass]
    public class GetProviderSumTest : TestBase
    {
        [TestMethod]
        public void GetProviderSumTest1()
        {
            network.init();
            double res = network.GetProviderSum("11111111");
            Assert.AreEqual(-1, res);
        }

        /*[TestMethod]
        public void GetProviderSumTestAssociatedWithCurrentTime()
        {
            network.init();
            double res = network.GetProviderSum("12070002");
            double price_1 = network.GetServerPrice("000001");
            double price_2 = network.GetServerPrice("000002");
            Assert.AreEqual((price_1 + price_2), res);
        }*/
    }
}
