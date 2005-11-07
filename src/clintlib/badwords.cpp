#include "pch.h"
#include "badwords.h"

// note, this became much more complex thatn I have time for, so I'm bailing out 
// and using a cheeseball algorithm that is probably on the order of n^2 for the
// text to be filtered. If it looks bad on perf, then I'll continue to develop
// the correct approach

/*
//------------------------------------------------------------------------------
// avoid polluting the global namespaces
//------------------------------------------------------------------------------
namespace FilterAutomata
{

    //--------------------------------------------------------------------------
    // valid action codes
    //--------------------------------------------------------------------------
    enum ActionCode
    {
        ACTION_DO_NOTHING,
        ACTION_STORE_CURRENT_POINTER,
        ACTION_WIPE_CURRENT_RANGE
    };

    //--------------------------------------------------------------------------
    // a cell in a state
    //--------------------------------------------------------------------------
    struct   Cell
    {
        ActionCode  actionCode;
        short       iNextState;

        Cell (void) : actionCode (ACTION_DO_NOTHING), iNextState (0) {}

        operator = (const Cell& cell)
        {
            actionCode = cell.actionCode;
            iNextState = cell.iNextState;
        }
    };

    //--------------------------------------------------------------------------
    // a state
    //--------------------------------------------------------------------------
    struct  State
    {
        Cell        cell[256];

        operator = (const State& state)
        {
            for (int i = 0; i < 256; i++)
                cell[i] = state.cell[i];
        }
    };

    //--------------------------------------------------------------------------
    // the actual finite state machine
    //--------------------------------------------------------------------------
    struct   Machine
    {
        private:
            State*          pState;
            unsigned int    iStateCount;
            unsigned int    iNextState;

        public:
            Machine (void) : iStateCount (16), iNextState (0), pState (new State[iStateCount]) {}

            State*          GetState (unsigned int iState)
            {
                while (iNextState >= iStateCount)
                {
                    // automatically double the size of the table each time we go over what we currently have
                    int     iNewStateCount = iStateCount * 2;
                    State*  pNewState = new State[iNewStateCount];
                    for (int i = 0; i < iStateCount; i++)
                        pNewState[i] = pState[i];
                    iStateCount = iNewStateCount;
                    delete[] pState;
                    pState = pNewState;
                    debugf ("Doubled FSM table size to %d\n", iStateCount);
                }
                return pState + iState;
            }

            unsigned int    GetNextState (void)
            {
                return iNextState++;
            }
    };

    //--------------------------------------------------------------------------
};

using FilterAutomata;

//------------------------------------------------------------------------------
// global variable
//------------------------------------------------------------------------------
Machine*    g_pFilterAutomata = 0;

//------------------------------------------------------------------------------
// function to add a word to the automata
//------------------------------------------------------------------------------
void    AddWordToFilterAutomata (char* pDirtyWord)
{
    // print out the word to confirm
    debugf ("Adding Dirty Word (%s)...", pDirtyWord);

    if (!g_pFilterAutomata)
        g_pFilterAutomata = new Machine;

    // first, run through the machine until we reach a point where we have raw data
    int iStartState = 0;
    State*  pState = g_pFilterAutomata->GetState (iStartState);
    while (*pDirtyWord && pState->cell[*pDirtyWord].nextState != 0)
    {
        pState = g_pFilterAutomata->GetState (pState->cell[*pDirtyWord].nextState);
        pDirtyWord++;
    }

    if (*pDirtyWord)
    {
        // here we have exhausted the word, so we should fill
    }
    else
    {
    }

    debugf ("done\n");
}

*/
//------------------------------------------------------------------------------
// whether or not to censor
//------------------------------------------------------------------------------
bool    g_bCensor = true;

//------------------------------------------------------------------------------
// bad word storage
//------------------------------------------------------------------------------
char**  g_pBadWordList = 0;         // this has a memory leak, but it is only one per app, so it shouldn't be a real problem
int     g_iBadWordListSize = 0;
int     g_iBadWordListMaxSize = 0;

