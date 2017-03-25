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

	void setPlayer(Player *player);
	void setPosition(const glm::vec2 &pos);

	void IA::setTileMap(TileMap *tileMap);

private:
	bool enemyHitPlayer() const;

private:
	glm::ivec2 tileMapDispl;
	glm::vec2 posIA;

	Texture spritesheet;
	Sprite *sprite;
	Player *player;
	TileMap *map;
};

#endif // _IA_INCLUDE