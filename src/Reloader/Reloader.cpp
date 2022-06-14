
/*-------------------------------------------------------------------------
 * Reloader.cpp
 *-------------------------------------------------------------------------
 *
 * This app does this:
 *
 * 1) Waits for Allegiance to exit.
 * 2) Looks in the regristy for ArtPath.
 * 3) Moves Art Files from Temp folder to Art folder.
 *    Moves Special files (Allegiance.exe and Allegiance.pdb) to current folder. (If they 
 *    exist in temp folder.)
 * 4) Reloads Allegiance.exe with the command-line specified to this app.
 *
 *
 * The command-line has two parts.  The first part is a number specifying the Process ID of 
 * the currently running Allegiance.exe.  The second part is the orignial command-line sent
 * to Allegiance.exe which is re-fed to the newly spawned Allegiance.exe.
 */



#include "pch.h"

#include "..\Zlib\zassert.h" //Imago 6/10
#include "..\Zlib\FTPSession.h"
#include "..\Clintlib\AutoDownload.h" // don't included in pch because build process doesn't realize
                                      // when AutoDownload.h changes, and since this project 
                                      // only has one cpp file, a pch doesn't help anyway (except 
                                      // that our build process requires it).



/*-------------------------------------------------------------------------
 * GetArtPath()
 *-------------------------------------------------------------------------
 * Returns:
 *      Art Path of client as specified in registry
 */
char * GetArtPath()
{
    static char pathStr[MAX_PATH + 16];

    HKEY hKey;
    DWORD dwType;
    DWORD cbValue = MAX_PATH;

    BOOL bResult = FALSE;

    if (ERROR_SUCCESS == ::RegOpenKeyEx(HKEY_CURRENT_USER, ALLEGIANCE_REGISTRY_KEY_ROOT, 0, KEY_READ, &hKey))
    {

        // Get the art path from the registry
        bResult = (ERROR_SUCCESS == ::RegQueryValueEx(hKey, "ArtPath", NULL, &dwType, (unsigned char*)&pathStr, &cbValue));

        ::RegCloseKey(hKey);

        int const cLen = strlen(pathStr);
        // convert to \ format
        for (int i = 0; i < cLen; ++i)
            if(pathStr[i] == '/')
                pathStr[i] = '\\';
        // assure last char has end
        if (cLen == 0 || pathStr[cLen-1] != '\\')
            strcat(pathStr, "\\");
    }

    if (!bResult) 
    {
//        ::MessageBox(NULL, "Error reading ArtPath from registry; assuming .\\Artwork\\ if this isn't right, you might need to reinstall.", "AutoUpdate Reloader Error", MB_ICONEXCLAMATION);

        strcpy(pathStr, ".\\Artwork\\");
    }

    return pathStr;
}




/*-------------------------------------------------------------------------
 * DisplayErrorMsg()
 *-------------------------------------------------------------------------
 * Purpose:
 *      report an error
 */
void DisplayErrorMsg(char *szError)
{
    int nErrorCode = GetLastError();

    // Give time for Allegiance to minimize
    ::Sleep(1000);

    char sz[100];
    sprintf(sz, "Error occured during auto update (Code = %d)", nErrorCode);

    ::MessageBox(NULL, szError, sz, MB_ICONEXCLAMATION);
}



class CAutoDownloadSink : public IAutoUpdateSink
{
  /*-------------------------------------------------------------------------
   * OnMoveError()
   *-------------------------------------------------------------------------
   * Purpose:
   *      Handles MoveFiles() Errors.  Give user option to try again.
   *
   * Returns: true if the user wants to try again.
   */
  bool OnMoveError(char *szErrorMessage)
  {
      static int cCalled = 0;

      //
      // The first few time this function is called, we ignore it and keep
      // trying to move the files.  It's possbile that Allegiance.exe isn't
      // done exiting and has some files open.
      //
      if (++cCalled > 25) // 25 secs
      {
          //
          // Give User the option to wait longer
          //
          int nErrorCode = GetLastError();

          // Give time for Allegiance to minimize
          ::Sleep(1000);

          char szTitle[100];
          sprintf(szTitle, "Error occured during auto update (Code = %d)", nErrorCode);

          char szMsg[500];

          sprintf(szMsg, "%s\r\n\r\n\r\n\r\nDo you wish to retry moving the files?", szErrorMessage);

          if (::MessageBox(NULL, szMsg, szTitle, MB_YESNO) == IDYES)
              return true;
          else
              return false;
      }
      else
      {
          Sleep(1000); // wait a bit for Allegiance.exe's files to close and try again.

          return true; // signal to try again
      }
  }
} g_AutoDownloadSink;


