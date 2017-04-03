#include "TrapSteelBars.h"
#include <iostream>
#include <GL/glut.h>

enum PlayerAnims
{
	DOWN, MOVING, UP
};

void TrapSteelBars::init(const glm::vec2 &pos, const glm::ivec2 &tileMapPos, Player *player, ShaderProgram &shaderProgram) {

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

	this->player = player;

	//Init sprite and position
	sprite->changeAnimation(DOWN);
	tileMapDispl = tileMapPos;
	posTrap = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x) + posTrap.x, float(tileMapDispl.y) + posTrap.y));

	bUp = false;

}

void TrapSteelBars::restart() {

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

	if (sprite->animation() == UP && AmISteppingOn() && player->canBeHit()) player->dealDamage(4, "steelbars");

}

void TrapSteelBars::render()
{
	sprite->render();
}

bool TrapSteelBars::AmISteppingOn() const {

	int x0, x1, y;

	x0 = player->getPosition().x / 32;
	x1 = (player->getPosition().x + (32 - 6)) / 32;
	y = (player->getPosition().y) / 64;

	if (x0 == (posTrap.x / 32) && y == (posTrap.y / 64)) return true;
	else if (x1 == (posTrap.x / 32) && y == (posTrap.y / 64)) return true;

	return false;

}