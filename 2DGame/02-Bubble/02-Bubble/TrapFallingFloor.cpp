#include "TrapFallingFloor.h"
#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Game.h"
#include "Player.h"
#include "TileMap.h"

#define JUMP_ANGLE_STEP 4
#define JUMP_HEIGHT 0
#define FALL_STEP 4

enum PlayerAnims
{
	NORMAL, SHAKE, FALLING, BROKEN
};

void TrapFallingFloor::init(glm::vec2 &trapPos ,const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{

	// Configuring the spritesheet
	spritesheet.loadFromFile("images/TrapsSheet.png", TEXTURE_PIXEL_FORMAT_RGBA);

	// Configuring a single sprite
	sprite = Sprite::createSprite(glm::ivec2(64, 64), glm::vec2(0.1f, 0.1f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(5);

	//KeyFrames
	sprite->setAnimationSpeed(NORMAL, 8);
	sprite->addKeyframe(NORMAL, glm::vec2(0.f, 0.2f));

	sprite->setAnimationSpeed(SHAKE, 5.5);
	sprite->addKeyframe(SHAKE, glm::vec2(0.1f, 0.2f));
	sprite->addKeyframe(SHAKE, glm::vec2(0.2f, 0.2f));

	sprite->setAnimationSpeed(FALLING, 8);
	sprite->addKeyframe(FALLING, glm::vec2(0.f, 0.2f));

	sprite->setAnimationSpeed(BROKEN, 8);
	sprite->addKeyframe(BROKEN, glm::vec2(0.3f, 0.2f));

	//Init sprite and position
	sprite->changeAnimation(NORMAL);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + trapPos.x), float(tileMapDispl.y + trapPos.y)));

	map->addTrapCollision(tileMapDispl.x + trapPos.x, tileMapDispl.y + trapPos.y);

	posTrap = trapPos;
	bDetected = false;

}

void TrapFallingFloor::update(int deltaTime)
{

	bool finishAction = sprite->update(deltaTime);

	if (finishAction) {

		switch (sprite->animation()) {

		case NORMAL:

			if (!bDetected && int(player->getPosition().x / 32) <= int(posTrap.x / 32) + 2 && int(player->getPosition().x / 32) >= int(posTrap.x / 32) - 3)
				sprite->changeAnimation(SHAKE);
			if (bDetected && AmISteppingOn(player->getPosition().x, player->getPosition().y, glm::ivec2(64, 64))) 
				sprite->changeAnimation(SHAKE);
			break;

		case SHAKE:

			if (!bDetected) sprite->changeAnimation(NORMAL);
			else sprite->changeAnimation(FALLING);
			bDetected = true;

			break;

		case FALLING:

			map->deleteTrapCollision(posTrap.x, posTrap.y);
			posTrap.y +=35;
			sprite->setPosition(glm::vec2(float(tileMapDispl.x + posTrap.x), float(tileMapDispl.y + posTrap.y)));
			if (map->collisionMoveDown(posTrap.x, posTrap.y, glm::ivec2(32, 64), 'l')) sprite->changeAnimation(BROKEN);
			break;

		}
	}

}

void TrapFallingFloor::render()
{
	sprite->render();
}

void TrapFallingFloor::setPlayer(Player *player)
{
	this->player = player;
}

void TrapFallingFloor::setTileMap(TileMap *map) { 

	this->map = map; 

}

float TrapFallingFloor::GetScreenX(int widthScreen) {

	return float(tileMapDispl.x + posTrap.x) / widthScreen;

}

float TrapFallingFloor::GetScreenY(int heightScreen) {

	return float(tileMapDispl.y + posTrap.y) / heightScreen;

}

bool TrapFallingFloor::AmISteppingOn(int posx, int posy, const glm::ivec2 &size) const {

	int x0, x1, y;

	x0 = posx / 32;
	x1 = (posx + (size.x - 1)) / 32;
	y = (posy) / 64;

	if (x0 == (posTrap.x / 32) && y == (posTrap.y / 64)) return true;
	else if (x1 == (posTrap.x / 32) && y == (posTrap.y / 64)) return true;

	return false;

}