using System;
using System.Collections.Generic;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Threading.Tasks;
using Wopr.Constants;
using static Wopr.TeamDirector;

namespace Wopr
{
    
    public class AssemblyLoader : MarshalByRefObject
    {
        private Dictionary<StrategyID, Type> _strategyTypesByStrategyID;

        public AssemblyLoader(string assemblyName)
        {
            var assemb = Assembly.LoadFrom(assemblyName);

            var types = from type in assemb.GetTypes()
                        where typeof(StrategyBase).IsAssignableFrom(type)
                        select type;

            _strategyTypesByStrategyID = types
               .ToDictionary(p => ((StrategyBase)Activator.CreateInstance(p)).StrategyID, r => r);
        }

        //public Dictionary<StrategyID, Type> LoadPlugins(string assemblyName)
        //{
        //    var assemb = Assembly.LoadFrom(assemblyName);

        //    var types = from type in assemb.GetTypes()
        //                where typeof(StrategyBase).IsAssignableFrom(type)
        //                select type;

        //    //Dictionary<StrategyID, StrategyInstanceCreationDelegate> instances = types.Select(
        //    //    v => new StrategyInstanceCreationDelegate(() =>
        //    //    {
        //    //        return (StrategyBase)Activator.CreateInstance(v);
        //    //    }))

        //    //    .ToDictionary(p => ((StrategyBase)Activator.CreateInstance(p)).StrategyID, r => r);

        //    Dictionary<StrategyID, Type> instances = types
        //        .ToDictionary(p => ((StrategyBase)Activator.CreateInstance(p)).StrategyID, r => r);

        //    return instances;
        //}

        public StrategyBase CreateInstance(StrategyID strategyID)
        {
            Type strategyType;

            if (_strategyTypesByStrategyID.TryGetValue(strategyID, out strategyType) == false)
                throw new NotSupportedException(strategyID.ToString());

            return (StrategyBase)Activator.CreateInstance(strategyType);
        }
    }
}
