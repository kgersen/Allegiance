#pragma once

#ifdef  _ALLEGIANCE_DEV_
#define ALLEGIANCE_REGISTRY_KEY_ROOT   "Software\\Microsoft\\Microsoft Games\\Allegiance\\Dev"
#else
#define ALLEGIANCE_REGISTRY_KEY_ROOT   "Software\\Microsoft\\Microsoft Games\\Allegiance\\1.1"
#endif


#if !defined(ALLSRV_STANDALONE)
  const char c_szSvcName[] = "AllSrv";
  #define HKLM_FedSrv        "SYSTEM\\CurrentControlSet\\Services\\AllSrv"
  #define HKLM_AllSrvUI      ALLEGIANCE_REGISTRY_KEY_ROOT "\\Server"
#else // !defined(ALLSRV_STANDALONE)
  const char c_szSvcName[] = "AllSrv";
  #define HKLM_FedSrv        ALLEGIANCE_REGISTRY_KEY_ROOT "\\Server"
  #define HKLM_AllSrvUI      HKLM_FedSrv
#endif // !defined(ALLSRV_STANDALONE)


