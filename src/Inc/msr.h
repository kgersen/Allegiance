//Imago #192 8/10
// This file can be assimilated into FedSrv.H or w/e once we have it nailed

// Keep Msr in sync with /AllegSkill/nph-GetMSRFromCallsign.cgi
typedef struct {
 float mu;
 float sigma;
 float rank; //This can be removed from use once ASGS is no longer supported
 char name[12];
} Msr;

typedef struct {
 float muSum;
 float sigmaSum;
} SideMsr;