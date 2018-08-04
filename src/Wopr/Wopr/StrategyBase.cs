using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Wopr;
using Wopr.Constants;

namespace Wopr
{
    public abstract class StrategyBase : MarshalByRefObject
    {
        //public delegate void OnStrategryCompleteHandler(string playerName, short sideIndex, bool isGameController, bool isCommander);
        //public event OnStrategryCompleteHandler OnStrategyComplete;

        protected AllegianceInterop.ClientConnection _client;
        protected MessageReceiver _messageReceiver;

        public StrategyID StrategyID { get; }
        public String PlayerName { get; set; }
        public bool IsGameController { get; set; }
        public bool IsCommander { get; set; }
        public string BotAuthenticationGuid { get; set; }
        public short SideIndex { get; set; }


        public bool IsStrategyComplete { get; set; } = false;

        

        public StrategyBase(StrategyID strategyID)
        {
            StrategyID = strategyID;
        }

        ~StrategyBase()
        {
            Console.WriteLine($"Strategy destroyed for {PlayerName}, {StrategyID.ToString()}");
        }

        public void Attach(AllegianceInterop.ClientConnection client, string botAuthenticationGuid, string playerName, short sideIndex, bool isGameController, bool isCommander)
        {
            _client = client;
            PlayerName = playerName;
            SideIndex = sideIndex;
            IsGameController = isGameController;
            IsCommander = isCommander;
            BotAuthenticationGuid = botAuthenticationGuid;

            _messageReceiver = new MessageReceiver(playerName);
            //client.OnAppMessage += _messageReceiver.OnAppMessage;

            AttachMessages(_messageReceiver, botAuthenticationGuid, playerName, sideIndex, isGameController, isCommander);
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
        
        public void OnAppMessage(byte[] bytes)
        {
            _messageReceiver.OnAppMessage(this._client, bytes);
        }

        public void Log(string message)
        {
            Console.WriteLine($"{StrategyID.ToString()} {PlayerName}: {message}");
        }

        public abstract void AttachMessages(MessageReceiver messageReceiver, string botAuthenticationGuid, string playerName, int sideIndex, bool isGameController, bool isCommander);
        //public abstract void DetachMessages(MessageReceiver messageReceiver);
       
    }
}
