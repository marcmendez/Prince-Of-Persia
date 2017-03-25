#include "trapSaw.h"
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
	UNACTIVE, MOVING, ACTIVE
};

void TrapSaw::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{

	// Configuring the spritesheet
	spritesheet.loadFromFile("images/TrapsSheet.png", TEXTURE_PIXEL_FORMAT_RGBA);

	// Configuring a single sprite
	sprite = Sprite::createSprite(glm::ivec2(64, 64), glm::vec2(0.1f, 0.1f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(3);

	//KeyFrames
	sprite->setAnimationSpeed(UNACTIVE, 10);
	sprite->addKeyframe(UNACTIVE, glm::vec2(0.f, 0.9f));

	sprite->setAnimationSpeed(MOVING, 10);
	sprite->addKeyframe(MOVING, glm::vec2(0.1f, 0.3f));
	sprite->addKeyframe(MOVING, glm::vec2(0.2f, 0.3f));
	sprite->addKeyframe(MOVING, glm::vec2(0.3f, 0.3f));

	sprite->setAnimationSpeed(ACTIVE, 10);
	sprite->addKeyframe(ACTIVE, glm::vec2(0.4f, 0.3f));

	//Init sprite and position
	sprite->changeAnimation(UNACTIVE);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x), float(tileMapDispl.y)));

	bUp = false;

}

void TrapSaw::update(int deltaTime)
{

	bool finishAction = sprite->update(deltaTime);

	if (finishAction) {

		switch (sprite->animation()) {

		case UNACTIVE:
			bUp = false;

			if (player->getPosition().y - posTrap.y <= 0 && int(player->getPosition().x / 32) == int(posTrap.x / 32) ||
				(int(player->getPosition().x / 32) == int(posTrap.x / 32) - 1 && int(player->getPosition().x / 16) % 2 == 1))
				sprite->changeAnimation(MOVING);
			break;

		case MOVING:

			if (bUp) sprite->changeAnimation(UNACTIVE);
			else sprite->changeAnimation(ACTIVE);
			break;

		case ACTIVE:
			bUp = true;
			if (player->getPosition().y - posTrap.y > 0 || int(player->getPosition().x / 32) != int(posTrap.x / 32) &&
				(int(player->getPosition().x / 32) != int(posTrap.x / 32) - 1 || int(player->getPosition().x / 16) % 2 == 0))
				sprite->changeAnimation(UNACTIVE);
			break;
		}
	}

	if (sprite->animation() == ACTIVE && AmISteppingOn()) player->dealDamage(4);

}

void TrapSaw::render()
{
	sprite->render();
}

void TrapSaw::setPlayer(Player *player)
{
	this->player = player;
}

void TrapSaw::setPosition(const glm::vec2 &pos)
{
	posTrap = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posTrap.x), float(tileMapDispl.y + posTrap.y)));
}

bool TrapSaw::AmISteppingOn() const {

	int x0, x1, y;

	x0 = player->getPosition().x / 32;
	x1 = (player->getPosition().x + (32 - 1)) / 32;
	y = (player->getPosition().y) / 64;

	if (x0 == (posTrap.x / 32) && y == (posTrap.y / 64)) return true;
	else if (x1 == (posTrap.x / 32) && y == (posTrap.y / 64)) return true;

	return false;

}