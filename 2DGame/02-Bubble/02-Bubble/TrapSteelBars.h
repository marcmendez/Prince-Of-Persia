#ifndef _TRAPSTEELBARS_INCLUDE
#define _TRAPSTEELBARS_INCLUDE

#include "Sprite.h"
#include "Player.h"


class TrapSteelBars
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
	glm::vec2 posTrap;

	Texture spritesheet;
	Sprite *sprite;
	Player *player;

	bool bUp;
};

#endif // _TRAPSTEELBARS_INCLUDE
