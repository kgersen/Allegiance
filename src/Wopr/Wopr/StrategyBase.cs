using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using Wopr;
using Wopr.Constants;
using Wopr.Entities;

namespace Wopr
{
    public abstract class StrategyBase : MarshalByRefObject
    {
        protected static Random _random = new Random();

        protected int _runningTasks = 0;

        //public delegate void OnStrategryCompleteHandler(string playerName, short sideIndex, bool isGameController, bool isCommander);
        //public event OnStrategryCompleteHandler OnStrategyComplete;

        protected AllegianceInterop.ClientConnection _client;
        protected MessageReceiver _messageReceiver;

        public StrategyID StrategyID { get; }
        public TimeSpan Timeout { get; }

        public DateTime StartTime { get; }

        public String PlayerName { get; set; }
        public bool IsGameController { get; set; }
        public bool IsCommander { get; set; }
        public string BotAuthenticationGuid { get; set; }
        public short SideIndex { get; set; }


        public bool IsStrategyComplete { get; set; } = false;

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
            Console.WriteLine($"Strategy destroyed for {PlayerName}, {StrategyID.ToString()}");
        }

        public void Attach(AllegianceInterop.ClientConnection client, GameInfo gameInfo, string botAuthenticationGuid, string playerName, short sideIndex, bool isGameController, bool isCommander)
        {
            _client = client;
            PlayerName = playerName;
            SideIndex = sideIndex;
            IsGameController = isGameController;
            IsCommander = isCommander;
            BotAuthenticationGuid = botAuthenticationGuid;
            GameInfo = gameInfo;

            _messageReceiver = new MessageReceiver(playerName);
            //client.OnAppMessage += _messageReceiver.OnAppMessage;

            AttachMessages(_messageReceiver, botAuthenticationGuid, playerName, sideIndex, isGameController, isCommander);

            Log($"Starting strategy: {StrategyID.ToString()}");

            Start();
        }

        //public void Detach()
        //{
        //    DetachMessages(_messageReceiver);
        //}

        public void FinishStrategy()
        {
            //OnStrategyComplete?.Invoke(_playerName, _sideIndex, _isGameController, _isCommander);
            IsStrategyComplete = true;
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
            _messageReceiver.OnAppMessage(this._client, bytes);
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

    }
}
