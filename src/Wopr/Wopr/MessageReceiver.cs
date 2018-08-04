
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
 
using System.IO;
using AllegianceInterop;
using Wopr.Constants;

namespace Wopr
{
    [Serializable]
    public class MessageReceiver : MarshalByRefObject
    {
        private string _playerName;

        public MessageReceiver(string playerName)
        {
            _playerName = playerName;
        }
        

        public void OnAppMessage(AllegianceInterop.ClientConnection client, byte[] messageData)
        {
            MemoryStream ms = new MemoryStream(messageData);
            BinaryReader br = new BinaryReader(ms);

            uint cbmsg = br.ReadUInt16();
            uint fmid = br.ReadUInt16();

            if (fmid != 35 
                && fmid != (int) MessageType.FM_S_HEAVY_SHIPS_UPDATE) // skip ping messages.
                Console.WriteLine($"{_playerName}: Received mesage id: {fmid} = {((MessageType)fmid).ToString()}");

            switch ((MessageType)fmid)
            {

                #region Allegiane Message Handlers

                // Generated from MessageType.cs with Expresso
                /*
                 Regex: 
                    (?<prefix>FM_)(?<name>\w+)\s*=\s*\d+,

                 Replacement: 
                    case MessageType.FM_${name}:
                    AllegianceInterop.FMD_${name} ${name}msg = new AllegianceInterop.FMD_${name}(messageData);
                    FMD_${name}?.Invoke(client, ${name}msg);
                    break;
                */

                case MessageType.FM_C_LOGONREQ:
                    AllegianceInterop.FMD_C_LOGONREQ C_LOGONREQmsg = new AllegianceInterop.FMD_C_LOGONREQ(messageData);
                    FMD_C_LOGONREQ?.Invoke(client, C_LOGONREQmsg);
                    break;
                case MessageType.FM_S_LOGONACK:
                    AllegianceInterop.FMD_S_LOGONACK S_LOGONACKmsg = new AllegianceInterop.FMD_S_LOGONACK(messageData);
                    FMD_S_LOGONACK?.Invoke(client, S_LOGONACKmsg);
                    break;
                case MessageType.FM_CS_LOGOFF:
                    AllegianceInterop.FMD_CS_LOGOFF CS_LOGOFFmsg = new AllegianceInterop.FMD_CS_LOGOFF(messageData);
                    FMD_CS_LOGOFF?.Invoke(client, CS_LOGOFFmsg);
                    break;
                case MessageType.FM_C_BUY_LOADOUT:
                    AllegianceInterop.FMD_C_BUY_LOADOUT C_BUY_LOADOUTmsg = new AllegianceInterop.FMD_C_BUY_LOADOUT(messageData);
                    FMD_C_BUY_LOADOUT?.Invoke(client, C_BUY_LOADOUTmsg);
                    break;
                case MessageType.FM_S_BUY_LOADOUT_ACK:
                    AllegianceInterop.FMD_S_BUY_LOADOUT_ACK S_BUY_LOADOUT_ACKmsg = new AllegianceInterop.FMD_S_BUY_LOADOUT_ACK(messageData);
                    FMD_S_BUY_LOADOUT_ACK?.Invoke(client, S_BUY_LOADOUT_ACKmsg);
                    break;
                case MessageType.FM_S_OBJECT_SPOTTED:
                    AllegianceInterop.FMD_S_OBJECT_SPOTTED S_OBJECT_SPOTTEDmsg = new AllegianceInterop.FMD_S_OBJECT_SPOTTED(messageData);
                    FMD_S_OBJECT_SPOTTED?.Invoke(client, S_OBJECT_SPOTTEDmsg);
                    break;
                case MessageType.FM_S_SET_CLUSTER:
                    AllegianceInterop.FMD_S_SET_CLUSTER S_SET_CLUSTERmsg = new AllegianceInterop.FMD_S_SET_CLUSTER(messageData);
                    FMD_S_SET_CLUSTER?.Invoke(client, S_SET_CLUSTERmsg);
                    break;
                case MessageType.FM_S_DEV_COMPLETED:
                    AllegianceInterop.FMD_S_DEV_COMPLETED S_DEV_COMPLETEDmsg = new AllegianceInterop.FMD_S_DEV_COMPLETED(messageData);
                    FMD_S_DEV_COMPLETED?.Invoke(client, S_DEV_COMPLETEDmsg);
                    break;
                case MessageType.FM_S_SET_BRIEFING_TEXT:
                    AllegianceInterop.FMD_S_SET_BRIEFING_TEXT S_SET_BRIEFING_TEXTmsg = new AllegianceInterop.FMD_S_SET_BRIEFING_TEXT(messageData);
                    FMD_S_SET_BRIEFING_TEXT?.Invoke(client, S_SET_BRIEFING_TEXTmsg);
                    break;
                case MessageType.FM_S_PLAYERINFO:
                    AllegianceInterop.FMD_S_PLAYERINFO S_PLAYERINFOmsg = new AllegianceInterop.FMD_S_PLAYERINFO(messageData);
                    FMD_S_PLAYERINFO?.Invoke(client, S_PLAYERINFOmsg);
                    break;
                case MessageType.FM_S_STATIC_MAP_INFO:
                    AllegianceInterop.FMD_S_STATIC_MAP_INFO S_STATIC_MAP_INFOmsg = new AllegianceInterop.FMD_S_STATIC_MAP_INFO(messageData);
                    FMD_S_STATIC_MAP_INFO?.Invoke(client, S_STATIC_MAP_INFOmsg);
                    break;
                case MessageType.FM_CS_PROJECTILE_INSTANCE:
                    AllegianceInterop.FMD_CS_PROJECTILE_INSTANCE CS_PROJECTILE_INSTANCEmsg = new AllegianceInterop.FMD_CS_PROJECTILE_INSTANCE(messageData);
                    FMD_CS_PROJECTILE_INSTANCE?.Invoke(client, CS_PROJECTILE_INSTANCEmsg);
                    break;
                case MessageType.FM_S_RANK_INFO:
                    AllegianceInterop.FMD_S_RANK_INFO S_RANK_INFOmsg = new AllegianceInterop.FMD_S_RANK_INFO(messageData);
                    FMD_S_RANK_INFO?.Invoke(client, S_RANK_INFOmsg);
                    break;
                case MessageType.FM_S_SET_START_TIME:
                    AllegianceInterop.FMD_S_SET_START_TIME S_SET_START_TIMEmsg = new AllegianceInterop.FMD_S_SET_START_TIME(messageData);
                    FMD_S_SET_START_TIME?.Invoke(client, S_SET_START_TIMEmsg);
                    break;
                case MessageType.FM_CS_SET_TEAM_INFO:
                    AllegianceInterop.FMD_CS_SET_TEAM_INFO CS_SET_TEAM_INFOmsg = new AllegianceInterop.FMD_CS_SET_TEAM_INFO(messageData);
                    FMD_CS_SET_TEAM_INFO?.Invoke(client, CS_SET_TEAM_INFOmsg);
                    break;
                case MessageType.FM_S_PLAYER_RESCUED:
                    AllegianceInterop.FMD_S_PLAYER_RESCUED S_PLAYER_RESCUEDmsg = new AllegianceInterop.FMD_S_PLAYER_RESCUED(messageData);
                    FMD_S_PLAYER_RESCUED?.Invoke(client, S_PLAYER_RESCUEDmsg);
                    break;
                case MessageType.FM_S_BALLOT:
                    AllegianceInterop.FMD_S_BALLOT S_BALLOTmsg = new AllegianceInterop.FMD_S_BALLOT(messageData);
                    FMD_S_BALLOT?.Invoke(client, S_BALLOTmsg);
                    break;
                case MessageType.FM_S_CANCEL_BALLOT:
                    AllegianceInterop.FMD_S_CANCEL_BALLOT S_CANCEL_BALLOTmsg = new AllegianceInterop.FMD_S_CANCEL_BALLOT(messageData);
                    FMD_S_CANCEL_BALLOT?.Invoke(client, S_CANCEL_BALLOTmsg);
                    break;
                case MessageType.FM_C_VOTE:
                    AllegianceInterop.FMD_C_VOTE C_VOTEmsg = new AllegianceInterop.FMD_C_VOTE(messageData);
                    FMD_C_VOTE?.Invoke(client, C_VOTEmsg);
                    break;
                case MessageType.FM_S_SHIP_DELETE:
                    AllegianceInterop.FMD_S_SHIP_DELETE S_SHIP_DELETEmsg = new AllegianceInterop.FMD_S_SHIP_DELETE(messageData);
                    FMD_S_SHIP_DELETE?.Invoke(client, S_SHIP_DELETEmsg);
                    break;
                case MessageType.FM_C_RANDOMIZE_TEAMS:
                    AllegianceInterop.FMD_C_RANDOMIZE_TEAMS C_RANDOMIZE_TEAMSmsg = new AllegianceInterop.FMD_C_RANDOMIZE_TEAMS(messageData);
                    FMD_C_RANDOMIZE_TEAMS?.Invoke(client, C_RANDOMIZE_TEAMSmsg);
                    break;
                case MessageType.FM_CS_CHATMESSAGE:
                    AllegianceInterop.FMD_CS_CHATMESSAGE CS_CHATMESSAGEmsg = new AllegianceInterop.FMD_CS_CHATMESSAGE(messageData);
                    FMD_CS_CHATMESSAGE?.Invoke(client, CS_CHATMESSAGEmsg);
                    break;
                case MessageType.FM_C_SHIP_UPDATE:
                    AllegianceInterop.FMD_C_SHIP_UPDATE C_SHIP_UPDATEmsg = new AllegianceInterop.FMD_C_SHIP_UPDATE(messageData);
                    FMD_C_SHIP_UPDATE?.Invoke(client, C_SHIP_UPDATEmsg);
                    break;
                case MessageType.FM_S_EXPORT:
                    AllegianceInterop.FMD_S_EXPORT S_EXPORTmsg = new AllegianceInterop.FMD_S_EXPORT(messageData);
                    FMD_S_EXPORT?.Invoke(client, S_EXPORTmsg);
                    break;
                case MessageType.FM_S_POSTER:
                    AllegianceInterop.FMD_S_POSTER S_POSTERmsg = new AllegianceInterop.FMD_S_POSTER(messageData);
                    FMD_S_POSTER?.Invoke(client, S_POSTERmsg);
                    break;
                case MessageType.FM_S_TREASURE:
                    AllegianceInterop.FMD_S_TREASURE S_TREASUREmsg = new AllegianceInterop.FMD_S_TREASURE(messageData);
                    FMD_S_TREASURE?.Invoke(client, S_TREASUREmsg);
                    break;
                case MessageType.FM_S_DESTROY_TREASURE:
                    AllegianceInterop.FMD_S_DESTROY_TREASURE S_DESTROY_TREASUREmsg = new AllegianceInterop.FMD_S_DESTROY_TREASURE(messageData);
                    FMD_S_DESTROY_TREASURE?.Invoke(client, S_DESTROY_TREASUREmsg);
                    break;
                case MessageType.FM_CS_NEW_SHIP_INSTANCE:
                    AllegianceInterop.FMD_CS_NEW_SHIP_INSTANCE CS_NEW_SHIP_INSTANCEmsg = new AllegianceInterop.FMD_CS_NEW_SHIP_INSTANCE(messageData);
                    FMD_CS_NEW_SHIP_INSTANCE?.Invoke(client, CS_NEW_SHIP_INSTANCEmsg);
                    break;
                case MessageType.FM_CS_DROP_PART:
                    AllegianceInterop.FMD_CS_DROP_PART CS_DROP_PARTmsg = new AllegianceInterop.FMD_CS_DROP_PART(messageData);
                    FMD_CS_DROP_PART?.Invoke(client, CS_DROP_PARTmsg);
                    break;
                case MessageType.FM_CS_SWAP_PART:
                    AllegianceInterop.FMD_CS_SWAP_PART CS_SWAP_PARTmsg = new AllegianceInterop.FMD_CS_SWAP_PART(messageData);
                    FMD_CS_SWAP_PART?.Invoke(client, CS_SWAP_PARTmsg);
                    break;
                case MessageType.FM_CS_PING:
                    AllegianceInterop.FMD_CS_PING CS_PINGmsg = new AllegianceInterop.FMD_CS_PING(messageData);
                    FMD_CS_PING?.Invoke(client, CS_PINGmsg);
                    break;
                case MessageType.FM_S_URLROOT:
                    AllegianceInterop.FMD_S_URLROOT S_URLROOTmsg = new AllegianceInterop.FMD_S_URLROOT(messageData);
                    FMD_S_URLROOT?.Invoke(client, S_URLROOTmsg);
                    break;
                case MessageType.FM_S_MISSION_STAGE:
                    AllegianceInterop.FMD_S_MISSION_STAGE S_MISSION_STAGEmsg = new AllegianceInterop.FMD_S_MISSION_STAGE(messageData);
                    FMD_S_MISSION_STAGE?.Invoke(client, S_MISSION_STAGEmsg);
                    break;
                case MessageType.FM_S_MISSIONDEF:
                    AllegianceInterop.FMD_S_MISSIONDEF S_MISSIONDEFmsg = new AllegianceInterop.FMD_S_MISSIONDEF(messageData);
                    FMD_S_MISSIONDEF?.Invoke(client, S_MISSIONDEFmsg);
                    break;
                case MessageType.FM_C_POSITIONREQ:
                    AllegianceInterop.FMD_C_POSITIONREQ C_POSITIONREQmsg = new AllegianceInterop.FMD_C_POSITIONREQ(messageData);
                    FMD_C_POSITIONREQ?.Invoke(client, C_POSITIONREQmsg);
                    break;
                case MessageType.FM_S_POSITIONREQ:
                    AllegianceInterop.FMD_S_POSITIONREQ S_POSITIONREQmsg = new AllegianceInterop.FMD_S_POSITIONREQ(messageData);
                    FMD_S_POSITIONREQ?.Invoke(client, S_POSITIONREQmsg);
                    break;
                case MessageType.FM_C_POSITIONACK:
                    AllegianceInterop.FMD_C_POSITIONACK C_POSITIONACKmsg = new AllegianceInterop.FMD_C_POSITIONACK(messageData);
                    FMD_C_POSITIONACK?.Invoke(client, C_POSITIONACKmsg);
                    break;
                case MessageType.FM_CS_DELPOSITIONREQ:
                    AllegianceInterop.FMD_CS_DELPOSITIONREQ CS_DELPOSITIONREQmsg = new AllegianceInterop.FMD_CS_DELPOSITIONREQ(messageData);
                    FMD_CS_DELPOSITIONREQ?.Invoke(client, CS_DELPOSITIONREQmsg);
                    break;
                case MessageType.FM_CS_LOCK_LOBBY:
                    AllegianceInterop.FMD_CS_LOCK_LOBBY CS_LOCK_LOBBYmsg = new AllegianceInterop.FMD_CS_LOCK_LOBBY(messageData);
                    FMD_CS_LOCK_LOBBY?.Invoke(client, CS_LOCK_LOBBYmsg);
                    break;
                case MessageType.FM_CS_LOCK_SIDES:
                    AllegianceInterop.FMD_CS_LOCK_SIDES CS_LOCK_SIDESmsg = new AllegianceInterop.FMD_CS_LOCK_SIDES(messageData);
                    FMD_CS_LOCK_SIDES?.Invoke(client, CS_LOCK_SIDESmsg);
                    break;
                case MessageType.FM_CS_PLAYER_READY:
                    AllegianceInterop.FMD_CS_PLAYER_READY CS_PLAYER_READYmsg = new AllegianceInterop.FMD_CS_PLAYER_READY(messageData);
                    FMD_CS_PLAYER_READY?.Invoke(client, CS_PLAYER_READYmsg);
                    break;
                case MessageType.FM_S_TEAM_READY:
                    AllegianceInterop.FMD_S_TEAM_READY S_TEAM_READYmsg = new AllegianceInterop.FMD_S_TEAM_READY(messageData);
                    FMD_S_TEAM_READY?.Invoke(client, S_TEAM_READYmsg);
                    break;
                case MessageType.FM_CS_FORCE_TEAM_READY:
                    AllegianceInterop.FMD_CS_FORCE_TEAM_READY CS_FORCE_TEAM_READYmsg = new AllegianceInterop.FMD_CS_FORCE_TEAM_READY(messageData);
                    FMD_CS_FORCE_TEAM_READY?.Invoke(client, CS_FORCE_TEAM_READYmsg);
                    break;
                case MessageType.FM_C_DOCKED:
                    AllegianceInterop.FMD_C_DOCKED C_DOCKEDmsg = new AllegianceInterop.FMD_C_DOCKED(messageData);
                    FMD_C_DOCKED?.Invoke(client, C_DOCKEDmsg);
                    break;
                case MessageType.FM_CS_CHANGE_TEAM_CIV:
                    AllegianceInterop.FMD_CS_CHANGE_TEAM_CIV CS_CHANGE_TEAM_CIVmsg = new AllegianceInterop.FMD_CS_CHANGE_TEAM_CIV(messageData);
                    FMD_CS_CHANGE_TEAM_CIV?.Invoke(client, CS_CHANGE_TEAM_CIVmsg);
                    break;
                case MessageType.FM_CS_AUTO_ACCEPT:
                    AllegianceInterop.FMD_CS_AUTO_ACCEPT CS_AUTO_ACCEPTmsg = new AllegianceInterop.FMD_CS_AUTO_ACCEPT(messageData);
                    FMD_CS_AUTO_ACCEPT?.Invoke(client, CS_AUTO_ACCEPTmsg);
                    break;
                case MessageType.FM_S_STATIONS_UPDATE:
                    AllegianceInterop.FMD_S_STATIONS_UPDATE S_STATIONS_UPDATEmsg = new AllegianceInterop.FMD_S_STATIONS_UPDATE(messageData);
                    FMD_S_STATIONS_UPDATE?.Invoke(client, S_STATIONS_UPDATEmsg);
                    break;
                case MessageType.FM_S_STATION_CAPTURE:
                    AllegianceInterop.FMD_S_STATION_CAPTURE S_STATION_CAPTUREmsg = new AllegianceInterop.FMD_S_STATION_CAPTURE(messageData);
                    FMD_S_STATION_CAPTURE?.Invoke(client, S_STATION_CAPTUREmsg);
                    break;
                case MessageType.FM_S_STATION_DESTROYED:
                    AllegianceInterop.FMD_S_STATION_DESTROYED S_STATION_DESTROYEDmsg = new AllegianceInterop.FMD_S_STATION_DESTROYED(messageData);
                    FMD_S_STATION_DESTROYED?.Invoke(client, S_STATION_DESTROYEDmsg);
                    break;
                case MessageType.FM_S_MONEY_CHANGE:
                    AllegianceInterop.FMD_S_MONEY_CHANGE S_MONEY_CHANGEmsg = new AllegianceInterop.FMD_S_MONEY_CHANGE(messageData);
                    FMD_S_MONEY_CHANGE?.Invoke(client, S_MONEY_CHANGEmsg);
                    break;
                case MessageType.FM_S_DOCKED:
                    AllegianceInterop.FMD_S_DOCKED S_DOCKEDmsg = new AllegianceInterop.FMD_S_DOCKED(messageData);
                    FMD_S_DOCKED?.Invoke(client, S_DOCKEDmsg);
                    break;
                case MessageType.FM_S_GAME_OVER:
                    AllegianceInterop.FMD_S_GAME_OVER S_GAME_OVERmsg = new AllegianceInterop.FMD_S_GAME_OVER(messageData);
                    FMD_S_GAME_OVER?.Invoke(client, S_GAME_OVERmsg);
                    break;
                case MessageType.FM_S_GAME_OVER_PLAYERS:
                    AllegianceInterop.FMD_S_GAME_OVER_PLAYERS S_GAME_OVER_PLAYERSmsg = new AllegianceInterop.FMD_S_GAME_OVER_PLAYERS(messageData);
                    FMD_S_GAME_OVER_PLAYERS?.Invoke(client, S_GAME_OVER_PLAYERSmsg);
                    break;
                case MessageType.FM_C_BUCKET_DONATE:
                    AllegianceInterop.FMD_C_BUCKET_DONATE C_BUCKET_DONATEmsg = new AllegianceInterop.FMD_C_BUCKET_DONATE(messageData);
                    FMD_C_BUCKET_DONATE?.Invoke(client, C_BUCKET_DONATEmsg);
                    break;
                case MessageType.FM_S_BUCKET_STATUS:
                    AllegianceInterop.FMD_S_BUCKET_STATUS S_BUCKET_STATUSmsg = new AllegianceInterop.FMD_S_BUCKET_STATUS(messageData);
                    FMD_S_BUCKET_STATUS?.Invoke(client, S_BUCKET_STATUSmsg);
                    break;
                case MessageType.FM_C_PLAYER_DONATE:
                    AllegianceInterop.FMD_C_PLAYER_DONATE C_PLAYER_DONATEmsg = new AllegianceInterop.FMD_C_PLAYER_DONATE(messageData);
                    FMD_C_PLAYER_DONATE?.Invoke(client, C_PLAYER_DONATEmsg);
                    break;
                case MessageType.FM_CS_SIDE_INACTIVE:
                    AllegianceInterop.FMD_CS_SIDE_INACTIVE CS_SIDE_INACTIVEmsg = new AllegianceInterop.FMD_CS_SIDE_INACTIVE(messageData);
                    FMD_CS_SIDE_INACTIVE?.Invoke(client, CS_SIDE_INACTIVEmsg);
                    break;
                case MessageType.FM_CS_FIRE_MISSILE:
                    AllegianceInterop.FMD_CS_FIRE_MISSILE CS_FIRE_MISSILEmsg = new AllegianceInterop.FMD_CS_FIRE_MISSILE(messageData);
                    FMD_CS_FIRE_MISSILE?.Invoke(client, CS_FIRE_MISSILEmsg);
                    break;
                case MessageType.FM_S_SIDE_TECH_CHANGE:
                    AllegianceInterop.FMD_S_SIDE_TECH_CHANGE S_SIDE_TECH_CHANGEmsg = new AllegianceInterop.FMD_S_SIDE_TECH_CHANGE(messageData);
                    FMD_S_SIDE_TECH_CHANGE?.Invoke(client, S_SIDE_TECH_CHANGEmsg);
                    break;
                case MessageType.FM_S_SIDE_ATTRIBUTE_CHANGE:
                    AllegianceInterop.FMD_S_SIDE_ATTRIBUTE_CHANGE S_SIDE_ATTRIBUTE_CHANGEmsg = new AllegianceInterop.FMD_S_SIDE_ATTRIBUTE_CHANGE(messageData);
                    FMD_S_SIDE_ATTRIBUTE_CHANGE?.Invoke(client, S_SIDE_ATTRIBUTE_CHANGEmsg);
                    break;
                case MessageType.FM_S_EJECT:
                    AllegianceInterop.FMD_S_EJECT S_EJECTmsg = new AllegianceInterop.FMD_S_EJECT(messageData);
                    FMD_S_EJECT?.Invoke(client, S_EJECTmsg);
                    break;
                case MessageType.FM_S_SINGLE_SHIP_UPDATE:
                    AllegianceInterop.FMD_S_SINGLE_SHIP_UPDATE S_SINGLE_SHIP_UPDATEmsg = new AllegianceInterop.FMD_S_SINGLE_SHIP_UPDATE(messageData);
                    FMD_S_SINGLE_SHIP_UPDATE?.Invoke(client, S_SINGLE_SHIP_UPDATEmsg);
                    break;
                case MessageType.FM_C_SUICIDE:
                    AllegianceInterop.FMD_C_SUICIDE C_SUICIDEmsg = new AllegianceInterop.FMD_C_SUICIDE(messageData);
                    FMD_C_SUICIDE?.Invoke(client, C_SUICIDEmsg);
                    break;
                case MessageType.FM_C_FIRE_EXPENDABLE:
                    AllegianceInterop.FMD_C_FIRE_EXPENDABLE C_FIRE_EXPENDABLEmsg = new AllegianceInterop.FMD_C_FIRE_EXPENDABLE(messageData);
                    FMD_C_FIRE_EXPENDABLE?.Invoke(client, C_FIRE_EXPENDABLEmsg);
                    break;
                case MessageType.FM_S_MISSILE_DESTROYED:
                    AllegianceInterop.FMD_S_MISSILE_DESTROYED S_MISSILE_DESTROYEDmsg = new AllegianceInterop.FMD_S_MISSILE_DESTROYED(messageData);
                    FMD_S_MISSILE_DESTROYED?.Invoke(client, S_MISSILE_DESTROYEDmsg);
                    break;
                case MessageType.FM_S_MINE_DESTROYED:
                    AllegianceInterop.FMD_S_MINE_DESTROYED S_MINE_DESTROYEDmsg = new AllegianceInterop.FMD_S_MINE_DESTROYED(messageData);
                    FMD_S_MINE_DESTROYED?.Invoke(client, S_MINE_DESTROYEDmsg);
                    break;
                case MessageType.FM_S_PROBE_DESTROYED:
                    AllegianceInterop.FMD_S_PROBE_DESTROYED S_PROBE_DESTROYEDmsg = new AllegianceInterop.FMD_S_PROBE_DESTROYED(messageData);
                    FMD_S_PROBE_DESTROYED?.Invoke(client, S_PROBE_DESTROYEDmsg);
                    break;
                case MessageType.FM_S_ASTEROID_DESTROYED:
                    AllegianceInterop.FMD_S_ASTEROID_DESTROYED S_ASTEROID_DESTROYEDmsg = new AllegianceInterop.FMD_S_ASTEROID_DESTROYED(messageData);
                    FMD_S_ASTEROID_DESTROYED?.Invoke(client, S_ASTEROID_DESTROYEDmsg);
                    break;
                case MessageType.FM_S_FIRE_EXPENDABLE:
                    AllegianceInterop.FMD_S_FIRE_EXPENDABLE S_FIRE_EXPENDABLEmsg = new AllegianceInterop.FMD_S_FIRE_EXPENDABLE(messageData);
                    FMD_S_FIRE_EXPENDABLE?.Invoke(client, S_FIRE_EXPENDABLEmsg);
                    break;
                case MessageType.FM_S_TREASURE_SETS:
                    AllegianceInterop.FMD_S_TREASURE_SETS S_TREASURE_SETSmsg = new AllegianceInterop.FMD_S_TREASURE_SETS(messageData);
                    FMD_S_TREASURE_SETS?.Invoke(client, S_TREASURE_SETSmsg);
                    break;
                case MessageType.FM_S_SHIP_STATUS:
                    AllegianceInterop.FMD_S_SHIP_STATUS S_SHIP_STATUSmsg = new AllegianceInterop.FMD_S_SHIP_STATUS(messageData);
                    FMD_S_SHIP_STATUS?.Invoke(client, S_SHIP_STATUSmsg);
                    break;
                case MessageType.FM_S_PROBES_UPDATE:
                    AllegianceInterop.FMD_S_PROBES_UPDATE S_PROBES_UPDATEmsg = new AllegianceInterop.FMD_S_PROBES_UPDATE(messageData);
                    FMD_S_PROBES_UPDATE?.Invoke(client, S_PROBES_UPDATEmsg);
                    break;
                case MessageType.FM_CS_ORDER_CHANGE:
                    AllegianceInterop.FMD_CS_ORDER_CHANGE CS_ORDER_CHANGEmsg = new AllegianceInterop.FMD_CS_ORDER_CHANGE(messageData);
                    FMD_CS_ORDER_CHANGE?.Invoke(client, CS_ORDER_CHANGEmsg);
                    break;
                case MessageType.FM_S_LEAVE_SHIP:
                    AllegianceInterop.FMD_S_LEAVE_SHIP S_LEAVE_SHIPmsg = new AllegianceInterop.FMD_S_LEAVE_SHIP(messageData);
                    FMD_S_LEAVE_SHIP?.Invoke(client, S_LEAVE_SHIPmsg);
                    break;
                case MessageType.FM_S_JOINED_MISSION:
                    AllegianceInterop.FMD_S_JOINED_MISSION S_JOINED_MISSIONmsg = new AllegianceInterop.FMD_S_JOINED_MISSION(messageData);
                    FMD_S_JOINED_MISSION?.Invoke(client, S_JOINED_MISSIONmsg);
                    break;
                case MessageType.FM_S_LOADOUT_CHANGE:
                    AllegianceInterop.FMD_S_LOADOUT_CHANGE S_LOADOUT_CHANGEmsg = new AllegianceInterop.FMD_S_LOADOUT_CHANGE(messageData);
                    FMD_S_LOADOUT_CHANGE?.Invoke(client, S_LOADOUT_CHANGEmsg);
                    break;
                case MessageType.FM_C_ACTIVE_TURRET_UPDATE:
                    AllegianceInterop.FMD_C_ACTIVE_TURRET_UPDATE C_ACTIVE_TURRET_UPDATEmsg = new AllegianceInterop.FMD_C_ACTIVE_TURRET_UPDATE(messageData);
                    FMD_C_ACTIVE_TURRET_UPDATE?.Invoke(client, C_ACTIVE_TURRET_UPDATEmsg);
                    break;
                case MessageType.FM_C_INACTIVE_TURRET_UPDATE:
                    AllegianceInterop.FMD_C_INACTIVE_TURRET_UPDATE C_INACTIVE_TURRET_UPDATEmsg = new AllegianceInterop.FMD_C_INACTIVE_TURRET_UPDATE(messageData);
                    FMD_C_INACTIVE_TURRET_UPDATE?.Invoke(client, C_INACTIVE_TURRET_UPDATEmsg);
                    break;
                case MessageType.FM_S_TELEPORT_ACK:
                    AllegianceInterop.FMD_S_TELEPORT_ACK S_TELEPORT_ACKmsg = new AllegianceInterop.FMD_S_TELEPORT_ACK(messageData);
                    FMD_S_TELEPORT_ACK?.Invoke(client, S_TELEPORT_ACKmsg);
                    break;
                case MessageType.FM_C_BOARD_SHIP:
                    AllegianceInterop.FMD_C_BOARD_SHIP C_BOARD_SHIPmsg = new AllegianceInterop.FMD_C_BOARD_SHIP(messageData);
                    FMD_C_BOARD_SHIP?.Invoke(client, C_BOARD_SHIPmsg);
                    break;
                case MessageType.FM_C_VIEW_CLUSTER:
                    AllegianceInterop.FMD_C_VIEW_CLUSTER C_VIEW_CLUSTERmsg = new AllegianceInterop.FMD_C_VIEW_CLUSTER(messageData);
                    FMD_C_VIEW_CLUSTER?.Invoke(client, C_VIEW_CLUSTERmsg);
                    break;
                case MessageType.FM_S_KILL_SHIP:
                    AllegianceInterop.FMD_S_KILL_SHIP S_KILL_SHIPmsg = new AllegianceInterop.FMD_S_KILL_SHIP(messageData);
                    FMD_S_KILL_SHIP?.Invoke(client, S_KILL_SHIPmsg);
                    break;
                case MessageType.FM_CS_SET_WINGID:
                    AllegianceInterop.FMD_CS_SET_WINGID CS_SET_WINGIDmsg = new AllegianceInterop.FMD_CS_SET_WINGID(messageData);
                    FMD_CS_SET_WINGID?.Invoke(client, CS_SET_WINGIDmsg);
                    break;
                case MessageType.FM_S_ICQ_CHAT_ACK:
                    AllegianceInterop.FMD_S_ICQ_CHAT_ACK S_ICQ_CHAT_ACKmsg = new AllegianceInterop.FMD_S_ICQ_CHAT_ACK(messageData);
                    FMD_S_ICQ_CHAT_ACK?.Invoke(client, S_ICQ_CHAT_ACKmsg);
                    break;
                case MessageType.FM_CS_CHATBUOY:
                    AllegianceInterop.FMD_CS_CHATBUOY CS_CHATBUOYmsg = new AllegianceInterop.FMD_CS_CHATBUOY(messageData);
                    FMD_CS_CHATBUOY?.Invoke(client, CS_CHATBUOYmsg);
                    break;
                case MessageType.FM_S_CREATE_CHAFF:
                    AllegianceInterop.FMD_S_CREATE_CHAFF S_CREATE_CHAFFmsg = new AllegianceInterop.FMD_S_CREATE_CHAFF(messageData);
                    FMD_S_CREATE_CHAFF?.Invoke(client, S_CREATE_CHAFFmsg);
                    break;
                case MessageType.FM_S_MISSILE_SPOOFED:
                    AllegianceInterop.FMD_S_MISSILE_SPOOFED S_MISSILE_SPOOFEDmsg = new AllegianceInterop.FMD_S_MISSILE_SPOOFED(messageData);
                    FMD_S_MISSILE_SPOOFED?.Invoke(client, S_MISSILE_SPOOFEDmsg);
                    break;
                case MessageType.FM_S_END_SPOOFING:
                    AllegianceInterop.FMD_S_END_SPOOFING S_END_SPOOFINGmsg = new AllegianceInterop.FMD_S_END_SPOOFING(messageData);
                    FMD_S_END_SPOOFING?.Invoke(client, S_END_SPOOFINGmsg);
                    break;
                case MessageType.FM_C_AUTODONATE:
                    AllegianceInterop.FMD_C_AUTODONATE C_AUTODONATEmsg = new AllegianceInterop.FMD_C_AUTODONATE(messageData);
                    FMD_C_AUTODONATE?.Invoke(client, C_AUTODONATEmsg);
                    break;
                case MessageType.FM_CS_MISSIONPARAMS:
                    AllegianceInterop.FMD_CS_MISSIONPARAMS CS_MISSIONPARAMSmsg = new AllegianceInterop.FMD_CS_MISSIONPARAMS(messageData);
                    FMD_CS_MISSIONPARAMS?.Invoke(client, CS_MISSIONPARAMSmsg);
                    break;
                case MessageType.FM_S_PAYDAY:
                    AllegianceInterop.FMD_S_PAYDAY S_PAYDAYmsg = new AllegianceInterop.FMD_S_PAYDAY(messageData);
                    FMD_S_PAYDAY?.Invoke(client, S_PAYDAYmsg);
                    break;
                case MessageType.FM_S_SET_MONEY:
                    AllegianceInterop.FMD_S_SET_MONEY S_SET_MONEYmsg = new AllegianceInterop.FMD_S_SET_MONEY(messageData);
                    FMD_S_SET_MONEY?.Invoke(client, S_SET_MONEYmsg);
                    break;
                case MessageType.FM_S_AUTODONATE:
                    AllegianceInterop.FMD_S_AUTODONATE S_AUTODONATEmsg = new AllegianceInterop.FMD_S_AUTODONATE(messageData);
                    FMD_S_AUTODONATE?.Invoke(client, S_AUTODONATEmsg);
                    break;
                case MessageType.FM_C_MUTE:
                    AllegianceInterop.FMD_C_MUTE C_MUTEmsg = new AllegianceInterop.FMD_C_MUTE(messageData);
                    FMD_C_MUTE?.Invoke(client, C_MUTEmsg);
                    break;
                case MessageType.FM_CS_SET_TEAM_LEADER:
                    AllegianceInterop.FMD_CS_SET_TEAM_LEADER CS_SET_TEAM_LEADERmsg = new AllegianceInterop.FMD_CS_SET_TEAM_LEADER(messageData);
                    FMD_CS_SET_TEAM_LEADER?.Invoke(client, CS_SET_TEAM_LEADERmsg);
                    break;
                case MessageType.FM_CS_SET_TEAM_INVESTOR:
                    AllegianceInterop.FMD_CS_SET_TEAM_INVESTOR CS_SET_TEAM_INVESTORmsg = new AllegianceInterop.FMD_CS_SET_TEAM_INVESTOR(messageData);
                    FMD_CS_SET_TEAM_INVESTOR?.Invoke(client, CS_SET_TEAM_INVESTORmsg);
                    break;
                case MessageType.FM_CS_SET_MISSION_OWNER:
                    AllegianceInterop.FMD_CS_SET_MISSION_OWNER CS_SET_MISSION_OWNERmsg = new AllegianceInterop.FMD_CS_SET_MISSION_OWNER(messageData);
                    FMD_CS_SET_MISSION_OWNER?.Invoke(client, CS_SET_MISSION_OWNERmsg);
                    break;
                case MessageType.FM_CS_QUIT_MISSION:
                    AllegianceInterop.FMD_CS_QUIT_MISSION CS_QUIT_MISSIONmsg = new AllegianceInterop.FMD_CS_QUIT_MISSION(messageData);
                    FMD_CS_QUIT_MISSION?.Invoke(client, CS_QUIT_MISSIONmsg);
                    break;
                case MessageType.FM_S_JOIN_SIDE:
                    AllegianceInterop.FMD_S_JOIN_SIDE S_JOIN_SIDEmsg = new AllegianceInterop.FMD_S_JOIN_SIDE(messageData);
                    FMD_S_JOIN_SIDE?.Invoke(client, S_JOIN_SIDEmsg);
                    break;
                case MessageType.FM_CS_QUIT_SIDE:
                    AllegianceInterop.FMD_CS_QUIT_SIDE CS_QUIT_SIDEmsg = new AllegianceInterop.FMD_CS_QUIT_SIDE(messageData);
                    FMD_CS_QUIT_SIDE?.Invoke(client, CS_QUIT_SIDEmsg);
                    break;
                case MessageType.FM_S_ENTER_GAME:
                    AllegianceInterop.FMD_S_ENTER_GAME S_ENTER_GAMEmsg = new AllegianceInterop.FMD_S_ENTER_GAME(messageData);
                    FMD_S_ENTER_GAME?.Invoke(client, S_ENTER_GAMEmsg);
                    break;
                case MessageType.FM_CS_RELOAD:
                    AllegianceInterop.FMD_CS_RELOAD CS_RELOADmsg = new AllegianceInterop.FMD_CS_RELOAD(messageData);
                    FMD_CS_RELOAD?.Invoke(client, CS_RELOADmsg);
                    break;
                //case MessageType.FM_S_THREAT:
                //    AllegianceInterop.FMD_S_THREAT S_THREATmsg = new AllegianceInterop.FMD_S_THREAT(messageData);
                //    FMD_S_THREAT?.Invoke(client, S_THREATmsg);
                //    break;
                //case MessageType.FM_S_GAME_STATE:
                //    AllegianceInterop.FMD_S_GAME_STATE S_GAME_STATEmsg = new AllegianceInterop.FMD_S_GAME_STATE(messageData);
                //    FMD_S_GAME_STATE?.Invoke(client, S_GAME_STATEmsg);
                //    break;
                case MessageType.FM_S_GAIN_FLAG:
                    AllegianceInterop.FMD_S_GAIN_FLAG S_GAIN_FLAGmsg = new AllegianceInterop.FMD_S_GAIN_FLAG(messageData);
                    FMD_S_GAIN_FLAG?.Invoke(client, S_GAIN_FLAGmsg);
                    break;
                case MessageType.FM_C_START_GAME:
                    AllegianceInterop.FMD_C_START_GAME C_START_GAMEmsg = new AllegianceInterop.FMD_C_START_GAME(messageData);
                    FMD_C_START_GAME?.Invoke(client, C_START_GAMEmsg);
                    break;
                case MessageType.FM_S_ACQUIRE_TREASURE:
                    AllegianceInterop.FMD_S_ACQUIRE_TREASURE S_ACQUIRE_TREASUREmsg = new AllegianceInterop.FMD_S_ACQUIRE_TREASURE(messageData);
                    FMD_S_ACQUIRE_TREASURE?.Invoke(client, S_ACQUIRE_TREASUREmsg);
                    break;
                case MessageType.FM_C_TREASURE_ACK:
                    AllegianceInterop.FMD_C_TREASURE_ACK C_TREASURE_ACKmsg = new AllegianceInterop.FMD_C_TREASURE_ACK(messageData);
                    FMD_C_TREASURE_ACK?.Invoke(client, C_TREASURE_ACKmsg);
                    break;
                case MessageType.FM_S_ADD_PART:
                    AllegianceInterop.FMD_S_ADD_PART S_ADD_PARTmsg = new AllegianceInterop.FMD_S_ADD_PART(messageData);
                    FMD_S_ADD_PART?.Invoke(client, S_ADD_PARTmsg);
                    break;
                case MessageType.FM_S_ENTER_LIFEPOD:
                    AllegianceInterop.FMD_S_ENTER_LIFEPOD S_ENTER_LIFEPODmsg = new AllegianceInterop.FMD_S_ENTER_LIFEPOD(messageData);
                    FMD_S_ENTER_LIFEPOD?.Invoke(client, S_ENTER_LIFEPODmsg);
                    break;
                case MessageType.FM_S_LIGHT_SHIPS_UPDATE:
                    AllegianceInterop.FMD_S_LIGHT_SHIPS_UPDATE S_LIGHT_SHIPS_UPDATEmsg = new AllegianceInterop.FMD_S_LIGHT_SHIPS_UPDATE(messageData);
                    FMD_S_LIGHT_SHIPS_UPDATE?.Invoke(client, S_LIGHT_SHIPS_UPDATEmsg);
                    break;
                case MessageType.FM_S_HEAVY_SHIPS_UPDATE:
                    AllegianceInterop.FMD_S_HEAVY_SHIPS_UPDATE S_HEAVY_SHIPS_UPDATEmsg = new AllegianceInterop.FMD_S_HEAVY_SHIPS_UPDATE(messageData);
                    FMD_S_HEAVY_SHIPS_UPDATE?.Invoke(client, S_HEAVY_SHIPS_UPDATEmsg);
                    break;
                case MessageType.FM_S_VIEW_CLUSTER:
                    AllegianceInterop.FMD_S_VIEW_CLUSTER S_VIEW_CLUSTERmsg = new AllegianceInterop.FMD_S_VIEW_CLUSTER(messageData);
                    FMD_S_VIEW_CLUSTER?.Invoke(client, S_VIEW_CLUSTERmsg);
                    break;
                case MessageType.FM_S_BOARD_NACK:
                    AllegianceInterop.FMD_S_BOARD_NACK S_BOARD_NACKmsg = new AllegianceInterop.FMD_S_BOARD_NACK(messageData);
                    FMD_S_BOARD_NACK?.Invoke(client, S_BOARD_NACKmsg);
                    break;
                case MessageType.FM_S_ASTEROIDS_UPDATE:
                    AllegianceInterop.FMD_S_ASTEROIDS_UPDATE S_ASTEROIDS_UPDATEmsg = new AllegianceInterop.FMD_S_ASTEROIDS_UPDATE(messageData);
                    FMD_S_ASTEROIDS_UPDATE?.Invoke(client, S_ASTEROIDS_UPDATEmsg);
                    break;
                case MessageType.FM_S_ASTEROID_DRAINED:
                    AllegianceInterop.FMD_S_ASTEROID_DRAINED S_ASTEROID_DRAINEDmsg = new AllegianceInterop.FMD_S_ASTEROID_DRAINED(messageData);
                    FMD_S_ASTEROID_DRAINED?.Invoke(client, S_ASTEROID_DRAINEDmsg);
                    break;
                case MessageType.FM_S_BUILDINGEFFECT_DESTROYED:
                    AllegianceInterop.FMD_S_BUILDINGEFFECT_DESTROYED S_BUILDINGEFFECT_DESTROYEDmsg = new AllegianceInterop.FMD_S_BUILDINGEFFECT_DESTROYED(messageData);
                    FMD_S_BUILDINGEFFECT_DESTROYED?.Invoke(client, S_BUILDINGEFFECT_DESTROYEDmsg);
                    break;
                case MessageType.FM_CS_REQUEST_MONEY:
                    AllegianceInterop.FMD_CS_REQUEST_MONEY CS_REQUEST_MONEYmsg = new AllegianceInterop.FMD_CS_REQUEST_MONEY(messageData);
                    FMD_CS_REQUEST_MONEY?.Invoke(client, CS_REQUEST_MONEYmsg);
                    break;
                case MessageType.FM_S_SHIP_RESET:
                    AllegianceInterop.FMD_S_SHIP_RESET S_SHIP_RESETmsg = new AllegianceInterop.FMD_S_SHIP_RESET(messageData);
                    FMD_S_SHIP_RESET?.Invoke(client, S_SHIP_RESETmsg);
                    break;
                case MessageType.FM_C_RIPCORD_REQUEST:
                    AllegianceInterop.FMD_C_RIPCORD_REQUEST C_RIPCORD_REQUESTmsg = new AllegianceInterop.FMD_C_RIPCORD_REQUEST(messageData);
                    FMD_C_RIPCORD_REQUEST?.Invoke(client, C_RIPCORD_REQUESTmsg);
                    break;
                case MessageType.FM_S_RIPCORD_ACTIVATE:
                    AllegianceInterop.FMD_S_RIPCORD_ACTIVATE S_RIPCORD_ACTIVATEmsg = new AllegianceInterop.FMD_S_RIPCORD_ACTIVATE(messageData);
                    FMD_S_RIPCORD_ACTIVATE?.Invoke(client, S_RIPCORD_ACTIVATEmsg);
                    break;
                case MessageType.FM_S_RIPCORD_DENIED:
                    AllegianceInterop.FMD_S_RIPCORD_DENIED S_RIPCORD_DENIEDmsg = new AllegianceInterop.FMD_S_RIPCORD_DENIED(messageData);
                    FMD_S_RIPCORD_DENIED?.Invoke(client, S_RIPCORD_DENIEDmsg);
                    break;
                case MessageType.FM_S_RIPCORD_ABORTED:
                    AllegianceInterop.FMD_S_RIPCORD_ABORTED S_RIPCORD_ABORTEDmsg = new AllegianceInterop.FMD_S_RIPCORD_ABORTED(messageData);
                    FMD_S_RIPCORD_ABORTED?.Invoke(client, S_RIPCORD_ABORTEDmsg);
                    break;
                case MessageType.FM_S_WARP_BOMB:
                    AllegianceInterop.FMD_S_WARP_BOMB S_WARP_BOMBmsg = new AllegianceInterop.FMD_S_WARP_BOMB(messageData);
                    FMD_S_WARP_BOMB?.Invoke(client, S_WARP_BOMBmsg);
                    break;
                case MessageType.FM_S_PROMOTE:
                    AllegianceInterop.FMD_S_PROMOTE S_PROMOTEmsg = new AllegianceInterop.FMD_S_PROMOTE(messageData);
                    FMD_S_PROMOTE?.Invoke(client, S_PROMOTEmsg);
                    break;
                case MessageType.FM_C_PROMOTE:
                    AllegianceInterop.FMD_C_PROMOTE C_PROMOTEmsg = new AllegianceInterop.FMD_C_PROMOTE(messageData);
                    FMD_C_PROMOTE?.Invoke(client, C_PROMOTEmsg);
                    break;
                case MessageType.FM_S_CREATE_BUCKETS:
                    AllegianceInterop.FMD_S_CREATE_BUCKETS S_CREATE_BUCKETSmsg = new AllegianceInterop.FMD_S_CREATE_BUCKETS(messageData);
                    FMD_S_CREATE_BUCKETS?.Invoke(client, S_CREATE_BUCKETSmsg);
                    break;
                case MessageType.FM_S_RELAUNCH_SHIP:
                    AllegianceInterop.FMD_S_RELAUNCH_SHIP S_RELAUNCH_SHIPmsg = new AllegianceInterop.FMD_S_RELAUNCH_SHIP(messageData);
                    FMD_S_RELAUNCH_SHIP?.Invoke(client, S_RELAUNCH_SHIPmsg);
                    break;
                //case MessageType.FM_C_BANDWIDTH:
                //    AllegianceInterop.FMD_C_BANDWIDTH C_BANDWIDTHmsg = new AllegianceInterop.FMD_C_BANDWIDTH(messageData);
                //    FMD_C_BANDWIDTH?.Invoke(client, C_BANDWIDTHmsg);
                //   break;
                case MessageType.FM_S_PINGDATA:
                    AllegianceInterop.FMD_S_PINGDATA S_PINGDATAmsg = new AllegianceInterop.FMD_S_PINGDATA(messageData);
                    FMD_S_PINGDATA?.Invoke(client, S_PINGDATAmsg);
                    break;
                case MessageType.FM_C_REQPINGDATA:
                    AllegianceInterop.FMD_C_REQPINGDATA C_REQPINGDATAmsg = new AllegianceInterop.FMD_C_REQPINGDATA(messageData);
                    FMD_C_REQPINGDATA?.Invoke(client, C_REQPINGDATAmsg);
                    break;
                case MessageType.FM_C_CHANGE_ALLIANCE:
                    AllegianceInterop.FMD_C_CHANGE_ALLIANCE C_CHANGE_ALLIANCEmsg = new AllegianceInterop.FMD_C_CHANGE_ALLIANCE(messageData);
                    FMD_C_CHANGE_ALLIANCE?.Invoke(client, C_CHANGE_ALLIANCEmsg);
                    break;
                case MessageType.FM_S_CHANGE_ALLIANCES:
                    AllegianceInterop.FMD_S_CHANGE_ALLIANCES S_CHANGE_ALLIANCESmsg = new AllegianceInterop.FMD_S_CHANGE_ALLIANCES(messageData);
                    FMD_S_CHANGE_ALLIANCES?.Invoke(client, S_CHANGE_ALLIANCESmsg);
                    break;
                case MessageType.FM_S_ASTEROID_MINED:
                    AllegianceInterop.FMD_S_ASTEROID_MINED S_ASTEROID_MINEDmsg = new AllegianceInterop.FMD_S_ASTEROID_MINED(messageData);
                    FMD_S_ASTEROID_MINED?.Invoke(client, S_ASTEROID_MINEDmsg);
                    break;
                case MessageType.FM_CS_HIGHLIGHT_CLUSTER:
                    AllegianceInterop.FMD_CS_HIGHLIGHT_CLUSTER CS_HIGHLIGHT_CLUSTERmsg = new AllegianceInterop.FMD_CS_HIGHLIGHT_CLUSTER(messageData);
                    FMD_CS_HIGHLIGHT_CLUSTER?.Invoke(client, CS_HIGHLIGHT_CLUSTERmsg);
                    break;

                // From messageslc.h
                case MessageType.FM_C_LOGON_LOBBY_OLD:
                    AllegianceInterop.FMD_C_LOGON_LOBBY_OLD C_LOGON_LOBBY_OLDmsg = new AllegianceInterop.FMD_C_LOGON_LOBBY_OLD(messageData);
                    FMD_C_LOGON_LOBBY_OLD?.Invoke(client, C_LOGON_LOBBY_OLDmsg);
                    break;
                case MessageType.FM_C_LOGOFF_LOBBY:
                    AllegianceInterop.FMD_C_LOGOFF_LOBBY C_LOGOFF_LOBBYmsg = new AllegianceInterop.FMD_C_LOGOFF_LOBBY(messageData);
                    FMD_C_LOGOFF_LOBBY?.Invoke(client, C_LOGOFF_LOBBYmsg);
                    break;
                case MessageType.FM_L_AUTO_UPDATE_INFO:
                    AllegianceInterop.FMD_L_AUTO_UPDATE_INFO L_AUTO_UPDATE_INFOmsg = new AllegianceInterop.FMD_L_AUTO_UPDATE_INFO(messageData);
                    FMD_L_AUTO_UPDATE_INFO?.Invoke(client, L_AUTO_UPDATE_INFOmsg);
                    break;
                case MessageType.FM_C_CREATE_MISSION_REQ:
                    AllegianceInterop.FMD_C_CREATE_MISSION_REQ C_CREATE_MISSION_REQmsg = new AllegianceInterop.FMD_C_CREATE_MISSION_REQ(messageData);
                    FMD_C_CREATE_MISSION_REQ?.Invoke(client, C_CREATE_MISSION_REQmsg);
                    break;
                case MessageType.FM_L_CREATE_MISSION_ACK:
                    AllegianceInterop.FMD_L_CREATE_MISSION_ACK L_CREATE_MISSION_ACKmsg = new AllegianceInterop.FMD_L_CREATE_MISSION_ACK(messageData);
                    FMD_L_CREATE_MISSION_ACK?.Invoke(client, L_CREATE_MISSION_ACKmsg);
                    break;
                case MessageType.FM_L_CREATE_MISSION_NACK:
                    AllegianceInterop.FMD_L_CREATE_MISSION_NACK L_CREATE_MISSION_NACKmsg = new AllegianceInterop.FMD_L_CREATE_MISSION_NACK(messageData);
                    FMD_L_CREATE_MISSION_NACK?.Invoke(client, L_CREATE_MISSION_NACKmsg);
                    break;
                case MessageType.FM_C_JOIN_GAME_REQ:
                    AllegianceInterop.FMD_C_JOIN_GAME_REQ C_JOIN_GAME_REQmsg = new AllegianceInterop.FMD_C_JOIN_GAME_REQ(messageData);
                    FMD_C_JOIN_GAME_REQ?.Invoke(client, C_JOIN_GAME_REQmsg);
                    break;
                case MessageType.FM_L_JOIN_GAME_NACK:
                    AllegianceInterop.FMD_L_JOIN_GAME_NACK L_JOIN_GAME_NACKmsg = new AllegianceInterop.FMD_L_JOIN_GAME_NACK(messageData);
                    FMD_L_JOIN_GAME_NACK?.Invoke(client, L_JOIN_GAME_NACKmsg);
                    break;
                case MessageType.FM_L_JOIN_MISSION:
                    AllegianceInterop.FMD_L_JOIN_MISSION L_JOIN_MISSIONmsg = new AllegianceInterop.FMD_L_JOIN_MISSION(messageData);
                    FMD_L_JOIN_MISSION?.Invoke(client, L_JOIN_MISSIONmsg);
                    break;
                case MessageType.FM_L_LOGON_ACK:
                    AllegianceInterop.FMD_L_LOGON_ACK L_LOGON_ACKmsg = new AllegianceInterop.FMD_L_LOGON_ACK(messageData);
                    FMD_L_LOGON_ACK?.Invoke(client, L_LOGON_ACKmsg);
                    break;
                case MessageType.FM_L_LOGON_NACK:
                    AllegianceInterop.FMD_L_LOGON_NACK L_LOGON_NACKmsg = new AllegianceInterop.FMD_L_LOGON_NACK(messageData);
                    FMD_L_LOGON_NACK?.Invoke(client, L_LOGON_NACKmsg);
                    break;
                case MessageType.FM_C_FIND_PLAYER:
                    AllegianceInterop.FMD_C_FIND_PLAYER C_FIND_PLAYERmsg = new AllegianceInterop.FMD_C_FIND_PLAYER(messageData);
                    FMD_C_FIND_PLAYER?.Invoke(client, C_FIND_PLAYERmsg);
                    break;
                case MessageType.FM_L_FOUND_PLAYER:
                    AllegianceInterop.FMD_L_FOUND_PLAYER L_FOUND_PLAYERmsg = new AllegianceInterop.FMD_L_FOUND_PLAYER(messageData);
                    FMD_L_FOUND_PLAYER?.Invoke(client, L_FOUND_PLAYERmsg);
                    break;
                case MessageType.FM_C_LOGON_LOBBY:
                    AllegianceInterop.FMD_C_LOGON_LOBBY C_LOGON_LOBBYmsg = new AllegianceInterop.FMD_C_LOGON_LOBBY(messageData);
                    FMD_C_LOGON_LOBBY?.Invoke(client, C_LOGON_LOBBYmsg);
                    break;
                case MessageType.FM_C_GET_SERVERS_REQ:
                    AllegianceInterop.FMD_C_GET_SERVERS_REQ C_GET_SERVERS_REQmsg = new AllegianceInterop.FMD_C_GET_SERVERS_REQ(messageData);
                    FMD_C_GET_SERVERS_REQ?.Invoke(client, C_GET_SERVERS_REQmsg);
                    break;
                case MessageType.FM_L_SERVERS_LIST:
                    AllegianceInterop.FMD_L_SERVERS_LIST L_SERVERS_LISTmsg = new AllegianceInterop.FMD_L_SERVERS_LIST(messageData);
                    FMD_L_SERVERS_LIST?.Invoke(client, L_SERVERS_LISTmsg);
                    break;

                // messagesall.h
                case MessageType.FM_LS_LOBBYMISSIONINFO:
                    AllegianceInterop.FMD_LS_LOBBYMISSIONINFO LS_LOBBYMISSIONINFOmsg = new AllegianceInterop.FMD_LS_LOBBYMISSIONINFO(messageData);
                    FMD_LS_LOBBYMISSIONINFO?.Invoke(client, LS_LOBBYMISSIONINFOmsg);
                    break;
                case MessageType.FM_LS_MISSION_GONE:
                    AllegianceInterop.FMD_LS_MISSION_GONE LS_MISSION_GONEmsg = new AllegianceInterop.FMD_LS_MISSION_GONE(messageData);
                    FMD_LS_MISSION_GONE?.Invoke(client, LS_MISSION_GONEmsg);
                    break;
                case MessageType.FM_LS_SQUAD_MEMBERSHIPS:
                    AllegianceInterop.FMD_LS_SQUAD_MEMBERSHIPS LS_SQUAD_MEMBERSHIPSmsg = new AllegianceInterop.FMD_LS_SQUAD_MEMBERSHIPS(messageData);
                    FMD_LS_SQUAD_MEMBERSHIPS?.Invoke(client, LS_SQUAD_MEMBERSHIPSmsg);
                    break;



                #endregion

                default:
                    Console.WriteLine("*** MessageReceiver::ReceiveMessage: Unknown message id received: " + fmid);
                    break;
            }
        }



