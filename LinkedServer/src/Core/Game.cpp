#include "Game.h"
#include "Monster.h"
#include "MonsterFactory.h"
#include "GameEntityFactory.h"
#include "Map.h"
#include "Player.h"
#include "PacketController.h"
#include "Projectile.h"
#include "HoshoyoExplosionSkill.h"
#include "CureBlessingSkill.h"
#include "SwapSkill.h"
#include "LinkSkill.h"

#define PLAYER_INITIAL_POSITION glm::vec3(133, 600, 2.5f)
#define PLAYER_MAXHP_BASIS 1000
#define PLAYER_HP_BASIS 1000
#define PLAYER_NAME "New Player"
#define PLAYER_ATTACK_BASIS 20
#define PLAYER_ASPD_BASIS 20
#define PLAYER_DEFENSE_BASIS 20
#define PLAYER_MAGICAL_POWER_BASIS 30
#define PLAYER_SPEED_BASIS 35

Game::Game()
{
	std::cout << "Game is being initialized..." << std::endl;
	this->createMap();
	std::cout << "Map Created." << std::endl;
	PacketController::players = &this->players;
	PacketController::game = this;
	int numberOfMonsters = this->createMonsters();
	std::cout << "Monsters Created. Number of Monsters: " << numberOfMonsters << "." << std::endl;
	std::cout << "Game was initialized successfully." << std::endl;
}

Game::~Game()
{
	delete map;
	delete monsterFactory;

	for (Player* player : players)
		delete player;

	for (Monster* monster : monsters)
		delete monster;
}

void Game::createMap()
{
	std::string mapPath = "./res/Maps/teste.png";
	std::string monsterMapPath = "./res/Maps/monsters23.png";
	std::string gameEntitiesMapPath = "./res/Maps/entities2.png";
	this->monsterFactory = new MonsterFactory();
	this->gameEntityFactory = new GameEntityFactory();
	this->map = new Map(mapPath, gameEntitiesMapPath, monsterMapPath, 3, monsterFactory, gameEntityFactory);
}

Player* Game::createNewPlayer(unsigned short clientId)
{
	Player* newPlayer = new Player(PLAYER_INITIAL_POSITION, &monsters, map);

	newPlayer->setClientId(clientId);
	newPlayer->setMaximumHpBasis(PLAYER_MAXHP_BASIS);
	newPlayer->setHp(PLAYER_HP_BASIS);
	newPlayer->setName(PLAYER_NAME);
	newPlayer->setAttackBasis(PLAYER_ATTACK_BASIS);
	newPlayer->setAttackSpeedBasis(PLAYER_ASPD_BASIS);
	newPlayer->setDefenseBasis(PLAYER_DEFENSE_BASIS);
	newPlayer->setMagicalPowerBasis(PLAYER_MAGICAL_POWER_BASIS);
	newPlayer->setSpeedBasis(PLAYER_SPEED_BASIS);

	/* FOR NOW, SKILLS MUST BE THE SAME AS THEY ARE DEFINED CLIENT-SIDE (SAME SLOTS, ALSO) */
	Skill* skill1 = new LinkSkill(PLAYER, &monsters, &players);
	skill1->setSlot(SLOT_1);
	newPlayer->addNewSkill(skill1);

	Skill* skill2 = new SwapSkill(PLAYER, &monsters, &players);
	skill2->setSlot(SLOT_2);
	newPlayer->addNewSkill(skill2);

	Skill* skill3 = new CureBlessingSkill(PLAYER, &monsters, &players);
	skill3->setSlot(SLOT_3);
	newPlayer->addNewSkill(skill3);

	Skill* skill4 = new HoshoyoExplosionSkill(PLAYER,&monsters, &players);
	skill4->setSlot(SLOT_4);
	newPlayer->addNewSkill(skill4);

	this->players.push_back(newPlayer);

	return newPlayer;
}

int Game::createMonsters()
{
	int numberOfMonsters = 0;

	// Criação dos Monstros e das Entidades
	for (int i = 0; i < MAP_SIZE; i++)
	{
		for (int j = 0; j < MAP_SIZE; j++)
		{
			MapCoordinate coordinate = map->getMapCoordinateForMapCreation(glm::vec3(i, j, 0));
			Monster *monster = coordinate.mapMonster.monster;

			if (coordinate.mapMonster.monsterExists)
			{
				if (!map->coordinateHasCollision(glm::vec3(i, j, 0)))
				{
					monster->setPosition(glm::vec3(i, j, 1.5f));
					monsters.push_back(monster);
					numberOfMonsters++;
				}
				else
					delete monster;
			}
		}
	}

	PacketController::monsters = &this->monsters;

	return numberOfMonsters;
}

Projectile* Game::createNewProjectile(glm::vec3 position, glm::vec3 direction, int playerId)
{
	Projectile* p = new Projectile(position, direction, playerId);
	this->projectiles.push_back(p);
	return p;
}

void Game::update()
{
	for (Player* player : this->players)
		player->update();
	// Monsters update
	for (unsigned int i = 0; i < monsters.size(); i++)
	{
		monsters[i]->update(map, &players);
	}

	for (unsigned int i = 0; i < monsters.size(); i++)
		if (monsters[i]->shouldBeDeleted())
		{
			delete monsters[i];
			monsters.erase(monsters.begin() + i);
		}
}

Monster* Game::getMonsterOfId(int id)
{
	for (int i = 0; i < monsters.size(); i++)
		if (monsters[i]->getId() == id)
			return monsters[i];

	return NULL;
}