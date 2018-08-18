using Newtonsoft.Json;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Reflection;
using System.Runtime.InteropServices;
using System.Security.Policy;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using Microsoft.Win32;
using Wopr.Constants;
using AllegianceInterop;
using System.Diagnostics;

namespace Wopr
{
    // This application uses C++/CLI (managed C++) to talk to the Allegiance core.
    // To make this work in a debugger, you must disable the LoaderLock exception in Debug->Windows->Exceptions, then ManagedExceptions->LoaderLock (Disable)
    class Program
    {
        static void Main(string[] args)
        {
            Program p = new Program();
            p.Run(args);
        }

        private void Run(string[] args)
        {
            //AllegianceInterop.ClientConnection cc = new ClientConnection();
            //cc.TestNativeStuff();

            //var ad1 = AppDomain.CreateDomain("domain 1");
            //var ad2 = AppDomain.CreateDomain("domain 2");

            //var c1 = (AllegianceInterop.ClientConnection) ad1.CreateInstanceAndUnwrap(Assembly.GetAssembly(typeof(AllegianceInterop.ClientConnection)).FullName, typeof(AllegianceInterop.ClientConnection).FullName);
            //var c2 = (AllegianceInterop.ClientConnection) ad2.CreateInstanceAndUnwrap(Assembly.GetAssembly(typeof(AllegianceInterop.ClientConnection)).FullName, typeof(AllegianceInterop.ClientConnection).FullName);

            //TestObject localObject = new TestObject();

            //localObject.number2 = 1;

            //c1.AddGameInfoChangedCallback((TestObject testObject) =>
            //{
            //    localObject.var1 = testObject.var1;
            //    localObject.number2 = testObject.number2;
            //    c2.UpdateTestObject(localObject);
            //});

            //c2.AddGameInfoChangedCallback((TestObject testObject) =>
            //{
            //    localObject.var1 = testObject.var1;
            //    localObject.number2 = testObject.number2;
            //    c1.UpdateTestObject(testObject);
            //});

            //Console.WriteLine($"c1: {c1.GetTestObjectNumber()}, c2: {c2.GetTestObjectNumber()}, local: {localObject.number2}");

            //c1.SetTestObjectNumber(2);

            //Console.WriteLine($"c1: {c1.GetTestObjectNumber()}, c2: {c2.GetTestObjectNumber()}, local: {localObject.number2}");

            //c2.SetTestObjectNumber(3);

            //Console.WriteLine($"c1: {c1.GetTestObjectNumber()}, c2: {c2.GetTestObjectNumber()}, local: {localObject.number2}");

            //c1.SetTestObjectRef(localObject);
            //c2.SetTestObjectRef(localObject);

            //Console.WriteLine($"c1: {c1.GetTestObjectRefNumber()}, c2: {c2.GetTestObjectRefNumber()}, local: {localObject.number2}");

            //localObject.number2 = 4;

            //Console.WriteLine($"c1: {c1.GetTestObjectRefNumber()}, c2: {c2.GetTestObjectRefNumber()}, local: {localObject.number2}");

            bool launchWithoutYellowCom = false;
            bool launchWithYellowComOnly = false;

            if (args.Length == 2 && args[0] == "-t")
            {
                byte[] data = File.ReadAllBytes(args[1]);
                new MessageReceiver("TestUser").OnAppMessage(null, data);
                return;
            }

            if (args.Length == 1 && args[0] == "-y")
            {
                Console.WriteLine("Will launch without yellow comm for debugging.");
                launchWithoutYellowCom = true;
            }

            if (args.Length == 1 && args[0] == "-c")
            {
                Console.WriteLine("Will launch with yellow comm only for debugging.");
                launchWithYellowComOnly = true;
            }

            if (Directory.Exists(@"c:\1\Logs\") == false)
                Directory.CreateDirectory(@"c:\1\Logs");

            if (launchWithYellowComOnly == false)
            {
                foreach (var file in Directory.GetFiles(@"c:\1\Logs\", "*.txt"))
                    File.Delete(file);
            }
            else
            {
                foreach (var file in Directory.GetFiles(@"c:\1\Logs\", "*yellowcom*.txt"))
                    File.Delete(file);
            }
            
            CancellationTokenSource cancellationTokenSource = new CancellationTokenSource();
            
            TeamDirector teamDirectorYellow = new TeamDirector(cancellationTokenSource, "127.0.0.1");
            TeamDirector teamDirectorBlue = new TeamDirector(cancellationTokenSource, "127.0.0.1");

            // Yellow Team

            if (launchWithoutYellowCom == false)
                 teamDirectorYellow.CreatePlayer("yellowcom@BOT", 0, false, true);

            if (launchWithYellowComOnly == false)
            {

                //teamDirectorYellow.CreatePlayer("yellow_XX@BOT", 0, false, false);

                for (int i = 0; i < 8; i++)
                    teamDirectorYellow.CreatePlayer("yellow_" + i + "@BOT", 0, false, false);

                //List<string> playerNames = new List<string>();
                //for (int i = 0; i < 10; i++)
                //    playerNames.Add("yellow_" + i + "@BOT");

                //playerNames.AsParallel().ForAll((string playerName) =>
                //{
                //    teamDirectorYellow.CreatePlayer(playerName, 0, false, false);
                //});


                // Blue Team
                teamDirectorBlue.CreatePlayer("bluecom@BOT", 1, true, true);

                for (int i = 0; i < 8; i++)
                    teamDirectorBlue.CreatePlayer("blue_" + i + "@BOT", 1, false, false);
            }

            //playerNames = new List<string>();
            //for (int i = 0; i < 10; i++)
            //    playerNames.Add("blue_" + i + "@BOT");

            //playerNames.AsParallel().ForAll((string playerName) =>
            //{
            //    teamDirectorBlue.CreatePlayer(playerName, 1, false, false);
            //});

            do
            {
                //Stopwatch sw = new Stopwatch();
                //sw.Start();

                //teamDirectorYellow.SendAndReceiveMessagesForAllClients();

                //sw.Stop();
                //if(sw.ElapsedMilliseconds > 100)
                //    Console.WriteLine($"teamDirectorYellow.SendAndReceiveMessagesForAllClients run time: {sw.ElapsedMilliseconds}");

                //sw = new Stopwatch();
                //sw.Start();

                //teamDirectorBlue.SendAndReceiveMessagesForAllClients();

                //sw.Stop();
                //if (sw.ElapsedMilliseconds > 100)
                //    Console.WriteLine($"teamDirectorBlue.SendAndReceiveMessagesForAllClients run time: {sw.ElapsedMilliseconds}");

                if (Console.KeyAvailable)
                {
                    ConsoleKeyInfo key = Console.ReadKey(true);
                    switch (key.Key)
                    {
                        case ConsoleKey.Q:
                            cancellationTokenSource.Cancel();
                            break;
                        default:
                            Console.WriteLine("\nPress q to quit.");
                            break;
                    }
                }

                teamDirectorYellow.UpdateTeamStrategies();
                teamDirectorBlue.UpdateTeamStrategies();

                Thread.Sleep(100);

                //var keyInfo = Console.ReadKey();
                //if (keyInfo.KeyChar == 'q')
                //{
                //    cancellationTokenSource.Cancel();
                //    break;
                //}
                //else
                //{
                //    Console.WriteLine("\nPress q to quit.");
                //}
            } while (cancellationTokenSource.IsCancellationRequested == false);

            
            Console.WriteLine("Disconnecting Yellow Team.");
            teamDirectorYellow.DisconnectAllClients();

            Console.WriteLine("Disconnecting Blue Team.");
            teamDirectorBlue.DisconnectAllClients();

            Console.WriteLine("Done.");
        }
        
    }
}
