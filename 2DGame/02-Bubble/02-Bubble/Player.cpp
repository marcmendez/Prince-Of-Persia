#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Player.h"
#include "Game.h"


#define JUMP_ANGLE_STEP 4
#define JUMP_HEIGHT 96
#define FALL_STEP 4


enum PlayerAnims
{
	STAND_LEFT, STAND_RIGHT, MOVE_LEFT, MOVE_RIGHT, START_RUN_RIGHT, START_RUN_LEFT, STOP_RUN_RIGHT, STOP_RUN_LEFT
};


void Player::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{
	bJumping = false;
	direction = "";
	spritesheet.setWrapS(GL_MIRRORED_REPEAT);
	spritesheet.loadFromFile("images/Spritesheet1.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(64, 64), glm::vec2(0.1, 0.1), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(8);

	sprite->setAnimationSpeed(STAND_LEFT, 8);
	sprite->addKeyframe(STAND_LEFT, glm::vec2(0.f, 0.f));

	sprite->setAnimationSpeed(STAND_RIGHT, 8);
	sprite->addKeyframe(STAND_RIGHT, glm::vec2(0.f, 0.f));

	sprite->setAnimationSpeed(START_RUN_LEFT, 8);
	sprite->addKeyframe(START_RUN_LEFT, glm::vec2(-0.1f, 0.1f));
	sprite->addKeyframe(START_RUN_LEFT, glm::vec2(-0.2f, 0.1f));
	sprite->addKeyframe(START_RUN_LEFT, glm::vec2(-0.3f, 0.1f));

	sprite->setAnimationSpeed(START_RUN_RIGHT, 8);
	sprite->addKeyframe(START_RUN_RIGHT, glm::vec2(0.0f, 0.1f));
	sprite->addKeyframe(START_RUN_RIGHT, glm::vec2(0.1f, 0.1f));
	sprite->addKeyframe(START_RUN_RIGHT, glm::vec2(0.2f, 0.1f));

	sprite->setAnimationSpeed(MOVE_LEFT, 8);
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(-0.4f, 0.1f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(-0.5f, 0.1f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(-0.6f, 0.1f));

	sprite->setAnimationSpeed(MOVE_RIGHT, 8);
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.3f, 0.1f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.4f, 0.1f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.5f, 0.1f));

	sprite->setAnimationSpeed(STOP_RUN_LEFT, 8);
	sprite->addKeyframe(STOP_RUN_LEFT, glm::vec2(-0.7f, 0.1f));
	sprite->addKeyframe(STOP_RUN_LEFT, glm::vec2(-0.8f, 0.1f));
	sprite->addKeyframe(STOP_RUN_LEFT, glm::vec2(-0.9f, 0.1f));
	sprite->addKeyframe(STOP_RUN_LEFT, glm::vec2(-1.f, 0.1f));

	sprite->setAnimationSpeed(STOP_RUN_RIGHT, 8);
	sprite->addKeyframe(STOP_RUN_RIGHT, glm::vec2(0.6f, 0.1f));
	sprite->addKeyframe(STOP_RUN_RIGHT, glm::vec2(0.7f, 0.1f));
	sprite->addKeyframe(STOP_RUN_RIGHT, glm::vec2(0.8f, 0.1f));
	sprite->addKeyframe(STOP_RUN_RIGHT, glm::vec2(0.9f, 0.1f));
		
	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
	
}

void Player::update(int deltaTime)
{

	bool finishAction = sprite->update(deltaTime);

	if(Game::instance().getSpecialKey(GLUT_KEY_LEFT) && (direction == "LEFT" || direction == ""))
	{
		direction = "LEFT";
		if (finishAction) {
			if (sprite->animation() == STAND_RIGHT || sprite->animation() == STAND_LEFT) sprite->changeAnimation(START_RUN_LEFT);
			if (sprite->animation() == START_RUN_LEFT) sprite->changeAnimation(MOVE_LEFT);
		}
		posPlayer.x -= 1;
		if(map->collisionMoveLeft(posPlayer, glm::ivec2(32, 64)))
		{
			posPlayer.x += 1;
			sprite->changeAnimation(STAND_LEFT);
		}
	}
	else if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT) && (direction == "RIGHT" || direction == ""))
	{
		direction = "RIGHT";

		if (finishAction) {
			if (sprite->animation() == STAND_RIGHT || sprite->animation() == STAND_LEFT) sprite->changeAnimation(START_RUN_RIGHT);
			if (sprite->animation() == START_RUN_RIGHT) sprite->changeAnimation(MOVE_RIGHT);
		}
		posPlayer.x += 1;
		if(map->collisionMoveRight(posPlayer, glm::ivec2(33, 64)))
		{
			posPlayer.x -= 1;
			sprite->changeAnimation(STAND_RIGHT);
		}
	}
	else
	{
		if (finishAction) {

			if (sprite->animation() == START_RUN_LEFT) sprite->changeAnimation(STOP_RUN_LEFT);
			else if (sprite->animation() == MOVE_LEFT) sprite->changeAnimation(STOP_RUN_LEFT);
			else if (sprite->animation() == STOP_RUN_LEFT) sprite->changeAnimation(STAND_LEFT);

			else if (sprite->animation() == START_RUN_RIGHT) sprite->changeAnimation(STOP_RUN_RIGHT);
			else if (sprite->animation() == MOVE_RIGHT) sprite->changeAnimation(STOP_RUN_RIGHT);
			else if (sprite->animation() == STOP_RUN_RIGHT) sprite->changeAnimation(STAND_RIGHT);

		}

		if (map->collisionMoveRight(posPlayer, glm::ivec2(32, 64)))
		{
			posPlayer.x -= 1;
			sprite->changeAnimation(STAND_RIGHT);
		}

		if (map->collisionMoveLeft(posPlayer, glm::ivec2(32, 64)))
		{
			posPlayer.x += 1;
			sprite->changeAnimation(STAND_LEFT);
		}

		if (sprite->animation() == MOVE_RIGHT) posPlayer.x += 1;
		if (sprite->animation() == MOVE_LEFT) posPlayer.x -= 1;
		if (sprite->animation() == STOP_RUN_RIGHT) posPlayer.x += 1;
		if (sprite->animation() == STOP_RUN_LEFT) posPlayer.x -= 1;

		if (sprite->animation() == STAND_RIGHT || sprite->animation() == STAND_LEFT) direction = "";
	}
	
	if(bJumping)
	{
		jumpAngle += JUMP_ANGLE_STEP;
		if(jumpAngle == 180)
		{
			bJumping = false;
			posPlayer.y = startY;
		}
		else
		{
			
			posPlayer.y = int(startY - 96 * sin(3.14159f * jumpAngle / 180.f));

			if(jumpAngle > 90)
				bJumping = !map->collisionMoveDown(posPlayer, glm::ivec2(32, 64), &posPlayer.y);
		}
	}
	else
	{
		posPlayer.y += FALL_STEP;
		if(map->collisionMoveDown(posPlayer, glm::ivec2(32, 64), &posPlayer.y))
		{
			if(Game::instance().getSpecialKey(GLUT_KEY_UP))
			{
				bJumping = true;
				jumpAngle = 0;
				startY = posPlayer.y;
			}
		}
		else posPlayer.y -= FALL_STEP;
	}
	
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}

void Player::render()
{
	sprite->render();
}

void Player::setTileMap(TileMap *tileMap)
{
	map = tileMap;
}

void Player::setPosition(const glm::vec2 &pos)
{
	posPlayer = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}




