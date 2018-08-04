using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Wopr;

namespace Strategies
{
    public class CommanderResearchAndExpand : Wopr.StrategyBase
    {
        public CommanderResearchAndExpand() : base(Wopr.Constants.StrategyID.CommanderResearchAndExpand)
        {
        }

        public override void AttachMessages(MessageReceiver messageReceiver, string botAuthenticationGuid, string playerName, int sideIndex, bool isGameController, bool isCommander)
        {
            InvestInTech(".Miner");
            InvestInTech("Outpost");
            InvestInTech("Teleport");
        }

        private void InvestInTech(string techName)
        {
            var side = _client.GetSide();
            var buckets = side.GetBuckets();
            var money = _client.GetMoney();
            //var station = side.GetCu

            foreach (var bucket in buckets)
            {
                if (bucket.GetName().StartsWith(techName) == true && side.CanBuy(bucket) == true)
                {
                    if (money > bucket.GetPrice()/* && bucket.GetPercentComplete() <= 0*/)
                    {
                        Log("Adding money to bucket. Bucket wants: " + bucket.GetBuyable().GetPrice() + ", we have: " + money);
                        _client.AddMoneyToBucket(bucket, bucket.GetPrice());
                    }
                    else
                    {
                        Log($"Not enough money to buy {techName}");
                        //Log("Already building: " + bucket.GetPercentComplete());
                    }
                }
            }
        }
    }
}