// returns true only if file exists
bool Exists(const char * szFileName)
{
        HANDLE hFile = CreateFile(szFileName, 
                                 0, 
                                 FILE_SHARE_READ, 
                                 NULL, 
                                 OPEN_EXISTING, 
                                 FILE_ATTRIBUTE_NORMAL, 
                                 NULL);

        if (hFile == INVALID_HANDLE_VALUE)
            return false;

        ::CloseHandle(hFile);

        return true;
}



bool GetFileTime(const char * szFileName, LPFILETIME pft)
{
    HANDLE hFile = CreateFile(szFileName, 
                           0, 
                           FILE_SHARE_READ, 
                           NULL, 
                           OPEN_EXISTING, 
                           FILE_ATTRIBUTE_NORMAL, 
                           NULL);

    if (hFile == INVALID_HANDLE_VALUE)
        return false;

    GetFileTime(hFile, NULL, NULL, pft);

    CloseHandle(hFile);

    return true;
}



//
// Rename mangled name to correct name
//
void RenameMangledFile(char * szMangled, const char * szCorrect)
{
        if (Exists(szMangled))
        {
            // force another autoupdate to ensure version is correct
            DeleteFile("Filelist.txt"); 
            DeleteFile("AutoUpdate\\Filelist.txt"); 

            char szBuffer[MAX_PATH*2 + 20];

            if (Exists(szCorrect))
            {
                FILETIME ftMangled;
                FILETIME ftCorrect;

                if (!GetFileTime(szMangled, &ftMangled) || 
                    !GetFileTime(szCorrect, &ftCorrect))
                {
                    sprintf(szBuffer, "Unable to examine at least one of these files: %s, %s.  Reboot your computer and try again.", szMangled, szCorrect);
                    DisplayErrorMsg(szBuffer);
                    ExitProcess(-1);
                    return;
                }
                
                if (CompareFileTime(&ftCorrect, &ftMangled) > 0) // if existing correct file is newer; keep it
                {
                    OutputDebugString("\nDeleting old file with mangled name ");
                    OutputDebugString(szMangled);
                    DeleteFile(szMangled);
                    return;
                }

                OutputDebugString("\nDeleting old file with correct name ");
                OutputDebugString(szCorrect);
                if (!DeleteFile(szCorrect))
                {
                    ::Sleep(2000);
                    if (!DeleteFile(szCorrect))
                    {
                        ::Sleep(10000);
                        DeleteFile(szCorrect);
                    }
                }
            }

            sprintf(szBuffer, "\nRenaming %s to %s\n", szMangled, szCorrect);
            OutputDebugString(szBuffer);
            if (!MoveFile(szMangled, szCorrect))
            {
                sprintf(szBuffer, "Unable to rename %s to %s.  Reboot your computer and run Allegiance to try again.", szMangled, szCorrect);
                DisplayErrorMsg(szBuffer);
                ExitProcess(-1);
            }
        }
}


/*-------------------------------------------------------------------------
 * RenameMangledFiles()
 *-------------------------------------------------------------------------
 *
 *  Rename files with mangled filenames as part of the PC-Gamer/Beta1 fix.
 *
 */
void RenameMangledFiles()
{
    for (int i = 0; i < g_cEXEFiles; ++i)
    {
        char szMangled[MAX_PATH];
        const char * szCorrect = CAutoDownloadUtil::GetEXEFileName(i);

        // we can only handle string of at least 8 characters
        if (strlen(szCorrect) < 8) 
            continue;
        //
        // create the mangled counterpart to GetEXEFileName(i)
        //
        strcpy(szMangled, szCorrect);
        strcpy(szMangled+8, szCorrect);

        RenameMangledFile(szMangled, szCorrect);
    }
    //
    // Now do special files
    //
    RenameMangledFile("AllegianAllegiance.exe", "Allegiance.exe");
    RenameMangledFile("AllegianAllegianceDebug.exe", "Allegiance.exe");
    RenameMangledFile("AllegianAllegianceRetail.exe", "Allegiance.exe");
    RenameMangledFile("AllegianAllegianceTest.exe", "Allegiance.exe");

    RenameMangledFile("AllegianAllegiance.pdb", "Allegiance.pdb");
    RenameMangledFile("AllegianAllegianceDebug.pdb", "Allegiance.pdb");
    RenameMangledFile("AllegianAllegianceRetail.pdb", "Allegiance.pdb");
    RenameMangledFile("AllegianAllegianceTest.pdb", "Allegiance.pdb");

    RenameMangledFile("AllegianAllegiance.sym", "Allegiance.sym");
    RenameMangledFile("AllegianAllegianceDebug.sym", "Allegiance.sym");
    RenameMangledFile("AllegianAllegianceRetail.sym", "Allegiance.sym");
    RenameMangledFile("AllegianAllegianceTest.sym", "Allegiance.sym");

    RenameMangledFile("AllegianAllegiance.map", "Allegiance.map");
    RenameMangledFile("AllegianAllegianceDebug.map", "Allegiance.map");
    RenameMangledFile("AllegianAllegianceRetail.map", "Allegiance.map");
    RenameMangledFile("AllegianAllegianceTest.map", "Allegiance.map");
}



