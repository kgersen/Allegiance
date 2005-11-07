
#ifndef _TECHINIT_H_
#define _TECHINIT_H_


HRESULT Initialize(CHAR * szDSN, CTechItemList * pCivList,
                    CTechItemList * pItemList);
VOID Terminate(CTechItemList * pCivList, CTechItemList * pItemList);


#endif


