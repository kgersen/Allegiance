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
            if (args.Length == 2 && args[0] == "-t")
            {
                byte[] data = File.ReadAllBytes(args[1]);
                new MessageReceiver("TestUser").OnAppMessage(null, data);
                return;
            }

            CancellationTokenSource cancellationTokenSource = new CancellationTokenSource();



            TeamDirector teamDirectorYellow = new TeamDirector(cancellationTokenSource, "127.0.0.1");
            TeamDirector teamDirectorBlue = new TeamDirector(cancellationTokenSource, "127.0.0.1");

            // Yellow Team
            teamDirectorYellow.CreatePlayer("yellowcom@BOT", 0, true, true);


            // Blue Team
            teamDirectorBlue.CreatePlayer("bluecom@BOT", 1, false, true);

            do
            {
                var keyInfo = Console.ReadKey();
                if (keyInfo.KeyChar == 'q')
                {
                    cancellationTokenSource.Cancel();
                    break;
                }
                else
                {
                    Console.WriteLine("\nPress q to quit.");
                }
            } while (true);

            
            Console.WriteLine("Disconnecting Yellow Team.");
            teamDirectorYellow.DisconnectAllClients();

            Console.WriteLine("Disconnecting Blue Team.");
            teamDirectorBlue.DisconnectAllClients();

            Console.WriteLine("Done.");
        }
        
    }
}
