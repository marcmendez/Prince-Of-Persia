#ifndef _PRINCESS_INCLUDE
#define _PRINCESS_INCLUDE

#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "Texture.h"
#include "Sprite.h"

class Princess
{

public:

	void init(string spritesheetResource, glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	void update(int deltaTime);
	void render();
	bool finished() { return bFinished; }
	void setStarted(bool state) { bStarted = state; }
	void restart();
	bool peach() { return bPeach;  }

private:
	Texture spritesheet;
	Sprite *sprite;
	bool bFinished;
	bool bStarted;
	bool bPeach;

};

#endif // _PRINCESS_INCLUDE

