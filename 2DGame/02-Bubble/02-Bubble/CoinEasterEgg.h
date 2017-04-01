#ifndef _COINEASTEREGG_INCLUDE
#define _COINEASTEREGG_INCLUDE

#include "Sprite.h"
#include "Player.h"


class CoinEasterEgg
{

public:

	void init(const glm::vec2 &pos, const glm::ivec2 &tileMapPos, Player *player, ShaderProgram &shaderProgram);
	void update(int deltaTime);
	void render();
	void restart();

private:
	bool AmISteppingOn() const;

private:
	glm::ivec2 tileMapDispl;
	glm::vec2 posCoin;
	bool bTaken, bDirectionUp;
	float moveAxisY;
	Texture spritesheet;
	Sprite *sprite;
	Player *player;
};

#endif // _TRAPSTEELBARS_INCLUDE
