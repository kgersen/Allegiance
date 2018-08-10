using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using AllegianceInterop;
using Wopr;
using Wopr.Constants;
using Wopr.Entities;

namespace Wopr.Strategies
{
    public abstract class StrategyBase : MarshalByRefObject
    {
        public delegate void OnStrategryCompleteHandler(StrategyBase strategy);
        public event OnStrategryCompleteHandler OnStrategyComplete;


        private static Dictionary<StrategyID, Type> _strategyTypesByStrategyID = null;

        protected static Random _random = new Random();

        protected int _runningTasks = 0;

        
        
        protected MessageReceiver _messageReceiver;

        public AllegianceInterop.ClientConnection ClientConnection { get; set; }


        public StrategyID StrategyID { get; }
        public TimeSpan Timeout { get; }

        public DateTime StartTime { get; }

        public String PlayerName { get; set; }
        public bool IsGameController { get; set; }
        public bool IsCommander { get; set; }
        public string BotAuthenticationGuid { get; set; }
        public short SideIndex { get; set; }


        //public bool IsStrategyComplete { get; set; } = false;

        protected GameInfo GameInfo { get; set; } 

        protected CancellationTokenSource _cancellationTokenSource { get; set; }

        public StrategyBase(StrategyID strategyID, TimeSpan timeout)
        {
            StrategyID = strategyID;
            Timeout = timeout;

            StartTime = DateTime.Now;

            _cancellationTokenSource = new CancellationTokenSource();
        }

        ~StrategyBase()
        {
            //if (_client != null)
            //{
            //    _client.OnAppMessage -= _messageReceiver.OnAppMessage;
            //    _client = null;
            //    _messageReceiver = null;
            //}

            Console.WriteLine($"Strategy destroyed for {PlayerName}, {StrategyID.ToString()}");
        }

        public void Attach(AllegianceInterop.ClientConnection client, GameInfo gameInfo, string botAuthenticationGuid, string playerName, short sideIndex, bool isGameController, bool isCommander)
        {
            ClientConnection = client;
            PlayerName = playerName;
            SideIndex = sideIndex;
            IsGameController = isGameController;
            IsCommander = isCommander;
            BotAuthenticationGuid = botAuthenticationGuid;
            GameInfo = gameInfo;

            _messageReceiver = new MessageReceiver(playerName);
            client.OnAppMessage += _messageReceiver.OnAppMessage;

            _messageReceiver.FMD_S_JOIN_SIDE += _messageReceiver_FMD_S_JOIN_SIDE;
            _messageReceiver.FMD_S_SHIP_STATUS += _messageReceiver_FMD_S_SHIP_STATUS;
            //_messageReceiver.FMD_S_EXPORT += _messageReceiver_FMD_S_EXPORT;
            _messageReceiver.FMD_S_CLUSTERINFO += _messageReceiver_FMD_S_CLUSTERINFO;

            AttachMessages(_messageReceiver, botAuthenticationGuid, playerName, sideIndex, isGameController, isCommander);

            Log($"Starting strategy: {StrategyID.ToString()}");

            Start();
        }

        private void _messageReceiver_FMD_S_CLUSTERINFO(ClientConnection client, AllegianceInterop.FMD_S_CLUSTERINFO message)
        {
            GameInfo.Clusters = message.Clusters;
        }

        //private void _messageReceiver_FMD_S_EXPORT(ClientConnection client, AllegianceInterop.FMD_S_EXPORT message)
        //{
        //    var exportObjectType = (ObjectType)message.objecttype;

        //    Log($"Export ObjectType {exportObjectType.ToString()} received.");
        //}


        private void _messageReceiver_FMD_S_SHIP_STATUS(ClientConnection client, AllegianceInterop.FMD_S_SHIP_STATUS message)
        {
            // Check to ensure that we don't have any money hanging around in the bank. 
            if (client.SideLeaderShipID(SideIndex) != client.GetShip()?.GetObjectID() && client.GetMoney() > 1000)
                SetAutoDonateAndDonateAllMoneyToTeamLeader(client.GetSide()?.GetShip(client.SideLeaderShipID(SideIndex)));

        }

