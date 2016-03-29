# Linked

## What is Linked?
Linked is a multiplayer game idealized by Hoshoyo. It is currently being developed by Hoshoyo and Felipeek. We are looking for contributors, specially artists that are able to make sprites and 3d objects. Linked is a non-profit project and is being made for fun.

## Tasks

#### Main Tasks
- [ ] Create main menu
- [ ] Create character selection menu
- [ ] Create settings menu (window size, sounds, shadows, go back to main menu)
- [ ] Create custom commands (client and server side)
- [ ] Create loading screens
- [ ] Create new types of monsters

#### Bug Fixes
- [ ] Avoid creating heaps of sf::Sound ([More info](http://stackoverflow.com/questions/26265964/sfml-internal-openal-error#_=_))
- [ ] Fix chat overflow
- [ ] Fix HP Bar texture and position
- [ ] Disable skills when dead
- [x] Re-enable mouse right button
- [ ] Re-enable skills cooldown icons

#### Future Tasks
- [ ] Create items logic
- [ ] Create equipments logic
- [ ] Implement deferred shading
- [ ] Add more lights to the forward renderer
- [ ] Shadow refactor
- [ ] Improve monster AI

#### Artistic Tasks (On Hold)
- [ ] Create game maps
- [ ] Create new textures
- [ ] Create new entities
- [ ] Create game sounds

## Game
![Monster AI](http://puu.sh/kRoVu/7463202ad3.jpg)
![Night](http://puu.sh/kVyID/37992b7699.jpg)
![MultiPlayer](http://puu.sh/kVyA1/de09997992.jpg)

## Build
To build Linked, you must open the solution Linked.sln in Visual Studio. Linked can be built with Visual Studio 2013 or Visual Studio 2015. When opening the solution, maybe you will be asked to retarget Linked's solution. In that case, retarget it (just make sure you are using VS2013 OR VS2015). Then, just build the solution on Build > Build Solution (x86)).

After building Linked, two binaries will be created: LinkedClient.exe and LinkedServer.exe. However, to correctly execute these files, you must follow the steps:

To execute LinkedClient.exe:

- The directories **LinkedClient\res**, **LinkedClient\shaders** and **LinkedClient\fonts** must be in the same directory as LinkedClient.exe
- All dlls inside **LinkedClient\lib\vcVERSION\** must be in the same directory as LinkedClient.exe. Just make sure you are opening the correct folder depending on your Visual Studio version.

To execute LinkedServer.exe:
- The directory **LinkedServer\res** must be in the same directory as LinkedServer.exe

Copy all the required directories to the same path as the executables and the game can be initialized.

## Play
To play the game Single Player, only the client is necessary. To play multiplayer, you must have the server running. To run the server, execute LinkedServer.exe, and to run the client, execute LinkedClient.exe.

## Developer Contact
1. Felipe Kersting (fekersting@inf.ufrgs.br)
2. Pedro Veiga (psveiga@inf.ufrgs.br)
