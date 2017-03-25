#ifndef _TRAPSAW_INCLUDE
#define _TRAPSAW_INCLUDE

#include "Sprite.h"
#include "Player.h"


class TrapSaw
{

public:

	void init(const glm::vec2 &pos, const glm::ivec2 &tileMapPos, Player *player, ShaderProgram &shaderProgram);
	void update(int deltaTime);
	void render();

	void setPlayer(Player *player);
	void setPosition(const glm::vec2 &pos);


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
