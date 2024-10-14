nb: this my own version of the code. The official community code is at https://github.com/FreeAllegiance/Allegiance 

update from october 2024: full port to CMake
# Installation

## Requirements
* CMake 3.5 or later
* Ninja (can work without but not tested)
* C++ compiler for Windows (only tested with VS Studio 2022)

## building
* with Visual Studio Code:
    * create a the file `.vscode/settings.json` with `{cmake.generator": "Ninja"}` in it.
    * launch VSCode and install the CMake extension - open the source folder and build in the CMake left side panel
* command line:
    * ninja: `cmake -DCMAKE_BUILD_TYPE:STRING=Debug -S . -B ./build -G Ninja`


# Main projects (binaries)

* Allegiance (`WinTrek` folder) : the main client program
* Server (`FedSrv` folder) : the game server
* Lobby (`lobby` folder) : the game lobby

# Sub projects
* IGC (`Igc` floder): game logic library
* `zlib`: various low level libraries used by all projects
* `engine`: 3d gfx engine
* `effect`: fx engine above `engine`
* `clintlib`: client library
* `utility` (nb: folder name is _Utility) : various high level libraries used by the main projects (notably: network code and collision detection)
* `soundengine`: sound engine
* `training`: single player training missions 

# Obsolete (or about to be) projects
* AGC
* AllSrvUI
* AutoUpdate
* cvh
* mdlc
* reloader

# Links
* Forums: https://www.freeallegiance.org/forums/

* Discord : https://discord.gg/TXGmynB

* Kage Code Documentation: http://www.freeallegiance.org/FAW/index.php/Code_documentation

* Dropbox build instructions: https://paper.dropbox.com/doc/FreeAllegiance-build-instructions-LVrOqhj4PnD0UWkohmxmW

* Trello Bug Reports/Feature Request: https://trello.com/b/0ApAZt6p/steam-release-testing-dx9
