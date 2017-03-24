#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "TrapFloor.h"
#include "Game.h"


#define JUMP_ANGLE_STEP 4
#define JUMP_HEIGHT 0
#define FALL_STEP 4


enum PlayerAnims
{
	DOWN, MOVING, UP
};


void TrapFloor::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{

	// Configuring the spritesheet
	spritesheet.loadFromFile("images/SpritesheetTrapFloor.png", TEXTURE_PIXEL_FORMAT_RGBA);

	// Configuring a single sprite
	sprite = Sprite::createSprite(glm::ivec2(64, 64), glm::vec2(0.1, 0.1), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(26);

	//Init sprite and position
	sprite->changeAnimation(1);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x), float(tileMapDispl.y)));

	bUp = false;

}

void TrapFloor::update(int deltaTime)
{

	bool finishAction = sprite->update(deltaTime);

	if (finishAction) {

		switch (sprite->animation()) {

		case DOWN:
			bUp = false;
			if (player->getPosition().y - posTrap.y <=  0 && int(player->getPosition().x/32) == int(posTrap.x/32)) 
				sprite->changeAnimation(MOVING);
			break;

		case MOVING:
			
			if(bUp) sprite->changeAnimation(DOWN);
			else sprite->changeAnimation(UP);
			break;

		case UP:
			bUp = true;
			if (player->getPosition().y - posTrap.y > 0 || int(player->getPosition().x / 32) != int(posTrap.x / 32)) sprite->changeAnimation(DOWN);
			break;
		}
	}
}

void TrapFloor::render()
{
	sprite->render();
}

void TrapFloor::setPlayer(Player *player)
{
	this->player = player;
}

void TrapFloor::setPosition(const glm::vec2 &pos)
{
	posTrap = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posTrap.x), float(tileMapDispl.y + posTrap.y)));
}

float TrapFloor::GetScreenX(int widthScreen) {

	return float(tileMapDispl.x + posTrap.x) / widthScreen;

}

float TrapFloor::GetScreenY(int heightScreen) {

	return float(tileMapDispl.y + posTrap.y) / heightScreen;

}





