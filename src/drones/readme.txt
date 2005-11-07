
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 

                       A Guide To The Drone Code

                       by Russ Tedrake   8/28/98

* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 


As of today, the drone code is officially full of more comments than 
anyone will ever read.  But they are there if you need them.  Make sure 
you resolution is high, your editor is set to full screen, and enjoy.

There are some general procedures that don't fit right in to the 
comments... I'll put them here instead.



Adding a Drone Type
-------------------

The drone class provides a complete framework for any drone.  By making 
an instantiation of that class, you get a drone class that is completely 
functional.  All of the interesting functions are made virtual so that 
the child classes can define all of the specifics of their behavior.  At 
the bottom of drone.h, you will see a handful of drone types that use 
nothing but the drone framework, and override just the GetNewDefaultGoal 
function.

Next, you need to add a constant into the DroneType enumeration, located 
in FEDSRC\inc\drones.h.

Next, in Drones::Create(), link this constant to the new drone class by 
adding to the switch statement near the end of the function.

You'll need to define which commands this new drone will accept, by 
adding to the switch statement in Drones::GetCommand(DroneType).  

And that should be it!  The drone will use all of the default drone 
functions, and should be fully functional.  The artwork spefications and 
other subtleties are handed to the drone code through the 
Drones::Create() function.

The WingManDrone class provides an excellent example of the typical drone 
class, and the TurretDrone class is an excellent example of a type of 
drone that doesn't fit the mold exactly.  The turret drones need to parse 
commands into goals a little bit differently than the rest of the drones 
(aka "defend" means slightly different things to a turret as to a wingman),
so they hand to redefine the SetGoal function.  



Adding an External Goal Type
----------------------------

By external goal type, I mean that players know about them, and can order
a drone to do this goal.

There are two abstract goal classes from which you can choose to inherit.
The Goal class does everything that a goal needs to do.  The TargetedGoal
class does all of that plus it keeps track of exactly one target for you,
references and all.

I have tried to keep all of the goals in goal.h and goal.cpp, except for
the goals that are specific to turrets and miners, which are in their 
respective files.

Instantiate your new class, inheritting from either Goal or TargetedGoal.
You'll need to define a constructor for the class.  You can look at any
of the other goals for how to do that.  It's important that you call the 
constructor for the class that you have derived from, too.

You'll notice that one of the parameters that you need to send into the
constructor is a CommandType.  Let's add our new commandType constant now.
The CommandType enumeration is in FEDSRV\inc\drones.h.

Next, to register the command with IGC, look at the Drones::SetCore()
function, and add a new line for your command.  Now you want to allow the
drones to accept your command, so go to Drones::GetDroneCommands(), and 
add your CommandType to the appropriate drone(s).

To create the mapping from the CommandType to the actual Goal class, add
your command to the switch statements in the Drone::SetGoal(CommandType).
Since a few of the drone types have re-defined SetGoal(CommandType), be 
sure to add your command to those functions, too, as appropriate.

There.  All that is left is to actually implement the goal, which probably
means implementing an Update(), a Done(), a DoneEvent(), and a Status()
for your class.  There are tons of examples in goal.h and goal.cpp!



Adding an Internal Goal Type
----------------------------

The other type of goal that you might want to add one that is just a slight
variation of an existing goal, and that players never have to know about.
For example, the DefendDestroyGoal is exactly the same as the DestroyGoal, 
except that it ends when the target leaves the sector.

These are easy to add.  Just derive your variation goal from the existing
goal, and only re-define the functions that you need to.  You don't have 
to worry about adding the command to IGC, since that is all for external
purposes.

Then, when you need to add one of these goals in  your code, use the
Drone::SetGoal(Goal*) instead of the SetGoal(CommandType, Target).  When
this goal is running, the display in the hud will say that you are doing 
the goal that you have derived from, since you didn't add a new type to IGC.



Questions??
-----------

If there is anything that is terribly unclear, then drop me a line at:

	RussT@umich.edu
