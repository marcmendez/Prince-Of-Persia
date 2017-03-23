#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Player.h"
#include "Game.h"


#define JUMP_ANGLE_STEP 4
#define JUMP_HEIGHT 0
#define FALL_STEP 4


enum PlayerAnims
{
	STAND_LEFT, STAND_RIGHT, MOVE_LEFT, MOVE_RIGHT, START_RUN_RIGHT, START_RUN_LEFT, STOP_RUN_RIGHT,
	STOP_RUN_LEFT, SHIFT_RIGHT, SHIFT_LEFT, TURN_LEFT_TO_RIGHT, TURN_RIGHT_TO_LEFT,
	CHANGE_DIRECTION_TO_LEFT, CHANGE_DIRECTION_TO_RIGHT,JUMP_RUN_RIGHT, JUMP_RUN_LEFT,
	START_JUMP_STAND_RIGHT, START_JUMP_STAND_LEFT, JUMP_STAND_RIGHT, JUMP_STAND_LEFT,
	STOP_JUMP_STAND_RIGHT, STOP_JUMP_STAND_LEFT,
	JUMP_UP_RIGHT, JUMP_UP_LEFT, GO_UP_RIGHT, GO_UP_LEFT
};


void Player::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{

	// Configuring the spritesheet
	spritesheet.setWrapS(GL_MIRRORED_REPEAT);
	spritesheet.loadFromFile("images/Spritesheet1.png", TEXTURE_PIXEL_FORMAT_RGBA);

	// Configuring a single sprite
	sprite = Sprite::createSprite(glm::ivec2(64, 64), glm::vec2(0.1, 0.1), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(26);

	// STAND LEFT
	sprite->setAnimationSpeed(STAND_LEFT, 8);
	sprite->addKeyframe(STAND_LEFT, glm::vec2(-0.1f, 0.f));

	// STAND RIGHT
	sprite->setAnimationSpeed(STAND_RIGHT, 8);
	sprite->addKeyframe(STAND_RIGHT, glm::vec2(0.f, 0.f));

	// RUN LEFT

	/* 1) START RUN */
	sprite->setAnimationSpeed(START_RUN_LEFT, 8);
	sprite->addKeyframe(START_RUN_LEFT, glm::vec2(-0.1f, 0.1f));
	sprite->addKeyframe(START_RUN_LEFT, glm::vec2(-0.2f, 0.1f));
	sprite->addKeyframe(START_RUN_LEFT, glm::vec2(-0.3f, 0.1f));

	/* 2) ITER. RUNNING */
	sprite->setAnimationSpeed(MOVE_LEFT, 8);
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(-0.1f, 0.3f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(-0.2f, 0.3f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(-0.3f, 0.3f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(-0.4f, 0.3f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(-0.5f, 0.3f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(-0.6f, 0.3f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(-0.7f, 0.3f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(-0.8f, 0.3f));

	/* 3) STOP RUNNING */
	sprite->setAnimationSpeed(STOP_RUN_LEFT, 8);
	sprite->addKeyframe(STOP_RUN_LEFT, glm::vec2(-0.7f, 0.1f));
	sprite->addKeyframe(STOP_RUN_LEFT, glm::vec2(-0.8f, 0.1f));
	sprite->addKeyframe(STOP_RUN_LEFT, glm::vec2(-0.9f, 0.1f));
	sprite->addKeyframe(STOP_RUN_LEFT, glm::vec2(-1.f, 0.1f));

	// RUN RIGHT

	/* 1) START RUN */
	sprite->setAnimationSpeed(START_RUN_RIGHT, 8);
	sprite->addKeyframe(START_RUN_RIGHT, glm::vec2(0.0f, 0.1f));
	sprite->addKeyframe(START_RUN_RIGHT, glm::vec2(0.1f, 0.1f));
	sprite->addKeyframe(START_RUN_RIGHT, glm::vec2(0.2f, 0.1f));

	/* 2) ITER. RUNNING */
	sprite->setAnimationSpeed(MOVE_RIGHT, 8);
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.0f, 0.3f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.1f, 0.3f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.2f, 0.3f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.3f, 0.3f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.4f, 0.3f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.5f, 0.3f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.6f, 0.3f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.7f, 0.3f));

	/* 3) STOP RUNNING */
	sprite->setAnimationSpeed(STOP_RUN_RIGHT, 8);
	sprite->addKeyframe(STOP_RUN_RIGHT, glm::vec2(0.6f, 0.1f));
	sprite->addKeyframe(STOP_RUN_RIGHT, glm::vec2(0.7f, 0.1f));
	sprite->addKeyframe(STOP_RUN_RIGHT, glm::vec2(0.8f, 0.1f));
	sprite->addKeyframe(STOP_RUN_RIGHT, glm::vec2(0.9f, 0.1f));

	// WALK SLOW RIGHT

	sprite->setAnimationSpeed(SHIFT_RIGHT, 8);
	sprite->addKeyframe(SHIFT_RIGHT, glm::vec2(0.0f, 0.6f));
	sprite->addKeyframe(SHIFT_RIGHT, glm::vec2(0.1f, 0.6f));
	sprite->addKeyframe(SHIFT_RIGHT, glm::vec2(0.2f, 0.6f));
	sprite->addKeyframe(SHIFT_RIGHT, glm::vec2(0.3f, 0.6f));
	sprite->addKeyframe(SHIFT_RIGHT, glm::vec2(0.4f, 0.6f));
	sprite->addKeyframe(SHIFT_RIGHT, glm::vec2(0.5f, 0.6f));
	sprite->addKeyframe(SHIFT_RIGHT, glm::vec2(0.6f, 0.6f));

	// WALK SLOW LEFT

	sprite->setAnimationSpeed(SHIFT_LEFT, 8);
	sprite->addKeyframe(SHIFT_LEFT, glm::vec2(-0.1f, 0.6f));
	sprite->addKeyframe(SHIFT_LEFT, glm::vec2(-0.2f, 0.6f));
	sprite->addKeyframe(SHIFT_LEFT, glm::vec2(-0.3f, 0.6f));
	sprite->addKeyframe(SHIFT_LEFT, glm::vec2(-0.4f, 0.6f));
	sprite->addKeyframe(SHIFT_LEFT, glm::vec2(-0.5f, 0.6f));
	sprite->addKeyframe(SHIFT_LEFT, glm::vec2(-0.6f, 0.6f));
	sprite->addKeyframe(SHIFT_LEFT, glm::vec2(-0.7f, 0.6f));

	// TURN WHILE NOT MOVING 

	/*LEFT->RIGHT*/
	sprite->setAnimationSpeed(TURN_LEFT_TO_RIGHT, 8);
	sprite->addKeyframe(TURN_LEFT_TO_RIGHT, glm::vec2(0.0f, 0.5f));
	sprite->addKeyframe(TURN_LEFT_TO_RIGHT, glm::vec2(0.1f, 0.5f));
	sprite->addKeyframe(TURN_LEFT_TO_RIGHT, glm::vec2(0.2f, 0.5f));
	sprite->addKeyframe(TURN_LEFT_TO_RIGHT, glm::vec2(0.3f, 0.5f));
	sprite->addKeyframe(TURN_LEFT_TO_RIGHT, glm::vec2(0.4f, 0.5f));
	sprite->addKeyframe(TURN_LEFT_TO_RIGHT, glm::vec2(0.5f, 0.5f));

	/*RIGHT->LEFT*/
	sprite->setAnimationSpeed(TURN_RIGHT_TO_LEFT, 8);
	sprite->addKeyframe(TURN_RIGHT_TO_LEFT, glm::vec2(-0.1f, 0.5f));
	sprite->addKeyframe(TURN_RIGHT_TO_LEFT, glm::vec2(-0.2f, 0.5f));
	sprite->addKeyframe(TURN_RIGHT_TO_LEFT, glm::vec2(-0.3f, 0.5f));
	sprite->addKeyframe(TURN_RIGHT_TO_LEFT, glm::vec2(-0.4f, 0.5f));
	sprite->addKeyframe(TURN_RIGHT_TO_LEFT, glm::vec2(-0.5f, 0.5f));
	sprite->addKeyframe(TURN_RIGHT_TO_LEFT, glm::vec2(-0.6f, 0.5f));

	// TURN WHILE MOVING 

	/*LEFT->RIGHT*/
	sprite->setAnimationSpeed(CHANGE_DIRECTION_TO_RIGHT, 8);
	sprite->addKeyframe(CHANGE_DIRECTION_TO_RIGHT, glm::vec2(-0.1f, 0.4f));
	sprite->addKeyframe(CHANGE_DIRECTION_TO_RIGHT, glm::vec2(-0.2f, 0.4f));
	sprite->addKeyframe(CHANGE_DIRECTION_TO_RIGHT, glm::vec2(-0.3f, 0.4f));
	sprite->addKeyframe(CHANGE_DIRECTION_TO_RIGHT, glm::vec2(-0.4f, 0.4f));
	sprite->addKeyframe(CHANGE_DIRECTION_TO_RIGHT, glm::vec2(-0.5f, 0.4f));
	sprite->addKeyframe(CHANGE_DIRECTION_TO_RIGHT, glm::vec2(-0.6f, 0.4f));
	sprite->addKeyframe(CHANGE_DIRECTION_TO_RIGHT, glm::vec2(-0.7f, 0.4f));
	sprite->addKeyframe(CHANGE_DIRECTION_TO_RIGHT, glm::vec2(-0.8f, 0.4f));
	sprite->addKeyframe(CHANGE_DIRECTION_TO_RIGHT, glm::vec2(-0.9f, 0.4f));
	sprite->addKeyframe(CHANGE_DIRECTION_TO_RIGHT, glm::vec2(-1.f, 0.4f));
	sprite->addKeyframe(CHANGE_DIRECTION_TO_RIGHT, glm::vec2(-1.f, 0.3f));

	/*RIGHT->LEFT*/
	sprite->setAnimationSpeed(CHANGE_DIRECTION_TO_LEFT, 8);
	sprite->addKeyframe(CHANGE_DIRECTION_TO_LEFT, glm::vec2(0.0f, 0.4f));
	sprite->addKeyframe(CHANGE_DIRECTION_TO_LEFT, glm::vec2(0.1f, 0.4f));
	sprite->addKeyframe(CHANGE_DIRECTION_TO_LEFT, glm::vec2(0.2f, 0.4f));
	sprite->addKeyframe(CHANGE_DIRECTION_TO_LEFT, glm::vec2(0.3f, 0.4f));
	sprite->addKeyframe(CHANGE_DIRECTION_TO_LEFT, glm::vec2(0.4f, 0.4f));
	sprite->addKeyframe(CHANGE_DIRECTION_TO_LEFT, glm::vec2(0.5f, 0.4f));
	sprite->addKeyframe(CHANGE_DIRECTION_TO_LEFT, glm::vec2(0.6f, 0.4f));
	sprite->addKeyframe(CHANGE_DIRECTION_TO_LEFT, glm::vec2(0.7f, 0.4f));
	sprite->addKeyframe(CHANGE_DIRECTION_TO_LEFT, glm::vec2(0.8f, 0.4f));
	sprite->addKeyframe(CHANGE_DIRECTION_TO_LEFT, glm::vec2(0.9f, 0.4f));
	sprite->addKeyframe(CHANGE_DIRECTION_TO_LEFT, glm::vec2(0.9f, 0.3f));

	// JUMP WHILE MOVING

	/* RIGHT */
	sprite->setAnimationSpeed(JUMP_RUN_RIGHT, 8);
	sprite->addKeyframe(JUMP_RUN_RIGHT, glm::vec2(0.0f, 0.2f));
	sprite->addKeyframe(JUMP_RUN_RIGHT, glm::vec2(0.1f, 0.2f));
	sprite->addKeyframe(JUMP_RUN_RIGHT, glm::vec2(0.2f, 0.2f));
	sprite->addKeyframe(JUMP_RUN_RIGHT, glm::vec2(0.3f, 0.2f));
	sprite->addKeyframe(JUMP_RUN_RIGHT, glm::vec2(0.4f, 0.2f));
	sprite->addKeyframe(JUMP_RUN_RIGHT, glm::vec2(0.5f, 0.2f));
	sprite->addKeyframe(JUMP_RUN_RIGHT, glm::vec2(0.6f, 0.2f));
	sprite->addKeyframe(JUMP_RUN_RIGHT, glm::vec2(0.7f, 0.2f));
	sprite->addKeyframe(JUMP_RUN_RIGHT, glm::vec2(0.8f, 0.2f));
	sprite->addKeyframe(JUMP_RUN_RIGHT, glm::vec2(0.9f, 0.2f));

	/* LEFT */
	sprite->setAnimationSpeed(JUMP_RUN_LEFT, 8);
	sprite->addKeyframe(JUMP_RUN_LEFT, glm::vec2(-0.1f, 0.2f));
	sprite->addKeyframe(JUMP_RUN_LEFT, glm::vec2(-0.2f, 0.2f));
	sprite->addKeyframe(JUMP_RUN_LEFT, glm::vec2(-0.3f, 0.2f));
	sprite->addKeyframe(JUMP_RUN_LEFT, glm::vec2(-0.4f, 0.2f));
	sprite->addKeyframe(JUMP_RUN_LEFT, glm::vec2(-0.5f, 0.2f));
	sprite->addKeyframe(JUMP_RUN_LEFT, glm::vec2(-0.6f, 0.2f));
	sprite->addKeyframe(JUMP_RUN_LEFT, glm::vec2(-0.7f, 0.2f));
	sprite->addKeyframe(JUMP_RUN_LEFT, glm::vec2(-0.8f, 0.2f));
	sprite->addKeyframe(JUMP_RUN_LEFT, glm::vec2(-0.9f, 0.2f));
	sprite->addKeyframe(JUMP_RUN_LEFT, glm::vec2(-1.0f, 0.2f));

	// JUMP WHILE NOT MOVING RIGHT
	
	/* START JUMPING */
	sprite->setAnimationSpeed(START_JUMP_STAND_RIGHT, 8);
	sprite->addKeyframe(START_JUMP_STAND_RIGHT, glm::vec2(0.0f, 0.7f));
	sprite->addKeyframe(START_JUMP_STAND_RIGHT, glm::vec2(0.1f, 0.7f));
	sprite->addKeyframe(START_JUMP_STAND_RIGHT, glm::vec2(0.2f, 0.7f));
	sprite->addKeyframe(START_JUMP_STAND_RIGHT, glm::vec2(0.3f, 0.7f));
	sprite->addKeyframe(START_JUMP_STAND_RIGHT, glm::vec2(0.4f, 0.7f));

	/* JUMP AREA */ 
	sprite->setAnimationSpeed(JUMP_STAND_RIGHT, 8);
	sprite->addKeyframe(JUMP_STAND_RIGHT, glm::vec2(0.5f, 0.7f));
	sprite->addKeyframe(JUMP_STAND_RIGHT, glm::vec2(0.6f, 0.7f));
	sprite->addKeyframe(JUMP_STAND_RIGHT, glm::vec2(0.7f, 0.7f));
	sprite->addKeyframe(JUMP_STAND_RIGHT, glm::vec2(0.8f, 0.7f));
	sprite->addKeyframe(JUMP_STAND_RIGHT, glm::vec2(0.9f, 0.7f));

	/* LAND JUMP */
	sprite->setAnimationSpeed(STOP_JUMP_STAND_RIGHT, 8);
	sprite->addKeyframe(STOP_JUMP_STAND_RIGHT, glm::vec2(0.7f, 0.5f));
	sprite->addKeyframe(STOP_JUMP_STAND_RIGHT, glm::vec2(0.8f, 0.5f));
	sprite->addKeyframe(STOP_JUMP_STAND_RIGHT, glm::vec2(0.9f, 0.5f));
	sprite->addKeyframe(STOP_JUMP_STAND_RIGHT, glm::vec2(0.8f, 0.6f));
	sprite->addKeyframe(STOP_JUMP_STAND_RIGHT, glm::vec2(0.9f, 0.6f));

	// JUMP WHILE NOT MOVING LEFT

	/* START JUMPING */
	sprite->setAnimationSpeed(START_JUMP_STAND_LEFT, 8);
	sprite->addKeyframe(START_JUMP_STAND_LEFT, glm::vec2(-0.1f, 0.7f));
	sprite->addKeyframe(START_JUMP_STAND_LEFT, glm::vec2(-0.2f, 0.7f));
	sprite->addKeyframe(START_JUMP_STAND_LEFT, glm::vec2(-0.3f, 0.7f));
	sprite->addKeyframe(START_JUMP_STAND_LEFT, glm::vec2(-0.4f, 0.7f));
	sprite->addKeyframe(START_JUMP_STAND_LEFT, glm::vec2(-0.5f, 0.7f));
	
	/* JUMP AREA */
	sprite->setAnimationSpeed(JUMP_STAND_LEFT, 8);
	sprite->addKeyframe(JUMP_STAND_LEFT, glm::vec2(-0.6f, 0.7f));
	sprite->addKeyframe(JUMP_STAND_LEFT, glm::vec2(-0.7f, 0.7f));
	sprite->addKeyframe(JUMP_STAND_LEFT, glm::vec2(-0.8f, 0.7f));
	sprite->addKeyframe(JUMP_STAND_LEFT, glm::vec2(-0.9f, 0.7f));
	sprite->addKeyframe(JUMP_STAND_LEFT, glm::vec2(-1.0f, 0.7f));

	/* LAND JUMP */
	sprite->setAnimationSpeed(STOP_JUMP_STAND_LEFT, 8);
	sprite->addKeyframe(STOP_JUMP_STAND_LEFT, glm::vec2(-0.8f, 0.5f));
	sprite->addKeyframe(STOP_JUMP_STAND_LEFT, glm::vec2(-0.9f, 0.5f));
	sprite->addKeyframe(STOP_JUMP_STAND_LEFT, glm::vec2(-1.0f, 0.5f));
	sprite->addKeyframe(STOP_JUMP_STAND_LEFT, glm::vec2(-0.9f, 0.6f));
	sprite->addKeyframe(STOP_JUMP_STAND_LEFT, glm::vec2(-1.0f, 0.6f));

	// JUMP WHILE NOT MOVING RIGHT (UP)

	/* START JUMP */
	sprite->setAnimationSpeed(JUMP_UP_RIGHT, 8);
	sprite->addKeyframe(JUMP_UP_RIGHT, glm::vec2(0.0f, 0.8f));
	sprite->addKeyframe(JUMP_UP_RIGHT, glm::vec2(0.1f, 0.8f));
	sprite->addKeyframe(JUMP_UP_RIGHT, glm::vec2(0.2f, 0.8f));
	sprite->addKeyframe(JUMP_UP_RIGHT, glm::vec2(0.3f, 0.8f));
	sprite->addKeyframe(JUMP_UP_RIGHT, glm::vec2(0.4f, 0.8f));
	sprite->addKeyframe(JUMP_UP_RIGHT, glm::vec2(0.5f, 0.8f));
	sprite->addKeyframe(JUMP_UP_RIGHT, glm::vec2(0.6f, 0.8f));
	sprite->addKeyframe(JUMP_UP_RIGHT, glm::vec2(0.7f, 0.8f));
	sprite->addKeyframe(JUMP_UP_RIGHT, glm::vec2(0.8f, 0.8f));
	sprite->addKeyframe(JUMP_UP_RIGHT, glm::vec2(0.9f, 0.8f));

	/* CLIMB */
	sprite->setAnimationSpeed(GO_UP_RIGHT, 8);
	sprite->addKeyframe(GO_UP_RIGHT, glm::vec2(0.0f, 0.9f));
	sprite->addKeyframe(GO_UP_RIGHT, glm::vec2(0.1f, 0.9f));
	sprite->addKeyframe(GO_UP_RIGHT, glm::vec2(0.2f, 0.9f));
	sprite->addKeyframe(GO_UP_RIGHT, glm::vec2(0.3f, 0.9f));
	sprite->addKeyframe(GO_UP_RIGHT, glm::vec2(0.4f, 0.9f));
	sprite->addKeyframe(GO_UP_RIGHT, glm::vec2(0.5f, 0.9f));
	sprite->addKeyframe(GO_UP_RIGHT, glm::vec2(0.6f, 0.9f));
	sprite->addKeyframe(GO_UP_RIGHT, glm::vec2(0.7f, 0.9f));
	sprite->addKeyframe(GO_UP_RIGHT, glm::vec2(0.8f, 0.9f));
	sprite->addKeyframe(GO_UP_RIGHT, glm::vec2(0.9f, 0.9f));
	sprite->addKeyframe(GO_UP_RIGHT, glm::vec2(0.9f, 0.0f));

	// JUMP WHILE NOT MOVING RIGHT (UP)

	/* START JUMP */
	sprite->setAnimationSpeed(JUMP_UP_LEFT, 8);
	sprite->addKeyframe(JUMP_UP_LEFT, glm::vec2(-0.1f, 0.8f));
	sprite->addKeyframe(JUMP_UP_LEFT, glm::vec2(-0.2f, 0.8f));
	sprite->addKeyframe(JUMP_UP_LEFT, glm::vec2(-0.3f, 0.8f));
	sprite->addKeyframe(JUMP_UP_LEFT, glm::vec2(-0.4f, 0.8f));
	sprite->addKeyframe(JUMP_UP_LEFT, glm::vec2(-0.5f, 0.8f));
	sprite->addKeyframe(JUMP_UP_LEFT, glm::vec2(-0.6f, 0.8f));
	sprite->addKeyframe(JUMP_UP_LEFT, glm::vec2(-0.7f, 0.8f));
	sprite->addKeyframe(JUMP_UP_LEFT, glm::vec2(-0.8f, 0.8f));
	sprite->addKeyframe(JUMP_UP_LEFT, glm::vec2(-0.9f, 0.8f));
	sprite->addKeyframe(JUMP_UP_LEFT, glm::vec2(-1.0f, 0.8f));
	
	/* CLIMB */
	sprite->setAnimationSpeed(GO_UP_LEFT, 8);
	sprite->addKeyframe(GO_UP_LEFT, glm::vec2(-0.1f, 0.9f));
	sprite->addKeyframe(GO_UP_LEFT, glm::vec2(-0.2f, 0.9f));
	sprite->addKeyframe(GO_UP_LEFT, glm::vec2(-0.3f, 0.9f));
	sprite->addKeyframe(GO_UP_LEFT, glm::vec2(-0.4f, 0.9f));
	sprite->addKeyframe(GO_UP_LEFT, glm::vec2(-0.5f, 0.9f));
	sprite->addKeyframe(GO_UP_LEFT, glm::vec2(-0.6f, 0.9f));
	sprite->addKeyframe(GO_UP_LEFT, glm::vec2(-0.7f, 0.9f));
	sprite->addKeyframe(GO_UP_LEFT, glm::vec2(-0.8f, 0.9f));
	sprite->addKeyframe(GO_UP_LEFT, glm::vec2(-0.9f, 0.9f));
	sprite->addKeyframe(GO_UP_LEFT, glm::vec2(-1.0f, 0.9f));
	sprite->addKeyframe(GO_UP_LEFT, glm::vec2(-1.0f, 0.9f));

	//Init sprite and position
	sprite->changeAnimation(1);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));

	// Start looking to right
	direction = 'r';
	
	bFalling = false; bJumping = false;

	
}

void Player::update(int deltaTime)
{

	bool finishAction = sprite->update(deltaTime);
	
	if (finishAction) {

		switch (sprite->animation()) {

		case STAND_RIGHT:

			/* IF RIGHT + UP */ if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT) && Game::instance().getSpecialKey(GLUT_KEY_UP)) sprite->changeAnimation(START_JUMP_STAND_RIGHT);
			/* IF RIGHT + SHIFT */ else if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT) && (Game::instance().getSpecialKey(113) || Game::instance().getSpecialKey(112))) sprite->changeAnimation(SHIFT_RIGHT);
			/* IF RIGHT */ else if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT)) sprite->changeAnimation(START_RUN_RIGHT); 
			/* IF LEFT */ else if (Game::instance().getSpecialKey(GLUT_KEY_LEFT)) sprite->changeAnimation(TURN_RIGHT_TO_LEFT);
			/* IF UP */ else if (Game::instance().getSpecialKey(GLUT_KEY_UP)) sprite->changeAnimation(JUMP_UP_RIGHT);
			
			break;

		case STAND_LEFT:

			/* IF LEFT + UP */ if (Game::instance().getSpecialKey(GLUT_KEY_LEFT) && Game::instance().getSpecialKey(GLUT_KEY_UP)) sprite->changeAnimation(START_JUMP_STAND_LEFT);
			/* IF LEFT + SHIFT */ else if (Game::instance().getSpecialKey(GLUT_KEY_LEFT) && (Game::instance().getSpecialKey(113) || Game::instance().getSpecialKey(112))) sprite->changeAnimation(SHIFT_LEFT);
			/* IF LEFT */ else if (Game::instance().getSpecialKey(GLUT_KEY_LEFT)) sprite->changeAnimation(START_RUN_LEFT);
			/* IF RIGHT */ else if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT)) sprite->changeAnimation(TURN_LEFT_TO_RIGHT);
			/* IF UP */ else if (Game::instance().getSpecialKey(GLUT_KEY_UP)) sprite->changeAnimation(JUMP_UP_LEFT);
		
			break;

		case START_RUN_RIGHT:

			/* IF RIGHT + UP */ if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT) && Game::instance().getSpecialKey(GLUT_KEY_UP)) sprite->changeAnimation(JUMP_RUN_RIGHT);
			/* IF RIGHT */ else if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT)) sprite->changeAnimation(MOVE_RIGHT);
			/* IF LEFT */ else if (Game::instance().getSpecialKey(GLUT_KEY_LEFT)) sprite->changeAnimation(CHANGE_DIRECTION_TO_LEFT);
			/* IF NO KEY*/ else sprite->changeAnimation(STOP_RUN_RIGHT);

			break;

		case START_RUN_LEFT:

			/* IF LEFT + UP */ if (Game::instance().getSpecialKey(GLUT_KEY_LEFT) && Game::instance().getSpecialKey(GLUT_KEY_UP)) sprite->changeAnimation(JUMP_RUN_LEFT);
			/* IF LEFT */ else if (Game::instance().getSpecialKey(GLUT_KEY_LEFT)) sprite->changeAnimation(MOVE_LEFT);
			/* IF RIGHT */ else if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT)) sprite->changeAnimation(CHANGE_DIRECTION_TO_RIGHT);
			/* IF NO KEY*/ else sprite->changeAnimation(STOP_RUN_LEFT);

			break;

		case MOVE_RIGHT:

			/* IF RIGHT + UP */ if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT) && Game::instance().getSpecialKey(GLUT_KEY_UP)) sprite->changeAnimation(JUMP_RUN_RIGHT);
			/* IF RIGHT */ else if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT)) sprite->changeAnimation(MOVE_RIGHT);
			/* IF LEFT */ else if (Game::instance().getSpecialKey(GLUT_KEY_LEFT)) sprite->changeAnimation(CHANGE_DIRECTION_TO_LEFT);
			/* IF NO KEY */ else sprite->changeAnimation(STOP_RUN_RIGHT);
				
			break;

		case MOVE_LEFT:

			/* IF LEFT + UP*/ if (Game::instance().getSpecialKey(GLUT_KEY_LEFT) && Game::instance().getSpecialKey(GLUT_KEY_UP)) sprite->changeAnimation(JUMP_RUN_LEFT);
			/* IF LEFT */ else if (Game::instance().getSpecialKey(GLUT_KEY_LEFT)) sprite->changeAnimation(MOVE_LEFT);
			/* IF RIGHT */ else if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT)) sprite->changeAnimation(CHANGE_DIRECTION_TO_RIGHT);	
			/* IF NO KEY */ else sprite->changeAnimation(STOP_RUN_LEFT);
				
			break;

		case CHANGE_DIRECTION_TO_RIGHT:

			/* IF RIGHT + UP */ if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT) && Game::instance().getSpecialKey(GLUT_KEY_UP)) sprite->changeAnimation(JUMP_RUN_RIGHT);
			/* IF RIGHT */ else if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT)) sprite->changeAnimation(MOVE_RIGHT);
			/* IF NO KEY */ else sprite->changeAnimation(STOP_RUN_RIGHT);
			
			break;

		case CHANGE_DIRECTION_TO_LEFT:

			/* IF LEFT + UP */ if (Game::instance().getSpecialKey(GLUT_KEY_LEFT) && Game::instance().getSpecialKey(GLUT_KEY_UP)) sprite->changeAnimation(JUMP_RUN_LEFT);
			/* IF LEFT */ else if (Game::instance().getSpecialKey(GLUT_KEY_LEFT)) sprite->changeAnimation(MOVE_LEFT);
			/* IF NO KEY */ else sprite->changeAnimation(STOP_RUN_LEFT);
			
			break;

		case JUMP_RUN_RIGHT:

			/* IF RIGHT */ if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT)) sprite->changeAnimation(MOVE_RIGHT);
			/* IF LEFT */ else if (Game::instance().getSpecialKey(GLUT_KEY_LEFT)) sprite->changeAnimation(CHANGE_DIRECTION_TO_LEFT);
			/*IF NO KEY */ else sprite->changeAnimation(STOP_RUN_RIGHT);
			
			break;

		case JUMP_RUN_LEFT:

			/* IF LEFT */ if (Game::instance().getSpecialKey(GLUT_KEY_LEFT)) sprite->changeAnimation(MOVE_LEFT);
			/* IF RIGHT */ else if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT)) sprite->changeAnimation(CHANGE_DIRECTION_TO_RIGHT);
			/* IF NO KEY */ else sprite->changeAnimation(STOP_RUN_LEFT);
				
			break;

		case START_JUMP_STAND_RIGHT:
			/* INDEPENDENTLY  OF KEY */ sprite->changeAnimation(JUMP_STAND_RIGHT);
			break;

		case START_JUMP_STAND_LEFT:
			/* INDEPENDENTLY  OF KEY */ sprite->changeAnimation(JUMP_STAND_LEFT);
			break;

		case JUMP_STAND_RIGHT:
			/* INDEPENDENTLY  OF KEY */ sprite->changeAnimation(STOP_JUMP_STAND_RIGHT);
			break;

		case JUMP_STAND_LEFT:
			/* INDEPENDENTLY  OF KEY */ sprite->changeAnimation(STOP_JUMP_STAND_LEFT);
			break;

		case TURN_LEFT_TO_RIGHT:
		case SHIFT_RIGHT:
		case STOP_RUN_RIGHT:
		case STOP_JUMP_STAND_RIGHT:
			/* INDEPENDENTLY  OF KEY */ sprite->changeAnimation(STAND_RIGHT);
			break;

		case TURN_RIGHT_TO_LEFT:
		case SHIFT_LEFT:
		case STOP_RUN_LEFT:
		case STOP_JUMP_STAND_LEFT:
			/* INDEPENDENTLY  OF KEY */ sprite->changeAnimation(STAND_LEFT);
			break;

		case JUMP_UP_RIGHT:

			if (map->canIMoveUp(posPlayer.x, posPlayer.y, glm::ivec2(32, 64), 'r')) sprite->changeAnimation(GO_UP_RIGHT);
			else sprite->changeAnimation(STAND_RIGHT);
			
			break;

		case GO_UP_RIGHT:
			/* INDEPENDENTLY  OF KEY */ sprite->changeAnimation(STAND_RIGHT);
			posPlayer.x += 48;
			posPlayer.y -= 64;

		case JUMP_UP_LEFT:
			if (map->canIMoveUp(posPlayer.x, posPlayer.y, glm::ivec2(32, 64), 'l'))sprite->changeAnimation(GO_UP_LEFT);
			else sprite->changeAnimation(STAND_LEFT);

			break;

		case GO_UP_LEFT:
			/* INDEPENDENTLY  OF KEY */ sprite->changeAnimation(STAND_LEFT);
			posPlayer.x -= 48;
			posPlayer.y -= 64;

		}
	}

	if (sprite->animation() == START_RUN_RIGHT  && !map->collisionMoveRight(posPlayer.x + 1.f, posPlayer.y, glm::ivec2(32, 64))) { posPlayer.x += 1.f; direction = 'r'; bJumping = false; }
	else if (sprite->animation() == SHIFT_RIGHT && !map->collisionMoveRight(posPlayer.x + 0.5f, posPlayer.y, glm::ivec2(32, 64))){ posPlayer.x += 0.5f; direction = 'r'; bJumping = false; }
	else if (sprite->animation() == MOVE_RIGHT && !map->collisionMoveRight(posPlayer.x + 1.f, posPlayer.y, glm::ivec2(32, 64))) { posPlayer.x += 1.f; direction = 'r'; bJumping = false; }
	else if (sprite->animation() == STOP_RUN_RIGHT && !map->collisionMoveRight(posPlayer.x + 1.f, posPlayer.y, glm::ivec2(32, 64))) { posPlayer.x += 1.f; direction = 'r'; bJumping = false; }
	else if (sprite->animation() == CHANGE_DIRECTION_TO_LEFT && !map->collisionMoveRight(posPlayer.x + 1.f, posPlayer.y, glm::ivec2(32, 64))) { posPlayer.x += 1.f; direction = 'r'; }
	else if (sprite->animation() == JUMP_RUN_RIGHT && !map->collisionMoveRight(posPlayer.x + 1.5f, posPlayer.y, glm::ivec2(32, 64))) { posPlayer.x += 1.5f; direction = 'r'; bJumping = true; }
	else if (sprite->animation() == JUMP_STAND_RIGHT && !map->collisionMoveRight(posPlayer.x + 1.5f, posPlayer.y, glm::ivec2(32, 64))) { posPlayer.x += 1.5f; direction = 'r'; bJumping = true; }
	else if (sprite->animation() == STOP_JUMP_STAND_RIGHT && !map->collisionMoveRight(posPlayer.x + 0.5f, posPlayer.y, glm::ivec2(32, 64))) { posPlayer.x += 0.5f; direction = 'r'; bJumping = true; }

	else if (sprite->animation() == STOP_JUMP_STAND_RIGHT || sprite->animation() == JUMP_STAND_RIGHT || sprite->animation() == JUMP_RUN_RIGHT || sprite->animation() == CHANGE_DIRECTION_TO_LEFT ||
		sprite->animation() == STOP_RUN_RIGHT || sprite->animation() == MOVE_RIGHT || sprite->animation() == SHIFT_RIGHT || sprite->animation() == START_RUN_RIGHT) sprite->changeAnimation(STAND_RIGHT);

	else if (sprite->animation() == START_RUN_LEFT && !map->collisionMoveLeft(posPlayer.x - 1.f, posPlayer.y, glm::ivec2(32, 64))) { posPlayer.x -= 1.f; direction = 'l'; bJumping = false; }
	else if (sprite->animation() == MOVE_LEFT && !map->collisionMoveLeft(posPlayer.x - 1.f, posPlayer.y, glm::ivec2(32, 64))) { posPlayer.x -= 1.f; direction = 'l'; bJumping = false; }
	else if (sprite->animation() == STOP_RUN_LEFT && !map->collisionMoveLeft(posPlayer.x - 1.f, posPlayer.y, glm::ivec2(32, 64))) { posPlayer.x -= 1.f; direction = 'l'; bJumping = false; }
	else if (sprite->animation() == SHIFT_LEFT && !map->collisionMoveLeft(posPlayer.x - 0.5f, posPlayer.y, glm::ivec2(32, 64))) { posPlayer.x -= 0.5f; direction = 'l'; bJumping = false; }
	else if (sprite->animation() == CHANGE_DIRECTION_TO_RIGHT && !map->collisionMoveLeft(posPlayer.x - 1.f, posPlayer.y, glm::ivec2(32, 64))) { posPlayer.x -= 1.f; direction = 'l'; bJumping = false; }
	else if (sprite->animation() == JUMP_RUN_LEFT && !map->collisionMoveLeft(posPlayer.x - 1.5f, posPlayer.y, glm::ivec2(32, 64))) { posPlayer.x -= 1.5f; direction = 'l'; bJumping = true; }
	else if (sprite->animation() == JUMP_STAND_LEFT && !map->collisionMoveLeft(posPlayer.x - 1.5f, posPlayer.y, glm::ivec2(32, 64))) { posPlayer.x -= 1.5f; direction = 'l'; bJumping = true; }
	else if (sprite->animation() == STOP_JUMP_STAND_LEFT && !map->collisionMoveLeft(posPlayer.x - 0.5f, posPlayer.y, glm::ivec2(32, 64))) { posPlayer.x -= 0.5f; direction = 'l'; bJumping = false; }

	else if (sprite->animation() == STOP_JUMP_STAND_LEFT || sprite->animation() == JUMP_STAND_LEFT || sprite->animation() == JUMP_RUN_LEFT || sprite->animation() == CHANGE_DIRECTION_TO_LEFT ||
		sprite->animation() == STOP_RUN_LEFT || sprite->animation() == MOVE_LEFT || sprite->animation() == SHIFT_LEFT || sprite->animation() == START_RUN_LEFT) sprite->changeAnimation(STAND_LEFT);
	
	if (!map->collisionMoveDown(posPlayer.x, posPlayer.y + FALL_STEP, glm::ivec2(32, 64), direction) && !bJumping) {
		bFalling = true; posPlayer.y += FALL_STEP;
	}
	
	if (map->collisionMoveDown(posPlayer.x, posPlayer.y + FALL_STEP, glm::ivec2(32, 64), direction) && bFalling) {
		bFalling = false; posPlayer.y += FALL_STEP;
	}

	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y - 6)));

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

float Player::GetScreenX(int widthScreen) {

	return float(tileMapDispl.x + posPlayer.x) / widthScreen;

}

float Player::GetScreenY(int heightScreen) {

	return float(tileMapDispl.y + posPlayer.y ) / heightScreen;

}





