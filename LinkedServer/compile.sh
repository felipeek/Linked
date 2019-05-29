#!/bin/bash

INCLUDE="-g -I./include/ -I./include/Core -I./include/Game -I./include/Game/AI -I./include/Game/Factory -I./include/Game/Skill -I./include/Map -I./include/Network -I./include/ResourceLoad"
INCLUDE_EXT="-I./include/External -I./include/External/freetype -I./include/External/GL -I./include/External/GLFW -I./include/External/glm"

mkdir -p build
mkdir -p build/Core
mkdir -p build/Game
mkdir -p build/Game/AI
mkdir -p build/Game/Factory
mkdir -p build/Game/Skill
mkdir -p build/Map
mkdir -p build/Network
mkdir -p build/ResourceLoad

g++ $INCLUDE $INCLUDE_EXT -o build/Core/Game.o -c src/Core/Game.cpp
g++ $INCLUDE $INCLUDE_EXT -o build/Core/LinkedTime.o -c src/Core/LinkedTime.cpp
g++ $INCLUDE $INCLUDE_EXT -o build/Game/BasicMonster.o -c src/Game/BasicMonster.cpp
g++ $INCLUDE $INCLUDE_EXT -o build/Game/BomberMonster.o -c src/Game/BomberMonster.cpp
g++ $INCLUDE $INCLUDE_EXT -o build/Game/Creature.o -c src/Game/Creature.cpp
g++ $INCLUDE $INCLUDE_EXT -o build/Game/DistanceMonster.o -c src/Game/DistanceMonster.cpp
g++ $INCLUDE $INCLUDE_EXT -o build/Game/Equipment.o -c src/Game/Equipment.cpp
g++ $INCLUDE $INCLUDE_EXT -o build/Game/GameEntity.o -c src/Game/GameEntity.cpp
g++ $INCLUDE $INCLUDE_EXT -o build/Game/Monster.o -c src/Game/Monster.cpp
g++ $INCLUDE $INCLUDE_EXT -o build/Game/Player.o -c src/Game/Player.cpp
g++ $INCLUDE $INCLUDE_EXT -o build/Game/Projectile.o -c src/Game/Projectile.cpp
g++ $INCLUDE $INCLUDE_EXT -o build/Game/TurretMonster.o -c src/Game/TurretMonster.cpp
g++ $INCLUDE $INCLUDE_EXT -o build/Game/WorldObject.o -c src/Game/WorldObject.cpp
g++ $INCLUDE $INCLUDE_EXT -o build/Game/AI/AI.o -c src/Game/AI/AI.cpp
g++ $INCLUDE $INCLUDE_EXT -o build/Game/AI/MonsterAI.o -c src/Game/AI/MonsterAI.cpp
g++ $INCLUDE $INCLUDE_EXT -o build/Game/Factory/Factory.o -c src/Game/Factory/Factory.cpp
g++ $INCLUDE $INCLUDE_EXT -o build/Game/Factory/GameEntityFactory.o -c src/Game/Factory/GameEntityFactory.cpp
g++ $INCLUDE $INCLUDE_EXT -o build/Game/Factory/MonsterFactory.o -c src/Game/Factory/MonsterFactory.cpp
g++ $INCLUDE $INCLUDE_EXT -o build/Game/Skill/CureBlessingSkill.o -c src/Game/Skill/CureBlessingSkill.cpp
g++ $INCLUDE $INCLUDE_EXT -o build/Game/Skill/HoshoyoExplosionSkill.o -c src/Game/Skill/HoshoyoExplosionSkill.cpp
g++ $INCLUDE $INCLUDE_EXT -o build/Game/Skill/LinkSkill.o -c src/Game/Skill/LinkSkill.cpp
g++ $INCLUDE $INCLUDE_EXT -o build/Game/Skill/MonsterEndureSkill.o -c src/Game/Skill/MonsterEndureSkill.cpp
g++ $INCLUDE $INCLUDE_EXT -o build/Game/Skill/MonsterExplosionSkill.o -c src/Game/Skill/MonsterExplosionSkill.cpp
g++ $INCLUDE $INCLUDE_EXT -o build/Game/Skill/Skill.o -c src/Game/Skill/Skill.cpp
g++ $INCLUDE $INCLUDE_EXT -o build/Game/Skill/SwapSkill.o -c src/Game/Skill/SwapSkill.cpp
g++ $INCLUDE $INCLUDE_EXT -o build/Map/Map.o -c src/Map/Map.cpp
g++ $INCLUDE $INCLUDE_EXT -o build/Map/MapEntity.o -c src/Map/MapEntity.cpp
g++ $INCLUDE $INCLUDE_EXT -o build/Map/MapMonster.o -c src/Map/MapMonster.cpp
g++ $INCLUDE $INCLUDE_EXT -o build/Map/MapTerrain.o -c src/Map/MapTerrain.cpp
g++ $INCLUDE $INCLUDE_EXT -o build/Network/ClientInfo.o -c src/Network/ClientInfo.cpp
g++ $INCLUDE $INCLUDE_EXT -o build/Network/ClientPacket.o -c src/Network/ClientPacket.cpp
g++ $INCLUDE $INCLUDE_EXT -o build/Network/Packet.o -c src/Network/Packet.cpp
g++ $INCLUDE $INCLUDE_EXT -o build/Network/PacketController.o -c src/Network/PacketController.cpp
g++ $INCLUDE $INCLUDE_EXT -o build/Network/UDPServer.o -c src/Network/UDPServer.cpp
g++ $INCLUDE $INCLUDE_EXT -o build/ResourceLoad/ImageLoader.o -c src/ResourceLoad/ImageLoader.cpp
g++ $INCLUDE $INCLUDE_EXT -o build/Main.o -c src/Main.cpp

g++ \
build/Core/Game.o \
build/Core/LinkedTime.o \
build/Game/BasicMonster.o \
build/Game/BomberMonster.o \
build/Game/Creature.o \
build/Game/DistanceMonster.o \
build/Game/Equipment.o \
build/Game/GameEntity.o \
build/Game/Monster.o \
build/Game/Player.o \
build/Game/Projectile.o \
build/Game/TurretMonster.o \
build/Game/WorldObject.o \
build/Game/AI/AI.o \
build/Game/AI/MonsterAI.o \
build/Game/Factory/Factory.o \
build/Game/Factory/GameEntityFactory.o \
build/Game/Factory/MonsterFactory.o \
build/Game/Skill/CureBlessingSkill.o \
build/Game/Skill/HoshoyoExplosionSkill.o \
build/Game/Skill/LinkSkill.o \
build/Game/Skill/MonsterEndureSkill.o \
build/Game/Skill/MonsterExplosionSkill.o \
build/Game/Skill/Skill.o \
build/Game/Skill/SwapSkill.o \
build/Map/Map.o \
build/Map/MapEntity.o \
build/Map/MapMonster.o \
build/Map/MapTerrain.o \
build/Network/ClientInfo.o \
build/Network/ClientPacket.o \
build/Network/Packet.o \
build/Network/PacketController.o \
build/Network/UDPServer.o \
build/ResourceLoad/ImageLoader.o \
build/Main.o \
-g -o linkedserver