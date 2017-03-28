#ifndef _PLAYER_INCLUDE
#define _PLAYER_INCLUDE


#include "Sprite.h"
#include "TileMap.h"




// Player is basically a Sprite that represents the player. As such it has
// all properties it needs to track its movement, jumping, and collisions.

class IA;

class Player
{

public:
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	void update(int deltaTime);
	void render();

	glm::vec2 getPosition() { return posPlayer; }
	
	void setTileMap(TileMap *tileMap);
	void setPosition(const glm::vec2 &pos);
	void dealDamage(int damage, string type);
	float Player::GetScreenX(int widthScreen);
	float Player::GetScreenY(int heightScreen);
	bool Player::isAttacking();
	void Player::setSultans(IA *sultans);
	bool isBlocking();
	bool isSwordOut();

	int getHealth () {
		return healthPoints;
	}
	
private:
	glm::ivec2 tileMapDispl;
	glm::vec2 posPlayer;

	bool bFalling, bJumping;
	int fallenDistance;
	char direction;

	Texture spritesheet;
	Sprite *sprite;
	TileMap *map;

	IA *sultans;

	int healthPoints;
	float jumped;
	bool fallStraight;
	bool saved; // quan guardes la espasa voluntariament
	

};





#endif // _PLAYER_INCLUDE


