#pragma once
#ifndef _IA_INCLUDE
#define _IA_INCLUDE


#include "Sprite.h"
#include "Player.h"
#include "TileMap.h"


class IA
{

public:

	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	void update(int deltaTime);
	void render();

	glm::vec2 getPosition() { return posIA; }

	void setPlayer(Player *player);
	void setPosition(const glm::vec2 &pos);
	void dealDamage(int damage);
	void IA::setTileMap(TileMap *tileMap);
	void dealDamageEnemy(int damage);

	bool enemyHitPlayer() const;

private:
	glm::ivec2 tileMapDispl;
	glm::vec2 posIA;

	Texture spritesheet;
	Sprite *sprite;
	Player *player;
	TileMap *map;

	bool notAttackedYet;
	int healthPoints;
};

#endif // _IA_INCLUDE