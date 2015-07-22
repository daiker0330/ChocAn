using System;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using ChocAnClient;

namespace NetworkInterfaceUnitTest
{
    [TestClass]
    public class SignInTest
    {
        RemoteProcessCall network = new RemoteProcessCall();
        [TestMethod]
        public void SignInTest1()
        {
            network.init();
            bool res = network.SignIn("hahaha");
            Assert.IsFalse(res);
        }

        [TestMethod]
        public void SignInTest2()
        {
            network.init();
            bool res = network.SignIn("120700010");
            Assert.IsTrue(res);
        }

        [TestMethod]
        public void SignInTest3()
        {
            network.init();
            bool res = network.SignIn("");
            Assert.IsFalse(res);
        }

        [TestMethod]
        public void ManySignInTest()
        {
            for(int i=1;i<=100;i++)
            {
                network.init();
                bool res = network.SignIn("hahaha");
                Assert.IsFalse(res);
            }
        }
    }
}
