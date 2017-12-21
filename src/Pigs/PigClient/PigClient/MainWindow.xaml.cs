using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.ComponentModel;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace WpfApp1
{

    public class PigInfo : INotifyPropertyChanged
    {
        public event PropertyChangedEventHandler PropertyChanged;

        private PigsLib.IPig _pig;
        public PigsLib.IPig Pig
        {
            get { return _pig; }
            set
            {
                _pig = value;
                PropertyChanged?.Invoke(this, new PropertyChangedEventArgs("Pig"));
            }
        }

        private string _name;
        public string Name
        {
            get { return _name; }
            set
            {
                _name = value;
                PropertyChanged?.Invoke(this, new PropertyChangedEventArgs("Name"));
            }
        }

        private string _state;
        private string _gameName;
        private int _money;
        private string _hull;
        private string _position;

        public string State
        {
            get { return _state; }
            set
            {
                _state = value;
                PropertyChanged?.Invoke(this, new PropertyChangedEventArgs("State"));
            }
        }

        public string GameName
        {
            get => _gameName; internal set
            {
                _gameName = value;
                PropertyChanged?.Invoke(this, new PropertyChangedEventArgs("GameName"));
            }
        }
        public int Money
        {
            get => _money; internal set
            {
                _money = value;
                PropertyChanged?.Invoke(this, new PropertyChangedEventArgs("Money"));
            }
        }

        public string Hull
        {
            get => _hull; internal set
            {
                _hull = value;
                PropertyChanged?.Invoke(this, new PropertyChangedEventArgs("Hull"));
            }
        }

        public string Position { get => _position; internal set
            {
                _position = value;
                PropertyChanged?.Invoke(this, new PropertyChangedEventArgs("Position"));
            }
         }
    }
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window, INotifyPropertyChanged
    {
        PigsLib.PigSession session = null;

        public MainWindow()
        {
            InitializeComponent();
            this.DataContext = this;
        }

        public event PropertyChangedEventHandler PropertyChanged;

        string _logText = "";
        public string logText
        {
            get { return _logText; }
            set {
                _logText = value;
                PropertyChanged?.Invoke(this, new PropertyChangedEventArgs("logText"));
            }
        }

        bool _pigButtonEnable = false;
        public bool pigButtonEnable
        {
            get { return _pigButtonEnable; }
            set { _pigButtonEnable = value;
                PropertyChanged?.Invoke(this, new PropertyChangedEventArgs("pigButtonEnable"));
            }
        }

        string _pigSript = "";
        public string pigScript
        {
            get { return _pigSript; }
            set
            {
                _pigSript = value;
                PropertyChanged?.Invoke(this, new PropertyChangedEventArgs("pigScript"));
            }
        }

        List<string> _pigSripts = new List<string>();
        public List<string> pigScripts
        {
            get { return _pigSripts; }
            set
            {
                _pigSripts = value;
                PropertyChanged?.Invoke(this, new PropertyChangedEventArgs("pigScripts"));
            }
        }

        ObservableCollection<PigInfo> _pigInfos = new ObservableCollection<PigInfo>();
        public ObservableCollection<PigInfo> pigInfos
        {
            get { return _pigInfos; }
            set
            {
                _pigInfos = value;
                PropertyChanged?.Invoke(this, new PropertyChangedEventArgs("pigInfos"));
            }
        }
        
        void log(string text)
        {
            logText += string.Format("{0}\n", text);
        }

        private void EnableEvents_Click(object sender, RoutedEventArgs e)
        {
            if (session != null)
            {
                session.ActivateAllEvents();
                log(String.Format("Activated all events"));
            }
        }
        private void DisableEvents_Click(object sender, RoutedEventArgs e)
        {
            if (session != null)
            {
                session.DeactivateAllEvents();
                log(String.Format("Deactivated all events."));
            }
        }

        private void Disconnect_Click(object sender, RoutedEventArgs e)
        {
            if (session != null)
            {
                session = null;
                logText = "";
            }
        }
        private void ClearLog_Click(object sender, RoutedEventArgs e)
        {
            logText = "";
        }

        private void Connect_Click(object sender, RoutedEventArgs e)
        {
            if (session == null)
            {
                log(String.Format("Connecting to PigSrv..."));
                session = new PigsLib.PigSession();
                if (session != null)
                {
                    pigButtonEnable = true;
                    log(String.Format("Connected to process: {0}", session.ProcessID));
                    session.OnEvent += Session_OnEvent;

                    log(String.Format("MaxPigs: {0}", session.MaxPigs));
                    log(String.Format("MissionServer: {0}", session.MissionServer));
                    log(String.Format("AccountServer: {0}", session.AccountServer));
                    //log(String.Format("LobbyMode: {0}", session.LobbyMode));
                    log(String.Format("ZoneAuthServer: {0}", session.ZoneAuthServer));
                   // log(String.Format("ZoneAuthTimeout: {0}", session.ZoneAuthTimeout));
                    log(String.Format("Art Path: {0}", session.ArtPath));
                    log(String.Format("Script Path {0}", session.ScriptDir));

                    // load all the scripts from scriptDir
                    var di = new DirectoryInfo(session.ScriptDir);
                    var newScripts = new List<string>();
                    foreach (var file in di.EnumerateFiles())
                    {
                        var justTheName = file.Name.Replace(".pig", "");
                        newScripts.Add(justTheName);
                    }
                    pigScripts = newScripts;
                    if(pigScripts.Count > 0)
                    {
                        pigScript = newScripts.First();
                    }

                    foreach (PigsLib.IPig p in session.Pigs)
                    {
                        pigInfos.Add(new PigInfo { Pig = p });
                    }
                    //session.ActivateAllEvents();
                    // log(String.Format("Attached to events.", session.ProcessID));
                }
            }
        }

        private void CreatePig_Click(object sender, RoutedEventArgs e)
        {
            pigButtonEnable = false;
           
            
            if (session != null)
            {
                try
                {
                    log(String.Format("Requesting new Pig.."));
                    var pig = session.CreatePig(pigScript);
                    log(String.Format("Request returned: {0}", pig));
                    if (pig != null)
                    {
                        pigInfos.Add(new PigInfo { Pig = pig });
                        log(String.Format("pig {0} : State {1}", pig.Name, pig.PigStateName));
                        RefreshPigs_Click(sender, e);
                    }
                }
                catch (Exception ex)
                {
                    log(String.Format("Error: {0}", ex));
                }
            }
            pigButtonEnable = true;
        }

        private void Session_OnEvent(AGCLib.IAGCEvent pEvent)
        {
            log(String.Format("Event {0}", pEvent));
        }

        private void RefreshPigs_Click(object sender, RoutedEventArgs e)
        {
            foreach (var pig in pigInfos)
            {
                try
                {
                    pig.Name = pig.Pig.Name;
                    pig.State = pig.Pig?.PigStateName ?? "";
                    pig.GameName = pig.Pig?.Game.Name ?? "";
                    pig.Money = pig.Pig.Money;
                    pig.Hull = pig.Pig?.Ship?.HullType?.Name ?? "";
                    pig.Position = pig.Pig?.Ship?.Position?.DisplayString ?? "";
                }
                catch (Exception)
                {
                }
            }
        }
    }
}
