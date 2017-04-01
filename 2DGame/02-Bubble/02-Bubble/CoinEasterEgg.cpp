#include "CoinEasterEgg.h"
#include <iostream>
#include <GL/glut.h>

enum PlayerAnims
{
	COIN, NO_COIN
};

void CoinEasterEgg::init(const glm::vec2 &pos, const glm::ivec2 &tileMapPos, Player *player, ShaderProgram &shaderProgram) {

	// Configuring the spritesheet
	spritesheet.loadFromFile("images/SpriteSheetEasterEgg.png", TEXTURE_PIXEL_FORMAT_RGBA);

	// Configuring a single sprite
	sprite = Sprite::createSprite(glm::ivec2(32,32), glm::vec2(0.1f, 0.5f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(2);

	//KeyFrames

	if (pos.y >= 512) {
		sprite->setAnimationSpeed(COIN, 5);
		sprite->addKeyframe(COIN, glm::vec2(0.0f, 0.5f));
		sprite->addKeyframe(COIN, glm::vec2(0.1f, 0.5f));
		sprite->addKeyframe(COIN, glm::vec2(0.2f, 0.5f));
		sprite->addKeyframe(COIN, glm::vec2(0.3f, 0.5f));
		sprite->addKeyframe(COIN, glm::vec2(0.4f, 0.5f));
	}
	else {
		sprite->setAnimationSpeed(COIN, 5);
		sprite->addKeyframe(COIN, glm::vec2(0.5f, 0.5f));
		sprite->addKeyframe(COIN, glm::vec2(0.6f, 0.5f));
		sprite->addKeyframe(COIN, glm::vec2(0.7f, 0.5f));
		sprite->addKeyframe(COIN, glm::vec2(0.8f, 0.5f));
		sprite->addKeyframe(COIN, glm::vec2(0.5f, 0.5f));

	}

	sprite->setAnimationSpeed(NO_COIN, 10);
	sprite->addKeyframe(NO_COIN, glm::vec2(0.9f, 0.0f));

	this->player = player;

	//Init sprite and position
	sprite->changeAnimation(COIN);
	tileMapDispl = tileMapPos;
	posCoin = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x) + posCoin.x, float(tileMapDispl.y) + posCoin.y));
	bTaken = false;
	bDirectionUp = true;
	moveAxisY = 0;
}

void CoinEasterEgg::restart() {

	sprite->setPosition(glm::vec2(float(tileMapDispl.x) + posCoin.x, float(tileMapDispl.y) + posCoin.y));
	bTaken = false;
	bDirectionUp = true;
	moveAxisY = 0;

	sprite->changeAnimation(COIN);

}

void CoinEasterEgg::update(int deltaTime)
{

	if (AmISteppingOn() && !bTaken) { player->addEasterEggPoint(); bTaken = true; sprite->changeAnimation(NO_COIN); }
	
	if (moveAxisY < 32 && bDirectionUp) moveAxisY +=1.5;
	if (moveAxisY >= 32) bDirectionUp = false;
	if (moveAxisY > 0 && !bDirectionUp) moveAxisY -=1.5;
	if (moveAxisY <= 0) bDirectionUp = true;

	sprite->update(deltaTime);
	sprite->setPosition(glm::vec2(float(tileMapDispl.x) + posCoin.x + 5, float(tileMapDispl.y) + posCoin.y - 8 + moveAxisY));

}

void CoinEasterEgg::render()
{
	sprite->render();
}

bool CoinEasterEgg::AmISteppingOn() const {

	int x0, x1, y;

	x0 = player->getPosition().x / 32;
	x1 = (player->getPosition().x + (32 - 1)) / 32;
	y = (player->getPosition().y) / 64;

	if (x0 == (posCoin.x / 32) && y == (posCoin.y / 64)) return true;
	else if (x1 == (posCoin.x / 32) && y == (posCoin.y / 64)) return true;

	return false;

}