//------------------------------------------------------------------------------
// bad word table management
//------------------------------------------------------------------------------
void    AddWord (char* pDirtyWord)
{
    // initialize the table if it hasn't already been initialized
    if (g_iBadWordListMaxSize == 0)
    {
        g_iBadWordListMaxSize = 8;
        g_pBadWordList = new char*[g_iBadWordListMaxSize];
    }

    // grow the table if necessary
    while (g_iBadWordListSize >= g_iBadWordListMaxSize)
    {
        int     iNewSize = g_iBadWordListMaxSize * 2;
        char**  pNewBadWordList = new char*[iNewSize];
        for (int i = 0; i < g_iBadWordListMaxSize; i++)
            pNewBadWordList[i] = g_pBadWordList[i];
        delete[] g_pBadWordList;
        g_iBadWordListMaxSize = iNewSize;
        g_pBadWordList = pNewBadWordList;
    }

    // store the word in the table
    g_pBadWordList[g_iBadWordListSize++] = pDirtyWord;
}

//------------------------------------------------------------------------------
// bad word table sort function
//------------------------------------------------------------------------------
int sortfunc (const void* a, const void* b)
{
   return strlen (*(const char**)(b)) - strlen(*(const char**)(a));
}

//------------------------------------------------------------------------------
// abbreviations for important constants
//------------------------------------------------------------------------------
#define ENDWORD         0x0d
#define ENDWORDLIST     0x20

//------------------------------------------------------------------------------
// function to build the automata that will be used to do the filtering
//------------------------------------------------------------------------------
void    BuildFilterAutomata (char* pBuffer)
{
    // skip to the end of the zone header
    while (*pBuffer != ENDWORD)
        pBuffer++;

    // now skip to the beginning of the first dirty word
    pBuffer += 2;

    // now, for each word in the list, install it into the table
    while (*pBuffer != ENDWORDLIST)
    {
        // save the pointer to the beginning of the current word
        char*   pDirtyWord = pBuffer;

        // advance the pointer to the end of the word
        while (*pBuffer != ENDWORD)
            pBuffer++;

        // set the null on the end of the word and advance to the beginning
        // of the next dirty word (skip the newline after the carriage return)
        *pBuffer++ = 0;
        pBuffer++;

        // add the word to the automata
        //AddWordToFilterAutomata (pDirtyWord);
        AddWord (pDirtyWord);

    }

    // now sort the list by length
    qsort (g_pBadWordList, g_iBadWordListSize, sizeof (char*), sortfunc);
}

//------------------------------------------------------------------------------
// table to look up lower case conversions
//------------------------------------------------------------------------------
//                            "\0........\t\n..\r.................. !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~................................................................................................................................."
char* gszConvertToLowerCase = "\0........\t\n..\r.................. !\"#$%&'()*+,-./0123456789:;<=>?@abcdefghijklmnopqrstuvwxyz[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~.................................................................................................................................";

//------------------------------------------------------------------------------
// resource id and other defines
//------------------------------------------------------------------------------
#define IDR_BADWORDS    1123
#define BADWORDS_DLL_NAME   "allbad"
#define DLL_SUFFIX   ".dll"

