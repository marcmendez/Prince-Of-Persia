#ifndef _TRAPFALLINGFLOOR_INCLUDE
#define _TRAPFALLINGFLOOR_INCLUDE

#include "Sprite.h"
#include "Player.h"
#include "TileMap.h"


class TrapFallingFloor
{

public:
	void init(glm::vec2 &trapPos,const glm::ivec2 &tileMapPos, Player *player, TileMap *map, ShaderProgram &shaderProgram);
	void update(int deltaTime);
	void render();
	void restart();

private:
	bool TrapFallingFloor::AmISteppingOn() const;

private:
	glm::ivec2 tileMapDispl;
	glm::vec2 posTrap;

	Texture spritesheet;
	Sprite *sprite;
	Player *player;
	TileMap *map;

	int fallen;
	bool bDetected;
};

#endif // _TRAPFALLINGFLOOR_INCLUDE