        private void SetAutoDonateAndDonateAllMoneyToTeamLeader(IshipIGCWrapper teamLeaderShip)
        {
            if (teamLeaderShip != null)
            {
                Log($"Setting auto-donate to {teamLeaderShip?.GetName()}, donating {ClientConnection.GetMoney()} to the new commander.");

                AllegianceInterop.FMD_C_AUTODONATE autoDonate = new AllegianceInterop.FMD_C_AUTODONATE(teamLeaderShip.GetObjectID(), ClientConnection.GetMoney());
                ClientConnection.SendMessageServer(autoDonate);
            }
        }

        // If a commander joins our side, then change autodonate to that player, and donate all your money!
        private void _messageReceiver_FMD_S_JOIN_SIDE(ClientConnection client, AllegianceInterop.FMD_S_JOIN_SIDE message)
        {
            if (message.shipID == client.GetShip().GetObjectID())
                return;

            if (client.GetSide()?.GetShip(message.shipID)?.GetName().EndsWith("com@BOT") == true)
            {
                SetAutoDonateAndDonateAllMoneyToTeamLeader(client.GetSide()?.GetShip(message.shipID));
            }
            else
            {
                Log($"New player joined side: {client.GetSide()?.GetShip(message.shipID)?.GetName()}");
            }
        }

        //public void Detach()
        //{
        //    DetachMessages(_messageReceiver);
        //}

        public void FinishStrategy()
        {
            // Don't receive any more messages from the client for this strategy. 
            ClientConnection.OnAppMessage -= _messageReceiver.OnAppMessage;

            OnStrategyComplete?.Invoke(this);
            //IsStrategyComplete = true;
        }

        public void Cancel()
        {
            _cancellationTokenSource.Cancel();

            for (int i = 0; i < 10 * 100 && _runningTasks > 0; i++)
                Thread.Sleep(100);

            if (_runningTasks > 0)
                throw new Exception("Error: All running tasks didn't exit after 10 seconds after a cancel was requested.");
        }
        
        public void OnAppMessage(byte[] bytes)
        {
            _messageReceiver.OnAppMessage(this.ClientConnection, bytes);
        }

        public void Log(string message)
        {
            Console.WriteLine($"{StrategyID.ToString()} {PlayerName}: {message}");

            string filename = $"c:\\1\\Logs\\{PlayerName}_strategy.txt";

            Exception lastException = null;
            for (int i = 0; i < 30; i++)
            {
                try
                {
                    File.AppendAllText(filename, $"{DateTime.Now} {StrategyID.ToString()} {PlayerName}: {message}\n");
                    lastException = null;
                    break;
                }
                catch (Exception ex)
                {
                    lastException = ex;
                    Thread.Sleep(10);
                }
            }

            if (lastException != null)
                throw lastException;
        }

        public abstract void AttachMessages(MessageReceiver messageReceiver, string botAuthenticationGuid, string playerName, int sideIndex, bool isGameController, bool isCommander);
        //public abstract void DetachMessages(MessageReceiver messageReceiver);
        public abstract void Start();

        internal static StrategyBase GetInstance(StrategyID strategyID, ClientConnection clientConnection, GameInfo gameInfo, string botAuthenticationGuid, string playerName, short sideIndex, bool isGameController, bool isCommander)
        {
            StrategyBase returnValue = null;

            if (_strategyTypesByStrategyID == null)
            {
                var types = from type in Assembly.GetAssembly(typeof(StrategyBase)).GetTypes()
                            where typeof(StrategyBase).IsAssignableFrom(type) && type.IsAbstract == false 
                            select type;

                _strategyTypesByStrategyID = types
                   .ToDictionary(p => ((StrategyBase)Activator.CreateInstance(p)).StrategyID, r => r);
            }

            if(_strategyTypesByStrategyID.ContainsKey(strategyID) == false)
                throw new NotSupportedException(strategyID.ToString());

            returnValue = (StrategyBase)Activator.CreateInstance(_strategyTypesByStrategyID[strategyID]);

            returnValue.Attach(clientConnection, gameInfo, botAuthenticationGuid, playerName, sideIndex, isGameController, isCommander);

            return returnValue;
        }
    }
}
