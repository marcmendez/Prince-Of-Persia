#include "TrapSteelBars.h"
#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Game.h"
#include "Player.h"


#define JUMP_ANGLE_STEP 4
#define JUMP_HEIGHT 0
#define FALL_STEP 4

enum PlayerAnims
{
	DOWN, MOVING, UP
};

void TrapSteelBars::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{

	// Configuring the spritesheet
	spritesheet.loadFromFile("images/TrapsSheet.png", TEXTURE_PIXEL_FORMAT_RGBA);

	// Configuring a single sprite
	sprite = Sprite::createSprite(glm::ivec2(64, 64), glm::vec2(0.1f, 0.1f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(3);

	//KeyFrames
	sprite->setAnimationSpeed(DOWN, 10);
	sprite->addKeyframe(DOWN, glm::vec2(0.f, 0.9f));

	sprite->setAnimationSpeed(MOVING, 10);
	sprite->addKeyframe(MOVING, glm::vec2(0.1f, 0.3f));
	sprite->addKeyframe(MOVING, glm::vec2(0.2f, 0.3f));
	sprite->addKeyframe(MOVING, glm::vec2(0.3f, 0.3f));

	sprite->setAnimationSpeed(UP, 10);
	sprite->addKeyframe(UP, glm::vec2(0.4f, 0.3f));

	//Init sprite and position
	sprite->changeAnimation(DOWN);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x), float(tileMapDispl.y)));

	bUp = false;

}

void TrapSteelBars::update(int deltaTime)
{

	bool finishAction = sprite->update(deltaTime);

	if (finishAction) {

		switch (sprite->animation()) {

		case DOWN:
			bUp = false;

			if (player->getPosition().y - posTrap.y <= 0 && int(player->getPosition().x / 32) == int(posTrap.x / 32) ||
				(int(player->getPosition().x / 32) == int(posTrap.x / 32) - 1 && int(player->getPosition().x / 16) % 2 == 1))
				sprite->changeAnimation(MOVING);
			break;

		case MOVING:

			if (bUp) sprite->changeAnimation(DOWN);
			else sprite->changeAnimation(UP);
			break;

		case UP:
			bUp = true;
			if (player->getPosition().y - posTrap.y > 0 || int(player->getPosition().x / 32) != int(posTrap.x / 32) &&
				(int(player->getPosition().x / 32) != int(posTrap.x / 32) - 1 || int(player->getPosition().x / 16) % 2 == 0)) 
				sprite->changeAnimation(DOWN);
			break;
		}
	}

	if (sprite->animation() == UP && AmISteppingOn()) player->dealDamage(4, "steelbars");

}

void TrapSteelBars::render()
{
	sprite->render();
}

void TrapSteelBars::setPlayer(Player *player)
{
	this->player = player;
}

void TrapSteelBars::setPosition(const glm::vec2 &pos)
{
	posTrap = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posTrap.x), float(tileMapDispl.y + posTrap.y)));
}

float TrapSteelBars::GetScreenX(int widthScreen) {

	return float(tileMapDispl.x + posTrap.x) / widthScreen;

}

float TrapSteelBars::GetScreenY(int heightScreen) {

	return float(tileMapDispl.y + posTrap.y) / heightScreen;

}

bool TrapSteelBars::AmISteppingOn() const {

	int x0, x1, y;

	x0 = player->getPosition().x / 32;
	x1 = (player->getPosition().x + (32 - 1)) / 32;
	y = (player->getPosition().y) / 64;

	if (x0 == (posTrap.x / 32) && y == (posTrap.y / 64)) return true;
	else if (x1 == (posTrap.x / 32) && y == (posTrap.y / 64)) return true;

	return false;

}