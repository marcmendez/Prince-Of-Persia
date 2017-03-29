#include "TrapDoor.h"
#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Game.h"
#include "Player.h"

enum PressurePlateAnims { PRESSED, UNPRESSED };
enum Door { CLOSED, MOVING_UP, MOVING_DOWN, OPEN };

void TrapDoor::init(glm::vec2 &doorPos, glm::vec2 &pressurePlatePos, const glm::ivec2 &tileMapPos, Player *player, TileMap *map, ShaderProgram &shaderProgram) {

	// Configuring the spritesheet
	spritesheet.loadFromFile("images/TrapsSheet.png", TEXTURE_PIXEL_FORMAT_RGBA);

	// Configuring a pressure plate sprite
	spritePressurePlate = Sprite::createSprite(glm::ivec2(64, 64), glm::vec2(0.1f, 0.1f), &spritesheet, &shaderProgram);
	spritePressurePlate->setNumberAnimations(2);

	//KeyFrames pressure plate sprite
	spritePressurePlate->setAnimationSpeed(UNPRESSED, 8);
	spritePressurePlate->addKeyframe(UNPRESSED, glm::vec2(0.1f, 0.f));

	spritePressurePlate->setAnimationSpeed(PRESSED, 8);
	spritePressurePlate->addKeyframe(PRESSED, glm::vec2(0.0f, 0.f));

	// Configuring a door sprite

	spriteDoor = Sprite::createSprite(glm::ivec2(64, 64), glm::vec2(0.1f, 0.1f), &spritesheet, &shaderProgram);
	spriteDoor->setNumberAnimations(4);

	spriteDoor->setAnimationSpeed(CLOSED, 8);
	spriteDoor->addKeyframe(CLOSED, glm::vec2(0.0f, 0.1f));

	spriteDoor->setAnimationSpeed(MOVING_UP, 8);
	spriteDoor->addKeyframe(MOVING_UP, glm::vec2(0.1f, 0.1f));
	spriteDoor->addKeyframe(MOVING_UP, glm::vec2(0.2f, 0.1f));
	spriteDoor->addKeyframe(MOVING_UP, glm::vec2(0.3f, 0.1f));
	spriteDoor->addKeyframe(MOVING_UP, glm::vec2(0.4f, 0.1f));
	spriteDoor->addKeyframe(MOVING_UP, glm::vec2(0.5f, 0.1f));;
	spriteDoor->addKeyframe(MOVING_UP, glm::vec2(0.6f, 0.1f));

	spriteDoor->setAnimationSpeed(MOVING_DOWN, 8);
	spriteDoor->addKeyframe(MOVING_DOWN, glm::vec2(0.6f, 0.1f));
	spriteDoor->addKeyframe(MOVING_DOWN, glm::vec2(0.5f, 0.1f));
	spriteDoor->addKeyframe(MOVING_DOWN, glm::vec2(0.4f, 0.1f));
	spriteDoor->addKeyframe(MOVING_DOWN, glm::vec2(0.3f, 0.1f));
	spriteDoor->addKeyframe(MOVING_DOWN, glm::vec2(0.2f, 0.1f));
	spriteDoor->addKeyframe(MOVING_DOWN, glm::vec2(0.1f, 0.1f));

	spriteDoor->setAnimationSpeed(OPEN, 1);
	spriteDoor->addKeyframe(OPEN, glm::vec2(0.7f, 0.1f));
	spriteDoor->addKeyframe(OPEN, glm::vec2(0.7f, 0.1f));
	spriteDoor->addKeyframe(OPEN, glm::vec2(0.7f, 0.1f));
	spriteDoor->addKeyframe(OPEN, glm::vec2(0.7f, 0.1f));

	this->player = player;
	this->map = map;

	//Init sprite and position
	spritePressurePlate->changeAnimation(UNPRESSED);
	spriteDoor->changeAnimation(CLOSED);

	tileMapDispl = tileMapPos;
	posDoor = doorPos; posPressurePlate = pressurePlatePos;

	spritePressurePlate->setPosition(glm::vec2(float(tileMapDispl.x + pressurePlatePos.x), float(tileMapDispl.y + pressurePlatePos.y)));
	spriteDoor->setPosition(glm::vec2(float(tileMapDispl.x + doorPos.x + 32), float(tileMapDispl.y + doorPos.y - 10)));

	map->addTrapCollision(doorPos.x, doorPos.y, 2);
	bPressed = false;

}

void TrapDoor::update(int deltaTime) {

	bool finishActionPressurePlate = spritePressurePlate->update(deltaTime);

	if (finishActionPressurePlate) {

		switch (spritePressurePlate->animation()) {

		case UNPRESSED:
			if (AmISteppingOn(player->getPosition().x, player->getPosition().y, glm::ivec2(64, 64))) spritePressurePlate->changeAnimation(PRESSED);
			break;
		case PRESSED:
			bPressed = true;
			if (!AmISteppingOn(player->getPosition().x, player->getPosition().y, glm::ivec2(64, 64))) spritePressurePlate->changeAnimation(UNPRESSED);
			break;
		}
	}

	bool finishActionDoor = spriteDoor->update(deltaTime);

	if (finishActionDoor) {

		switch (spriteDoor->animation()) {

		case CLOSED:
			if (bPressed) spriteDoor->changeAnimation(MOVING_UP);
			map->addTrapCollision(posDoor.x, posDoor.y, 2);
			break;
		case MOVING_UP:
			map->deleteTrapCollision(posDoor.x, posDoor.y);
			spriteDoor->changeAnimation(OPEN);
			break;
		case MOVING_DOWN:
			map->addTrapCollision(posDoor.x, posDoor.y, 2);
			spriteDoor->changeAnimation(CLOSED);
			break;
		case OPEN:
			if (!AmISteppingOn(player->getPosition().x, player->getPosition().y, glm::ivec2(64, 64))) {
				bPressed = false;
				spriteDoor->changeAnimation(MOVING_DOWN);
			}
		}

	}

}

void TrapDoor::render() {
	spriteDoor->render();
	spritePressurePlate->render();
}

bool TrapDoor::AmISteppingOn(int posx, int posy, const glm::ivec2 &size) const {

	int x0, x1, y;

	x0 = posx / 32;
	x1 = (posx + (32 - 1)) / 32;
	y = (posy) / 64;

	if (x0 == (posPressurePlate.x / 32) && y == (posPressurePlate.y / 64)) return true;
	else if (x1 == (posPressurePlate.x / 32) && y == (posPressurePlate.y / 64)) return true;

	return false;

}