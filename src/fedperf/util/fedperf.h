// 	This file should define all the objects and counters that 
//	are to be monitored by the NT Performance Monitor. The
// 	definitions need to conform to following rules:
//	1) #define values should be even numbers starting from 0
//	2) There should not be any gap in the even numbers.
//  3) There should only be one object definition.
//  4) The counters #defined should have a one to one relationship
//      with the fields defined in your structure in toolcntr.h.

#define OBJECT_FEDSRV_OBJECT                  0
#define COUNTER_S_LOGIN_ATTEMPTS              2
#define COUNTER_S_LOGINS                      4
#define COUNTER_S_LOGINS_FAILED               6
#define COUNTER_S_LOGOFFS                     8
#define COUNTER_S_TIMEOUTS                   10
#define COUNTER_S_TIMEOUTS_PER_SEC           12
#define COUNTER_S_RELOGINS                   14
#define COUNTER_S_PACKETS_IN                 16
#define COUNTER_S_PACKETS_IN_PER_SEC         18
#define COUNTER_S_PACKETS_OUT                20
#define COUNTER_S_PACKETS_OUT_PER_SEC        22
#define COUNTER_S_BYTES_IN                   24
#define COUNTER_S_BYTES_IN_PER_SEC           26
#define COUNTER_S_BYTES_OUT                  28
#define COUNTER_S_BYTES_OUT_PER_SEC          30
#define COUNTER_S_PLAYERS_ONLINE             32
#define COUNTER_S_AVG_UPDATE_LATENCY         34
#define COUNTER_S_MAX_UPDATE_LATENCY         36
#define COUNTER_S_SYS_MESSAGES_IN            38
#define COUNTER_S_SYS_MESSAGES_IN_PER_SEC    40
#define COUNTER_S_DPLAY_INBOUND_QUEUE_LEN    42
#define COUNTER_S_DPLAY_INBOUND_QUEUE_SIZE   44
#define COUNTER_S_DPLAY_OUTBOUND_QUEUE_LEN   46
#define COUNTER_S_DPLAY_OUTBOUND_QUEUE_SIZE  48
#define COUNTER_S_TIME_IGC_WORK              50
#define COUNTER_S_TIME_BETWEEN_INNER_LOOPS   52
#define COUNTER_S_TIME_NETWORK_MESSAGES      54
#define COUNTER_S_TIME_MSG_PUMP              56
#define COUNTER_S_TARGET_CYCLE_TIME          58

#define OBJECT_ALLLOBBY_OBJECT               60
#define COUNTER_L_LOGINS                     62
#define COUNTER_L_LOGOFFS                    64
#define COUNTER_L_PLAYERS_MISSIONS           66
#define COUNTER_L_PLAYERS_LOBBY              68
#define COUNTER_L_PLAYERS_DROPPED            70
#define COUNTER_L_DPLAY_INBOUND_QUEUE_LEN    72
#define COUNTER_L_DPLAY_INBOUND_QUEUE_SIZE   74
#define COUNTER_L_DPLAY_OUTBOUND_QUEUE_LEN   76
#define COUNTER_L_DPLAY_OUTBOUND_QUEUE_SIZE  78
#define COUNTER_L_TIME_BETWEEN_INNER_LOOPS   80
#define COUNTER_L_MISSIONS                   82
#define COUNTER_L_SERVERS                    84

#define OBJECT_PERSERVER_OBJECT              86
#define COUNTER_PS_PLAYERS                   88
#define COUNTER_PS_MISSIONS                  90
#define COUNTER_PS_PERCENT_LOAD              92

////	The next available counter index is *** 94 ***


