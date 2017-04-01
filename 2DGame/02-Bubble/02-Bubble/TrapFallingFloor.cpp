#include "TrapFallingFloor.h"
#include <GL/glut.h>
#include "Player.h"
#include "TileMap.h"

enum TrapFallingFloorAnims
{
	NORMAL, SHAKE, FALLING, BROKEN
};

void TrapFallingFloor::init(glm::vec2 &trapPos, const glm::ivec2 &tileMapPos, Player *player, TileMap *map, ShaderProgram &shaderProgram) {

	// Configuring the spritesheet
	spritesheet.loadFromFile("images/TrapsSheet.png", TEXTURE_PIXEL_FORMAT_RGBA);

	// Configuring a single sprite
	sprite = Sprite::createSprite(glm::ivec2(64, 64), glm::vec2(0.1f, 0.1f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(5);

	if (trapPos.y <= 512) {

		//KeyFrames
		sprite->setAnimationSpeed(NORMAL, 8);
		sprite->addKeyframe(NORMAL, glm::vec2(0.f, 0.2f));

		sprite->setAnimationSpeed(SHAKE, 3);
		sprite->addKeyframe(SHAKE, glm::vec2(0.1f, 0.2f));
		sprite->addKeyframe(SHAKE, glm::vec2(0.2f, 0.2f));

		sprite->setAnimationSpeed(FALLING, 8);
		sprite->addKeyframe(FALLING, glm::vec2(0.f, 0.2f));

		sprite->setAnimationSpeed(BROKEN, 8);
		sprite->addKeyframe(BROKEN, glm::vec2(0.3f, 0.2f));

	}
	else {

		sprite->setAnimationSpeed(NORMAL, 8);
		sprite->addKeyframe(NORMAL, glm::vec2(0.4f, 0.2f));

		sprite->setAnimationSpeed(SHAKE, 3);
		sprite->addKeyframe(SHAKE, glm::vec2(0.5f, 0.2f));
		sprite->addKeyframe(SHAKE, glm::vec2(0.6f, 0.2f));

		sprite->setAnimationSpeed(FALLING, 8);
		sprite->addKeyframe(FALLING, glm::vec2(0.4f, 0.2f));

		sprite->setAnimationSpeed(BROKEN, 8);
		sprite->addKeyframe(BROKEN, glm::vec2(0.7f, 0.2f));


	}


	//Init map and player
	this->map = map;
	this->player = player;

	//Init sprite and position
	sprite->changeAnimation(NORMAL);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + trapPos.x), float(tileMapDispl.y + trapPos.y)));

	map->addTrapCollision(tileMapDispl.x + trapPos.x, tileMapDispl.y + trapPos.y, 1);

	posTrap = trapPos;
	bDetected = false;
	fallen = 0;
}

void TrapFallingFloor::restart() {
	bDetected = false;
	if (fallen != 0) { 
		map->addTrapCollision(tileMapDispl.x + posTrap.x, tileMapDispl.y + posTrap.y, 1); 
		fallen = 0;
	}
	sprite->changeAnimation(NORMAL);
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posTrap.x), float(tileMapDispl.y + posTrap.y)));
}
void TrapFallingFloor::update(int deltaTime)
{

	bool finishAction = sprite->update(deltaTime);

	if (finishAction) {

		switch (sprite->animation()) {

		case NORMAL:

			if (!bDetected && int(player->getPosition().x / 32) <= int(posTrap.x / 32) + 2 && int(player->getPosition().x / 32) >= int(posTrap.x / 32) - 3)
				sprite->changeAnimation(SHAKE);
			if (bDetected && AmISteppingOn())
				sprite->changeAnimation(SHAKE);
			break;

		case SHAKE:

			if (!bDetected) sprite->changeAnimation(NORMAL);
			else sprite->changeAnimation(FALLING);
			bDetected = true;

			break;

		case FALLING:

			map->deleteTrapCollision(posTrap.x, posTrap.y + fallen);
			fallen +=32;
			sprite->setPosition(glm::vec2(float(tileMapDispl.x + posTrap.x), float(tileMapDispl.y + posTrap.y + fallen)));
			if (map->collisionMoveDown(posTrap.x, posTrap.y + fallen, glm::ivec2(32, 64), 'l')) sprite->changeAnimation(BROKEN);
			break;

		}
	}

}

void TrapFallingFloor::render()
{
	sprite->render();
}

bool TrapFallingFloor::AmISteppingOn() const {

	int x0, x1, y;

	x0 = player->getPosition().x / 32;
	x1 = (player->getPosition().x + (32 - 1)) / 32;
	y = (player->getPosition().y) / 64;

	if (x0 == (posTrap.x / 32) && y == (posTrap.y / 64)) return true;
	else if (x1 == (posTrap.x / 32) && y == (posTrap.y / 64)) return true;

	return false;

}

