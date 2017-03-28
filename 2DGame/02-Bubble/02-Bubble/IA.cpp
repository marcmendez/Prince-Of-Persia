#include "IA.h"
#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Game.h"
#include "IA.h"


#define IA_VISION 32*6
#define ATTACK_RANGE 32



enum PlayerAnims
{
	STAND_RIGHT, STAND_LEFT,
	MOVE_RIGHT, MOVE_LEFT,
	BLOCK_RIGHT, BLOCK_LEFT,
	ATTACK_RIGHT, ATTACK_LEFT,
	DYING_RIGHT, DYING_LEFT,
	DEAD_LEFT, DEAD_RIGHT
};

void IA::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{

	// Configuring the spritesheet
	spritesheet.setWrapS(GL_MIRRORED_REPEAT);
	spritesheet.loadFromFile("images/spriteSheetEnemy.png", TEXTURE_PIXEL_FORMAT_RGBA);

	// Configuring a single sprite
	sprite = Sprite::createSprite(glm::ivec2(64, 64), glm::vec2(0.1f, 0.1f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(12);

	//KeyFrames
	sprite->setAnimationSpeed(STAND_RIGHT, 8);
	sprite->addKeyframe(STAND_RIGHT, glm::vec2(0.6f, 0.0f));

	sprite->setAnimationSpeed(STAND_LEFT, 8);
	sprite->addKeyframe(STAND_LEFT, glm::vec2(-0.7f, 0.0f));

	sprite->setAnimationSpeed(MOVE_RIGHT, 8);
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.2f, 0.0f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.3f, 0.0f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.4f, 0.0f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.5f, 0.0f));

	sprite->setAnimationSpeed(MOVE_LEFT, 8);
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(-0.3f, 0.0f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(-0.4f, 0.0f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(-0.5f, 0.0f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(-0.6f, 0.0f));

	sprite->setAnimationSpeed(BLOCK_RIGHT, 8);
	sprite->addKeyframe(BLOCK_RIGHT, glm::vec2(0.6f, 0.0f));
	sprite->addKeyframe(BLOCK_RIGHT, glm::vec2(0.7f, 0.0f));
	sprite->addKeyframe(BLOCK_RIGHT, glm::vec2(0.8f, 0.0f));
	sprite->addKeyframe(BLOCK_RIGHT, glm::vec2(0.9f, 0.0f));

	sprite->setAnimationSpeed(BLOCK_LEFT, 8);
	sprite->addKeyframe(BLOCK_LEFT, glm::vec2(-0.7f, 0.0f));
	sprite->addKeyframe(BLOCK_LEFT, glm::vec2(-0.8f, 0.0f));
	sprite->addKeyframe(BLOCK_LEFT, glm::vec2(-0.9f, 0.0f));
	sprite->addKeyframe(BLOCK_LEFT, glm::vec2(-1.0f, 0.0f));

	sprite->setAnimationSpeed(ATTACK_RIGHT, 6);
	sprite->addKeyframe(ATTACK_RIGHT, glm::vec2(0.0f, 0.1f));
	sprite->addKeyframe(ATTACK_RIGHT, glm::vec2(0.1f, 0.1f));
	sprite->addKeyframe(ATTACK_RIGHT, glm::vec2(0.2f, 0.1f));
	sprite->addKeyframe(ATTACK_RIGHT, glm::vec2(0.3f, 0.1f));
	sprite->addKeyframe(ATTACK_RIGHT, glm::vec2(0.4f, 0.1f));

	sprite->setAnimationSpeed(ATTACK_LEFT, 6);
	sprite->addKeyframe(ATTACK_LEFT, glm::vec2(-0.1f, 0.1f));
	sprite->addKeyframe(ATTACK_LEFT, glm::vec2(-0.2f, 0.1f));
	sprite->addKeyframe(ATTACK_LEFT, glm::vec2(-0.3f, 0.1f));
	sprite->addKeyframe(ATTACK_LEFT, glm::vec2(-0.4f, 0.1f));
	sprite->addKeyframe(ATTACK_LEFT, glm::vec2(-0.5f, 0.1f));

	sprite->setAnimationSpeed(DYING_RIGHT, 8);
	sprite->addKeyframe(DYING_RIGHT, glm::vec2(0.5f, 0.1f));
	sprite->addKeyframe(DYING_RIGHT, glm::vec2(0.6f, 0.1f));
	sprite->addKeyframe(DYING_RIGHT, glm::vec2(0.7f, 0.1f));
	sprite->addKeyframe(DYING_RIGHT, glm::vec2(0.8f, 0.1f));
	sprite->addKeyframe(DYING_RIGHT, glm::vec2(0.9f, 0.1f));
	sprite->addKeyframe(DYING_RIGHT, glm::vec2(0.0f, 0.2f));


	sprite->setAnimationSpeed(DYING_LEFT, 8);
	sprite->addKeyframe(DYING_LEFT, glm::vec2(-0.6f, 0.1f));
	sprite->addKeyframe(DYING_LEFT, glm::vec2(-0.7f, 0.1f));
	sprite->addKeyframe(DYING_LEFT, glm::vec2(-0.8f, 0.1f));
	sprite->addKeyframe(DYING_LEFT, glm::vec2(-0.9f, 0.1f));
	sprite->addKeyframe(DYING_LEFT, glm::vec2(-1.0f, 0.1f));
	sprite->addKeyframe(DYING_LEFT, glm::vec2(-0.1f, 0.2f));


	sprite->setAnimationSpeed(DEAD_RIGHT, 8);
	sprite->addKeyframe(DEAD_RIGHT, glm::vec2(0.1f, 0.2f));

	sprite->setAnimationSpeed(DEAD_LEFT, 8);
	sprite->addKeyframe(DEAD_LEFT, glm::vec2(-0.2f, 0.2f));

	//Init sprite and position
	sprite->changeAnimation(STAND_LEFT);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posIA.x), float(tileMapDispl.y + posIA.y)));

	healthPoints = 3;
}

