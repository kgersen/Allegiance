using Microsoft.Win32;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Reflection;
using System.Security.Policy;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using Wopr.Constants;

namespace Wopr
{
    // Spins up clients and assigns strategies to them. 

    [Serializable]
    public class TeamDirector // : IDisposable
    {
        [Serializable]
        public delegate StrategyBase StrategyInstanceCreationDelegate();


        [NonSerialized]
        private CancellationTokenSource _cancellationTokenSource;

        private readonly string _lobbyAddress;
        private readonly string _botAuthenticationGuid;
        //private Dictionary<StrategyID, StrategyInstanceCreationDelegate> _strategiesByStrategyID;
        private AppDomain _currentAppDomain;
        private AssemblyLoader _assemblyLoader;

        private Dictionary<String, AllegianceInterop.ClientConnection> _connectedClientsByPlayerName = new Dictionary<String, AllegianceInterop.ClientConnection>();
        
        private Dictionary<String, StrategyBase> _currentStrategyByPlayerName { get; set; } = new Dictionary<string, StrategyBase>();
        private Dictionary<String, AllegianceInterop.ClientConnection.OnAppMessageDelegate> _currentStrategyAppMessageDelegateByPlayerName { get; set; } = new Dictionary<string, AllegianceInterop.ClientConnection.OnAppMessageDelegate>();

        [NonSerialized]
        Task _messagePump;

        public TeamDirector(CancellationTokenSource cancellationTokenSource, string lobbyAddress)
        {
            _cancellationTokenSource = cancellationTokenSource;
            _lobbyAddress = lobbyAddress;

            using (var view32 = RegistryKey.OpenBaseKey(RegistryHive.LocalMachine,
                                                RegistryView.Registry32))
            {
                using (var key = view32.OpenSubKey(@"SOFTWARE\Microsoft\Microsoft Games\Allegiance\1.4\Server"))
                {
                    _botAuthenticationGuid = (string)key.GetValue("BotAuthenticationGuid");
                }
            }

            LoadStrategies();

            _messagePump = Task.Run(() =>
            {
                SendAndReceiveMessagesForAllClients(cancellationTokenSource);

                
            });
        }

        //public void Dispose()
        //{
        //    _cancellationTokenSource.Cancel();

        //    lock (_connectedClients)
        //    {
        //        _connectedClients.AsParallel().ForAll((AllegianceInterop.ClientConnection connection) =>
        //        {
        //            connection.DisconnectLobby();
        //            connection.DisconnectServer();
        //        });
        //    }
        //}

        public void DisconnectAllClients()
        {
            lock (_connectedClientsByPlayerName)
            {
                _connectedClientsByPlayerName.Values.AsParallel().ForAll((AllegianceInterop.ClientConnection connection) =>
                {
                    connection.DisconnectLobby();
                    connection.DisconnectServer();
                });
            }
        }

        private void SendAndReceiveMessagesForAllClients(CancellationTokenSource cancellationTokenSource)
        {
            while (cancellationTokenSource.IsCancellationRequested == false)
            {
                lock (_connectedClientsByPlayerName)
                {
                    _connectedClientsByPlayerName.Values.AsParallel().ForAll((AllegianceInterop.ClientConnection connection) =>
                    {
                        connection.SendAndReceiveUpdate();
                    });
                }

                foreach (var strategy in _currentStrategyByPlayerName.Values.ToArray())
                {
                    if (strategy.IsStrategyComplete == true)
                        StrategyComplete(strategy);
                }

                Thread.Sleep(10);
            }
        }

        public void LoadStrategies()
        {
            var an = Assembly.GetExecutingAssembly().GetName();

            var appDomainSetup = new AppDomainSetup
            {
                PrivateBinPath = AppDomain.CurrentDomain.BaseDirectory,
                ShadowCopyDirectories = AppDomain.CurrentDomain.BaseDirectory,
                ShadowCopyFiles = "true"
            };

            _currentAppDomain = AppDomain.CreateDomain("V1", (Evidence)null, appDomainSetup);
            Console.WriteLine(_currentAppDomain.BaseDirectory);

            _currentAppDomain.Load(typeof(AssemblyLoader).Assembly.FullName);

            string target = Path.Combine(_currentAppDomain.BaseDirectory, "Strategies.dll");

            _assemblyLoader = (AssemblyLoader)Activator.CreateInstance(
                _currentAppDomain,
                typeof(AssemblyLoader).Assembly.FullName,
                typeof(AssemblyLoader).FullName,
                false,
                BindingFlags.Public | BindingFlags.Instance,
                null,
                new object[] { target },
                null,
                null).Unwrap();

            //string target = @"C:\Source\git\Allegiance\src\Wopr\Strategies\bin\Debug\Strategies_V1.dll";
            

            //_strategiesByStrategyID = loader.LoadPlugins(target);

            //_strategiesByStrategyID = loadedStrategies.ToDictionary(p => p.StrategyID, r => r);
        }

        //public StrategyBase GetStrategyInstance(StrategyID strategyID)
        //{
        //    StrategyBase returnValue;

        //    StrategyInstanceCreationDelegate strategyCreationDeletgate;
        //    if (_strategiesByStrategyID.TryGetValue(strategyID, out strategyCreationDeletgate) == true)
        //    {
        //        returnValue = strategyCreationDeletgate();
        //    }
        //    else
        //    {
        //        throw new NotSupportedException(strategyID.ToString());
        //    }

        //    return returnValue;
        //}