//------------------------------------------------------------------------------
// function to load the bad words
//------------------------------------------------------------------------------
void    LoadBadWords (void)
{
    // load the bad words resource dll from the artwork directory
    char        szFilename[MAX_PATH + 1];
    HRESULT     hr = UTL::getFile (BADWORDS_DLL_NAME, DLL_SUFFIX, szFilename, false, false);
    HMODULE     hModule = 0;
    if (hr == S_OK)
    {
        debugf ("Attempting to load %s\n", szFilename);
	    hModule = LoadLibrary (szFilename);
    }

    // load from anywhere 
    if (!hModule)
    {
        debugf ("Attempting to load " BADWORDS_DLL_NAME DLL_SUFFIX "\n");
	    hModule = LoadLibrary (BADWORDS_DLL_NAME DLL_SUFFIX);
    }

    // if the load worked, proceed normally
	if (hModule)
    {
        debugf ("Load of " BADWORDS_DLL_NAME " succeeded, bad word filtering will be enabled\n");

        // find the binary resource containing the scrambled data
	    HRSRC   hResource = FindResource (hModule, MAKEINTRESOURCE(IDR_BADWORDS), "BINARY");
        if (hResource)
        {
            // get the size of that resource so we can allocate a buffer for it
            int     iSize = SizeofResource (hModule, hResource);
            if (iSize > 0)
            {
                // load the resource into memory
                HGLOBAL hResourceData = LoadResource (hModule, hResource);
                if (hResourceData)
                {
                    // lock the resource memory so we can copy it
                    void*   pResourceData = LockResource (hResourceData);
                    if (pResourceData)
                    {
                        // allocate the buffer for our own copy of the data
                        char*   pBuffer = new char[iSize];
                        if (pBuffer)
                        {
                            // copy the data
                            CopyMemory (pBuffer, pResourceData, iSize);

                            // it's encrypted in a really dumb way, so decode it, and convert it all to lower case
                            for (int i = 0; i < iSize; i++)
                            {
                                char   iCharacter = pBuffer[i] ^ 0xcd;
                                pBuffer[i] = gszConvertToLowerCase[iCharacter];
                            }

                            // build the filter automata
                            BuildFilterAutomata (pBuffer);

                            // release the buffer
                            // actually, don't because the pointers are stored directly in the table now. Yes, this is a memory leak on a global scale.
                            //delete[] pBuffer;
                        }
                    }
                }
            }
        }
        FreeLibrary (hModule);
    }
    else
        debugf ("FAILED TO LOAD " BADWORDS_DLL_NAME ", bad word filtering will be disabled\n");
}

//------------------------------------------------------------------------------
// function to filter the bad words
//------------------------------------------------------------------------------
void    FilterBadWords (char* szString)
{
    // this algorithm absolutely stinks. There's all sorts of holes in it. It
    // just deserves to die horribly, but I have no time to do anything
    // better.

    // this is the set of characters we will use to obscure bad words
    char*    random = "*&^%$#@!!@#$%^&*@$^*!#%&&%#!*^$@";

    // copy the string and make it totally lowercase
    char*   szLowerCopy = new char[strlen (szString) + 1];
    strcpy (szLowerCopy, szString);
    char*   tmp = szLowerCopy;
    while (*tmp)
    {
        *tmp = gszConvertToLowerCase[*tmp];
        tmp++;
    }

    // loop over all of the bad words and look for them in the string
    for (int i = 0; i < g_iBadWordListSize; i++)
    {
        // perform the search on the lower case copy
        char*   szFound = strstr (szLowerCopy, g_pBadWordList[i]);
        while (szFound)
        {
            // we found a match, so figure the offset and copy the 
            char*   cpy = g_pBadWordList[i];
            tmp = szString + (szFound - szLowerCopy);
            while (*cpy)
            {
                // we have to make the changes to the original and the copy
                // so that we don't end up in an infinite loop
                char    exclamation = random[(*cpy++) % 32];
                *tmp++ = exclamation;
                *szFound++ = exclamation;
            }

            // now search again, in case there are more instances of the word
            szFound = strstr (szLowerCopy, g_pBadWordList[i]);
        }
    }

    // delete the buffer
    delete[] szLowerCopy;
}

//------------------------------------------------------------------------------
// censor a zstring, returning a new one
//------------------------------------------------------------------------------
ZString CensorBadWords (const ZString& string)
{
    if (g_bCensor)
    {
        char*       szBuffer = new char[string.GetLength () + 1];
        strcpy (szBuffer, string);
        FilterBadWords (szBuffer);
        ZString     result (szBuffer);
        delete[] szBuffer;
        return result;
    }
    else
        return string;
}

//------------------------------------------------------------------------------
// toggle the censor flag
//------------------------------------------------------------------------------
void    ToggleCensorDisplay (void)
{
    g_bCensor = ! g_bCensor;
}

//------------------------------------------------------------------------------
// get the censor flag value
//------------------------------------------------------------------------------
bool    CensorDisplay (void)
{
    return g_bCensor;
}

//------------------------------------------------------------------------------