        #region Allegiance Message Events

        // Generated from MessageType.cs with Expresso
        /*
         Regex: 
            (?<name>\w+)\s*=\s*\d+,

         Replacement: 
            public delegate void ${name}_Event(AllegianceInterop.ClientConnection client, AllegianceInterop.${name} message);
            public event ${name}_Event ${name};
        */

        public delegate void FMD_C_LOGONREQ_Event(AllegianceInterop.ClientConnection client, AllegianceInterop.FMD_C_LOGONREQ message);
        public event FMD_C_LOGONREQ_Event FMD_C_LOGONREQ;
        public delegate void FMD_S_LOGONACK_Event(AllegianceInterop.ClientConnection client, AllegianceInterop.FMD_S_LOGONACK message);
        public event FMD_S_LOGONACK_Event FMD_S_LOGONACK;
        public delegate void FMD_CS_LOGOFF_Event(AllegianceInterop.ClientConnection client, AllegianceInterop.FMD_CS_LOGOFF message);
        public event FMD_CS_LOGOFF_Event FMD_CS_LOGOFF;
        public delegate void FMD_C_BUY_LOADOUT_Event(AllegianceInterop.ClientConnection client, AllegianceInterop.FMD_C_BUY_LOADOUT message);
        public event FMD_C_BUY_LOADOUT_Event FMD_C_BUY_LOADOUT;
        public delegate void FMD_S_BUY_LOADOUT_ACK_Event(AllegianceInterop.ClientConnection client, AllegianceInterop.FMD_S_BUY_LOADOUT_ACK message);
        public event FMD_S_BUY_LOADOUT_ACK_Event FMD_S_BUY_LOADOUT_ACK;
        public delegate void FMD_S_OBJECT_SPOTTED_Event(AllegianceInterop.ClientConnection client, AllegianceInterop.FMD_S_OBJECT_SPOTTED message);
        public event FMD_S_OBJECT_SPOTTED_Event FMD_S_OBJECT_SPOTTED;
        public delegate void FMD_S_SET_CLUSTER_Event(AllegianceInterop.ClientConnection client, AllegianceInterop.FMD_S_SET_CLUSTER message);
        public event FMD_S_SET_CLUSTER_Event FMD_S_SET_CLUSTER;
        public delegate void FMD_S_DEV_COMPLETED_Event(AllegianceInterop.ClientConnection client, AllegianceInterop.FMD_S_DEV_COMPLETED message);
        public event FMD_S_DEV_COMPLETED_Event FMD_S_DEV_COMPLETED;
        public delegate void FMD_S_SET_BRIEFING_TEXT_Event(AllegianceInterop.ClientConnection client, AllegianceInterop.FMD_S_SET_BRIEFING_TEXT message);
        public event FMD_S_SET_BRIEFING_TEXT_Event FMD_S_SET_BRIEFING_TEXT;
        public delegate void FMD_S_PLAYERINFO_Event(AllegianceInterop.ClientConnection client, AllegianceInterop.FMD_S_PLAYERINFO message);
        public event FMD_S_PLAYERINFO_Event FMD_S_PLAYERINFO;
        public delegate void FMD_S_STATIC_MAP_INFO_Event(AllegianceInterop.ClientConnection client, AllegianceInterop.FMD_S_STATIC_MAP_INFO message);
        public event FMD_S_STATIC_MAP_INFO_Event FMD_S_STATIC_MAP_INFO;
        public delegate void FMD_CS_PROJECTILE_INSTANCE_Event(AllegianceInterop.ClientConnection client, AllegianceInterop.FMD_CS_PROJECTILE_INSTANCE message);
        public event FMD_CS_PROJECTILE_INSTANCE_Event FMD_CS_PROJECTILE_INSTANCE;
        public delegate void FMD_S_RANK_INFO_Event(AllegianceInterop.ClientConnection client, AllegianceInterop.FMD_S_RANK_INFO message);
        public event FMD_S_RANK_INFO_Event FMD_S_RANK_INFO;
        public delegate void FMD_S_SET_START_TIME_Event(AllegianceInterop.ClientConnection client, AllegianceInterop.FMD_S_SET_START_TIME message);
        public event FMD_S_SET_START_TIME_Event FMD_S_SET_START_TIME;
        public delegate void FMD_CS_SET_TEAM_INFO_Event(AllegianceInterop.ClientConnection client, AllegianceInterop.FMD_CS_SET_TEAM_INFO message);
        public event FMD_CS_SET_TEAM_INFO_Event FMD_CS_SET_TEAM_INFO;
        public delegate void FMD_S_PLAYER_RESCUED_Event(AllegianceInterop.ClientConnection client, AllegianceInterop.FMD_S_PLAYER_RESCUED message);
        public event FMD_S_PLAYER_RESCUED_Event FMD_S_PLAYER_RESCUED;
        public delegate void FMD_S_BALLOT_Event(AllegianceInterop.ClientConnection client, AllegianceInterop.FMD_S_BALLOT message);
        public event FMD_S_BALLOT_Event FMD_S_BALLOT;
        public delegate void FMD_S_CANCEL_BALLOT_Event(AllegianceInterop.ClientConnection client, AllegianceInterop.FMD_S_CANCEL_BALLOT message);
        public event FMD_S_CANCEL_BALLOT_Event FMD_S_CANCEL_BALLOT;
        public delegate void FMD_C_VOTE_Event(AllegianceInterop.ClientConnection client, AllegianceInterop.FMD_C_VOTE message);
        public event FMD_C_VOTE_Event FMD_C_VOTE;
        public delegate void FMD_S_SHIP_DELETE_Event(AllegianceInterop.ClientConnection client, AllegianceInterop.FMD_S_SHIP_DELETE message);
        public event FMD_S_SHIP_DELETE_Event FMD_S_SHIP_DELETE;
        public delegate void FMD_C_RANDOMIZE_TEAMS_Event(AllegianceInterop.ClientConnection client, AllegianceInterop.FMD_C_RANDOMIZE_TEAMS message);
        public event FMD_C_RANDOMIZE_TEAMS_Event FMD_C_RANDOMIZE_TEAMS;
        public delegate void FMD_CS_CHATMESSAGE_Event(AllegianceInterop.ClientConnection client, AllegianceInterop.FMD_CS_CHATMESSAGE message);
        public event FMD_CS_CHATMESSAGE_Event FMD_CS_CHATMESSAGE;
        public delegate void FMD_C_SHIP_UPDATE_Event(AllegianceInterop.ClientConnection client, AllegianceInterop.FMD_C_SHIP_UPDATE message);
        public event FMD_C_SHIP_UPDATE_Event FMD_C_SHIP_UPDATE;
        public delegate void FMD_S_EXPORT_Event(AllegianceInterop.ClientConnection client, AllegianceInterop.FMD_S_EXPORT message);
        public event FMD_S_EXPORT_Event FMD_S_EXPORT;
        public delegate void FMD_S_POSTER_Event(AllegianceInterop.ClientConnection client, AllegianceInterop.FMD_S_POSTER message);
        public event FMD_S_POSTER_Event FMD_S_POSTER;
        public delegate void FMD_S_TREASURE_Event(AllegianceInterop.ClientConnection client, AllegianceInterop.FMD_S_TREASURE message);
        public event FMD_S_TREASURE_Event FMD_S_TREASURE;
        public delegate void FMD_S_DESTROY_TREASURE_Event(AllegianceInterop.ClientConnection client, AllegianceInterop.FMD_S_DESTROY_TREASURE message);
        public event FMD_S_DESTROY_TREASURE_Event FMD_S_DESTROY_TREASURE;
        public delegate void FMD_CS_NEW_SHIP_INSTANCE_Event(AllegianceInterop.ClientConnection client, AllegianceInterop.FMD_CS_NEW_SHIP_INSTANCE message);
        public event FMD_CS_NEW_SHIP_INSTANCE_Event FMD_CS_NEW_SHIP_INSTANCE;
        public delegate void FMD_CS_DROP_PART_Event(AllegianceInterop.ClientConnection client, AllegianceInterop.FMD_CS_DROP_PART message);
        public event FMD_CS_DROP_PART_Event FMD_CS_DROP_PART;
        public delegate void FMD_CS_SWAP_PART_Event(AllegianceInterop.ClientConnection client, AllegianceInterop.FMD_CS_SWAP_PART message);
        public event FMD_CS_SWAP_PART_Event FMD_CS_SWAP_PART;
        public delegate void FMD_CS_PING_Event(AllegianceInterop.ClientConnection client, AllegianceInterop.FMD_CS_PING message);
        public event FMD_CS_PING_Event FMD_CS_PING;
        public delegate void FMD_S_URLROOT_Event(AllegianceInterop.ClientConnection client, AllegianceInterop.FMD_S_URLROOT message);
        public event FMD_S_URLROOT_Event FMD_S_URLROOT;
        public delegate void FMD_S_MISSION_STAGE_Event(AllegianceInterop.ClientConnection client, AllegianceInterop.FMD_S_MISSION_STAGE message);
        public event FMD_S_MISSION_STAGE_Event FMD_S_MISSION_STAGE;
        public delegate void FMD_S_MISSIONDEF_Event(AllegianceInterop.ClientConnection client, AllegianceInterop.FMD_S_MISSIONDEF message);
        public event FMD_S_MISSIONDEF_Event FMD_S_MISSIONDEF;
        public delegate void FMD_C_POSITIONREQ_Event(AllegianceInterop.ClientConnection client, AllegianceInterop.FMD_C_POSITIONREQ message);
        public event FMD_C_POSITIONREQ_Event FMD_C_POSITIONREQ;
        public delegate void FMD_S_POSITIONREQ_Event(AllegianceInterop.ClientConnection client, AllegianceInterop.FMD_S_POSITIONREQ message);
        public event FMD_S_POSITIONREQ_Event FMD_S_POSITIONREQ;
        public delegate void FMD_C_POSITIONACK_Event(AllegianceInterop.ClientConnection client, AllegianceInterop.FMD_C_POSITIONACK message);
        public event FMD_C_POSITIONACK_Event FMD_C_POSITIONACK;
        public delegate void FMD_CS_DELPOSITIONREQ_Event(AllegianceInterop.ClientConnection client, AllegianceInterop.FMD_CS_DELPOSITIONREQ message);
        public event FMD_CS_DELPOSITIONREQ_Event FMD_CS_DELPOSITIONREQ;
        public delegate void FMD_CS_LOCK_LOBBY_Event(AllegianceInterop.ClientConnection client, AllegianceInterop.FMD_CS_LOCK_LOBBY message);
        public event FMD_CS_LOCK_LOBBY_Event FMD_CS_LOCK_LOBBY;
        public delegate void FMD_CS_LOCK_SIDES_Event(AllegianceInterop.ClientConnection client, AllegianceInterop.FMD_CS_LOCK_SIDES message);
        public event FMD_CS_LOCK_SIDES_Event FMD_CS_LOCK_SIDES;
        public delegate void FMD_CS_PLAYER_READY_Event(AllegianceInterop.ClientConnection client, AllegianceInterop.FMD_CS_PLAYER_READY message);
        public event FMD_CS_PLAYER_READY_Event FMD_CS_PLAYER_READY;
        public delegate void FMD_S_TEAM_READY_Event(AllegianceInterop.ClientConnection client, AllegianceInterop.FMD_S_TEAM_READY message);
        public event FMD_S_TEAM_READY_Event FMD_S_TEAM_READY;
        public delegate void FMD_CS_FORCE_TEAM_READY_Event(AllegianceInterop.ClientConnection client, AllegianceInterop.FMD_CS_FORCE_TEAM_READY message);
        public event FMD_CS_FORCE_TEAM_READY_Event FMD_CS_FORCE_TEAM_READY;
        public delegate void FMD_C_DOCKED_Event(AllegianceInterop.ClientConnection client, AllegianceInterop.FMD_C_DOCKED message);
        public event FMD_C_DOCKED_Event FMD_C_DOCKED;
        public delegate void FMD_CS_CHANGE_TEAM_CIV_Event(AllegianceInterop.ClientConnection client, AllegianceInterop.FMD_CS_CHANGE_TEAM_CIV message);
        public event FMD_CS_CHANGE_TEAM_CIV_Event FMD_CS_CHANGE_TEAM_CIV;
        public delegate void FMD_CS_AUTO_ACCEPT_Event(AllegianceInterop.ClientConnection client, AllegianceInterop.FMD_CS_AUTO_ACCEPT message);
        public event FMD_CS_AUTO_ACCEPT_Event FMD_CS_AUTO_ACCEPT;
        public delegate void FMD_S_STATIONS_UPDATE_Event(AllegianceInterop.ClientConnection client, AllegianceInterop.FMD_S_STATIONS_UPDATE message);
        public event FMD_S_STATIONS_UPDATE_Event FMD_S_STATIONS_UPDATE;
        public delegate void FMD_S_STATION_CAPTURE_Event(AllegianceInterop.ClientConnection client, AllegianceInterop.FMD_S_STATION_CAPTURE message);
        public event FMD_S_STATION_CAPTURE_Event FMD_S_STATION_CAPTURE;
        public delegate void FMD_S_STATION_DESTROYED_Event(AllegianceInterop.ClientConnection client, AllegianceInterop.FMD_S_STATION_DESTROYED message);
        public event FMD_S_STATION_DESTROYED_Event FMD_S_STATION_DESTROYED;
        public delegate void FMD_S_MONEY_CHANGE_Event(AllegianceInterop.ClientConnection client, AllegianceInterop.FMD_S_MONEY_CHANGE message);
        public event FMD_S_MONEY_CHANGE_Event FMD_S_MONEY_CHANGE;
        public delegate void FMD_S_DOCKED_Event(AllegianceInterop.ClientConnection client, AllegianceInterop.FMD_S_DOCKED message);
        public event FMD_S_DOCKED_Event FMD_S_DOCKED;
        public delegate void FMD_S_GAME_OVER_Event(AllegianceInterop.ClientConnection client, AllegianceInterop.FMD_S_GAME_OVER message);
        public event FMD_S_GAME_OVER_Event FMD_S_GAME_OVER;
        public delegate void FMD_S_GAME_OVER_PLAYERS_Event(AllegianceInterop.ClientConnection client, AllegianceInterop.FMD_S_GAME_OVER_PLAYERS message);
        public event FMD_S_GAME_OVER_PLAYERS_Event FMD_S_GAME_OVER_PLAYERS;
        public delegate void FMD_C_BUCKET_DONATE_Event(AllegianceInterop.ClientConnection client, AllegianceInterop.FMD_C_BUCKET_DONATE message);
        public event FMD_C_BUCKET_DONATE_Event FMD_C_BUCKET_DONATE;
        public delegate void FMD_S_BUCKET_STATUS_Event(AllegianceInterop.ClientConnection client, AllegianceInterop.FMD_S_BUCKET_STATUS message);
        public event FMD_S_BUCKET_STATUS_Event FMD_S_BUCKET_STATUS;
        public delegate void FMD_C_PLAYER_DONATE_Event(AllegianceInterop.ClientConnection client, AllegianceInterop.FMD_C_PLAYER_DONATE message);
        public event FMD_C_PLAYER_DONATE_Event FMD_C_PLAYER_DONATE;
        public delegate void FMD_CS_SIDE_INACTIVE_Event(AllegianceInterop.ClientConnection client, AllegianceInterop.FMD_CS_SIDE_INACTIVE message);
        public event FMD_CS_SIDE_INACTIVE_Event FMD_CS_SIDE_INACTIVE;
        public delegate void FMD_CS_FIRE_MISSILE_Event(AllegianceInterop.ClientConnection client, AllegianceInterop.FMD_CS_FIRE_MISSILE message);
        public event FMD_CS_FIRE_MISSILE_Event FMD_CS_FIRE_MISSILE;
        public delegate void FMD_S_SIDE_TECH_CHANGE_Event(AllegianceInterop.ClientConnection client, AllegianceInterop.FMD_S_SIDE_TECH_CHANGE message);
        public event FMD_S_SIDE_TECH_CHANGE_Event FMD_S_SIDE_TECH_CHANGE;
        public delegate void FMD_S_SIDE_ATTRIBUTE_CHANGE_Event(AllegianceInterop.ClientConnection client, AllegianceInterop.FMD_S_SIDE_ATTRIBUTE_CHANGE message);
        public event FMD_S_SIDE_ATTRIBUTE_CHANGE_Event FMD_S_SIDE_ATTRIBUTE_CHANGE;
        public delegate void FMD_S_EJECT_Event(AllegianceInterop.ClientConnection client, AllegianceInterop.FMD_S_EJECT message);
        public event FMD_S_EJECT_Event FMD_S_EJECT;
        public delegate void FMD_S_SINGLE_SHIP_UPDATE_Event(AllegianceInterop.ClientConnection client, AllegianceInterop.FMD_S_SINGLE_SHIP_UPDATE message);
        public event FMD_S_SINGLE_SHIP_UPDATE_Event FMD_S_SINGLE_SHIP_UPDATE;
        public delegate void FMD_C_SUICIDE_Event(AllegianceInterop.ClientConnection client, AllegianceInterop.FMD_C_SUICIDE message);
        public event FMD_C_SUICIDE_Event FMD_C_SUICIDE;
        public delegate void FMD_C_FIRE_EXPENDABLE_Event(AllegianceInterop.ClientConnection client, AllegianceInterop.FMD_C_FIRE_EXPENDABLE message);
        public event FMD_C_FIRE_EXPENDABLE_Event FMD_C_FIRE_EXPENDABLE;
        public delegate void FMD_S_MISSILE_DESTROYED_Event(AllegianceInterop.ClientConnection client, AllegianceInterop.FMD_S_MISSILE_DESTROYED message);
        public event FMD_S_MISSILE_DESTROYED_Event FMD_S_MISSILE_DESTROYED;
        public delegate void FMD_S_MINE_DESTROYED_Event(AllegianceInterop.ClientConnection client, AllegianceInterop.FMD_S_MINE_DESTROYED message);
        public event FMD_S_MINE_DESTROYED_Event FMD_S_MINE_DESTROYED;
        public delegate void FMD_S_PROBE_DESTROYED_Event(AllegianceInterop.ClientConnection client, AllegianceInterop.FMD_S_PROBE_DESTROYED message);
        public event FMD_S_PROBE_DESTROYED_Event FMD_S_PROBE_DESTROYED;
        public delegate void FMD_S_ASTEROID_DESTROYED_Event(AllegianceInterop.ClientConnection client, AllegianceInterop.FMD_S_ASTEROID_DESTROYED message);
        public event FMD_S_ASTEROID_DESTROYED_Event FMD_S_ASTEROID_DESTROYED;
        public delegate void FMD_S_FIRE_EXPENDABLE_Event(AllegianceInterop.ClientConnection client, AllegianceInterop.FMD_S_FIRE_EXPENDABLE message);
        public event FMD_S_FIRE_EXPENDABLE_Event FMD_S_FIRE_EXPENDABLE;
        public delegate void FMD_S_TREASURE_SETS_Event(AllegianceInterop.ClientConnection client, AllegianceInterop.FMD_S_TREASURE_SETS message);
        public event FMD_S_TREASURE_SETS_Event FMD_S_TREASURE_SETS;
        public delegate void FMD_S_SHIP_STATUS_Event(AllegianceInterop.ClientConnection client, AllegianceInterop.FMD_S_SHIP_STATUS message);
        public event FMD_S_SHIP_STATUS_Event FMD_S_SHIP_STATUS;
        public delegate void FMD_S_PROBES_UPDATE_Event(AllegianceInterop.ClientConnection client, AllegianceInterop.FMD_S_PROBES_UPDATE message);
        public event FMD_S_PROBES_UPDATE_Event FMD_S_PROBES_UPDATE;
        public delegate void FMD_CS_ORDER_CHANGE_Event(AllegianceInterop.ClientConnection client, AllegianceInterop.FMD_CS_ORDER_CHANGE message);
        public event FMD_CS_ORDER_CHANGE_Event FMD_CS_ORDER_CHANGE;
        public delegate void FMD_S_LEAVE_SHIP_Event(AllegianceInterop.ClientConnection client, AllegianceInterop.FMD_S_LEAVE_SHIP message);
        public event FMD_S_LEAVE_SHIP_Event FMD_S_LEAVE_SHIP;
        public delegate void FMD_S_JOINED_MISSION_Event(AllegianceInterop.ClientConnection client, AllegianceInterop.FMD_S_JOINED_MISSION message);
        public event FMD_S_JOINED_MISSION_Event FMD_S_JOINED_MISSION;
        public delegate void FMD_S_LOADOUT_CHANGE_Event(AllegianceInterop.ClientConnection client, AllegianceInterop.FMD_S_LOADOUT_CHANGE message);
        public event FMD_S_LOADOUT_CHANGE_Event FMD_S_LOADOUT_CHANGE;
        public delegate void FMD_C_ACTIVE_TURRET_UPDATE_Event(AllegianceInterop.ClientConnection client, AllegianceInterop.FMD_C_ACTIVE_TURRET_UPDATE message);
        public event FMD_C_ACTIVE_TURRET_UPDATE_Event FMD_C_ACTIVE_TURRET_UPDATE;
        public delegate void FMD_C_INACTIVE_TURRET_UPDATE_Event(AllegianceInterop.ClientConnection client, AllegianceInterop.FMD_C_INACTIVE_TURRET_UPDATE message);
        public event FMD_C_INACTIVE_TURRET_UPDATE_Event FMD_C_INACTIVE_TURRET_UPDATE;
        public delegate void FMD_S_TELEPORT_ACK_Event(AllegianceInterop.ClientConnection client, AllegianceInterop.FMD_S_TELEPORT_ACK message);
        public event FMD_S_TELEPORT_ACK_Event FMD_S_TELEPORT_ACK;
        public delegate void FMD_C_BOARD_SHIP_Event(AllegianceInterop.ClientConnection client, AllegianceInterop.FMD_C_BOARD_SHIP message);
        public event FMD_C_BOARD_SHIP_Event FMD_C_BOARD_SHIP;
        public delegate void FMD_C_VIEW_CLUSTER_Event(AllegianceInterop.ClientConnection client, AllegianceInterop.FMD_C_VIEW_CLUSTER message);
        public event FMD_C_VIEW_CLUSTER_Event FMD_C_VIEW_CLUSTER;
        public delegate void FMD_S_KILL_SHIP_Event(AllegianceInterop.ClientConnection client, AllegianceInterop.FMD_S_KILL_SHIP message);
        public event FMD_S_KILL_SHIP_Event FMD_S_KILL_SHIP;
        public delegate void FMD_CS_SET_WINGID_Event(AllegianceInterop.ClientConnection client, AllegianceInterop.FMD_CS_SET_WINGID message);
        public event FMD_CS_SET_WINGID_Event FMD_CS_SET_WINGID;
        public delegate void FMD_S_ICQ_CHAT_ACK_Event(AllegianceInterop.ClientConnection client, AllegianceInterop.FMD_S_ICQ_CHAT_ACK message);
        public event FMD_S_ICQ_CHAT_ACK_Event FMD_S_ICQ_CHAT_ACK;
        public delegate void FMD_CS_CHATBUOY_Event(AllegianceInterop.ClientConnection client, AllegianceInterop.FMD_CS_CHATBUOY message);
        public event FMD_CS_CHATBUOY_Event FMD_CS_CHATBUOY;
        public delegate void FMD_S_CREATE_CHAFF_Event(AllegianceInterop.ClientConnection client, AllegianceInterop.FMD_S_CREATE_CHAFF message);
        public event FMD_S_CREATE_CHAFF_Event FMD_S_CREATE_CHAFF;
        public delegate void FMD_S_MISSILE_SPOOFED_Event(AllegianceInterop.ClientConnection client, AllegianceInterop.FMD_S_MISSILE_SPOOFED message);
        public event FMD_S_MISSILE_SPOOFED_Event FMD_S_MISSILE_SPOOFED;
        public delegate void FMD_S_END_SPOOFING_Event(AllegianceInterop.ClientConnection client, AllegianceInterop.FMD_S_END_SPOOFING message);
        public event FMD_S_END_SPOOFING_Event FMD_S_END_SPOOFING;
        public delegate void FMD_C_AUTODONATE_Event(AllegianceInterop.ClientConnection client, AllegianceInterop.FMD_C_AUTODONATE message);
        public event FMD_C_AUTODONATE_Event FMD_C_AUTODONATE;
        public delegate void FMD_CS_MISSIONPARAMS_Event(AllegianceInterop.ClientConnection client, AllegianceInterop.FMD_CS_MISSIONPARAMS message);
        public event FMD_CS_MISSIONPARAMS_Event FMD_CS_MISSIONPARAMS;
        public delegate void FMD_S_PAYDAY_Event(AllegianceInterop.ClientConnection client, AllegianceInterop.FMD_S_PAYDAY message);
        public event FMD_S_PAYDAY_Event FMD_S_PAYDAY;
        public delegate void FMD_S_SET_MONEY_Event(AllegianceInterop.ClientConnection client, AllegianceInterop.FMD_S_SET_MONEY message);
        public event FMD_S_SET_MONEY_Event FMD_S_SET_MONEY;
        public delegate void FMD_S_AUTODONATE_Event(AllegianceInterop.ClientConnection client, AllegianceInterop.FMD_S_AUTODONATE message);
        public event FMD_S_AUTODONATE_Event FMD_S_AUTODONATE;
        public delegate void FMD_C_MUTE_Event(AllegianceInterop.ClientConnection client, AllegianceInterop.FMD_C_MUTE message);
        public event FMD_C_MUTE_Event FMD_C_MUTE;
        public delegate void FMD_CS_SET_TEAM_LEADER_Event(AllegianceInterop.ClientConnection client, AllegianceInterop.FMD_CS_SET_TEAM_LEADER message);
        public event FMD_CS_SET_TEAM_LEADER_Event FMD_CS_SET_TEAM_LEADER;
        public delegate void FMD_CS_SET_TEAM_INVESTOR_Event(AllegianceInterop.ClientConnection client, AllegianceInterop.FMD_CS_SET_TEAM_INVESTOR message);
        public event FMD_CS_SET_TEAM_INVESTOR_Event FMD_CS_SET_TEAM_INVESTOR;
        public delegate void FMD_CS_SET_MISSION_OWNER_Event(AllegianceInterop.ClientConnection client, AllegianceInterop.FMD_CS_SET_MISSION_OWNER message);
        public event FMD_CS_SET_MISSION_OWNER_Event FMD_CS_SET_MISSION_OWNER;
        public delegate void FMD_CS_QUIT_MISSION_Event(AllegianceInterop.ClientConnection client, AllegianceInterop.FMD_CS_QUIT_MISSION message);
        public event FMD_CS_QUIT_MISSION_Event FMD_CS_QUIT_MISSION;
        public delegate void FMD_S_JOIN_SIDE_Event(AllegianceInterop.ClientConnection client, AllegianceInterop.FMD_S_JOIN_SIDE message);
        public event FMD_S_JOIN_SIDE_Event FMD_S_JOIN_SIDE;
        public delegate void FMD_CS_QUIT_SIDE_Event(AllegianceInterop.ClientConnection client, AllegianceInterop.FMD_CS_QUIT_SIDE message);
        public event FMD_CS_QUIT_SIDE_Event FMD_CS_QUIT_SIDE;
        public delegate void FMD_S_ENTER_GAME_Event(AllegianceInterop.ClientConnection client, AllegianceInterop.FMD_S_ENTER_GAME message);
        public event FMD_S_ENTER_GAME_Event FMD_S_ENTER_GAME;
        public delegate void FMD_CS_RELOAD_Event(AllegianceInterop.ClientConnection client, AllegianceInterop.FMD_CS_RELOAD message);
        public event FMD_CS_RELOAD_Event FMD_CS_RELOAD;
        //public delegate void FMD_S_THREAT_Event(AllegianceInterop.ClientConnection client, AllegianceInterop.FMD_S_THREAT message);
        //public event FMD_S_THREAT_Event FMD_S_THREAT;
        //public delegate void FMD_S_GAME_STATE_Event(AllegianceInterop.ClientConnection client, AllegianceInterop.FMD_S_GAME_STATE message);
        //public event FMD_S_GAME_STATE_Event FMD_S_GAME_STATE;
        public delegate void FMD_S_GAIN_FLAG_Event(AllegianceInterop.ClientConnection client, AllegianceInterop.FMD_S_GAIN_FLAG message);
        public event FMD_S_GAIN_FLAG_Event FMD_S_GAIN_FLAG;
        public delegate void FMD_C_START_GAME_Event(AllegianceInterop.ClientConnection client, AllegianceInterop.FMD_C_START_GAME message);
        public event FMD_C_START_GAME_Event FMD_C_START_GAME;
        public delegate void FMD_S_ACQUIRE_TREASURE_Event(AllegianceInterop.ClientConnection client, AllegianceInterop.FMD_S_ACQUIRE_TREASURE message);
        public event FMD_S_ACQUIRE_TREASURE_Event FMD_S_ACQUIRE_TREASURE;
        public delegate void FMD_C_TREASURE_ACK_Event(AllegianceInterop.ClientConnection client, AllegianceInterop.FMD_C_TREASURE_ACK message);
        public event FMD_C_TREASURE_ACK_Event FMD_C_TREASURE_ACK;
        public delegate void FMD_S_ADD_PART_Event(AllegianceInterop.ClientConnection client, AllegianceInterop.FMD_S_ADD_PART message);
        public event FMD_S_ADD_PART_Event FMD_S_ADD_PART;
        public delegate void FMD_S_ENTER_LIFEPOD_Event(AllegianceInterop.ClientConnection client, AllegianceInterop.FMD_S_ENTER_LIFEPOD message);
        public event FMD_S_ENTER_LIFEPOD_Event FMD_S_ENTER_LIFEPOD;
        public delegate void FMD_S_LIGHT_SHIPS_UPDATE_Event(AllegianceInterop.ClientConnection client, AllegianceInterop.FMD_S_LIGHT_SHIPS_UPDATE message);
        public event FMD_S_LIGHT_SHIPS_UPDATE_Event FMD_S_LIGHT_SHIPS_UPDATE;
        public delegate void FMD_S_HEAVY_SHIPS_UPDATE_Event(AllegianceInterop.ClientConnection client, AllegianceInterop.FMD_S_HEAVY_SHIPS_UPDATE message);
        public event FMD_S_HEAVY_SHIPS_UPDATE_Event FMD_S_HEAVY_SHIPS_UPDATE;
        public delegate void FMD_S_VIEW_CLUSTER_Event(AllegianceInterop.ClientConnection client, AllegianceInterop.FMD_S_VIEW_CLUSTER message);
        public event FMD_S_VIEW_CLUSTER_Event FMD_S_VIEW_CLUSTER;
        public delegate void FMD_S_BOARD_NACK_Event(AllegianceInterop.ClientConnection client, AllegianceInterop.FMD_S_BOARD_NACK message);
        public event FMD_S_BOARD_NACK_Event FMD_S_BOARD_NACK;
        public delegate void FMD_S_ASTEROIDS_UPDATE_Event(AllegianceInterop.ClientConnection client, AllegianceInterop.FMD_S_ASTEROIDS_UPDATE message);
        public event FMD_S_ASTEROIDS_UPDATE_Event FMD_S_ASTEROIDS_UPDATE;
        public delegate void FMD_S_ASTEROID_DRAINED_Event(AllegianceInterop.ClientConnection client, AllegianceInterop.FMD_S_ASTEROID_DRAINED message);
        public event FMD_S_ASTEROID_DRAINED_Event FMD_S_ASTEROID_DRAINED;
        public delegate void FMD_S_BUILDINGEFFECT_DESTROYED_Event(AllegianceInterop.ClientConnection client, AllegianceInterop.FMD_S_BUILDINGEFFECT_DESTROYED message);
        public event FMD_S_BUILDINGEFFECT_DESTROYED_Event FMD_S_BUILDINGEFFECT_DESTROYED;
        public delegate void FMD_CS_REQUEST_MONEY_Event(AllegianceInterop.ClientConnection client, AllegianceInterop.FMD_CS_REQUEST_MONEY message);
        public event FMD_CS_REQUEST_MONEY_Event FMD_CS_REQUEST_MONEY;
        public delegate void FMD_S_SHIP_RESET_Event(AllegianceInterop.ClientConnection client, AllegianceInterop.FMD_S_SHIP_RESET message);
        public event FMD_S_SHIP_RESET_Event FMD_S_SHIP_RESET;
        public delegate void FMD_C_RIPCORD_REQUEST_Event(AllegianceInterop.ClientConnection client, AllegianceInterop.FMD_C_RIPCORD_REQUEST message);
        public event FMD_C_RIPCORD_REQUEST_Event FMD_C_RIPCORD_REQUEST;
        public delegate void FMD_S_RIPCORD_ACTIVATE_Event(AllegianceInterop.ClientConnection client, AllegianceInterop.FMD_S_RIPCORD_ACTIVATE message);
        public event FMD_S_RIPCORD_ACTIVATE_Event FMD_S_RIPCORD_ACTIVATE;
        public delegate void FMD_S_RIPCORD_DENIED_Event(AllegianceInterop.ClientConnection client, AllegianceInterop.FMD_S_RIPCORD_DENIED message);
        public event FMD_S_RIPCORD_DENIED_Event FMD_S_RIPCORD_DENIED;
        public delegate void FMD_S_RIPCORD_ABORTED_Event(AllegianceInterop.ClientConnection client, AllegianceInterop.FMD_S_RIPCORD_ABORTED message);
        public event FMD_S_RIPCORD_ABORTED_Event FMD_S_RIPCORD_ABORTED;
        public delegate void FMD_S_WARP_BOMB_Event(AllegianceInterop.ClientConnection client, AllegianceInterop.FMD_S_WARP_BOMB message);
        public event FMD_S_WARP_BOMB_Event FMD_S_WARP_BOMB;
        public delegate void FMD_S_PROMOTE_Event(AllegianceInterop.ClientConnection client, AllegianceInterop.FMD_S_PROMOTE message);
        public event FMD_S_PROMOTE_Event FMD_S_PROMOTE;
        public delegate void FMD_C_PROMOTE_Event(AllegianceInterop.ClientConnection client, AllegianceInterop.FMD_C_PROMOTE message);
        public event FMD_C_PROMOTE_Event FMD_C_PROMOTE;
        public delegate void FMD_S_CREATE_BUCKETS_Event(AllegianceInterop.ClientConnection client, AllegianceInterop.FMD_S_CREATE_BUCKETS message);
        public event FMD_S_CREATE_BUCKETS_Event FMD_S_CREATE_BUCKETS;
        public delegate void FMD_S_RELAUNCH_SHIP_Event(AllegianceInterop.ClientConnection client, AllegianceInterop.FMD_S_RELAUNCH_SHIP message);
        public event FMD_S_RELAUNCH_SHIP_Event FMD_S_RELAUNCH_SHIP;
        //public delegate void FMD_C_BANDWIDTH_Event(AllegianceInterop.ClientConnection client, AllegianceInterop.FMD_C_BANDWIDTH message);
        //public event FMD_C_BANDWIDTH_Event FMD_C_BANDWIDTH;
        public delegate void FMD_S_PINGDATA_Event(AllegianceInterop.ClientConnection client, AllegianceInterop.FMD_S_PINGDATA message);
        public event FMD_S_PINGDATA_Event FMD_S_PINGDATA;
        public delegate void FMD_C_REQPINGDATA_Event(AllegianceInterop.ClientConnection client, AllegianceInterop.FMD_C_REQPINGDATA message);
        public event FMD_C_REQPINGDATA_Event FMD_C_REQPINGDATA;
        public delegate void FMD_C_CHANGE_ALLIANCE_Event(AllegianceInterop.ClientConnection client, AllegianceInterop.FMD_C_CHANGE_ALLIANCE message);
        public event FMD_C_CHANGE_ALLIANCE_Event FMD_C_CHANGE_ALLIANCE;
        public delegate void FMD_S_CHANGE_ALLIANCES_Event(AllegianceInterop.ClientConnection client, AllegianceInterop.FMD_S_CHANGE_ALLIANCES message);
        public event FMD_S_CHANGE_ALLIANCES_Event FMD_S_CHANGE_ALLIANCES;
        public delegate void FMD_S_ASTEROID_MINED_Event(AllegianceInterop.ClientConnection client, AllegianceInterop.FMD_S_ASTEROID_MINED message);
        public event FMD_S_ASTEROID_MINED_Event FMD_S_ASTEROID_MINED;
        public delegate void FMD_CS_HIGHLIGHT_CLUSTER_Event(AllegianceInterop.ClientConnection client, AllegianceInterop.FMD_CS_HIGHLIGHT_CLUSTER message);
        public event FMD_CS_HIGHLIGHT_CLUSTER_Event FMD_CS_HIGHLIGHT_CLUSTER;