/*-------------------------------------------------------------------------
 * WinMain()
 *-------------------------------------------------------------------------
 *
 * See top of file for description.
 *
 */
int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
    // make sure this is commmented out
//    DebugBreak();

    RenameMangledFiles(); // PC-Gamer/Beta 1 build fix

    char *pCmdLine = lpCmdLine;

    DWORD dwProcessID = atol(pCmdLine);
    HANDLE hProcess;

    if(dwProcessID == 0)
    {
        DisplayErrorMsg("Allegiance.exe gave Reloader.exe an invalid command-line.");
        return 1;
    }

    char * szArtPath = GetArtPath();

    hProcess = ::OpenProcess(PROCESS_TERMINATE, false, dwProcessID);

    if (hProcess == NULL)
    {
        if (GetLastError() != ERROR_INVALID_PARAMETER) // returns this if Process of this ID doesn't exist
        {
            DisplayErrorMsg("Couldn't Access Allegiance.exe Process.  Press OK to try continue update anyway.");
        }
        else
        {
            //
            //  We are assuming that Allegiance is already killed itself. So everything is
            //  okay and just fall through.
            //
        }
    }
    else
    {
        //
        //  Give Allegiance some time to exit
        //
        int counter = 0;
                                  
        while (hProcess && counter++ < 40) // wait about 4 seconds (don't make too high!)
        {
            Sleep(100);

            ::CloseHandle(hProcess);

            // Note: win98 returns a non NULL handle even if the process is dead
            hProcess = ::OpenProcess(PROCESS_ALL_ACCESS, false, dwProcessID);
        }

        if (hProcess)
        {
            //
            // Took too long to shut down!
            // Let's terminate Allegiance (to be sure it shuts down)
            //
            if (::TerminateProcess(hProcess, 0) == 0) 
            {
                DisplayErrorMsg("Couldn't Terminate Allegiance.  Press OK to try continue update anyway.");
            }

            ::Sleep(1000); // give some time for terminatation
        }
    }

    if(hProcess)
    ::CloseHandle(hProcess);

    //
    //  At this point Allegiance is dead.
    //

    ::Sleep(1000); // we wait a little more to help ensure Allegiance is completely dead.


    if (!CAutoDownloadUtil::MoveFiles("AutoUpdate\\", szArtPath, false, false, NULL, &g_AutoDownloadSink))
    {
        DisplayErrorMsg("Couldn't move at least one of the downloaded files.  Reboot and try again.  As a last resort, you may need to reinstall.");
        return 3;
    }

    //
    //  Reload Allegiance
    //
    if (pCmdLine && pCmdLine[0] != 0)
        pCmdLine = strchr(pCmdLine+1, ' '); // fast forward a bit

    bool bLaunchMinimized = false;

    if (pCmdLine && pCmdLine[0] != 0)
    {
        if (_strnicmp(pCmdLine, "-Minimized", 10))
        {
            bLaunchMinimized = true;
        }

        pCmdLine = strchr(pCmdLine+1, ' '); // fast forward a bit
    }

    char szNewCommandLine[512] = {""};

    if (pCmdLine && pCmdLine[0] != 0)
        strncpy(szNewCommandLine, pCmdLine, 500);   // re-feed the original command-line back into Allegiance.exe

    strcat(szNewCommandLine, " -reloaded");

    if((int)ShellExecute(0, 
                         "Open", 
                         "Allegiance.exe",
                         szNewCommandLine, 
                         NULL,
                         bLaunchMinimized ? SW_SHOWNOACTIVATE : SW_SHOWNORMAL
                         ) <= 32)
    {
        DisplayErrorMsg("Couldn't automatically restart Allegiance.  Please manually restart the game.");
        return 5;
    }

	return 0;
}



