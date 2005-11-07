
#include "pch.h"


CTechItemList gCivList;
CTechItemList gItemList;


//
// Console helper functions.
//
VOID ClearScreen()
{
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    DWORD dwConSize, cbWritten;
    COORD coordScreen = { 0, 0 };
    HANDLE hConsole;

    hConsole = CreateFile("CONOUT$",
                            GENERIC_READ | GENERIC_WRITE,
                            FILE_SHARE_READ | FILE_SHARE_WRITE,
                            NULL,
                            OPEN_EXISTING,
                            0,
                            NULL);

    if (NULL != hConsole)
    {
        if (TRUE == GetConsoleScreenBufferInfo(hConsole, &csbi))
        {
            dwConSize = csbi.dwSize.X * csbi.dwSize.Y;
            if (TRUE == FillConsoleOutputCharacter(hConsole,
                                                    ' ',
                                                    dwConSize,
                                                    coordScreen,
                                                    &cbWritten))
            {
                if (TRUE == GetConsoleScreenBufferInfo(hConsole, &csbi))
                {
                    if (TRUE == FillConsoleOutputAttribute(hConsole,
                                                            csbi.wAttributes,
                                                            dwConSize,
                                                            coordScreen,
                                                            &cbWritten))
                    {
                        SetConsoleCursorPosition(hConsole, coordScreen);
                    }
                }
            }
        }
        CloseHandle(hConsole);
    }
}


VOID GetLine(CHAR * szValid, DWORD dwMax, CHAR * pch, DWORD * pdw)
{
    CHAR s[256], *pchStart;
    BOOL fValid;

    *pch = '\0';
    *pdw = 0;
    fValid = FALSE;

    do
    {
        gets(s);
        pchStart = s;
        while(' ' == *pchStart)
            pchStart++;
        if (NULL != strchr(szValid, *pchStart))
        {
            *pch = *pchStart;
            fValid = TRUE;
        } else
        {
            while((*pchStart >= '0') && (*pchStart <= '9'))
            {
                *pdw = *pdw * 10 + (*pchStart - '0');
                *pchStart++;
            }

            if ((*pdw > 0) && (*pdw <= dwMax))
                fValid = TRUE;
        }

        if (FALSE == fValid)
        {
            printf("Invalid selection.\n> ");
            *pdw = 0;
        }
    } while(FALSE == fValid);
}


//
// Printing functions.
//
VOID PrintBoughtItems(CTechItemList * pBoughtList)
{
    CTechItem * pItem;
    DWORD dwIndex;

    if (pBoughtList->Length() > 0)
    {
        printf("Items bought:\n");
        printf("--------------------------\n");
        for(dwIndex = pBoughtList->Length(); dwIndex > 0; dwIndex--)
        {
            pItem = pBoughtList->Dequeue();
            printf("%s (%s)\n", pItem->GetName(), pItem->GetObjectTypeName());
            pBoughtList->Enqueue(pItem);
        }
        printf("\n");
    }
}


VOID PrintBuyOptions(CTechItemList * pCanBuyList, DWORD dwLastLength)
{
    DWORD dwIndex, dwLoop;
    CTechItem * pItem;

    dwIndex = 1;
    dwLoop = pCanBuyList->Length();

    if (dwLastLength > 0)
    {
        printf("Available items:\n");
        printf("--------------------------\n");
        for( ; (dwLoop > 0) && (dwIndex < dwLastLength); dwIndex++, dwLoop--)
        {
            pItem = pCanBuyList->Dequeue();
            printf("%2d) %s (%s)\n", dwIndex, pItem->GetName(),
                pItem->GetObjectTypeName());

            pCanBuyList->Enqueue(pItem);
        }
        printf("\n");
    }

    if (pCanBuyList->Length() >= dwLastLength)
    {
        //
        // These are items new since last time.
        //
        printf("New items:\n");
        printf("--------------------------\n");

        for( ; dwLoop > 0; dwIndex++, dwLoop--)
        {
            pItem = pCanBuyList->Dequeue();

            printf("%2d) %s (%s)\n", dwIndex,
                pItem->GetName(), pItem->GetObjectTypeName());

            pCanBuyList->Enqueue(pItem);
        }

        printf("\n");
    }
}


VOID BuyItem(DWORD dwBuy, PURCHASE_DATA * pPurchaseData)
{
    CTechItem * pItem;

    pItem = pPurchaseData->CanBuyList.RemoveNth(dwBuy - 1);
    if (NULL != pItem)
        pItem->Purchase(pPurchaseData);
}



BOOL AutoProcessCiv(CCivTechItem * pCiv)
{
    PURCHASE_DATA PurchaseData;
    BOOL fBought, fSuccess;

    fSuccess = TRUE;

    printf("====================\n");
    printf("%s\n", pCiv->GetName());
    printf("====================\n");

    PurchaseData.ttbmCurrent.ClearAll();
    PurchaseData.dwLastCanBuyLength = 0;

    if (TRUE == gItemList.CopyTo(&(PurchaseData.TechItemList)))
    {
        pCiv->Purchase(&PurchaseData);
        do
        {
			// these lines cause the list to be printed at every step along the way
            //PrintBoughtItems(&(PurchaseData.BoughtList));
            //PrintBuyOptions(&(PurchaseData.CanBuyList),
            //                PurchaseData.dwLastCanBuyLength);
            PurchaseData.dwLastCanBuyLength = PurchaseData.CanBuyList.Length();

            fBought = FALSE;
            if (PurchaseData.CanBuyList.Length() > 0)
            {
                BuyItem(1, &PurchaseData);
                fBought = TRUE;
            }
        } while(TRUE == fBought);
    } else
        fSuccess = FALSE;

    printf("\n\n");
    PrintBoughtItems(&(PurchaseData.BoughtList));
    printf("\n\n");

    PurchaseData.TechItemList.Empty();
    PurchaseData.CanBuyList.Empty();
    PurchaseData.BoughtList.Empty();

    return(fSuccess);
}


