#ifndef _TRAPSTEELBARS_INCLUDE
#define _TRAPSTEELBARS_INCLUDE

#include "Sprite.h"
#include "Player.h"


class TrapSteelBars
{

public:

	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	void update(int deltaTime);
	void render();

	void setPlayer(Player *player);
	void setPosition(const glm::vec2 &pos);

	float TrapSteelBars::GetScreenX(int widthScreen);
	float TrapSteelBars::GetScreenY(int heightScreen);

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
