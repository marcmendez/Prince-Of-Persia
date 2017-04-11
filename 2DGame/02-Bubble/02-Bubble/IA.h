#pragma once
#ifndef _IA_INCLUDE
#define _IA_INCLUDE


#include "Sprite.h"
#include "TileMap.h"

class Player;
class IA
{

public:

	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram, Player *player, int minPos, int maxPos);
	void update(int deltaTime);
	void render();

	glm::vec2 getPosition() { return posIA; }

	void setPlayerStats(Player *player);
	void setPosition(const glm::vec2 &pos);
	void IA::setTileMap(TileMap *tileMap);
	bool isBlockingEnemy();
	void dealDamageEnemy(int damage);
	bool sultanIsBlockking();
	bool sultanIsAttacking();
	void reset();
	void restartPos();
	int getHealthEnemy() {
		return healthPoints;
	}

private:
	glm::ivec2 tileMapDispl;
	glm::vec2 posIA;
	glm::vec2 posPlayer;
	glm::vec2 posIAInit;

	Texture spritesheet;
	Sprite *sprite;
	TileMap *map;
	Player *player;


	int healthPoints;
	bool playerBlocking;
	bool playerAttacking;
	int playerHealth;
	int minPos, maxPos;
};

#endif // _IA_INCLUDE