        public void ChangeStrategy(StrategyID strategyID, string playerName, short sideIndex, bool isGameController, bool isCommander)
        {
            AllegianceInterop.ClientConnection clientConnection;

            if (_connectedClientsByPlayerName.TryGetValue(playerName, out clientConnection) == false)
            {
                throw new Exception("Couldn't get connection for playerName: " + playerName);
            }

            AllegianceInterop.ClientConnection.OnAppMessageDelegate currentStrategyOnAppMessageDelegate;
            if (_currentStrategyAppMessageDelegateByPlayerName.TryGetValue(playerName, out currentStrategyOnAppMessageDelegate) == true)
            {
                clientConnection.OnAppMessage -= currentStrategyOnAppMessageDelegate;
                _currentStrategyAppMessageDelegateByPlayerName.Remove(playerName);
            }

            StrategyBase currentStrategy;
            if (_currentStrategyByPlayerName.TryGetValue(playerName, out currentStrategy) == false)
            {
                currentStrategy = null;
            }
            else
            {
                //currentStrategy.OnStrategyComplete -= Strategy_OnStrategyComplete;
                _currentStrategyByPlayerName.Remove(playerName);
            }

            StrategyBase strategy = _assemblyLoader.CreateInstance(strategyID);
            //if (_strategiesByStrategyID.TryGetValue(strategyID, out strategy) == true)
            //{
                strategy.Attach(clientConnection, _botAuthenticationGuid, playerName, sideIndex, isGameController, isCommander);

                // can't use an event handler across app domain boundaries, the handler class will not serialize correctly (dictionaries remain empty.)
                //strategy.OnStrategyComplete += Strategy_OnStrategyComplete;

                // Can't hook the transparent object directly to the client, the events raised by c++/cli won't trigger on the managed end
                // becuase they are attached to a copy of the object in the far application domain, so we will attach to the event on the 
                // near application domain, then directly call the MessageReceiver handler in the far domain passing the byte array 
                // through instead of the tranparent proxied object.
                AllegianceInterop.ClientConnection.OnAppMessageDelegate onAppMessageDelegate = new AllegianceInterop.ClientConnection.OnAppMessageDelegate((AllegianceInterop.ClientConnection messageClientConnection, byte[] bytes) =>
                {
                    strategy.OnAppMessage(bytes);
                });

                clientConnection.OnAppMessage += onAppMessageDelegate;

                _currentStrategyAppMessageDelegateByPlayerName.Add(playerName, onAppMessageDelegate);
                _currentStrategyByPlayerName.Add(playerName, strategy);

                
            //}
            //else
            //{
            //    throw new Exception($"Error, couldn't find the {strategyID.ToString()} strategy in the loaded strategy list.");
            //}

        }

        public void CreatePlayer(string playerName, short sideIndex, bool isGameController, bool isCommander)
        {
            AllegianceInterop.ClientConnection clientConnection = new AllegianceInterop.ClientConnection();

            lock (_connectedClientsByPlayerName)
            {
                _connectedClientsByPlayerName.Add(playerName, clientConnection);
            }

            //StrategyBase strategy;
            //if (_strategiesByStrategyID.TryGetValue(StrategyID.ConnectToGame, out strategy) == true)
            //{
            //    strategy.Attach(clientConnection, _botAuthenticationGuid, playerName, sideIndex, isGameController, isCommander);

            //    strategy.OnStrategyComplete += Strategy_OnConnectToGameStrategyComplete;

            //    // Can't hook the transparent object directly to the client, the events raised by c++/cli won't trigger on the managed end
            //    // becuase they are attached to a copy of the object in the far application domain, so we will attach to the event on the 
            //    // near application domain, then directly call the MessageReceiver handler in the far domain passing the byte array 
            //    // through instead of the tranparent proxied object.
            //    clientConnection.OnAppMessage += new AllegianceInterop.ClientConnection.OnAppMessageDelegate((AllegianceInterop.ClientConnection messageClientConnection, byte[] bytes) =>
            //    {
            //        strategy.OnAppMessage(bytes);
            //    });
            //}
            //else
            //{
            //    throw new Exception("Error, couldn't find the ConnectToGame strategy in the loaded strategy list.");
            //}

            ChangeStrategy(StrategyID.ConnectToGame, playerName, sideIndex, isGameController, isCommander);
            

            clientConnection.ConnectToLobby(_cancellationTokenSource, _lobbyAddress, playerName, _botAuthenticationGuid);
        }

        private void StrategyComplete(StrategyBase strategy)
        {
            //StrategyBase currentStrategy;
            //if (CurrentStrategyByPlayerName.TryGetValue(strategy.PlayerName, out currentStrategy) == true)
            //{
                Console.WriteLine($"TeamDirector {strategy.PlayerName}: {strategy.StrategyID} has completed.");

                if (strategy.IsCommander == true)
                {
                    switch (strategy.StrategyID)
                    {
                        case StrategyID.ConnectToGame:
                            ChangeStrategy(StrategyID.CommanderResearchAndExpand, strategy.PlayerName, strategy.SideIndex, strategy.IsGameController, strategy.IsCommander);
                            break;

                        default:
                            Console.WriteLine($"TeamDirector {strategy.PlayerName}: couldn't determine next strategy from the current strategy. (Should probably set a default here?)");
                            break;

                    }
                }
                else
                {
                    Console.WriteLine($"TeamDirector {strategy.PlayerName}: need to setup pilot strategies.");
                }
            //}
            //else
            //{
            //    Console.WriteLine($"TeamDirector {strategy.PlayerName}: no current strategy could be found for this player, this player is probably stuck.");
            //}

        }
            

        

        //private void clientConnection_OnAppMessage(AllegianceInterop.ClientConnection clientConnection, byte[] bytes)
        //{
        //    _currentStrategies.AsParallel().ForAll((StrategyBase strategy) =>
        //    {
        //        strategy.OnAppMessage(bytes);
        //    });

        //}
    }
}
