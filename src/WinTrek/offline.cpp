
#include "pch.h"
#include "offline.h"

#if 0
VOID LoadDefaultMap(ImissionIGC * pCore, bool fSwarm, SideID sideID)
{
    Time now = pCore->GetLastUpdate();

    /*
    {
        DataClusterIGC  c1;

        c1.starSeed = 3682;
        c1.lightDirection.z = c1.lightDirection.y = 0.0f;
        c1.lightDirection.x = 1.0f;
        c1.lightColor = 0x00ffffff;

        for (SectorID i = 0; (i < 3); i++)
        {
            c1.clusterID = 25 + i;
            const char* names[3] = {"Goron.IL", "L'Sau.CW", "Nadir"};
            strcpy(c1.name, names[i]);
            c1.nDebris = 500 + 100 * i;
            c1.nStars = 1000 - 200 * i;
            c1.screenX = 0.5f * cos((pi / 1.5f) * (float)i);
            c1.screenY = 0.5f * sin((pi / 1.5f) * (float)i);

            IclusterIGC* s = (IclusterIGC*)pCore->CreateObject(now, OT_cluster,
                                                  &c1, sizeof(c1));
            s->SetActive(true);
            s->Release();
        }
    }
    {
        for (SectorID i = 25; (i <= 27); i++)
        {
            IclusterIGC*    cluster = pCore->GetCluster(i);
            assert (cluster);

            for (int j = 0; (j < 3); j++)
            {
                float   latr = ((float)(j * j)) * 45.0f * pi / 180.0f;
                float   lonr = ((float)j) * 45.0f * pi / 180.0f;

                float   cosLatr = cos(latr);

                Vector  position(cosLatr * cos(lonr), cosLatr * sin(lonr), sin(latr));

                const char* names[] = {"f05", "nebplnt02", "neb09",
                                       "f05", "nebplnt01", "neb10",
                                       "f05", "nebplnt35", "neb07"};

                cluster->GetClusterSite()->AddPoster(names[(i - 25) * 3 + j], position, (float)(j * j + 2) * 40.0f);
            }
        }
    }

    {
        DataAsteroidIGC   p1;

        for (AsteroidID i = 0; (i <= 44); i++)
        {
            p1.asteroidDef.asteroidID = i;
            strcpy(p1.asteroidDef.modelName, i % 2 == 0
                                           ? "bgrnd05"
                                           : "bgrnd03");

            p1.asteroidDef.textureName[0] = '\0';
            p1.asteroidDef.radius = (short)randomInt(50, 105);
            p1.clusterID = 25 + i % 3;
            p1.name[0] = '\0';

            {
                float yaw = random(0, 2 * pi);
                float sinPitch = random(-1, 1);
                float cosPitch = sqrt(1.0f - sinPitch * sinPitch);

                float   r = random(200.0f, 3000.0f);
                p1.position.x = r * cosPitch * cos(yaw);
                p1.position.y = r * cosPitch * sin(yaw);
                p1.position.z = r * sinPitch;
            }

            {
                //Parts get a random orientation
                float yaw = random(0, 2 * pi);
                float sinPitch = random(-1, 1);
                float cosPitch = sqrt(1.0f - sinPitch * sinPitch);

                Vector  v(cosPitch * cos(yaw), cosPitch * sin(yaw), sinPitch);
                Orientation o(v);
                p1.forward = o.GetForward();
                p1.up = o.GetUp();
            }
            {
                //and a random rotation
                float yaw = random(0, 2 * pi);
                float sinPitch = random(-1, 1);
                float cosPitch = sqrt(1.0f - sinPitch * sinPitch);

                Vector  v(cosPitch * cos(yaw), cosPitch * sin(yaw), sinPitch);
                Rotation    r;
                p1.rotation.axis(v);
                p1.rotation.angle(random(-0.25f, 0.25f));
            }
            p1.mass = 1000.0f;

            IObject*   u = pCore->CreateObject(now,
                                                   OT_asteroid,
                                                   &p1, sizeof(p1));
            u->Release();
        }
    }

    {
        DataWarpIGC  w1;
        w1.warpDef.warpID = 0;
        w1.warpDef.destinationID = 1;
        strcpy(w1.warpDef.textureName, "plnt02");
        w1.warpDef.radius = 100;
        w1.clusterID = 27;
        strcpy(w1.name, "L'Sau");
        w1.position.x = 1600.0f;
        w1.position.y =  500.0f;
        w1.position.z =    0.0f;
        w1.up.x = w1.up.z = w1.forward.x = w1.forward.y = 0.0f;
        w1.up.y = w1.forward.z = 1.0f;
        w1.rotation.axis(w1.forward);
        w1.rotation.angle(0.1f);

        IObject*   u = pCore->CreateObject(now,
                                               OT_warp,
                                               &w1, sizeof(w1));
        u->Release();
    }
    {
        DataWarpIGC  w1;
        w1.warpDef.warpID = 1;
        w1.warpDef.destinationID = 0;
        strcpy(w1.warpDef.textureName, "plnt19");
        w1.warpDef.radius = 100;
        w1.clusterID = 26;
        strcpy(w1.name, "Nadir");
        w1.position.x =     0.0f;
        w1.position.y = -1500.0f;
        w1.position.z =     0.0f;
        w1.up.x = w1.up.z = w1.forward.x = w1.forward.y = 0.0f;
        w1.up.y = w1.forward.z = 1.0f;
        w1.rotation.axis(w1.forward);
        w1.rotation.angle(0.1f);

        IObject*   u = pCore->CreateObject(now,
                                               OT_warp,
                                               &w1, sizeof(w1));
        u->Release();
    }
    
    {
        DataWarpIGC  w1;
        w1.warpDef.warpID = 2;
        w1.warpDef.destinationID = 3;
        strcpy(w1.warpDef.textureName, "plnt02");
        w1.warpDef.radius = 100;
        w1.clusterID = 25;
        strcpy(w1.name, "Nadir");
        w1.position.x = 2300.0f;
        w1.position.y = 2300.0f;
        w1.position.z =  800.0f;
        w1.up.x = w1.up.z = w1.forward.x = w1.forward.y = 0.0f;
        w1.up.y = w1.forward.z = 1.0f;
        w1.rotation.axis(w1.forward);
        w1.rotation.angle(0.1f);

        IObject*   u = pCore->CreateObject(now,
                                               OT_warp,
                                               &w1, sizeof(w1));
        u->Release();
    }
    {
        DataWarpIGC  w1;
        w1.warpDef.warpID = 3;
        w1.warpDef.destinationID = 2;
        strcpy(w1.warpDef.textureName, "plnt19");
        w1.warpDef.radius = 100;
        w1.clusterID = 27;
        strcpy(w1.name, "Goron");
        w1.position.x =  -500.0f;
        w1.position.y = -1600.0f;
        w1.position.z =     0.0f;
        w1.up.x = w1.up.z = w1.forward.x = w1.forward.y = 0.0f;
        w1.up.y = w1.forward.z = 1.0f;
        w1.rotation.axis(w1.forward);
        w1.rotation.angle(0.1f);

        IObject*   u = pCore->CreateObject(now,
                                               OT_warp,
                                               &w1, sizeof(w1));
        u->Release();
    }
    */

    {
        DataStationTypeIGC  st;
        for (StationTypeID i = 0; i < 3; i++)
        {
            st.stationTypeID = i;
            st.maxArmorHitPoints = 5000.0f;
            st.maxShieldHitPoints = 250.0f;
            st.armorRegeneration = 10.0f;
            st.shieldRegeneration = 5.0f;
            const char* models[] = {"ss21a", "ss90", "acs08"}; //{"acs05", "acs05", "acs05"}; 
            strcpy(st.modelName, models[i]);
            strcpy(st.iconName, "station");
            const char* names[] = {"Ordinance", "Energy", "Electronics"};
            strcpy(st.name, names[i]);
            const char* descs[] = {"Ordinance", "Energy", "Electronics"};
            strcpy(st.description, descs[i]);
            Money prices[] = {1000, 1000, 1500};
            st.price = prices[i];
            st.timeToBuild = 100;
            st.textureName[0] = '\0';
            st.radius = 200;
            st.ttbmRequired.ClearAll();
            st.ttbmEffects.ClearAll();
            st.ttbmLocal.ClearAll();
            st.signature = 2.5f;
            st.scannerRange = 2000.0f;
            st.aabmBuild = (c_aabmSpecial << 0); 
            st.exteriorSound = 380;
            st.interiorSound = 420;
            st.interiorAlertSound = 460;

            st.successorStationTypeID = /*(i == 2) ? 0 :*/ NA;
            StationClassID classes[] = { c_scOrdinance, c_scResearch, c_scElectronics };
            st.classID = classes[i];
            /*
            st.dsuUpgrade.price = 0;
            st.dsuUpgrade.stationUpgradeID = i + 100;
            strcpy(st.dsuUpgrade.modelName, "acs05");
            strcpy(st.dsuUpgrade.name, "upgrade");
            st.dsuUpgrade.description[0] = '\0';
            st.dsuUpgrade.timeToBuild = 60;
            st.dsuUpgrade.ttbmRequired.ClearAll();
            st.dsuUpgrade.ttbmEffects.ClearAll();
            */

            st.sabmCapabilities = c_sabmUnload | c_sabmStart | c_sabmRestart | c_sabmLand | c_sabmRepair | c_sabmLoadoutMenu | c_sabmRipcord | c_sabmReload;

            IObject*   u = pCore->CreateObject(now,
                                                   OT_stationType,
                                                   &st, sizeof(st));
            u->Release();
        }
    }
    {
        const struct
        {
            const char*     name;
            const char*     modelName;
            float           length;
            float           mass;
            float           speed;
            HitPoints       hitPoints;
            float           scannerRange;
            float           maxTurnRates;
            float           turnAccelerations;
            float           acceleration;
            float           starts[c_cAnimStates];
            float           stops[c_cAnimStates];
            char*           hp1FrameName;
            char            hp1MountClass;
            char*           hp2FrameName;
            char            hp2MountClass;
        }   hulls[7] = {
						//used in offline 1
                        {"Stinger", "fig02",
                         15.0f, 100.0f, 100.0f, 250, 3000.0f, 60.0f, 90.0f, 40.0f,
                         { 1.0f, 16.0f, 121.0f, 120.0f, 76.0f}, 
                         {15.0f, 75.0f, 133.0f,  91.0f, 90.0f},
                         "lwepemt1X", '\0', "wepattX",   'c'},
						//used in offline 2
                        {"BF Bomber", "bom07b",
                         20.0f, 120.0f, 89.25f, 3208, 3000.0f, 90.0f, 45.0f, 25.0,
                         { 1.0f, 16.0,  44.0f, 76.0f, 60.0f},
                         {15.0f, 43.0f, 59.0f, 91.0f, 75.0f},
                         "lwepemt",  '\0', "rwepemt",  '\0'},
						//used in offline 3
                        {"Scarab", "fig04",
                         15.0f, 50.0f, 100.0f, 250, 3000.0f, 90.0f, 270.0f, 10.0f,
                         { 1.0f, 16.0f, 57.0f, 57.0f, 57.0f},
                         {15.0f, 56.0f, 71.0f, 71.0f, 71.0f},
                         "lwepmnt1X", 'd', "rwepmnt1X", 'd'},
                        {"Fox", "fig12",
                         15.0f, 100.0f, 100.0f, 250, 1000.0f, 90.0f, 360.0f, 20.0,
                         { 1.0f, 16.0,   57.0f, 57.0f, 57.0f},
                         {15.0f, 94.0f,  62.0f, 62.0f, 62.0f},
                         "lwepattX",  'c', "rwepattX",  'c'},
                        {"Turret", "utl25",
                         10.0f, 50.0f, 50.0f, 125, 1000.0f, 60.0f, 120.0f, 5.0,
                         { 1.0f, 0.0,    1.0f, 31.0f,  91.0f},
                         {30.0f, 0.0f,  30.0f, 90.0f, 120.0f},
                         "wepemt",  '\0', "wepemt",  '\0'},
                        {"Mining", "utl19",
                         35.0f, 500.0f, 50.0f, 500, 1000.0f, 15.0f, 30.0f, 5.0,
                         { 1.0f, 0.0,    1.0f, 0.0f,  0.0f},
                         {15.0f, 0.0f,  15.0f, 0.0f,  0.0f},
                         "wepatt",  'c', "wepemt",  '\0'},
                        {"Ambulance", "fig01",
                         35.0f, 500.0f, 50.0f, 500, 1000.0f, 15.0f, 30.0f, 5.0,
                         { 1.0f, 0.0,    1.0f, 0.0f,  0.0f},
                         {15.0f, 0.0f,  15.0f, 0.0f,  0.0f},
                         "wepatt",  'c', "wepemt",  '\0'},
                        };

        //crude way of allocating space on the stack for a buffer for the extra data
        char             hbfr[sizeof(DataHullTypeIGC) + 10 * sizeof(HardpointData)];
        DataHullTypeIGC* h1 = (DataHullTypeIGC*)hbfr;
        for (HullID i = 0; (i < 7); i++)
        {
            strcpy(h1->modelName, hulls[i].modelName);
            strcpy(h1->iconName, "fighter");
            strcpy(h1->pilotHUDName, "dialog");
            strcpy(h1->observerHUDName, "dialog");
            h1->textureName[0] = '\0';
            strcpy(h1->name, hulls[i].name);
            h1->description[0] = '\0';
            h1->price = 1000;
            h1->mass = hulls[i].mass;

            h1->preferredPartsTypes[0] = NA;

            h1->maxEnergy = 2000.0f;
            h1->rechargeRate = 100.0f;

            h1->hullID = i + 1;
            h1->successorHullID = NA;
            h1->length = (short)hulls[i].length;

            h1->ttbmRequired.ClearAll();
            h1->ttbmEffects.ClearAll();

            h1->maxFuel = 100.0f;
            h1->maxAmmo = 2000;

            h1->habmCapabilities = 0;

            h1->speed = hulls[i].speed;
            {
                for (int j = 0; (j < 3); j++)
                {
                    h1->maxTurnRates[j]      = hulls[i].maxTurnRates * pi / 180.0f;
                    h1->turnTorques[j] = hulls[i].mass * hulls[i].turnAccelerations * pi / 180.0f;
                }
            }
            h1->thrust = hulls[i].mass * hulls[i].acceleration;
            h1->sideMultiplier = 1.0f;
            h1->backMultiplier = 1.0f;
            h1->signature = 1.0f;

            h1->scannerRange = hulls[i].scannerRange;
            h1->hitPoints = hulls[i].hitPoints;

            memset(h1->psEquipment, 0, sizeof(h1->psEquipment));

            h1->interiorSound = 140;
            h1->exteriorSound = 180;
            h1->mainThrusterInteriorSound = 220;
            h1->mainThrusterExteriorSound = 260;
            h1->manuveringThrusterInteriorSound = 300;
            h1->manuveringThrusterExteriorSound = 340;
            h1->maxWeapons = 4;
            h1->maxFixedWeapons = 4;
            h1->hardpointOffset = sizeof(DataHullTypeIGC);
            {
                HardpointData*  hd = (HardpointData*)(&hbfr[sizeof(DataHullTypeIGC)]);
                strcpy(hd[0].frameName, hulls[i].hp1FrameName);
                strcpy(hd[0].hudName, "dialog");

                hd[0].bFixed = true;
                hd[0].minDot = 0.5f;
                hd[0].partMask = 0;

                strcpy(hd[1].frameName, hulls[i].hp2FrameName);
                strcpy(hd[1].hudName, "dialog");

                hd[1].bFixed = true;
                hd[1].minDot = 0.5f;
                hd[1].partMask = 0;

                strcpy(hd[2].frameName, hulls[i].hp1FrameName);
                strcpy(hd[2].hudName, "dialog");

                hd[2].bFixed = true;
                hd[2].minDot = 0.5f;
                hd[2].partMask = 0;

                strcpy(hd[3].frameName, hulls[i].hp2FrameName);
                strcpy(hd[3].hudName, "dialog");

                hd[3].bFixed = true;
                hd[3].minDot = 0.5f;
                hd[3].partMask = 0;
            }

            //Create some default hull types (-4 ... -1)
            IObject*   u = pCore->CreateObject(now, OT_hullType,
                                               h1, sizeof(DataHullTypeIGC) + sizeof(HardpointData) * 4);

            u->Release();
        }
    }

    //Define a default missile and magazine
    {
        DataMissileTypeIGC  m1;

        m1.width = 1;
        m1.bDirectional = false;

        m1.modelName[0] = '\0';
        strcpy(m1.iconName, "missle");
        strcpy(m1.textureName, "f02");
        m1.radius = 2.0f;
        m1.rotation = 160.0f * pi / 180.0f;
        m1.color.r = m1.color.g = m1.color.b =
                                  m1.color.a = 0.0f;
        m1.expendabletypeID = 1;

        m1.maxLock = 1.0f;
        m1.chaffResistance = 1.0f;

        m1.acceleration = 100.0f;
        m1.turnRate = 2.0f;
        m1.initialSpeed = 10.0f; //0.0f;

        m1.lifespan = 4.0f;
        m1.armTime = 0.25f;

        m1.lockTime = 0.5f;
        m1.loadTime = 0.5f;
        m1.readyTime = 0.5f;

        m1.dispersion = 0.05f;
        m1.lockAngle = pi / 12.0f;

        m1.signature = 2.0f;
        m1.hitPoints = 0;
        m1.power = 25.0f;
        m1.blastPower = m1.blastRadius = 0.0f;
        m1.damageType = c_dtmEnergy;

        m1.launchSound         = 500;
        m1.ambientSound        = 540;

        m1.launcherDef.ttbmRequired.ClearAll();
        m1.launcherDef.ttbmEffects.ClearAll();

        m1.launcherDef.mass = 0.1f;
        m1.launcherDef.signature = 0.5f;
        m1.launcherDef.price = 5;
        strcpy(m1.launcherDef.modelName, "mis05");
        strcpy(m1.launcherDef.name, "Plasma torpedo");
        strcpy(m1.launcherDef.iconName, "missle");
        strcpy(m1.launcherDef.description, "Fast, cheap, weak");
        m1.launcherDef.partMask = 0;
        m1.launcherDef.expendableSize = 1;

        IObject*   u = pCore->CreateObject(now, OT_missileType,
                                            &m1, sizeof(m1));

        u->Release();
    }
    {
        DataLauncherTypeIGC m1;
        //Other fields are not used (data is gotten from the missile type)
        m1.partID = 7;
        m1.expendabletypeID = 1;
        m1.amount = 100;
        m1.successorPartID = NA;
        strcpy(m1.inventoryLineMDL, "invsmissile");

        IObject*   u = pCore->CreateObject(now, OT_partType,
                                            &m1, sizeof(m1));

        u->Release();
    }

    {
        DataMissileTypeIGC  m1;

        strcpy(m1.modelName, "mis12");
        strcpy(m1.iconName, "aleph");
        m1.width        = 1;
        m1.bDirectional = false;

        m1.textureName[0] = '\0';
        m1.radius = 4.0f;
        m1.rotation = 0.0f;
        m1.color.r = m1.color.g = m1.color.b =
                                  m1.color.a = 0.0f;

        m1.expendabletypeID = 2;

        m1.maxLock = 0.25f;
        m1.chaffResistance = 1.0f;

        m1.acceleration = 50.0f;
        m1.turnRate = 0.125f;
        m1.initialSpeed = 25.0f;

        m1.lifespan = 6.0f;
        m1.armTime = 0.25f;

        m1.lockTime = 5.0f;
        m1.loadTime = 2.0f;
        m1.readyTime = 0.5f;

        m1.dispersion = 0.05f;
        m1.lockAngle = pi / 24.0f;

        m1.signature = 5.0f;
        m1.hitPoints = 20;
        m1.power = 500.0f;
        m1.blastPower = 1000.0f;
        m1.blastRadius = 100.0f;
        m1.damageType = c_dtmEnergy;

        m1.launchSound         = 500;
        m1.ambientSound        = 540;

        m1.launcherDef.ttbmRequired.ClearAll();
        m1.launcherDef.ttbmEffects.ClearAll();

        m1.launcherDef.mass = 2.0f;
        m1.launcherDef.signature = 0.5f;
        m1.launcherDef.price = 50;
        strcpy(m1.launcherDef.modelName, "mis12");
        strcpy(m1.launcherDef.iconName, "missle");
        strcpy(m1.launcherDef.name, "Titan missile");
        strcpy(m1.launcherDef.description, "Big & painful");
        m1.launcherDef.partMask = 0;
        m1.launcherDef.expendableSize = 1;

        IObject*   u = pCore->CreateObject(now, OT_missileType,
                                            &m1, sizeof(m1));

        u->Release();
    }
    {
        DataLauncherTypeIGC m1;
        //Other fields are not used (data is gotten from the missile type)
        m1.partID = 8;
        m1.expendabletypeID = 2;
        m1.amount = 10;

        m1.successorPartID = NA;
        strcpy(m1.inventoryLineMDL, "invsmissile");

        IObject*   u = pCore->CreateObject(now, OT_partType,
                                            &m1, sizeof(m1));

        u->Release();
    }
    {
        DataMineTypeIGC  m1;

        strcpy(m1.modelName, "mis12");
        strcpy(m1.iconName, "missle");
        m1.textureName[0] = '\0';
        m1.radius = 2.0f;
        m1.rotation = 0.0f;
        m1.color.r = m1.color.g = m1.color.b =
                                  m1.color.a = 0.0f;

        m1.lifespan = 600.0f;
        m1.loadTime = 2.0f;
        m1.signature = 5.0f;

        m1.placementRadius = 20.0f;
        m1.hitPoints = 20;
        m1.expendabletypeID = 3;
        m1.munitionCount = 50;

        m1.munitionDef.power = 20.0f;
        m1.munitionDef.hitpoints = 0;
        m1.munitionDef.damageType = c_dtmEnergy;
        m1.munitionDef.rotation = 160.0f * pi / 180.0f;

        m1.munitionDef.modelName[0] = '\0';
        strcpy(m1.munitionDef.textureName, "f03");

        m1.munitionDef.radius = 2.0f;
        m1.munitionDef.rotation = 0.0f;
        m1.munitionDef.color.r = m1.munitionDef.color.g =
                                 m1.munitionDef.color.b =
                                 m1.munitionDef.color.a = 0.0f;

        m1.launcherDef.ttbmRequired.ClearAll();
        m1.launcherDef.ttbmEffects.ClearAll();

        m1.launcherDef.mass = 2.0f;
        m1.launcherDef.signature = 0.5f;
        m1.launcherDef.price = 50;
        strcpy(m1.launcherDef.modelName, "mis12");
        strcpy(m1.launcherDef.iconName, "missle");
        strcpy(m1.launcherDef.name, "Calthrop");
        strcpy(m1.launcherDef.description, "Small & annoying");
        m1.launcherDef.partMask = 0;
        m1.launcherDef.expendableSize = 1;

        IObject*   u = pCore->CreateObject(now, OT_mineType,
                                            &m1, sizeof(m1));

        u->Release();
    }
    {
        DataLauncherTypeIGC m1;
        //Other fields are not used (data is gotten from the missile type)
        m1.partID = 9;
        m1.expendabletypeID = 3;
        m1.amount = 10;
        strcpy(m1.inventoryLineMDL, "invsmine");

        m1.successorPartID = NA;

        IObject*   u = pCore->CreateObject(now, OT_partType,
                                           &m1, sizeof(m1));

        u->Release();
    }


    {
        DataChaffTypeIGC  m1;

        m1.modelName[0] = '\0';
        m1.iconName[0] = '\0';
        strcpy(m1.textureName, "chaff");
        m1.radius = 2.0f;
        m1.rotation = 0.0f;
        m1.color.r = m1.color.g = m1.color.b =
                                  m1.color.a = 0.0f;

        m1.lifespan = 4.0f;
        m1.loadTime = 2.0f;

        m1.chaffStrength = 5.0f;

        //None of these are used ... but set them anyway
        m1.signature = 0.0f;
        m1.hitPoints = 0;

        m1.expendabletypeID = 4;

        m1.launcherDef.ttbmRequired.ClearAll();
        m1.launcherDef.ttbmEffects.ClearAll();

        m1.launcherDef.mass = 2.0f;
        m1.launcherDef.signature = 0.5f;
        m1.launcherDef.price = 50;

        strcpy(m1.launcherDef.modelName, "mis12");
        strcpy(m1.launcherDef.iconName, "missle");
        strcpy(m1.launcherDef.name, "Tinfoil");
        strcpy(m1.launcherDef.description, "Small & annoying");
        m1.launcherDef.partMask = 0;
        m1.launcherDef.expendableSize = 1;

        IObject*   u = pCore->CreateObject(now, OT_chaffType,
                                           &m1, sizeof(m1));

        u->Release();
    }
    {
        DataLauncherTypeIGC m1;
        //Other fields are not used (data is gotten from the missile type)
        m1.partID = 30;
        m1.expendabletypeID = 4;
        m1.amount = 10;

        m1.successorPartID = NA;
        strcpy(m1.inventoryLineMDL, "invchaff");

        IObject*   u = pCore->CreateObject(now, OT_partType,
                                           &m1, sizeof(m1));

        u->Release();
    }
    //create some default projectile types
    {
        DataProjectileTypeIGC  pt1;

        pt1.width = 0;
        pt1.bDirectional = false;
        pt1.projectileTypeID = 1;
        pt1.power = 1.0f;
        pt1.blastPower = 0.0f; //20.0f;
        pt1.blastRadius = 0.0f; //5.0f;
        pt1.speed = 500.0f;
        pt1.absoluteF = false;
        pt1.modelName[0] = '\0';
        strcpy(pt1.textureName, "f22");
        pt1.lifespan = 1.25f;
        pt1.radius = 2.5f;
        pt1.rotation = 240.0f * pi / 180.0f;
        pt1.color.r = pt1.color.g = pt1.color.b =
                                    pt1.color.a = 0.0f;
        pt1.damageType = c_dtmEnergy;
        pt1.ambientSound = NA;

        IObject*   u = pCore->CreateObject(now, OT_projectileType,
                                               &pt1, sizeof(pt1));

        u->Release();
    }
    {
        DataProjectileTypeIGC  pt1;

        pt1.width = 1;
        pt1.bDirectional = false;
        pt1.projectileTypeID = 2;
        pt1.power = 9.0f;
        pt1.blastPower = 0.0f;
        pt1.blastRadius = 0.0f;
        pt1.speed = 625.0f;
        pt1.absoluteF = false;
        pt1.modelName[0] = '\0';
        strcpy(pt1.textureName, "f08");
        pt1.lifespan = 0.65f;
        pt1.radius = 2.5f;
        pt1.rotation = 120.0f * pi / 180.0f;
        pt1.color.r = pt1.color.g = pt1.color.b =
                                    pt1.color.a = 0.0f;
        pt1.damageType = c_dtmParticle;
        pt1.ambientSound = NA;

        IObject*   u = pCore->CreateObject(now, OT_projectileType,
                                               &pt1, sizeof(pt1));

        u->Release();
    }
    {
        DataProjectileTypeIGC  pt1;

        pt1.width = 0;
        pt1.bDirectional = false;
        pt1.projectileTypeID = 3;
        pt1.power = 12.0f;
        pt1.blastPower = 0.0f;
        pt1.blastRadius = 0.0f;
        pt1.speed = 1000.0f;
        pt1.absoluteF = true;
        pt1.modelName[0] = '\0';
        strcpy(pt1.textureName, "f20");
        pt1.lifespan = 0.75f;
        pt1.radius = 3.0f;
        pt1.rotation = 200.0f * pi / 180.0f;
        pt1.color.r = pt1.color.g = pt1.color.b =
                                    pt1.color.a = 0.0f;
        pt1.damageType = c_dtmParticle;
        pt1.ambientSound = NA;

        IObject*   u = pCore->CreateObject(now, OT_projectileType,
                                               &pt1, sizeof(pt1));

        u->Release();
    }
    {
        DataProjectileTypeIGC  pt1;

        pt1.width = 1;
        pt1.bDirectional = true;
        pt1.projectileTypeID = 4;
        pt1.power = 12.0f;
        pt1.blastPower = 0.0f;
        pt1.blastRadius = 0.0f;
        pt1.speed = 1000.0f;
        pt1.absoluteF = true;
        pt1.modelName[0] = '\0';
        strcpy(pt1.textureName, "fx95");
        pt1.lifespan = 1.5f;
        pt1.radius = 3.5f;
        pt1.rotation = 200.0f * pi / 180.0f;
        pt1.color.r = pt1.color.g = pt1.color.b =
                                    pt1.color.a = 0.0f;
        pt1.damageType = c_dtmParticle;
        pt1.ambientSound = NA;

        IObject*   u = pCore->CreateObject(now, OT_projectileType,
                                               &pt1, sizeof(pt1));

        u->Release();
    }
    {
        DataProbeTypeIGC  m1;

        strcpy(m1.modelName, "acs34");
        strcpy(m1.iconName, "missle");

        m1.textureName[0] = '\0';
        m1.radius = 2.0f;
        m1.rotation = 0.0f;
        m1.color.r = m1.color.g = m1.color.b =
                                  m1.color.a = 0.0f;

        m1.lifespan = 300.0f;
        m1.loadTime = 2.0f;
        m1.signature = 0.1f;

        m1.scannerRange = 20000.0f;
        m1.hitPoints = 20;
        m1.expendabletypeID = 5002;

        m1.projectileTypeID = 1;
        m1.accuracy = 1.0f;
        m1.dtimeBurst = 0.2f;
        m1.dispersion = 0.0f;

        m1.launcherDef.ttbmRequired.ClearAll();
        m1.launcherDef.ttbmEffects.ClearAll();

        m1.launcherDef.mass = 2.0f;
        m1.launcherDef.signature = 0.5f;
        m1.launcherDef.price = 50;
        strcpy(m1.launcherDef.modelName, "acs48");
        strcpy(m1.launcherDef.iconName, "missle");
        strcpy(m1.launcherDef.name, "Eyes");
        strcpy(m1.launcherDef.description, "Can see forever");
        m1.launcherDef.partMask = 0;
        m1.launcherDef.expendableSize = 1;
        m1.ambientSound = 720;

        IObject*   u = pCore->CreateObject(now, OT_probeType,
                                            &m1, sizeof(m1));

        u->Release();
    }
    {
        DataLauncherTypeIGC m1;
        //Other fields are not used (data is gotten from the missile type)
        m1.partID = 10;
        m1.expendabletypeID = 5002;
        m1.amount = 10;

        m1.successorPartID = NA;
        strcpy(m1.inventoryLineMDL, "invmine");

        IObject*   u = pCore->CreateObject(now, OT_partType,
                                           &m1, sizeof(m1));

        u->Release();
    }

    //Create some default weapons
    {
        DataWeaponTypeIGC  w1;
        strcpy(w1.modelName, "wep01");
        strcpy(w1.iconName, "missle");
        strcpy(w1.name, "Particle Cannon");
        w1.description[0] = '\0';
        w1.price = 100;
        w1.mass = 1.0f;
        w1.partID = 1;
        w1.successorPartID = NA;
        w1.equipmentType = ET_Weapon;
        w1.partMask = 0;
        w1.signature = 0.25f;
        w1.dtimeBurst = 0.25f;
        w1.dtimeReady = 0.25f;
        w1.energyPerShot = 35.0f;
        w1.dispersion = 0.02f;
        w1.projectileTypeID = 1;
        w1.cAmmoPerShot = 0;
        w1.activateSound = 80;
        w1.singleShotSound = 2;
        w1.burstSound = 42;
        strcpy(w1.inventoryLineMDL, "invsweapon");

        w1.ttbmRequired.ClearAll();
        w1.ttbmEffects.ClearAll();

        IObject*   u = pCore->CreateObject(now, OT_partType,
                                               &w1, sizeof(w1));

        u->Release();
    }
    {
        DataWeaponTypeIGC  w1;
        strcpy(w1.modelName, "wep01");
        strcpy(w1.iconName, "missle");
        strcpy(w1.name, "Machine gun");
        w1.description[0] = '\0';
        w1.price = 100;
        w1.mass = 1.0f;
        w1.partID = 2;
        w1.successorPartID = NA;
        w1.equipmentType = ET_Weapon;
        w1.partMask = 0;
        w1.signature = 0.25f;
        w1.dtimeBurst = 0.065f;
        w1.dtimeReady = 0.25f;
        w1.energyPerShot = 0.0f;
        w1.dispersion = 0.02f;
        w1.projectileTypeID = 2;
        w1.cAmmoPerShot = 1;
        w1.activateSound = 80;
        w1.singleShotSound = 9;
        w1.burstSound = 49;
        strcpy(w1.inventoryLineMDL, "invsweapon");

        w1.ttbmRequired.ClearAll();
        w1.ttbmEffects.ClearAll();

        IObject*   u = pCore->CreateObject(now, OT_partType,
                                               &w1, sizeof(w1));

        u->Release();
    }
    {
        DataWeaponTypeIGC  w1;
        strcpy(w1.modelName, "wep01");
        strcpy(w1.iconName, "missle");
        strcpy(w1.name, "Plasma Rifle");
        w1.description[0] = '\0';
        w1.price = 100;
        w1.mass = 1.0f;
        w1.partID = 21;
        w1.successorPartID = NA;
        w1.equipmentType = ET_Weapon;
        w1.partMask = 0;
        w1.signature = 0.25f;
        w1.dtimeBurst = 0.1f;
        w1.dtimeReady = 0.25f;
        w1.energyPerShot = 10.0f;
        w1.dispersion = 0.0f;
        w1.projectileTypeID = 3;
        w1.cAmmoPerShot = 0;
        w1.activateSound = 80;
        w1.singleShotSound = 1;
        w1.burstSound = 41;
        strcpy(w1.inventoryLineMDL, "invsweapon");

        w1.ttbmRequired.ClearAll();
        w1.ttbmEffects.ClearAll();

        IObject*   u = pCore->CreateObject(now, OT_partType,
                                               &w1, sizeof(w1));

        u->Release();
    }
    {
        DataWeaponTypeIGC  w1;
        strcpy(w1.modelName, "wep05");
        strcpy(w1.iconName, "missle");
        strcpy(w1.name, "Sniper Rifle");
        w1.description[0] = '\0';
        w1.price = 100;
        w1.mass = 1.0f;
        w1.partID = 22;
        w1.successorPartID = NA;
        w1.equipmentType = ET_Weapon;
        w1.partMask = 0;
        w1.signature = 0.25f;

        w1.dtimeBurst = 0.15f;
        w1.dtimeReady = 0.25f;
        w1.energyPerShot = 0.0f;

        w1.dispersion = 0.0f; //1f;

        w1.projectileTypeID = 4;
        w1.cAmmoPerShot = 1;
        w1.activateSound = 80;
        w1.singleShotSound = 3;
        w1.burstSound = 43;

        w1.ttbmRequired.ClearAll();
        w1.ttbmEffects.ClearAll();

        strcpy(w1.inventoryLineMDL, "invsweapon");

        IObject*   u = pCore->CreateObject(now, OT_partType,
                                               &w1, sizeof(w1));

        u->Release();
        /*
        DataWeaponTypeIGC  w1;
        strcpy(w1.modelName, "wep05");
        strcpy(w1.name, "Double Particle Accelerator");
        w1.description[0] = '\0';
        w1.price = 100;
        w1.mass = 1.0f;
        w1.partID = 2;
        w1.equipmentType = ET_Weapon;
        w1.partMask = 0;
        w1.signature = 0.25f;
        w1.dtimeBurst = 0.320f;
        w1.dtimeReady = 0.25f;
        w1.energyPerShot = 50.0f;
        w1.dispersion = 0.01f;
        w1.projectileTypeID = 2;
        w1.cAmmoPerShot = 1;
        w1.soundID = particleSound;

        w1.ttbmRequired.ClearAll();
        w1.ttbmRequired.SetBit(c_ttbBuildBlaster);
        w1.ttbmEffects.ClearAll();

        IObject*   u = pCore->CreateObject(now, OT_partType,
                                               &w1, sizeof(w1));

        u->Release();
        */
    }
    {
        DataShieldTypeIGC  w1;
        strcpy(w1.modelName, "acs34");
        strcpy(w1.iconName, "missle");
        strcpy(w1.name, "Particle dissipator");
        w1.description[0] = '\0';
        w1.price = 100;
        w1.mass = 4.0f;
        w1.partID = 3;
        w1.successorPartID = NA;
        w1.equipmentType = ET_Shield;
        w1.partMask = 0;
        w1.signature = 0.50f;
        w1.rateRegen = 10.0f;
        w1.maxStrength = 500.0f;
        w1.reflect[0] = 0.2f;
        w1.reflect[1] = 0.05f;
        w1.activateSound = 580;
        w1.deactivateSound = 620;

        w1.ttbmRequired.ClearAll();
        w1.ttbmEffects.ClearAll();
        strcpy(w1.inventoryLineMDL, "invshield");

        IObject*   u = pCore->CreateObject(now, OT_partType,
                                               &w1, sizeof(w1));

        u->Release();
    }
    {
        DataCloakTypeIGC  w1;
        strcpy(w1.modelName, "acs34");
        strcpy(w1.iconName, "missle");
        strcpy(w1.name, "Hide&Seek");
        w1.description[0] = '\0';
        w1.price = 100;
        w1.mass = 4.0f;
        w1.partID = 4;
        w1.successorPartID = NA;
        w1.equipmentType = ET_Cloak;
        w1.partMask = 0;
        w1.signature = 0.50f;

        w1.maxCloaking = 0.8f;
        w1.energyConsumption = 125.0f;
        w1.onRate = 0.25f;
        w1.offRate = 0.5f;
        w1.engageSound = 640;
        w1.disengageSound = 660;

        w1.ttbmRequired.ClearAll();
        w1.ttbmEffects.ClearAll();
        strcpy(w1.inventoryLineMDL, "invcloak");

        IObject*   u = pCore->CreateObject(now, OT_partType,
                                               &w1, sizeof(w1));

        u->Release();
    }
    {
        DataAfterburnerTypeIGC  w1;
        strcpy(w1.modelName, "acs48");
        strcpy(w1.iconName, "missle");
        strcpy(w1.name, "Booster Mk4");
        w1.description[0] = '\0';
        w1.price = 100;
        w1.mass = 8.0f;
        w1.partID = 5;
        w1.successorPartID = NA;
        w1.equipmentType = ET_Afterburner;
        w1.partMask = 0;
        w1.signature = 3.0f;
        w1.fuelConsumption = 0.001f;
        w1.maxThrust = 4000.0f;
        w1.onRate = 0.2f;
        w1.offRate = 0.5f;
        w1.interiorSound = 680;
        w1.exteriorSound = 700;

        w1.ttbmRequired.ClearAll();
        w1.ttbmEffects.ClearAll();
        strcpy(w1.inventoryLineMDL, "invafterburn");

        IObject*   u = pCore->CreateObject(now, OT_partType,
                                               &w1, sizeof(w1));

        u->Release();
    }
    {
        DataPackTypeIGC  w1;
        strcpy(w1.modelName, "wep07");
        strcpy(w1.iconName, "missle");
        strcpy(w1.name, "Ammo pack");
        w1.description[0] = '\0';
        w1.price = 100;
        w1.mass = 8.0f;
        w1.partID = 6;
        w1.successorPartID = NA;
        w1.equipmentType = ET_Pack;
        w1.partMask = 0;
        w1.signature = 3.0f;

        w1.packType = c_packAmmo;
        w1.amount = 250;

        w1.ttbmRequired.ClearAll();
        w1.ttbmEffects.ClearAll();

        IObject*   u = pCore->CreateObject(now, OT_partType,
                                               &w1, sizeof(w1));

        u->Release();
    }
    {
        DataPackTypeIGC  w1;
        strcpy(w1.modelName, "wep07");
        strcpy(w1.iconName, "missle");
        strcpy(w1.name, "Fuel pack");
        w1.description[0] = '\0';
        w1.price = 100;
        w1.mass = 8.0f;
        w1.partID = 11;
        w1.successorPartID = NA;
        w1.equipmentType = ET_Pack;
        w1.partMask = 0;
        w1.signature = 3.0f;

        w1.packType = c_packFuel;
        w1.amount = 100;

        w1.ttbmRequired.ClearAll();
        w1.ttbmEffects.ClearAll();

        IObject*   u = pCore->CreateObject(now, OT_partType,
                                               &w1, sizeof(w1));

        u->Release();
    }
    {
        DataDevelopmentIGC d;
        d.timeToBuild = 100;
        d.developmentID = 1;
        d.price = 500;
        strcpy(d.modelName, "wep05");
        strcpy(d.iconName, "missle");
        strcpy(d.name, "Kick Butt Blasters");
        strcpy(d.description, "Kick Butt Blasters");
        d.ttbmRequired.ClearAll();
        d.ttbmEffects.ClearAll();
        d.gas.Initialize();
        IObject*   u = pCore->CreateObject(now, OT_development,
                                                   &d, sizeof(d));
        u->Release();
    }

    {
        DataDroneTypeIGC   d;

		//parameters for drone type 1 -- called by a trekkey
        d.timeToBuild = 30;
        d.droneTypeID = 1;
        d.hullTypeID = 6;
        d.pilotType = c_ptBuilder;

        d.moveSkill = d.shootSkill = 1.0f;
        d.bravery = 0.5f;

        d.price = 500;
        strcpy(d.modelName, "utl19");
        d.iconName[0] = '\0';
        strcpy(d.name, "Construction");
        strcpy(d.description, "Construction drone");

        d.ttbmRequired.ClearAll();
        d.ttbmEffects.ClearAll();

        {
			//create the drone type
            IObject*   u = pCore->CreateObject(now, OT_droneType,
                                               &d, sizeof(d));
            u->Release();
        }

        d.droneTypeID = 2;
        d.hullTypeID = 2;
        d.pilotType = c_ptWingman;

        strcpy(d.modelName, "bom01a");
        d.iconName[0] = '\0';
        strcpy(d.name, "Wingman");
        strcpy(d.description, "Unarmed wingman");

        {
            IObject*   u = pCore->CreateObject(now, OT_droneType,
                                               &d, sizeof(d));
            u->Release();
        }
    }

    {
        //Create the 3 civilizations
        DataCivilizationIGC c;
        strcpy(c.name, "Rix");
        c.lifepod = 1;
        c.constructionDroneTypeID = 1;
        strcpy(c.hudName, "dialog");
        c.nPreferredPartTypes = 0;

        c.initialStationTypeID = 0;
        c.civilizationID = 0;
        c.ttbmBaseTechs.ClearAll();
        c.ttbmNoDevTechs.ClearAll();
        c.gasBaseAttributes.Initialize();

        {
            IObject*   u = pCore->CreateObject(now, OT_civilization,
                                                   &c, sizeof(c));

            u->Release();
        }

        strcpy(c.name, "Iron league");
        c.initialStationTypeID = 1;
        c.civilizationID = 1;
        {
            IObject*   u = pCore->CreateObject(now, OT_civilization,
                                                   &c, sizeof(c));

            u->Release();
        }

        strcpy(c.name, "Belorian");
        c.initialStationTypeID = 2;
        c.civilizationID = 2;
        {
            IObject*   u = pCore->CreateObject(now, OT_civilization,
                                                   &c, sizeof(c));

            u->Release();
        }
    }

    /*
    {
        //Create the 3 sides
        DataSideIGC s;
        strcpy(s.name, "Majestic");
        s.civilizationID = 0;
        s.sideID = 0;

        s.ttbmDevelopmentTechs.ClearAll();
        s.gas.Initialize();
        s.color.SetRGBA(1.0f, 0.0f, 0.0f);

        {
            IObject*   u = pCore->CreateObject(now, OT_side,
                                                   &s, sizeof(s));

            u->Release();
        }

        strcpy(s.name, "Ponderous");
        s.civilizationID = 1;
        s.sideID = 1;
        s.gas.SetAttribute(c_gaTopSpeed, 1.25f);
        s.color.SetRGBA(0.0f, 1.0f, 0.0f);

        {
            IObject*   u = pCore->CreateObject(now, OT_side,
                                                   &s, sizeof(s));

            u->Release();
        }

        strcpy(s.name, "Agile");
        s.civilizationID = 2;
        s.sideID = 2;
        s.color.SetRGBA(0.0f, 1.0f, 1.0f);
        {
            IObject*   u = pCore->CreateObject(now, OT_side,
                                                   &s, sizeof(s));

            u->Release();
        }
    }

    {
        DataStationIGC  s1;

        for (SectorID i = 25; (i <= 27); i++)
        {
            s1.stationTypeID = i - 25;
            s1.stationID = i;
            s1.clusterID = i;
            s1.sideID = i - 25;
            const char* names[] = {"IL Starbase", "RU Starbase", "CW Starbase"};
            strcpy(s1.name, names[i-25]);
            s1.position.x = 0.0f;
            s1.position.y = 0.0f;
            s1.position.z = 0.0f;
            s1.up.x = s1.up.z = s1.forward.x = s1.forward.y = 0.0f;
            s1.up.y = s1.forward.z = 1.0f;
            s1.rotation.axis(s1.forward);
            s1.rotation.angle(0.1f);

            IObject*   u = pCore->CreateObject(now,
                                                   OT_station,
                                                   &s1, sizeof(s1));
            u->Release();
        }
    }
    {
        DataTreasureIGC t1;
        t1.partID = 7;
        t1.amount = 500;
        t1.lifespan = 1000.0f;
        t1.p0.x = 700.0f; t1.p0.y = 400.0f; t1.p0.z = 900.0f;
        t1.v0 = Vector::GetZero();

        t1.time0 = now;

        ItreasureIGC*   t = (ItreasureIGC*)pCore->CreateObject(now, OT_treasure,
                                                   &t1, sizeof(t1));

        assert (t);
        t->SetCluster(pCore->GetCluster(27));
        t->Release();
    }

    if (fSwarm)
    {
        srand(48324);
        for (int i = 0; (i < 30); i++)
        {
            {
                Vector  p(700.0f, 400.0f, 0.0f);
                p.x += random(-100.0f, 100.0f);
                p.y += random(-100.0f, 100.0f);
                p.z += random(-100.0f, 100.0f);
				//p.z = 0.0f;
                const PartData  partData[2] = {{1, 0}, {3, 0}};
                g_drones.Create(trekClient.m_pCoreIGC, c_dtDummy,
                                "Target",
                                2,
                                pCore->GetSide(2),
                                2, partData,
                                pCore->GetCluster(27), p);
            }
        }
    }

    {
        for (SectorID i = 25; (i <= 27); i++)
        {
            for (int j = 0; (j < 3); j++)
            {
                Vector  p;
                p.x = cos(((float)j) * 120.0f * pi / 180.0f) * 300;
                p.y = sin(((float)j) * 120.0f * pi / 180.0f) * 300;
                p.z = 0.0f;
                const PartData  partData[2] = {{1, 0}, {3, 0}};
                g_drones.Create(trekClient.m_pCoreIGC, c_dtTurret,
                                "Turret",
                                5,
                                pCore->GetSide(i - 25),
                                2, partData,
                                pCore->GetCluster(i), p);
            }
            {
                Vector  p;
                p.x =400.0f;
                p.y =  p.z = 0.0f;

                const PartData  partData[1] = {{3, 0}};
                Drone* d=g_drones.Create(trekClient.m_pCoreIGC, c_dtMining,
                                "Miner",
                                6,
                                pCore->GetSide(i - 25),
                                1, partData,
                                pCore->GetCluster(i), p);
            }
        }
    }
    */
#ifdef foo
    //NYI hack to read all of the data in
    {
        char    artwork[MAX_PATH];

        HRESULT hr = UTL::getFile("gamedata", ".dat", artwork,
                                  true, true);

        FILE*   f = fopen(artwork, "rb");

        int datasize;
        int n = fread(&datasize, sizeof(datasize), 1, f);

        assert (n == 1);
        char*   pdata = new char[datasize];

        int c = fread(pdata, sizeof(char), datasize, f);
        assert (c == datasize);

        trekClient.m_pCoreIGC->Import(now, -1, pdata, datasize);
        delete [] pdata;

        fclose(f);
    }
#endif
    //Put the client on the given side
    {
        MissionParams mp;
        mp.nTeams = 3;
        const CivilizationListIGC * plistCiv = trekClient.m_pCoreIGC->GetCivilizations();
        int cCivs = plistCiv->n();

        mp.bShowHomeSector = false;

        mp.nNeutralSectorAsteroids = 40;
        mp.nPlayerSectorAsteroids = 20;

        mp.nNeutralSectorMineableAsteroids = 4;
        mp.nPlayerSectorMineableAsteroids = 2;

        mp.nNeutralSectorSpecialAsteroids = 2;
        mp.nPlayerSectorSpecialAsteroids = 1;

        mp.nNeutralSectorTreasures = 15;
        mp.nPlayerSectorTreasures = 5;

        mp.nPlayerSectorTreasureRate = 0.01f;
        mp.nNeutralSectorTreasureRate = 0.05f;

        mp.tsiPlayerStart = 0;
        mp.tsiNeutralStart = 1;
        mp.tsiPlayerRegenerate = 2;
        mp.tsiNeutralRegenerate = 3;

        mp.nMinPlayersPerTeam = 1;
        mp.nMaxPlayersPerTeam = 1;

        for (SideID i = 0; (i < mp.nTeams); i++)
            mp.rgCivID[i] = (*plistCiv)[i % cCivs]->data()->GetObjectID();

        trekClient.m_pCoreIGC->GenerateMission(now, &mp);
    }
    //trekClient.SetSide(trekClient.m_pCoreIGC->GetSide(sideID - 1));
}


VOID LoadMDLMap(WinTrekClient * pClient, const ZString& strMap,
                bool fSwarm, SideID sideID)
{
    TRef<IshipIGC> pShip;

    gpMissionManager = MissionManager::Create(pClient->GetMissionContext());
    gpMissionManager->CreateDefaults();
    gpMissionManager->LoadTechTree("techtree");
    gpMissionManager->LoadClusters(strMap);

    pShip = gpMissionManager->CreateShip("loadoutFighter", sideID, "Majik");
    pClient->BuyShip(pShip);
}


TRef<MissionManager> gpMissionManager = NULL;

#endif 0