void IA::update(int deltaTime)
{

	bool finishAction = sprite->update(deltaTime);


	int playerX = player->getPosition().x;
	int playerY = player->getPosition().y;
	int playerHealth = player->getHealth();
	int sultanX = posIA.x;
	int sultanY = posIA.y;
	bool visionRange = (playerY - sultanY == 0) && (abs(playerX - sultanX) <= IA_VISION);
	bool fightRange = (playerY - sultanY == 0) && (abs(playerX - sultanX) <= ATTACK_RANGE);
	bool block = player->isBlocking();

	if (finishAction) {

		switch (sprite->animation()) {

		case STAND_LEFT:
				
			if (healthPoints == 0) sprite->changeAnimation(DYING_LEFT);
			else if (playerHealth == 0) sprite->changeAnimation(STAND_LEFT);
			else if (playerX > sultanX)	sprite->changeAnimation(STAND_RIGHT);
			else if (fightRange && block) sprite->changeAnimation(BLOCK_LEFT);
			else if (fightRange) sprite->changeAnimation(ATTACK_RIGHT);
			else if (visionRange) sprite->changeAnimation(MOVE_LEFT);
			
			break;

		case STAND_RIGHT:

			if (healthPoints == 0) sprite->changeAnimation(DYING_RIGHT);
			else if (playerHealth == 0) sprite->changeAnimation(STAND_RIGHT);
			else if (playerX < sultanX)	sprite->changeAnimation(STAND_LEFT);
			else if (fightRange) sprite->changeAnimation(ATTACK_RIGHT);
			else if (visionRange) sprite->changeAnimation(MOVE_RIGHT);

			break;

		case MOVE_LEFT:

			if (healthPoints == 0) sprite->changeAnimation(DYING_LEFT);
			else if (playerHealth == 0) sprite->changeAnimation(STAND_LEFT);
			else if (playerX > sultanX)	sprite->changeAnimation(STAND_RIGHT);
			else if (fightRange && block) sprite->changeAnimation(BLOCK_LEFT);
			else if (fightRange) sprite->changeAnimation(ATTACK_LEFT);
			else if (visionRange) sprite->changeAnimation(MOVE_LEFT);
		
			break;

		case MOVE_RIGHT:

			if (healthPoints == 0) sprite->changeAnimation(DYING_RIGHT);
			else if (playerHealth == 0) sprite->changeAnimation(STAND_RIGHT);
			else if (playerX < sultanX)	sprite->changeAnimation(STAND_LEFT);
			else if (fightRange) sprite->changeAnimation(ATTACK_RIGHT);
			else if (visionRange) sprite->changeAnimation(MOVE_RIGHT);
			
			break;

		case ATTACK_LEFT:

			if (healthPoints == 0) sprite->changeAnimation(DYING_LEFT);
			else if (playerHealth == 0) sprite->changeAnimation(STAND_LEFT);
			else if (playerX > sultanX)	sprite->changeAnimation(STAND_RIGHT);
			else if (fightRange && block) sprite->changeAnimation(BLOCK_LEFT);
			else if (fightRange) sprite->changeAnimation(ATTACK_LEFT);
			else if (visionRange) sprite->changeAnimation(MOVE_LEFT);


			break;

		case ATTACK_RIGHT:

			if (healthPoints == 0) sprite->changeAnimation(DYING_RIGHT);
			else if (playerHealth == 0) sprite->changeAnimation(STAND_RIGHT);
			else if (playerX < sultanX)	sprite->changeAnimation(STAND_LEFT);
			else if (fightRange) sprite->changeAnimation(ATTACK_RIGHT);
			else if (visionRange) sprite->changeAnimation(MOVE_RIGHT);



			break;

		case BLOCK_LEFT:

			if (healthPoints == 0) sprite->changeAnimation(DYING_LEFT);
			else sprite->changeAnimation(ATTACK_LEFT);

			break;

		case BLOCK_RIGHT:

			if (healthPoints == 0) sprite->changeAnimation(DYING_RIGHT);
			else sprite->changeAnimation(ATTACK_RIGHT);

			break;
			
		case DYING_LEFT:

			sprite->changeAnimation(DEAD_LEFT);

			break;

		case DYING_RIGHT:

			sprite->changeAnimation(DEAD_RIGHT);

			break;


		}
	}
	if (sprite->animation() == MOVE_LEFT && sprite->getFrame() == 0) posIA.x -= 2;
	if (sprite->animation() == MOVE_RIGHT && sprite->getFrame() == 0) posIA.x += 2;
	if (sprite->animation() == BLOCK_RIGHT && sprite->getFrame() % 2) posIA.x -= 1;
	if (sprite->animation() == BLOCK_LEFT && sprite->getFrame() %  2) posIA.x += 1;

	if ((sprite->animation() == ATTACK_LEFT || sprite->animation() == ATTACK_RIGHT) && sprite->getFrame() == 5  && !block) {
		player->dealDamage(1,"enemy");

	}

	if ((sprite->animation() != BLOCK_RIGHT || sprite->animation() != BLOCK_LEFT) && player->isAttacking())
		dealDamageEnemy(1);

	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posIA.x), float(tileMapDispl.y + posIA.y)));
}

void IA::render()
{
	sprite->render();
}

void IA::setPlayer(Player *player)
{
	this->player = player;
}

void IA::setPosition(const glm::vec2 &pos)
{
	posIA = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posIA.x), float(tileMapDispl.y + posIA.y)));
}


void IA::setTileMap(TileMap *tileMap)
{
	map = tileMap;
}

void IA::dealDamageEnemy(int damage) {
	
	if (sprite->animation() != BLOCK_RIGHT && sprite->animation() != BLOCK_LEFT ) {
		healthPoints -= 1;
			
	}



}

bool IA::isBlockingEnemy() {

	if (sprite->animation() != BLOCK_RIGHT && sprite->animation() != BLOCK_LEFT) {
		return false;

	}
	else return true;



}