VOID CheckForUnboughtItems()
{
    CTechItem * pItem;

    while(NULL != (pItem = gItemList.Dequeue()))
    {
        if (FALSE == pItem->WasEverBought())
        {
            printf("Warning: Never bought %s (%s).\n",
                pItem->GetName(), pItem->GetObjectTypeName());
        }

        delete pItem;
    }
}


VOID AutoProcess()
{
    CCivTechItem * pCiv;
    DWORD dwIndex;

    for(dwIndex = gCivList.Length(); dwIndex > 0; dwIndex--)
    {
        pCiv = (CCivTechItem *) gCivList.Dequeue();
        AutoProcessCiv(pCiv);
        gCivList.Enqueue(pCiv);
    }
    
    CheckForUnboughtItems();
}


CCivTechItem * PickCiv()
{
    DWORD dwIndex, dwLoop, dwTemp;
    CCivTechItem * pCiv, * pRet;
    CHAR chCommand;

    //
    // Pick a civ, quit.
    //
    printf("Available Factions:\n");
    printf("------------------------\n");

    for(dwLoop = gCivList.Length(), dwIndex = 0; dwLoop > 0;
        dwLoop--, dwIndex++)
    {
        pCiv = (CCivTechItem *) gCivList.Dequeue();
        printf("%d) %s\n", dwIndex + 1, pCiv->GetName());
        gCivList.Enqueue(pCiv);
    }

    printf("\n");
    pRet = NULL;

    printf("Choose a faction or (Q)uit:\n> ");
    GetLine("Qq", gCivList.Length(), &chCommand, &dwTemp);

    if ('\0' == chCommand)
    {
        for(dwLoop = gCivList.Length(), dwIndex = 1; dwLoop > 0;
            dwLoop--, dwIndex++)
        {
            pCiv = (CCivTechItem *) gCivList.Dequeue();
            if (dwIndex == dwTemp)
                pRet = pCiv;
            gCivList.Enqueue(pCiv);
        }
    }

    return(pRet);
}


VOID InteractiveProcess()
{
    PURCHASE_DATA PurchaseData;
    BOOL fDone, fDoneCiv;
    CCivTechItem * pCiv;
    CHAR chCommand;
    DWORD dwTemp;

    fDone = FALSE;

    do
    {
        pCiv = PickCiv();
        if (NULL != pCiv)
        {
            PurchaseData.ttbmCurrent.ClearAll();
            PurchaseData.dwLastCanBuyLength = 0;

            //
            // Initialize our lists.
            //
            if (TRUE == gItemList.CopyTo(&(PurchaseData.TechItemList)))
            {
                fDoneCiv = FALSE;
                pCiv->Purchase(&PurchaseData);

                do
                {
                    ClearScreen();

                    printf("====================\n");
                    printf("%s\n", pCiv->GetName());
                    printf("====================\n");
                    printf("\n");

                    PrintBoughtItems(&(PurchaseData.BoughtList));

                    //
                    // Print out the list of options.
                    //
                    PrintBuyOptions(&(PurchaseData.CanBuyList),
                                    PurchaseData.dwLastCanBuyLength);
                    PurchaseData.dwLastCanBuyLength =
                                    PurchaseData.CanBuyList.Length();

                    //
                    // Get our choice and act on it.
                    //
                    printf("Pick a number, (C)hange Faction or (Q)uit:\n> ");
                    GetLine("qQcC",
                            PurchaseData.CanBuyList.Length(),
                            &chCommand,
                            &dwTemp);

                    switch(chCommand)
                    {
                        case 'c':
                        case 'C':
                            fDoneCiv = TRUE;
                            break;
                        case 'q':
                        case 'Q':
                            fDone = TRUE;
                            break;
                        default:
                            BuyItem(dwTemp, &PurchaseData);
                            break;
                    }
                } while((FALSE == fDoneCiv) && (FALSE == fDone));

                //
                // Clean up our lists.
                //
                PurchaseData.TechItemList.Empty();
                PurchaseData.CanBuyList.Empty();
                PurchaseData.BoughtList.Empty();

                if (FALSE == fDone)
                    ClearScreen();
            }
        } else
            fDone = TRUE;
    } while(FALSE == fDone);

}


VOID Usage(CHAR * argv0)
{
    printf("Usage: %s [/a]\n", argv0);
    printf("/a = autoparse civ trees\n");
}


int _cdecl main(int argc, char * argv[])
{
    BOOL fAutoProcess;
    BOOL fUsage;
    INT iIndex;
    HRESULT hr;

    fAutoProcess = FALSE;
    fUsage = FALSE;
    for(iIndex = 1; iIndex < argc; iIndex++)
    {
        if (0 == strcmp(argv[iIndex], "/a"))
            fAutoProcess = TRUE;
        else
            fUsage = TRUE;
    }

    if (FALSE == fUsage)
    {
        if (FALSE == fAutoProcess)
            ClearScreen();

        hr = Initialize("Federation", &gCivList, &gItemList);
        if (SUCCEEDED(hr))
        {
            if (TRUE == fAutoProcess)
                AutoProcess();
            else
                InteractiveProcess();
            Terminate(&gCivList, &gItemList);
        }
    } else
        Usage(argv[0]);

    return(0);
}



