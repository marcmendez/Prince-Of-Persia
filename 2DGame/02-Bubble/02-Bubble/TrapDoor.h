#ifndef _TRAPDOOR_INCLUDE
#define _TRAPDOOR_INCLUDE

#include "Sprite.h"
#include "Player.h"

class TrapDoor
{

public:

	void init(glm::vec2 &doorPos, glm::vec2 &pressurePlatePos, const glm::ivec2 &tileMapPos, Player *player, TileMap *map, ShaderProgram &shaderProgram);
	void update(int deltaTime);
	void render();
	void restart();

private:

	bool TrapDoor::AmISteppingOn(int posx, int posy, const glm::ivec2 &size) const;

	glm::ivec2 tileMapDispl;
	glm::vec2 posDoor; 
	glm::vec2 posPressurePlate;

	Texture spritesheet;
	Sprite *spriteDoor;
	Sprite *spritePressurePlate;

	Player *player;
	TileMap *map;

	bool bPressed;

};

#endif // _TRAPDOOR_INCLUDE
