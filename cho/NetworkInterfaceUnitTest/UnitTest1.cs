using System;
using Microsoft.VisualStudio.TestTools.UnitTesting;

namespace NetworkInterfaceUnitTest
{
    [TestClass]
    public class SingleTest : TestBase
    {
        [TestMethod]
        public void InitTest()
        {
            network.init();
        }
    }
}
