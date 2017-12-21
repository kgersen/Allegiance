using System;
using System.Collections.Generic;
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
                        log(String.Format("pig {0} : State {1}", pig.Name, pig.PigStateName));

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

        private void Button_Click(object sender, RoutedEventArgs e)
        {

        }
    }
}
