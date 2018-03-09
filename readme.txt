Requirements

0. About document

This document shall contain "natural language" requirements. As such, these requirements are considered to be system-level at the lowest, but otherwise they are use cases or simply natural descriptions of what the software IS / DOES.

0.1 Project info / requirements
0.1.1 Coding standards
0.1.1.1 Function naming:
0.1.1.1.1 All class names shall be one-word, Uppercase.
0.1.1.1.2 All public functions of a class shall be PascalCase.
0.1.1.1.3 All private functions of a class shall be camelCase.
0.1.2 Tooling:
0.1.2.1 The source code shall be C++11 compatible. Backward compatibility is not considered.
0.1.2.2 The variable sizes and operations shall support 64-bit widths.
0.1.3 Variable types:
0.1.3.1 Numbers shall be represented by integer types.
0.1.3.2 Real numbers may only be used during output to file/screen. Internally, to obtain a real number, the program must use integers for computation.

1. General description

1.1 The software simulates one or more vehicles on a road.
1.2 For the purpose of the simulation, a road is said to consist of segments / tiles. A vehicle "moves" on a tile by learning what the tile difficulty is at the point of entering, then by taking that time to cross the tile. [see 2.3 "Time Tracker"]
1.3 The input data about road and vehicles is read from a text file.
1.4 The output shall consist of: each vehicle reporting its position, and each tile report  ing its status

2. Modules

2.1 Road Manager
2.1.1 The RM handles portions of the road called segments / tiles.
2.1.2 Tiles are simple abstractions for real-life concepts: stations, road portions, traffic lights and so on.
2.1.3 The most fundamental property of a tile is "difficulty to cross". This difficulty is based on an ideal, uniform speed (e.g. 50km/h). If a vehicle is traveling at a higher or lower speed, then it should recalculate difficulty and total time spent on tile.
2.1.3.1 TODO: CHANGE THIS DIFFICULTY TO AN ACTUAL REAL-LIFE MEASUREMENT (i.e. LENGTH IN METERS)
2.1.4 A segment has an implied direction of movement. As such, if a vehicle's route takes it in both directions over a road, the two parts should be separate segments. This stems from the needed logical separation of having different levels of traffic flow (e.g. at rush hour, congestion affects one direction but not the other).
2.1.5 Tiles can have different properties, depending on their type. For example: a station tile will have associated properties related to the chance for a passenger to arrive. A "regular" tile will have properties related to changes in difficulty, based on current simulation time (e.g. "rush hour").
2.1.6 Tiles should have a list of other adjacent tiles, allowing a sanity check for the vehicle routes (see 2.2.4). Saying that TileA has TileB and TileC adjacent means that a vehicle may pass FROM A to B, but not necessarily from B TO A.


2.2 Dispatch Manager ("DM")
2.2.1 The Dispatch Manager handles a number of items: vehicles and drivers. A vehicle is available to use by only one driver at a time. However, several drivers may rotate during one "day" in their usage of the vehicle.
2.2.2 Vehicles:
2.2.2.1 A vehicle is represented by some relevant properties: its current driver, its fuel level, its occupancy status (vs maximum), and its general condition (ability to travel at max road speed, chance to break down).
2.2.2.2 The vehicle is available for usage at any time of the day, restricted only by its current condition and its fuel level.
2.2.2.3 A vehicle may determine it needs refueling. In this case the DM is notified, and upon reaching the end of the route, the vehicle may be retired for a certain in-sim time. //TODO: This should take into consideration vehicle type, if tramway then the fuel is practically infinite.
2.2.2.4 For convenience, travel time from garage to the entry point of the route is not considered. Vehicles are immaterial, will "appear" on a tile when requested, and will disappear from a tile if needed (end of shift etc).
2.2.3 Drivers:
2.2.3.1 A driver is represented by some relevant properties: his current fatigue, his schedule, the line he is assigned to serve.
2.2.3.2 A driver may have some preferred hours, but the DM is free to disregard this. The only rule is to never exceed 8 hours of work. The information will be given in input files.
2.2.4 Routes:
2.2.4.1 The DM will set-up certain routes. A route is a list of segments that a vehicle will follow. These routes may either be baked in (BAH!), or given via config files. In any case, the DM should do a sanity check (see 2.1.6).
2.2.4.2 Routes will have some attributes, such as demand for vehicles. The DM is responsible for managing the available vehicles versus the demand.
2.2.4.3 Another property of a route should be service times. These should be given as "first vehicle leaving" and "last vehicle to start a round".

2.3 Time Tracker
2.3.1 The TT is a module which keeps track of current simulation time.
2.3.2 Simulation time is expressed in "ticks", which are not unlike real-life seconds. However, the passing of a tick is NOT tied to any particular hardware implementation or real-life measurement unit. Simply, a tick will elapse when there are no more operations to be performed in that tick.
2.3.3 The Start Moment and the End Moment shall be configurable. By default, start will be on tick 0 and end will be on tick 86400 (equivalent of 24h).
2.3.4 The timer shall not "wrap around". If one needs to simulate several days, simply use multiples of 86400 for the end moment. (implementation hint: use the largest integer value available on your platform).
2.3.5 The TT shall offer an interface function, to display the time in a user-friendly format: current day, hour and minute.

2.4 Passenger Manager
2.4.1 A passenger is an entity which can do the following actions in the sim: wait in a station until a vehicle with the correct line nr. arrives; travels with said vehicle until a certain road segment; disembarks.
2.4.1.1 Caveat: a passenger may wait for several lines, and climb into the first vehicle with a matching number.
2.4.1.2 TODO: In order to determine the possible lines to use, the passenger may request information about available lines from the Dispatch Manager. (e.g. "I need to get from tile A to tile B, which lines cover this?").
2.4.2 Arrival in a station:
2.4.2.1 Unless explicitly required via input files, passengers are generated during "randomization" sequences.
2.4.2.2 Chance of generating passengers is a property of station-tiles. The PM should either know this information from ini files, or otherwise query the road manager for it. (technical debt?!)
2.4.2.3 Upon generation, the passenger should have a "willingness" attribute which indicates how long they will wait for the correct vehicle. If a vehicle fails to arrive, they may simply drop off. If a passenger drops off, they may still be counted for statistical purposes.
2.4.3 Boarding / de-boarding:
2.4.3.1 Upon arrival of a vehicle, all affected passengers may board the vehicle (TODO: add vehicle limitations / crowding settings).
2.4.3.2 Difficulty added to the tile is the sum of waiting for all passengers to exit and all passengers to board.
2.4.3.3 A passenger that just de-boarded will remain in the system if they have more segments to cover in their travel. If they do not, they may be destroyed.
2.4.3.4 De-boarded passengers will add to a station's quota of expected passengers.
2.4.4 Passengers traveling on a vehicle do not "do" anything per se. They may, however, be counted for the purpose of establishing difficulty of boarding/de-boarding. (e.g. if the vehicle is almost full, it is slightly more difficult for people to get in/out).
2.4.5 Reporting each passenger's trip may be more time consuming than it's worth.//
