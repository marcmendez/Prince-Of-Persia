#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "princess.h"

enum PlayerAnims { TALK, SHY, WAITING };


void Princess::init(string spritesheetResource, glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram) {

	spritesheet.loadFromFile(spritesheetResource, TEXTURE_PIXEL_FORMAT_RGBA);
	bPeach = (spritesheetResource == "images/SpriteSheetPeach.png");
	sprite = Sprite::createSprite(glm::ivec2(64, 64), glm::vec2(0.1, 1), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(3);

	sprite->setAnimationSpeed(TALK, 2.5);
	sprite->addKeyframe(TALK, glm::vec2(0.8f, 0.0f));
	sprite->addKeyframe(TALK, glm::vec2(0.7f, 0.0f));
	sprite->addKeyframe(TALK, glm::vec2(0.6f, 0.0f));
	sprite->addKeyframe(TALK, glm::vec2(0.5f, 0.0f));
	sprite->addKeyframe(TALK, glm::vec2(0.4f, 0.0f));
	sprite->addKeyframe(TALK, glm::vec2(0.3f, 0.0f));
	sprite->addKeyframe(TALK, glm::vec2(0.2f, 0.0f));
	sprite->addKeyframe(TALK, glm::vec2(0.1f, 0.0f));
	sprite->addKeyframe(TALK, glm::vec2(0.0f, 0.0f));
	sprite->addKeyframe(TALK, glm::vec2(0.0f, 0.0f));
	sprite->addKeyframe(TALK, glm::vec2(0.0f, 0.0f));
	sprite->addKeyframe(TALK, glm::vec2(0.0f, 0.0f));
	sprite->addKeyframe(TALK, glm::vec2(0.0f, 0.0f));

	sprite->setAnimationSpeed(SHY, 1);
	sprite->addKeyframe(SHY, glm::vec2(0.0f, 0.0f));
	sprite->addKeyframe(SHY, glm::vec2(0.0f, 0.0f));
	sprite->addKeyframe(SHY, glm::vec2(0.0f, 0.0f));
	sprite->addKeyframe(SHY, glm::vec2(0.0f, 0.0f));

	sprite->setAnimationSpeed(WAITING, 1);
	sprite->addKeyframe(WAITING, glm::vec2(0.9f, 0.0f));


	sprite->changeAnimation(TALK);
	sprite->setPosition(glm::vec2(float(tileMapPos.x + (75 * 32)), float(tileMapPos.y + (13 * 64) - 8)));
	bFinished = false;
	bStarted = false;
}

void Princess::restart() {
	bFinished = false;
	bStarted = false;
	sprite->changeAnimation(TALK);
}

void Princess::update(int deltaTime) {

	bool finishAction = sprite->update(deltaTime);

	if (finishAction) {

		switch (sprite->animation()) {
		
		case WAITING:
			if (bStarted == true) sprite->changeAnimation(TALK);
		case TALK:
			sprite->changeAnimation(SHY);
		case SHY:
			bFinished = true;

		}

	}
	
}

void Princess::render() {
	sprite->render();
}