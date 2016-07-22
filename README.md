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
- [x] Disable skills when dead
- [x] Re-enable mouse right button
- [x] Re-enable skills cooldown icons

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
You can easily build Linked client through the command line following the steps below.

1. Open a Windows Terminal (cmd).
2. Using the terminal, run the Visual Studio batch file **vcvarsall.bat**. This file is normally located at C:\Program Files (x86)\Microsoft Visual Studio **14.0**\VC\vcvarsall.bat, where 14.0 is the version of the compiler.
3. Using the same terminal, run the batch file **build.bat**, which is located in the LinkedClient directory.
4. Linked Client will be compiled and created in **LinkedClient/bin/cmdbuild**.

If the above doesn't work or if you want to build Linked server too, you can build Linked directly from Visual Studio. To do this, follow the steps below.

1. Open **Linked.sln** using Visual Studio 2015.
2. Change the Solution Platform to **x86** and Solution Configurations to **Release**.
3. Right-click Linked solution and select Build Solution.
4. Linked Client will be compiled and created in **LinkedClient/bin/release**. Linked Server will be compiled and created in **LinkedServer/bin/release**.

After building Linked, the game will only work when Linked resources are moved along with the executable. To do this, follow the steps below.

If you want to run LinkedClient.exe:

1. Go to **LinkedClient/** and copy the directory **res/** and the file **config.linked**.
2. Move both to the same folder of your **LinkedClient.exe**.
3. Go to **LinkedClient/lib/win32** and copy all **.dll** inside this folder.
4. Move all **.dll** to the same folder of your **LinkedClient.exe**.

If you want to run LinkedServer.exe:

1. Go to **LinkedServer/** and copy the directory **res/**.
2. Move the directory to the same folder of your **LinkedServer.exe**.

Now the game can be initialized, but you may want to change Linked basic configurations. To do this, follow the steps below.

1. Go to the directory of your **LinkedClient.exe** and find the file **config.linked**.
2. Change the file. Network related fields like ***serverip*** and ***serverport*** are only meaningful if the ***multiplayer*** field is **true**.

Now you can play Linked.

## Play
To play the game Single Player, just run LinkedClient.exe. To play multiplayer, run LinkedServer.exe and then LinkedClient.exe.

## Developer Contact
1. Felipe Kersting (fekersting@inf.ufrgs.br)
2. Pedro Veiga (psveiga@inf.ufrgs.br)
