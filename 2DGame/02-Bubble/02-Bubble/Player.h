#ifndef _PLAYER_INCLUDE
#define _PLAYER_INCLUDE


#include "Sprite.h"
#include "TileMap.h"


// Player is basically a Sprite that represents the player. As such it has
// all properties it needs to track its movement, jumping, and collisions.


class Player
{

public:
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	void update(int deltaTime);
	void render();

	glm::vec2 getPosition() { return posPlayer; }
	
	void setTileMap(TileMap *tileMap);
	void setPosition(const glm::vec2 &pos);
	float Player::GetScreenX(int widthScreen);
	float Player::GetScreenY(int heightScreen);
	
private:
	glm::ivec2 tileMapDispl;
	glm::vec2 posPlayer;

	bool bFalling, bJumping;
	char direction;

	Texture spritesheet;
	Sprite *sprite;
	TileMap *map;
	

};


#endif // _PLAYER_INCLUDE


