#ifndef _TORCH_INCLUDE
#define _TORCH_INCLUDE

#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "Texture.h"
#include "Sprite.h"

class Torch
{

public:

	void init(const glm::vec2 torchPos, glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	void update(int deltaTime);
	void render();

	void setPosition(const glm::vec2 &pos);


private:
	glm::ivec2 tileMapDispl;
	glm::vec2 posTorch;
	Texture spritesheet;
	Sprite *sprite;

};

#endif // _TORCH_INCLUDE

