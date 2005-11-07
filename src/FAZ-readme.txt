FAZ - Free Allegiance Zone

version 1.0.0.6

Database
--------

 update (to do in that order):
	- 'profile' database - src\database\profile.sql - create a db named 'profile' and run this sql script
	- Fed database update for 'profile' - src\database\update-federation.sql - run this on the 'federation' db
	- Fed database fixes - src\database\fix-federation.sql - run this on the 'federation' db
	- A+/DN factions - src\database\add-new-factions.sql - run this on the 'federation' db

Sources
-------
Fixes/Changes:

- Integrate Authentication. Source/include of external dll not provided for security reasons.
- Temporary multicore support by using 'DefaultGames' system (Core1,Core2,... registry) and 'core game' icons on clients. See src\FedSrv\FedSrv.CPP(7434).
- Screen resolution changes removed on loadout screen.
- "#start": "force start" command for game owner added. beta feature only.(define FAZBETA)
- added 'Core name", 'Map connectivity' and 'Total Money' in esc-i (game info) screen. (bug #74).

Artwork (see FAZ autoupdate for files)
-------

- updated introscreen.mdl for AZ ui
- updated mdl/bmp for player added factions (GT, Dreg, TF).
- new main backgrounds bmps by Pook.

