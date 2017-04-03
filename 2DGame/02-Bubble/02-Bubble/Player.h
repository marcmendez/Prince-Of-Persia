#ifndef _PLAYER_INCLUDE
#define _PLAYER_INCLUDE

#include "Sprite.h"
#include "TileMap.h"
#include "IA.h"



// Player is basically a Sprite that represents the player. As such it has
// all properties it needs to track its movement, jumping, and collisions.


class Player
{

public:
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	void update(int deltaTime);
	void render();
	void restart();

	glm::vec2 getPosition() { return posPlayer; }
	
	void setTileMap(TileMap *tileMap);
	void setPosition(const glm::vec2 &pos);
	void dealDamage(int damage, string type);
	float Player::GetScreenX(int widthScreen);
	float Player::GetScreenY(int heightScreen);
	bool Player::isAttacking();
	void Player::setSultans(vector<IA*>sultans);
	bool isBlocking();
	bool isSwordOut();
	bool canBeHit();

	void addEasterEggPoint() {
		++easterEggPoint;
	}

	int getHealth() {
		return healthPoints;
	}

	int getEasterEggPoints() {
		return easterEggPoint;
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

	vector<IA*> sultans;


	int healthPoints;
	int easterEggPoint;
	string lastDamageType;
	float jumped;
	bool fallStraight;
	bool saved; // quan guardes la espasa voluntariament
	

};

#endif // _PLAYER_INCLUDE


