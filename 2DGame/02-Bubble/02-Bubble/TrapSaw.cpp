#include "trapSaw.h"
#include <iostream>
#include <GL/glut.h>
#include "Player.h"


#define JUMP_ANGLE_STEP 4
#define JUMP_HEIGHT 0
#define FALL_STEP 4

enum PlayerAnims
{
	UNACTIVE, MOVING, ACTIVE
};

void TrapSaw::init(const glm::vec2 &pos, const glm::ivec2 &tileMapPos, Player *player, ShaderProgram &shaderProgram)
{

	// Configuring the spritesheet
	spritesheet.loadFromFile("images/TrapsSheet.png", TEXTURE_PIXEL_FORMAT_RGBA);

	// Configuring a single sprite
	sprite = Sprite::createSprite(glm::ivec2(64, 64), glm::vec2(0.1f, 0.1f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(3);

	//KeyFrames
	sprite->setAnimationSpeed(UNACTIVE, 5);
	sprite->addKeyframe(UNACTIVE, glm::vec2(0.f, 0.4f));

	sprite->setAnimationSpeed(MOVING, 10);
	sprite->addKeyframe(MOVING, glm::vec2(0.1f, 0.4f));
	sprite->addKeyframe(MOVING, glm::vec2(0.2f, 0.4f));
	

	sprite->setAnimationSpeed(ACTIVE, 10);
	sprite->addKeyframe(ACTIVE, glm::vec2(0.3f, 0.4f));

	this->player = player;
	//Init sprite and position
	sprite->changeAnimation(UNACTIVE);
	tileMapDispl = tileMapPos;
	posTrap = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posTrap.x), float(tileMapDispl.y + posTrap.y)));

	bUp = false;

}

void TrapSaw::restart() {
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
				(int(player->getPosition().x / 32) == int(posTrap.x / 32) - 1))
				sprite->changeAnimation(MOVING);
			break;

		case MOVING:

			if (bUp) sprite->changeAnimation(UNACTIVE);
			else sprite->changeAnimation(ACTIVE);
			break;

		case ACTIVE:
			bUp = true;
			if ((player->getPosition().y - posTrap.y > 0 || int(player->getPosition().x / 32) != int(posTrap.x / 32) &&
				(int(player->getPosition().x / 32) != int(posTrap.x / 32) - 1 || int(player->getPosition().x / 16) % 2 == 0)) && player->getHealth() != 0)
				sprite->changeAnimation(MOVING);
			else if (player->getHealth() != 0) {
				bUp = false; sprite->changeAnimation(MOVING);
			}
			break;
		}
	}

	if (sprite->animation() == ACTIVE && AmISteppingOn()) player->dealDamage(4, "saw");

}

void TrapSaw::render()
{
	sprite->render();
}

bool TrapSaw::AmISteppingOn() const {

	int x, y;

	x = (player->getPosition().x + (32 - 1)) / 32;
	y = (player->getPosition().y) / 64;

	if (x == (posTrap.x / 32) && y == (posTrap.y / 64)) return true;

	return false;

}