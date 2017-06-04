# Installation

Download and install the lastest "Visual Studio Community Edition" : https://www.visualstudio.com/downloads/

Download and install the latest Microsoft DirectX SDK (jun 2010): https://www.microsoft.com/en-us/download/details.aspx?id=6812 (if you don't want to install it, just extract the DirectX `include` and `lib` folders somewhere and set the environment variable `DXSDK_DIR` to their parent directory)

Launch Visual Studio and open the main solution `src\VS2017\Allegiance.sln`

# Main projects

* Allegiance (`WinTrek` folder) : the main client program
* Server (`FedSrv` folder) : the game server
* Lobby (`lobby` folder) : the game lobby
* IGC (`Igc` floder): game logic library

# Sub projects
* `zlib`: various low level libraries used by all projects
* `engine`: 3d gfx engine
* `effect`: fx engine above `engine`
* `clintlib`: client library
* `utility` (nb: folder name is _Utility) : various high level libraries used by the main projects (notably: network code and collision detection)
* `soundengine`: sound engine
* `training`: single player training missions 

# Obsolete (or about to) projects
* AGC
* AllSrvUI
* AutoUpdate
* cvh
* mdlc
* pigs
* reloader