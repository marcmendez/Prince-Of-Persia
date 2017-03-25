#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Torch.h"

enum PlayerAnims { FIRE };

void Torch::init(const glm::vec2 torchPos, glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram) {

	spritesheet.loadFromFile("images/TorchSheet.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(64, 64), glm::vec2(0.1, 1), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(1);

	sprite->setAnimationSpeed(FIRE, 8);
	sprite->addKeyframe(FIRE, glm::vec2(0.0f, 0.0f));
	sprite->addKeyframe(FIRE, glm::vec2(0.1f, 0.0f));
	sprite->addKeyframe(FIRE, glm::vec2(0.2f, 0.0f));
	sprite->addKeyframe(FIRE, glm::vec2(0.3f, 0.0f));
	sprite->addKeyframe(FIRE, glm::vec2(0.4f, 0.0f));
	sprite->addKeyframe(FIRE, glm::vec2(0.5f, 0.0f));
	sprite->addKeyframe(FIRE, glm::vec2(0.6f, 0.0f));
	sprite->addKeyframe(FIRE, glm::vec2(0.7f, 0.0f));
	sprite->addKeyframe(FIRE, glm::vec2(0.8f, 0.0f));

	sprite->changeAnimation(0);
	sprite->setPosition(glm::vec2(float(tileMapPos.x + torchPos.x + 43), float(tileMapPos.y + torchPos.y - 43)));

}

void Torch::update(int deltaTime) {
	sprite->update(deltaTime);
}

void Torch::render() {
	sprite->render();
}