        // From messageslc.h
        public delegate void FMD_C_LOGON_LOBBY_OLD_Event(AllegianceInterop.ClientConnection client, AllegianceInterop.FMD_C_LOGON_LOBBY_OLD message);
        public event FMD_C_LOGON_LOBBY_OLD_Event FMD_C_LOGON_LOBBY_OLD;
        public delegate void FMD_C_LOGOFF_LOBBY_Event(AllegianceInterop.ClientConnection client, AllegianceInterop.FMD_C_LOGOFF_LOBBY message);
        public event FMD_C_LOGOFF_LOBBY_Event FMD_C_LOGOFF_LOBBY;
        public delegate void FMD_L_AUTO_UPDATE_INFO_Event(AllegianceInterop.ClientConnection client, AllegianceInterop.FMD_L_AUTO_UPDATE_INFO message);
        public event FMD_L_AUTO_UPDATE_INFO_Event FMD_L_AUTO_UPDATE_INFO;
        public delegate void FMD_C_CREATE_MISSION_REQ_Event(AllegianceInterop.ClientConnection client, AllegianceInterop.FMD_C_CREATE_MISSION_REQ message);
        public event FMD_C_CREATE_MISSION_REQ_Event FMD_C_CREATE_MISSION_REQ;
        public delegate void FMD_L_CREATE_MISSION_ACK_Event(AllegianceInterop.ClientConnection client, AllegianceInterop.FMD_L_CREATE_MISSION_ACK message);
        public event FMD_L_CREATE_MISSION_ACK_Event FMD_L_CREATE_MISSION_ACK;
        public delegate void FMD_L_CREATE_MISSION_NACK_Event(AllegianceInterop.ClientConnection client, AllegianceInterop.FMD_L_CREATE_MISSION_NACK message);
        public event FMD_L_CREATE_MISSION_NACK_Event FMD_L_CREATE_MISSION_NACK;
        public delegate void FMD_C_JOIN_GAME_REQ_Event(AllegianceInterop.ClientConnection client, AllegianceInterop.FMD_C_JOIN_GAME_REQ message);
        public event FMD_C_JOIN_GAME_REQ_Event FMD_C_JOIN_GAME_REQ;
        public delegate void FMD_L_JOIN_GAME_NACK_Event(AllegianceInterop.ClientConnection client, AllegianceInterop.FMD_L_JOIN_GAME_NACK message);
        public event FMD_L_JOIN_GAME_NACK_Event FMD_L_JOIN_GAME_NACK;
        public delegate void FMD_L_JOIN_MISSION_Event(AllegianceInterop.ClientConnection client, AllegianceInterop.FMD_L_JOIN_MISSION message);
        public event FMD_L_JOIN_MISSION_Event FMD_L_JOIN_MISSION;
        public delegate void FMD_L_LOGON_ACK_Event(AllegianceInterop.ClientConnection client, AllegianceInterop.FMD_L_LOGON_ACK message);
        public event FMD_L_LOGON_ACK_Event FMD_L_LOGON_ACK;
        public delegate void FMD_L_LOGON_NACK_Event(AllegianceInterop.ClientConnection client, AllegianceInterop.FMD_L_LOGON_NACK message);
        public event FMD_L_LOGON_NACK_Event FMD_L_LOGON_NACK;
        public delegate void FMD_C_FIND_PLAYER_Event(AllegianceInterop.ClientConnection client, AllegianceInterop.FMD_C_FIND_PLAYER message);
        public event FMD_C_FIND_PLAYER_Event FMD_C_FIND_PLAYER;
        public delegate void FMD_L_FOUND_PLAYER_Event(AllegianceInterop.ClientConnection client, AllegianceInterop.FMD_L_FOUND_PLAYER message);
        public event FMD_L_FOUND_PLAYER_Event FMD_L_FOUND_PLAYER;
        public delegate void FMD_C_LOGON_LOBBY_Event(AllegianceInterop.ClientConnection client, AllegianceInterop.FMD_C_LOGON_LOBBY message);
        public event FMD_C_LOGON_LOBBY_Event FMD_C_LOGON_LOBBY;
        public delegate void FMD_C_GET_SERVERS_REQ_Event(AllegianceInterop.ClientConnection client, AllegianceInterop.FMD_C_GET_SERVERS_REQ message);
        public event FMD_C_GET_SERVERS_REQ_Event FMD_C_GET_SERVERS_REQ;
        public delegate void FMD_L_SERVERS_LIST_Event(AllegianceInterop.ClientConnection client, AllegianceInterop.FMD_L_SERVERS_LIST message);
        public event FMD_L_SERVERS_LIST_Event FMD_L_SERVERS_LIST;

        // messagesall.h
        public delegate void FMD_LS_LOBBYMISSIONINFO_Event(AllegianceInterop.ClientConnection client, AllegianceInterop.FMD_LS_LOBBYMISSIONINFO message);
        public event FMD_LS_LOBBYMISSIONINFO_Event FMD_LS_LOBBYMISSIONINFO;
        public delegate void FMD_LS_MISSION_GONE_Event(AllegianceInterop.ClientConnection client, AllegianceInterop.FMD_LS_MISSION_GONE message);
        public event FMD_LS_MISSION_GONE_Event FMD_LS_MISSION_GONE;
        public delegate void FMD_LS_SQUAD_MEMBERSHIPS_Event(AllegianceInterop.ClientConnection client, AllegianceInterop.FMD_LS_SQUAD_MEMBERSHIPS message);
        public event FMD_LS_SQUAD_MEMBERSHIPS_Event FMD_LS_SQUAD_MEMBERSHIPS;

        #endregion
    }
}
