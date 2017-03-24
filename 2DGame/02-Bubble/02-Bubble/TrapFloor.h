#include "Sprite.h"
#include "TileMap.h"

class TrapFloor
{

public:
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	void update(int deltaTime);
	void render();

	void setPlayer(Player *player);
	void setPosition(const glm::vec2 &pos);
	float TrapFloor::GetScreenX(int widthScreen);
	float TrapFloor::GetScreenY(int heightScreen);

private:
	glm::ivec2 tileMapDispl;
	glm::vec2 posTrap;

	Texture spritesheet;
	Sprite *sprite;
	Player *player;

	bool bUp;


};




