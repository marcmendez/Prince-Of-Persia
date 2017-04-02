#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Player.h"
#include "Game.h"


#define JUMP_ANGLE_STEP 4
#define JUMP_HEIGHT 0
#define FALL_STEP 4
#define  FALLEN_ANGLE 0.4
#define PLAYER_VISION 32*6
#define NUMBER_ENEMIES 2
#define ATTACK_RANGE 32

enum PlayerAnims
{
	STAND_LEFT, STAND_RIGHT,
	MOVE_LEFT, MOVE_RIGHT, 
	START_RUN_RIGHT, START_RUN_LEFT,
	STOP_RUN_RIGHT,	STOP_RUN_LEFT,
	SHIFT_RIGHT, SHIFT_LEFT, 
	TURN_LEFT_TO_RIGHT, TURN_RIGHT_TO_LEFT,
	CHANGE_DIRECTION_TO_LEFT, CHANGE_DIRECTION_TO_RIGHT,
	JUMP_RUN_RIGHT, JUMP_RUN_LEFT,
	START_JUMP_STAND_RIGHT, START_JUMP_STAND_LEFT, 
	JUMP_STAND_RIGHT, JUMP_STAND_LEFT,
	STOP_JUMP_STAND_RIGHT, STOP_JUMP_STAND_LEFT,
	JUMP_UP_RIGHT_FLOOR, JUMP_UP_LEFT_FLOOR, 
	JUMP_UP_RIGHT, JUMP_UP_LEFT, 
	GO_UP_RIGHT, GO_UP_LEFT,
	GOING_TO_FALL_LEFT, GOING_TO_FALL_RIGHT,
	FALLING_RIGHT, FALLING_LEFT, 
	TOUCH_FLOOR_LEFT, TOUCH_FLOOR_RIGHT,
	WAKE_UP_RIGHT, WAKE_UP_LEFT,
	SWORD_OUT_RIGHT, SWORD_OUT_LEFT,
	MOVE_SWORD_RIGHT, MOVE_SWORD_LEFT,
	STAND_SWORD_RIGHT, STAND_SWORD_LEFT,
	ATTACK_RIGHT, ATTACK_LEFT, 
	BLOCK_RIGHT, BLOCK_LEFT,
	HIDE_SWORD_RIGHT, HIDE_SWORD_LEFT,
	DEATH_BY_FALLING_RIGHT, DEATH_BY_FALLING_LEFT,
	DEATH_BY_STEELBARS_RIGHT, DEATH_BY_STEELBARS_LEFT,
	DEATH_BY_ENEMY_RIGHT, DEATH_BY_ENEMY_RIGHT_STOP, DEATH_BY_ENEMY_LEFT, DEATH_BY_ENEMY_LEFT_STOP,
	DEATH_BY_SAW

};

void Player::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{

	// Configuring the spritesheet
	spritesheet.setWrapS(GL_MIRRORED_REPEAT);
	spritesheet.loadFromFile("images/Spritesheet1.png", TEXTURE_PIXEL_FORMAT_RGBA);

	// Configuring a single sprite
	sprite = Sprite::createSprite(glm::ivec2(64, 64), glm::vec2(0.1, 0.05), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(57);

	// STAND LEFT
	sprite->setAnimationSpeed(STAND_LEFT, 8);
	sprite->addKeyframe(STAND_LEFT, glm::vec2(-0.1f, 0.f));

	// STAND RIGHT
	sprite->setAnimationSpeed(STAND_RIGHT, 1000);
	sprite->addKeyframe(STAND_RIGHT, glm::vec2(0.f, 0.f));

	// RUN LEFT

	/* 1) START RUN */
	sprite->setAnimationSpeed(START_RUN_LEFT, 8);
	sprite->addKeyframe(START_RUN_LEFT, glm::vec2(-0.1f, 0.05f));
	sprite->addKeyframe(START_RUN_LEFT, glm::vec2(-0.2f, 0.05f));
	sprite->addKeyframe(START_RUN_LEFT, glm::vec2(-0.3f, 0.05f));

	/* 2) ITER. RUNNING */
	sprite->setAnimationSpeed(MOVE_LEFT, 8);
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(-0.1f, 0.15f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(-0.2f, 0.15f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(-0.3f, 0.15f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(-0.4f, 0.15f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(-0.5f, 0.15f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(-0.6f, 0.15f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(-0.7f, 0.15f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(-0.8f, 0.15f));

	/* 3) STOP RUNNING */
	sprite->setAnimationSpeed(STOP_RUN_LEFT, 8);
	sprite->addKeyframe(STOP_RUN_LEFT, glm::vec2(-0.7f, 0.05f));
	sprite->addKeyframe(STOP_RUN_LEFT, glm::vec2(-0.8f, 0.05f));
	sprite->addKeyframe(STOP_RUN_LEFT, glm::vec2(-0.9f, 0.05f));
	sprite->addKeyframe(STOP_RUN_LEFT, glm::vec2(-1.f, 0.05f));

	// RUN RIGHT

	/* 1) START RUN */
	sprite->setAnimationSpeed(START_RUN_RIGHT, 8);
	sprite->addKeyframe(START_RUN_RIGHT, glm::vec2(0.0f, 0.05f));
	sprite->addKeyframe(START_RUN_RIGHT, glm::vec2(0.1f, 0.05f));
	sprite->addKeyframe(START_RUN_RIGHT, glm::vec2(0.2f, 0.05f));

	/* 2) ITER. RUNNING */
	sprite->setAnimationSpeed(MOVE_RIGHT, 8);
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.0f, 0.15f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.1f, 0.15f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.2f, 0.15f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.3f, 0.15f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.4f, 0.15f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.5f, 0.15f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.6f, 0.15f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.7f, 0.15f));

	/* 3) STOP RUNNING */
	sprite->setAnimationSpeed(STOP_RUN_RIGHT, 8);
	sprite->addKeyframe(STOP_RUN_RIGHT, glm::vec2(0.6f, 0.05f));
	sprite->addKeyframe(STOP_RUN_RIGHT, glm::vec2(0.7f, 0.05f));
	sprite->addKeyframe(STOP_RUN_RIGHT, glm::vec2(0.8f, 0.05f));
	sprite->addKeyframe(STOP_RUN_RIGHT, glm::vec2(0.9f, 0.05f));

	// WALK SLOW RIGHT

	sprite->setAnimationSpeed(SHIFT_RIGHT, 8);
	sprite->addKeyframe(SHIFT_RIGHT, glm::vec2(0.0f, 0.3f));
	sprite->addKeyframe(SHIFT_RIGHT, glm::vec2(0.1f, 0.3f));
	sprite->addKeyframe(SHIFT_RIGHT, glm::vec2(0.2f, 0.3f));
	sprite->addKeyframe(SHIFT_RIGHT, glm::vec2(0.3f, 0.3f));
	sprite->addKeyframe(SHIFT_RIGHT, glm::vec2(0.4f, 0.3f));
	sprite->addKeyframe(SHIFT_RIGHT, glm::vec2(0.5f, 0.3f));
	sprite->addKeyframe(SHIFT_RIGHT, glm::vec2(0.6f, 0.3f));

	// WALK SLOW LEFT

	sprite->setAnimationSpeed(SHIFT_LEFT, 8);
	sprite->addKeyframe(SHIFT_LEFT, glm::vec2(-0.1f, 0.3f));
	sprite->addKeyframe(SHIFT_LEFT, glm::vec2(-0.2f, 0.3f));
	sprite->addKeyframe(SHIFT_LEFT, glm::vec2(-0.3f, 0.3f));
	sprite->addKeyframe(SHIFT_LEFT, glm::vec2(-0.4f, 0.3f));
	sprite->addKeyframe(SHIFT_LEFT, glm::vec2(-0.5f, 0.3f));
	sprite->addKeyframe(SHIFT_LEFT, glm::vec2(-0.6f, 0.3f));
	sprite->addKeyframe(SHIFT_LEFT, glm::vec2(-0.7f, 0.3f));


	// TURN WHILE NOT MOVING 

	/*LEFT->RIGHT*/
	sprite->setAnimationSpeed(TURN_LEFT_TO_RIGHT, 8);
	sprite->addKeyframe(TURN_LEFT_TO_RIGHT, glm::vec2(0.0f, 0.25f));
	sprite->addKeyframe(TURN_LEFT_TO_RIGHT, glm::vec2(0.1f, 0.25f));
	sprite->addKeyframe(TURN_LEFT_TO_RIGHT, glm::vec2(0.2f, 0.25f));
	sprite->addKeyframe(TURN_LEFT_TO_RIGHT, glm::vec2(0.3f, 0.25f));
	sprite->addKeyframe(TURN_LEFT_TO_RIGHT, glm::vec2(0.4f, 0.25f));
	sprite->addKeyframe(TURN_LEFT_TO_RIGHT, glm::vec2(0.5f, 0.25f));

	/*RIGHT->LEFT*/
	sprite->setAnimationSpeed(TURN_RIGHT_TO_LEFT, 8);
	sprite->addKeyframe(TURN_RIGHT_TO_LEFT, glm::vec2(-0.1f, 0.25f));
	sprite->addKeyframe(TURN_RIGHT_TO_LEFT, glm::vec2(-0.2f, 0.25f));
	sprite->addKeyframe(TURN_RIGHT_TO_LEFT, glm::vec2(-0.3f, 0.25f));
	sprite->addKeyframe(TURN_RIGHT_TO_LEFT, glm::vec2(-0.4f, 0.25f));
	sprite->addKeyframe(TURN_RIGHT_TO_LEFT, glm::vec2(-0.5f, 0.25f));
	sprite->addKeyframe(TURN_RIGHT_TO_LEFT, glm::vec2(-0.6f, 0.25f));

	// TURN WHILE MOVING 

	/*LEFT->RIGHT*/
	sprite->setAnimationSpeed(CHANGE_DIRECTION_TO_RIGHT, 8);
	sprite->addKeyframe(CHANGE_DIRECTION_TO_RIGHT, glm::vec2(-0.1f, 0.2f));
	sprite->addKeyframe(CHANGE_DIRECTION_TO_RIGHT, glm::vec2(-0.2f, 0.2f));
	sprite->addKeyframe(CHANGE_DIRECTION_TO_RIGHT, glm::vec2(-0.3f, 0.2f));
	sprite->addKeyframe(CHANGE_DIRECTION_TO_RIGHT, glm::vec2(-0.4f, 0.2f));
	sprite->addKeyframe(CHANGE_DIRECTION_TO_RIGHT, glm::vec2(-0.5f, 0.2f));
	sprite->addKeyframe(CHANGE_DIRECTION_TO_RIGHT, glm::vec2(-0.6f, 0.2f));
	sprite->addKeyframe(CHANGE_DIRECTION_TO_RIGHT, glm::vec2(-0.7f, 0.2f));
	sprite->addKeyframe(CHANGE_DIRECTION_TO_RIGHT, glm::vec2(-0.8f, 0.2f));
	sprite->addKeyframe(CHANGE_DIRECTION_TO_RIGHT, glm::vec2(-0.9f, 0.2f));
	sprite->addKeyframe(CHANGE_DIRECTION_TO_RIGHT, glm::vec2(-1.f, 0.2f));
	sprite->addKeyframe(CHANGE_DIRECTION_TO_RIGHT, glm::vec2(-1.f, 0.15f));

	/*RIGHT->LEFT*/
	sprite->setAnimationSpeed(CHANGE_DIRECTION_TO_LEFT, 8);
	sprite->addKeyframe(CHANGE_DIRECTION_TO_LEFT, glm::vec2(0.0f, 0.2f));
	sprite->addKeyframe(CHANGE_DIRECTION_TO_LEFT, glm::vec2(0.1f, 0.2f));
	sprite->addKeyframe(CHANGE_DIRECTION_TO_LEFT, glm::vec2(0.2f, 0.2f));
	sprite->addKeyframe(CHANGE_DIRECTION_TO_LEFT, glm::vec2(0.3f, 0.2f));
	sprite->addKeyframe(CHANGE_DIRECTION_TO_LEFT, glm::vec2(0.4f, 0.2f));
	sprite->addKeyframe(CHANGE_DIRECTION_TO_LEFT, glm::vec2(0.5f, 0.2f));
	sprite->addKeyframe(CHANGE_DIRECTION_TO_LEFT, glm::vec2(0.6f, 0.2f));
	sprite->addKeyframe(CHANGE_DIRECTION_TO_LEFT, glm::vec2(0.7f, 0.2f));
	sprite->addKeyframe(CHANGE_DIRECTION_TO_LEFT, glm::vec2(0.8f, 0.2f));
	sprite->addKeyframe(CHANGE_DIRECTION_TO_LEFT, glm::vec2(0.9f, 0.2f));
	sprite->addKeyframe(CHANGE_DIRECTION_TO_LEFT, glm::vec2(0.9f, 0.15f));

	// JUMP WHILE MOVING

	/* RIGHT */
	sprite->setAnimationSpeed(JUMP_RUN_RIGHT, 8);
	sprite->addKeyframe(JUMP_RUN_RIGHT, glm::vec2(0.0f, 0.1f));
	sprite->addKeyframe(JUMP_RUN_RIGHT, glm::vec2(0.1f, 0.1f));
	sprite->addKeyframe(JUMP_RUN_RIGHT, glm::vec2(0.2f, 0.1f));
	sprite->addKeyframe(JUMP_RUN_RIGHT, glm::vec2(0.3f, 0.1f));
	sprite->addKeyframe(JUMP_RUN_RIGHT, glm::vec2(0.4f, 0.1f));
	sprite->addKeyframe(JUMP_RUN_RIGHT, glm::vec2(0.5f, 0.1f));
	sprite->addKeyframe(JUMP_RUN_RIGHT, glm::vec2(0.6f, 0.1f));
	sprite->addKeyframe(JUMP_RUN_RIGHT, glm::vec2(0.7f, 0.1f));
	sprite->addKeyframe(JUMP_RUN_RIGHT, glm::vec2(0.8f, 0.1f));
	sprite->addKeyframe(JUMP_RUN_RIGHT, glm::vec2(0.9f, 0.1f));

	/* LEFT */
	sprite->setAnimationSpeed(JUMP_RUN_LEFT, 8);
	sprite->addKeyframe(JUMP_RUN_LEFT, glm::vec2(-0.1f, 0.1f));
	sprite->addKeyframe(JUMP_RUN_LEFT, glm::vec2(-0.2f, 0.1f));
	sprite->addKeyframe(JUMP_RUN_LEFT, glm::vec2(-0.3f, 0.1f));
	sprite->addKeyframe(JUMP_RUN_LEFT, glm::vec2(-0.4f, 0.1f));
	sprite->addKeyframe(JUMP_RUN_LEFT, glm::vec2(-0.5f, 0.1f));
	sprite->addKeyframe(JUMP_RUN_LEFT, glm::vec2(-0.6f, 0.1f));
	sprite->addKeyframe(JUMP_RUN_LEFT, glm::vec2(-0.7f, 0.1f));
	sprite->addKeyframe(JUMP_RUN_LEFT, glm::vec2(-0.8f, 0.1f));
	sprite->addKeyframe(JUMP_RUN_LEFT, glm::vec2(-0.9f, 0.1f));
	sprite->addKeyframe(JUMP_RUN_LEFT, glm::vec2(-1.0f, 0.1f));

	// JUMP WHILE NOT MOVING RIGHT

	/* START JUMPING */
	sprite->setAnimationSpeed(START_JUMP_STAND_RIGHT, 8);
	sprite->addKeyframe(START_JUMP_STAND_RIGHT, glm::vec2(0.0f, 0.35f));
	sprite->addKeyframe(START_JUMP_STAND_RIGHT, glm::vec2(0.1f, 0.35f));
	sprite->addKeyframe(START_JUMP_STAND_RIGHT, glm::vec2(0.2f, 0.35f));
	sprite->addKeyframe(START_JUMP_STAND_RIGHT, glm::vec2(0.3f, 0.35f));
	sprite->addKeyframe(START_JUMP_STAND_RIGHT, glm::vec2(0.4f, 0.35f));

	/* JUMP AREA */
	sprite->setAnimationSpeed(JUMP_STAND_RIGHT, 8);
	sprite->addKeyframe(JUMP_STAND_RIGHT, glm::vec2(0.5f, 0.35f));
	sprite->addKeyframe(JUMP_STAND_RIGHT, glm::vec2(0.6f, 0.35f));
	sprite->addKeyframe(JUMP_STAND_RIGHT, glm::vec2(0.7f, 0.35f));
	sprite->addKeyframe(JUMP_STAND_RIGHT, glm::vec2(0.8f, 0.35f));
	sprite->addKeyframe(JUMP_STAND_RIGHT, glm::vec2(0.9f, 0.35f));

	/* LAND JUMP */
	sprite->setAnimationSpeed(STOP_JUMP_STAND_RIGHT, 8);
	sprite->addKeyframe(STOP_JUMP_STAND_RIGHT, glm::vec2(0.7f, 0.25f));
	sprite->addKeyframe(STOP_JUMP_STAND_RIGHT, glm::vec2(0.8f, 0.25f));
	sprite->addKeyframe(STOP_JUMP_STAND_RIGHT, glm::vec2(0.9f, 0.25f));
	sprite->addKeyframe(STOP_JUMP_STAND_RIGHT, glm::vec2(0.8f, 0.3f));
	sprite->addKeyframe(STOP_JUMP_STAND_RIGHT, glm::vec2(0.9f, 0.3f));

	// JUMP WHILE NOT MOVING LEFT

	/* START JUMPING */
	sprite->setAnimationSpeed(START_JUMP_STAND_LEFT, 8);
	sprite->addKeyframe(START_JUMP_STAND_LEFT, glm::vec2(-0.1f, 0.35f));
	sprite->addKeyframe(START_JUMP_STAND_LEFT, glm::vec2(-0.2f, 0.35f));
	sprite->addKeyframe(START_JUMP_STAND_LEFT, glm::vec2(-0.3f, 0.35f));
	sprite->addKeyframe(START_JUMP_STAND_LEFT, glm::vec2(-0.4f, 0.35f));
	sprite->addKeyframe(START_JUMP_STAND_LEFT, glm::vec2(-0.5f, 0.35f));

	/* JUMP AREA */
	sprite->setAnimationSpeed(JUMP_STAND_LEFT, 8);
	sprite->addKeyframe(JUMP_STAND_LEFT, glm::vec2(-0.6f, 0.35f));
	sprite->addKeyframe(JUMP_STAND_LEFT, glm::vec2(-0.7f, 0.35f));
	sprite->addKeyframe(JUMP_STAND_LEFT, glm::vec2(-0.8f, 0.35f));
	sprite->addKeyframe(JUMP_STAND_LEFT, glm::vec2(-0.9f, 0.35f));
	sprite->addKeyframe(JUMP_STAND_LEFT, glm::vec2(-1.0f, 0.35f));

	/* LAND JUMP */
	sprite->setAnimationSpeed(STOP_JUMP_STAND_LEFT, 8);
	sprite->addKeyframe(STOP_JUMP_STAND_LEFT, glm::vec2(-0.8f, 0.25f));
	sprite->addKeyframe(STOP_JUMP_STAND_LEFT, glm::vec2(-0.9f, 0.25f));
	sprite->addKeyframe(STOP_JUMP_STAND_LEFT, glm::vec2(-1.0f, 0.25f));
	sprite->addKeyframe(STOP_JUMP_STAND_LEFT, glm::vec2(-0.9f, 0.3f));
	sprite->addKeyframe(STOP_JUMP_STAND_LEFT, glm::vec2(-1.0f, 0.3f));

	// JUMP WHILE NOT MOVING RIGHT (UP)

	/* START JUMP */
	sprite->setAnimationSpeed(JUMP_UP_RIGHT_FLOOR, 8);
	sprite->addKeyframe(JUMP_UP_RIGHT_FLOOR, glm::vec2(0.0f, 0.4f));
	sprite->addKeyframe(JUMP_UP_RIGHT_FLOOR, glm::vec2(0.1f, 0.4f));
	sprite->addKeyframe(JUMP_UP_RIGHT_FLOOR, glm::vec2(0.2f, 0.4f));
	sprite->addKeyframe(JUMP_UP_RIGHT_FLOOR, glm::vec2(0.3f, 0.4f));
	sprite->addKeyframe(JUMP_UP_RIGHT_FLOOR, glm::vec2(0.4f, 0.4f));
	sprite->addKeyframe(JUMP_UP_RIGHT_FLOOR, glm::vec2(0.5f, 0.4f));
	sprite->addKeyframe(JUMP_UP_RIGHT_FLOOR, glm::vec2(0.6f, 0.4f));

	/* AIR */
	sprite->setAnimationSpeed(JUMP_UP_RIGHT, 8);
	sprite->addKeyframe(JUMP_UP_RIGHT, glm::vec2(0.7f, 0.4f));
	sprite->addKeyframe(JUMP_UP_RIGHT, glm::vec2(0.8f, 0.4f));
	sprite->addKeyframe(JUMP_UP_RIGHT, glm::vec2(0.9f, 0.4f));

	/* CLIMB */
	sprite->setAnimationSpeed(GO_UP_RIGHT, 8);
	sprite->addKeyframe(GO_UP_RIGHT, glm::vec2(0.0f, 0.45f));
	sprite->addKeyframe(GO_UP_RIGHT, glm::vec2(0.1f, 0.45f));
	sprite->addKeyframe(GO_UP_RIGHT, glm::vec2(0.2f, 0.45f));
	sprite->addKeyframe(GO_UP_RIGHT, glm::vec2(0.3f, 0.45f));
	sprite->addKeyframe(GO_UP_RIGHT, glm::vec2(0.4f, 0.45f));
	sprite->addKeyframe(GO_UP_RIGHT, glm::vec2(0.5f, 0.45f));
	sprite->addKeyframe(GO_UP_RIGHT, glm::vec2(0.6f, 0.45f));
	sprite->addKeyframe(GO_UP_RIGHT, glm::vec2(0.7f, 0.45f));
	sprite->addKeyframe(GO_UP_RIGHT, glm::vec2(0.8f, 0.45f));
	sprite->addKeyframe(GO_UP_RIGHT, glm::vec2(0.9f, 0.45f));
	sprite->addKeyframe(GO_UP_RIGHT, glm::vec2(0.9f, 0.0f));

	// JUMP WHILE NOT MOVING RIGHT (UP)

	/* START JUMP */
	sprite->setAnimationSpeed(JUMP_UP_LEFT_FLOOR, 8);
	sprite->addKeyframe(JUMP_UP_LEFT_FLOOR, glm::vec2(-0.1f, 0.4f));
	sprite->addKeyframe(JUMP_UP_LEFT_FLOOR, glm::vec2(-0.2f, 0.4f));
	sprite->addKeyframe(JUMP_UP_LEFT_FLOOR, glm::vec2(-0.3f, 0.4f));
	sprite->addKeyframe(JUMP_UP_LEFT_FLOOR, glm::vec2(-0.4f, 0.4f));
	sprite->addKeyframe(JUMP_UP_LEFT_FLOOR, glm::vec2(-0.5f, 0.4f));
	sprite->addKeyframe(JUMP_UP_LEFT_FLOOR, glm::vec2(-0.6f, 0.4f));
	sprite->addKeyframe(JUMP_UP_LEFT_FLOOR, glm::vec2(-0.7f, 0.4f));

	/* AIR */
	sprite->setAnimationSpeed(JUMP_UP_LEFT, 8);
	sprite->addKeyframe(JUMP_UP_LEFT, glm::vec2(-0.8f, 0.4f));
	sprite->addKeyframe(JUMP_UP_LEFT, glm::vec2(-0.9f, 0.4f));
	sprite->addKeyframe(JUMP_UP_LEFT, glm::vec2(-1.0f, 0.4f));
	
	/* CLIMB */
	sprite->setAnimationSpeed(GO_UP_LEFT, 8);
	sprite->addKeyframe(GO_UP_LEFT, glm::vec2(-0.1f, 0.45f));
	sprite->addKeyframe(GO_UP_LEFT, glm::vec2(-0.2f, 0.45f));
	sprite->addKeyframe(GO_UP_LEFT, glm::vec2(-0.3f, 0.45f));
	sprite->addKeyframe(GO_UP_LEFT, glm::vec2(-0.4f, 0.45f));
	sprite->addKeyframe(GO_UP_LEFT, glm::vec2(-0.5f, 0.45f));
	sprite->addKeyframe(GO_UP_LEFT, glm::vec2(-0.6f, 0.45f));
	sprite->addKeyframe(GO_UP_LEFT, glm::vec2(-0.7f, 0.45f));
	sprite->addKeyframe(GO_UP_LEFT, glm::vec2(-0.8f, 0.45f));
	sprite->addKeyframe(GO_UP_LEFT, glm::vec2(-0.9f, 0.45f));
	sprite->addKeyframe(GO_UP_LEFT, glm::vec2(-1.0f, 0.45f));
	sprite->addKeyframe(GO_UP_LEFT, glm::vec2(-1.0f, 0.45f));

	/* FALLING */
	sprite->setAnimationSpeed(GOING_TO_FALL_LEFT, 10);
	sprite->addKeyframe(GOING_TO_FALL_LEFT, glm::vec2(-0.3f, 0.0f));
	sprite->addKeyframe(GOING_TO_FALL_LEFT, glm::vec2(-0.4f, 0.0f));

	/* FRAME FALLING */
	sprite->setAnimationSpeed(FALLING_LEFT, 8);
	sprite->addKeyframe(FALLING_LEFT, glm::vec2(-0.5f, 0.0f));

	/* FRAME TOUCHING THE FLOOR */
	sprite->setAnimationSpeed(TOUCH_FLOOR_LEFT, 1);
	sprite->addKeyframe(TOUCH_FLOOR_LEFT, glm::vec2(-0.1f, 0.65f));

	/* WAKING UP FROM FALLEN */
	sprite->setAnimationSpeed(WAKE_UP_LEFT, 8);
	sprite->addKeyframe(WAKE_UP_LEFT, glm::vec2(-0.2f, 0.65f));
	sprite->addKeyframe(WAKE_UP_LEFT, glm::vec2(-0.3f, 0.65f));
	sprite->addKeyframe(WAKE_UP_LEFT, glm::vec2(-0.4f, 0.65f));
	sprite->addKeyframe(WAKE_UP_LEFT, glm::vec2(-0.5f, 0.65f));
	sprite->addKeyframe(WAKE_UP_LEFT, glm::vec2(-0.6f, 0.65f));

	/* FALLING */
	sprite->setAnimationSpeed(GOING_TO_FALL_RIGHT, 10);
	sprite->addKeyframe(GOING_TO_FALL_RIGHT, glm::vec2(0.2f, 0.0f));
	sprite->addKeyframe(GOING_TO_FALL_RIGHT, glm::vec2(0.3f, 0.0f));

	/* FRAME FALLING */
	sprite->setAnimationSpeed(FALLING_RIGHT, 8);
	sprite->addKeyframe(FALLING_RIGHT, glm::vec2(0.4f, 0.0f));

	/* FRAME TOUCHING THE FLOOR */
	sprite->setAnimationSpeed(TOUCH_FLOOR_RIGHT, 1);
	sprite->addKeyframe(TOUCH_FLOOR_RIGHT, glm::vec2(0.0f, 0.65f));

	/* WAKING UP FROM FALLEN */
	sprite->setAnimationSpeed(WAKE_UP_RIGHT, 8);
	sprite->addKeyframe(WAKE_UP_RIGHT, glm::vec2(0.1f, 0.65f));
	sprite->addKeyframe(WAKE_UP_RIGHT, glm::vec2(0.2f, 0.65f));
	sprite->addKeyframe(WAKE_UP_RIGHT, glm::vec2(0.3f, 0.65f));
	sprite->addKeyframe(WAKE_UP_RIGHT, glm::vec2(0.4f, 0.65f));
	sprite->addKeyframe(WAKE_UP_RIGHT, glm::vec2(0.5f, 0.65f));


	//FIGHTING

	/* GETTING SWORD OUT */
	sprite->setAnimationSpeed(SWORD_OUT_RIGHT, 8);
	sprite->addKeyframe(SWORD_OUT_RIGHT, glm::vec2(0.0f, 0.5f));
	sprite->addKeyframe(SWORD_OUT_RIGHT, glm::vec2(0.1f, 0.5f));
	sprite->addKeyframe(SWORD_OUT_RIGHT, glm::vec2(0.2f, 0.5f));
	sprite->addKeyframe(SWORD_OUT_RIGHT, glm::vec2(0.3f, 0.5f));


	/* GETTING SWORD OUT */
	sprite->setAnimationSpeed(SWORD_OUT_LEFT, 8);
	sprite->addKeyframe(SWORD_OUT_LEFT, glm::vec2(-0.1f, 0.5f));
	sprite->addKeyframe(SWORD_OUT_LEFT, glm::vec2(-0.2f, 0.5f));
	sprite->addKeyframe(SWORD_OUT_LEFT, glm::vec2(-0.3f, 0.5f));
	sprite->addKeyframe(SWORD_OUT_LEFT, glm::vec2(-0.4f, 0.5f));

	/* STANDING WITH SWORD OUT*/
	sprite->setAnimationSpeed(STAND_SWORD_LEFT, 8);
	sprite->addKeyframe(STAND_SWORD_LEFT, glm::vec2(-0.5f, 0.5f));

	/* STANDING WITH SWORD OUT*/
	sprite->setAnimationSpeed(STAND_SWORD_RIGHT, 8);
	sprite->addKeyframe(STAND_SWORD_RIGHT, glm::vec2(0.4f, 0.5f));

	/*MOVING WITH THE SWORD*/
	sprite->setAnimationSpeed(MOVE_SWORD_RIGHT, 8);
	//sprite->addKeyframe(MOVE_SWORD_RIGHT, glm::vec2(0.4f, 0.5f));
	sprite->addKeyframe(MOVE_SWORD_RIGHT, glm::vec2(0.0f, 0.55f));

	/*MOVING WITH THE SWORD*/
	sprite->setAnimationSpeed(MOVE_SWORD_LEFT, 8);
	//sprite->addKeyframe(MOVE_SWORD_LEFT, glm::vec2(-0.5f, 0.5f));
	sprite->addKeyframe(MOVE_SWORD_LEFT, glm::vec2(-0.1f, 0.55f));

	/* ATTACK */
	sprite->setAnimationSpeed(ATTACK_RIGHT, 8);
	sprite->addKeyframe(ATTACK_RIGHT, glm::vec2(0.1f, 0.55f));
	sprite->addKeyframe(ATTACK_RIGHT, glm::vec2(0.2f, 0.55f));
	sprite->addKeyframe(ATTACK_RIGHT, glm::vec2(0.3f, 0.55f));
	sprite->addKeyframe(ATTACK_RIGHT, glm::vec2(0.4f, 0.55f));

	/* ATTACK */
	sprite->setAnimationSpeed(ATTACK_LEFT, 8);
	sprite->addKeyframe(ATTACK_LEFT, glm::vec2(-0.2f, 0.55f));
	sprite->addKeyframe(ATTACK_LEFT, glm::vec2(-0.3f, 0.55f));
	sprite->addKeyframe(ATTACK_LEFT, glm::vec2(-0.4f, 0.55f));
	sprite->addKeyframe(ATTACK_LEFT, glm::vec2(-0.5f, 0.55f));

	/* BLOCKING */
	sprite->setAnimationSpeed(BLOCK_RIGHT, 8);
	sprite->addKeyframe(BLOCK_RIGHT, glm::vec2(0.0f, 0.75f));
	sprite->addKeyframe(BLOCK_RIGHT, glm::vec2(0.1f, 0.75f));
	sprite->addKeyframe(BLOCK_RIGHT, glm::vec2(0.2f, 0.75f));
	sprite->addKeyframe(BLOCK_RIGHT, glm::vec2(0.3f, 0.75f));

	/* BLOCKING */
	sprite->setAnimationSpeed(BLOCK_LEFT, 8);
	sprite->addKeyframe(BLOCK_LEFT, glm::vec2(-0.1f, 0.75f));
	sprite->addKeyframe(BLOCK_LEFT, glm::vec2(-0.2f, 0.75f));
	sprite->addKeyframe(BLOCK_LEFT, glm::vec2(-0.3f, 0.75f));
	sprite->addKeyframe(BLOCK_LEFT, glm::vec2(-0.4f, 0.75f));


	/* HIDE SWORD */
	sprite->setAnimationSpeed(HIDE_SWORD_RIGHT, 8);
	sprite->addKeyframe(HIDE_SWORD_RIGHT, glm::vec2(0.3f, 0.6f));
	sprite->addKeyframe(HIDE_SWORD_RIGHT, glm::vec2(0.4f, 0.6f));
	sprite->addKeyframe(HIDE_SWORD_RIGHT, glm::vec2(0.5f, 0.6f));
	sprite->addKeyframe(HIDE_SWORD_RIGHT, glm::vec2(0.6f, 0.6f));
	sprite->addKeyframe(HIDE_SWORD_RIGHT, glm::vec2(0.7f, 0.6f));
	sprite->addKeyframe(HIDE_SWORD_RIGHT, glm::vec2(0.8f, 0.6f));

	/* HIDE SWORD */
	sprite->setAnimationSpeed(HIDE_SWORD_LEFT, 8);
	sprite->addKeyframe(HIDE_SWORD_LEFT, glm::vec2(-0.4f, 0.6f));
	sprite->addKeyframe(HIDE_SWORD_LEFT, glm::vec2(-0.5f, 0.6f));
	sprite->addKeyframe(HIDE_SWORD_LEFT, glm::vec2(-0.6f, 0.6f));
	sprite->addKeyframe(HIDE_SWORD_LEFT, glm::vec2(-0.7f, 0.6f));
	sprite->addKeyframe(HIDE_SWORD_LEFT, glm::vec2(-0.8f, 0.6f));
	sprite->addKeyframe(HIDE_SWORD_LEFT, glm::vec2(-0.9f, 0.6f));

	/* DEATH BY FALLING RIGHT*/
	sprite->setAnimationSpeed(DEATH_BY_FALLING_RIGHT, 8);
	sprite->addKeyframe(DEATH_BY_FALLING_RIGHT, glm::vec2(0.1f, 0.7f));

	/* DEATH BY FALLING LEFT*/
	sprite->setAnimationSpeed(DEATH_BY_FALLING_LEFT, 8);
	sprite->addKeyframe(DEATH_BY_FALLING_LEFT, glm::vec2(-0.2f, 0.7f));

	/* DEATH BY STEELBARS RIGHT*/
	sprite->setAnimationSpeed(DEATH_BY_STEELBARS_RIGHT, 8);
	sprite->addKeyframe(DEATH_BY_STEELBARS_RIGHT, glm::vec2(0.2f, 0.7f));

	/* DEATH BY STEELBARS LEFT*/
	sprite->setAnimationSpeed(DEATH_BY_FALLING_LEFT, 8);
	sprite->addKeyframe(DEATH_BY_FALLING_LEFT, glm::vec2(-0.3f, 0.7f));

	/* DEATH BY ENEMY RIGHT ACTION*/
	sprite->setAnimationSpeed(DEATH_BY_ENEMY_RIGHT, 8);
	sprite->addKeyframe(DEATH_BY_ENEMY_RIGHT, glm::vec2(0.2f, 0.7f));
	sprite->addKeyframe(DEATH_BY_ENEMY_RIGHT, glm::vec2(0.3f, 0.7f));
	sprite->addKeyframe(DEATH_BY_ENEMY_RIGHT, glm::vec2(0.4f, 0.7f));

	/* DEATH BY SAW*/
	sprite->setAnimationSpeed(DEATH_BY_SAW, 8);
	sprite->addKeyframe(DEATH_BY_SAW , glm::vec2(0.6f, 0.7f));


	//Init sprite and position
	sprite->changeAnimation(1);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));

	// Start looking to right
	direction = 'r';
	
	// Init bFalling + bJumping
	bFalling = false; bJumping = false; fallenDistance = 0; 
	healthPoints = 3; easterEggPoint = 0; jumped = 0;
	fallStraight = true;
	saved = false;
	
}

void Player::restart() {
	bFalling = false; bJumping = false; fallenDistance = 0;
	healthPoints = 3; easterEggPoint = 0; jumped = 0;
	fallStraight = true;
	saved = false;
	sprite->changeAnimation(STAND_RIGHT);
}

void Player::update(int deltaTime)
{
	bool finishAction = sprite->update(deltaTime);

	int minDistX = 50000000;
	IA* sultan = new IA();

	int contador = 0;
	
	

	for each (IA* sultanIter in sultans){
		if (abs(minDistX > abs(sultanIter->getPosition().x - posPlayer.x)) && posPlayer.y == sultanIter->getPosition().y) {
			minDistX = abs(sultanIter->getPosition().x - posPlayer.x);
			sultan = sultanIter;
			contador++;
		}
	}
	if (contador == 0) sultan = sultans[0];
	
	bool fightRange = (posPlayer.y - sultan->getPosition().y == 0) && (abs(posPlayer.x - sultan->getPosition().x) <= ATTACK_RANGE);

	if (finishAction) {

		if (sultan->getPosition().y - posPlayer.y == 0
			&& sultan->getPosition().x - posPlayer.x <= PLAYER_VISION
			&& sultan->getPosition().x - posPlayer.x > 0
			&& !isSwordOut()
			&& sultan->getHealthEnemy() > 0
			&& !saved) sprite->changeAnimation(SWORD_OUT_RIGHT);
		
		else if (sultan->getPosition().y - posPlayer.y == 0
			&& sultan->getPosition().x - posPlayer.x <= PLAYER_VISION
			&& sultan->getPosition().x - posPlayer.x > 0
			&& !isSwordOut()
			&& sultan->getHealthEnemy() > 0
			&& saved && Game::instance().getSpecialKey(GLUT_KEY_UP)) sprite->changeAnimation(SWORD_OUT_RIGHT);

		else if (sultan->getPosition().y - posPlayer.y == 0
			&& posPlayer.x - sultan->getPosition().x <= PLAYER_VISION
			&& posPlayer.x - sultan->getPosition().x > 0
			&& !isSwordOut()
			&& sultan->getHealthEnemy() > 0
			&& !saved) sprite->changeAnimation(SWORD_OUT_LEFT);
		
		else if (sultan->getPosition().y - posPlayer.y == 0
			&& posPlayer.x - sultan->getPosition().x <= PLAYER_VISION
			&& posPlayer.x - sultan->getPosition().x > 0
			&& !isSwordOut()
			&& sultan->getHealthEnemy() > 0
			&& saved&& Game::instance().getSpecialKey(GLUT_KEY_UP)) sprite->changeAnimation(SWORD_OUT_LEFT);

		else switch (sprite->animation()) {

		case STAND_RIGHT:

			/* IF RIGHT + UP */ if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT) && Game::instance().getSpecialKey(GLUT_KEY_UP)) sprite->changeAnimation(START_JUMP_STAND_RIGHT);
			/* IF RIGHT + SHIFT */ else if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT) && (Game::instance().getSpecialKey(113) || Game::instance().getSpecialKey(112))) sprite->changeAnimation(SHIFT_RIGHT);
			/* IF RIGHT */ else if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT)) sprite->changeAnimation(START_RUN_RIGHT);
			/* IF LEFT */ else if (Game::instance().getSpecialKey(GLUT_KEY_LEFT)) sprite->changeAnimation(TURN_RIGHT_TO_LEFT);
			/* IF UP */ else if (Game::instance().getSpecialKey(GLUT_KEY_UP)) sprite->changeAnimation(JUMP_UP_RIGHT_FLOOR);

			break;

		case STAND_LEFT:

			
			/* IF LEFT + UP */ if (Game::instance().getSpecialKey(GLUT_KEY_LEFT) && Game::instance().getSpecialKey(GLUT_KEY_UP)) sprite->changeAnimation(START_JUMP_STAND_LEFT);
			/* IF LEFT + SHIFT */ else if (Game::instance().getSpecialKey(GLUT_KEY_LEFT) && (Game::instance().getSpecialKey(113) || Game::instance().getSpecialKey(112))) sprite->changeAnimation(SHIFT_LEFT);
			/* IF LEFT */ else if (Game::instance().getSpecialKey(GLUT_KEY_LEFT)) sprite->changeAnimation(START_RUN_LEFT);
			/* IF RIGHT */ else if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT)) sprite->changeAnimation(TURN_LEFT_TO_RIGHT);
			/* IF UP */ else if (Game::instance().getSpecialKey(GLUT_KEY_UP))  sprite->changeAnimation(JUMP_UP_LEFT_FLOOR); bJumping = true;
			
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

		case JUMP_UP_RIGHT_FLOOR:
			bJumping = true;
			sprite->changeAnimation(JUMP_UP_RIGHT);
			break;

		case JUMP_UP_RIGHT:

			if (map->canIMoveUpRight(posPlayer.x, posPlayer.y, glm::ivec2(32, 64))) sprite->changeAnimation(GO_UP_RIGHT);
			else { sprite->changeAnimation(STAND_RIGHT); bJumping = false; }
			break;

		case GO_UP_RIGHT:
			/* INDEPENDENTLY  OF KEY */ sprite->changeAnimation(STAND_RIGHT);
			posPlayer.x += 10; posPlayer.y -= 64;
			bJumping = false;  jumped = 0;
			break;

		case JUMP_UP_LEFT:
			if (map->canIMoveUpLeft(posPlayer.x, posPlayer.y, glm::ivec2(32, 64))) sprite->changeAnimation(GO_UP_LEFT);
			else { sprite->changeAnimation(STAND_LEFT); bJumping = false; jumped = 0; }

			break;

		case JUMP_UP_LEFT_FLOOR:
			bJumping = true;
			sprite->changeAnimation(JUMP_UP_LEFT);
			break;

		case GO_UP_LEFT:
			/* INDEPENDENTLY  OF KEY */ sprite->changeAnimation(STAND_LEFT);
			posPlayer.y -= 64;
			bJumping = false; jumped = 0;
			break;

			/* HIDE_SWORD_RIGHT, HIDE_SWORD_LEFT*/
		
		case GOING_TO_FALL_LEFT:
			sprite->changeAnimation(FALLING_LEFT);

			break;

		case GOING_TO_FALL_RIGHT:
			sprite->changeAnimation(FALLING_RIGHT);

			break;

		case FALLING_LEFT:

			break;

		case FALLING_RIGHT:

			break;

		case TOUCH_FLOOR_LEFT:
			sprite->changeAnimation(WAKE_UP_LEFT);

			break;

		case TOUCH_FLOOR_RIGHT:
			sprite->changeAnimation(WAKE_UP_RIGHT);
			break;

		case WAKE_UP_RIGHT:
			sprite->changeAnimation(STAND_RIGHT);

			break;

		case WAKE_UP_LEFT:
			sprite->changeAnimation(STAND_LEFT);
			break;

		case SWORD_OUT_LEFT:
			sprite->changeAnimation(STAND_SWORD_LEFT);

			break;

		case SWORD_OUT_RIGHT:
			sprite->changeAnimation(STAND_SWORD_RIGHT);

			break;

		case STAND_SWORD_LEFT:

			if (Game::instance().getSpecialKey(113) || Game::instance().getSpecialKey(112)) sprite->changeAnimation(ATTACK_LEFT);
			else if (Game::instance().getSpecialKey(GLUT_KEY_LEFT) ) sprite->changeAnimation(MOVE_SWORD_LEFT);
			else if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT))  sprite->changeAnimation(MOVE_SWORD_LEFT); 
			else if (Game::instance().getSpecialKey(GLUT_KEY_UP)) sprite->changeAnimation(BLOCK_LEFT);
			else if (Game::instance().getSpecialKey(GLUT_KEY_DOWN)) {
				sprite->changeAnimation(HIDE_SWORD_LEFT); saved = true;
			}

			if (sultan->getPosition().y - posPlayer.y <= 0 && posPlayer.x - sultan->getPosition().x <= PLAYER_VISION && posPlayer.x - sultan->getPosition().x <= 0)
				sprite->changeAnimation(STAND_SWORD_RIGHT);
				
			break;

		case STAND_SWORD_RIGHT:

			if (Game::instance().getSpecialKey(113) || Game::instance().getSpecialKey(112)) sprite->changeAnimation(ATTACK_RIGHT);
			else if (Game::instance().getSpecialKey(GLUT_KEY_LEFT) )sprite->changeAnimation(MOVE_SWORD_RIGHT);
			else if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT) )sprite->changeAnimation(MOVE_SWORD_RIGHT); 
			else if (Game::instance().getSpecialKey(GLUT_KEY_UP)) sprite->changeAnimation(BLOCK_RIGHT);
			else if (Game::instance().getSpecialKey(GLUT_KEY_DOWN)) {
				sprite->changeAnimation(HIDE_SWORD_RIGHT); saved = true;
			}

			if (sultan->getPosition().y - posPlayer.y <= 0 && sultan->getPosition().x - posPlayer.x <= PLAYER_VISION && posPlayer.x - sultan->getPosition().x > 0)
				sprite->changeAnimation(STAND_SWORD_LEFT);

			break;

		case MOVE_SWORD_LEFT:
			if (Game::instance().getSpecialKey(113) || Game::instance().getSpecialKey(112)) sprite->changeAnimation(ATTACK_LEFT);
			else if (!Game::instance().getSpecialKey(GLUT_KEY_LEFT) && !Game::instance().getSpecialKey(GLUT_KEY_RIGHT))sprite->changeAnimation(STAND_SWORD_LEFT);

			if (sultan->getPosition().y - posPlayer.y == 0 && posPlayer.x - sultan->getPosition().x <= PLAYER_VISION && posPlayer.x - sultan->getPosition().x < 0)
				sprite->changeAnimation(STAND_SWORD_RIGHT);

			break;

		case MOVE_SWORD_RIGHT:

			if (Game::instance().getSpecialKey(113) || Game::instance().getSpecialKey(112)) sprite->changeAnimation(ATTACK_RIGHT);
			else if (!Game::instance().getSpecialKey(GLUT_KEY_LEFT) && !Game::instance().getSpecialKey(GLUT_KEY_RIGHT))sprite->changeAnimation(STAND_SWORD_RIGHT);

			if (sultan->getPosition().y - posPlayer.y == 0 && sultan->getPosition().x - posPlayer.x <= PLAYER_VISION && sultan->getPosition().x - posPlayer.x < 0)
				sprite->changeAnimation(STAND_SWORD_LEFT);

			break;

		case ATTACK_LEFT:

			if (!sultan->sultanIsBlockking() && fightRange)sultan->dealDamageEnemy(1);
			sprite->changeAnimation(STAND_SWORD_LEFT);

			break;

		case ATTACK_RIGHT:

			if (!sultan->sultanIsBlockking() && fightRange)sultan->dealDamageEnemy(1);
			sprite->changeAnimation(STAND_SWORD_RIGHT);

			break;

		case BLOCK_LEFT:

			sprite->changeAnimation(STAND_SWORD_LEFT);

			break;

		case BLOCK_RIGHT:

			sprite->changeAnimation(STAND_SWORD_RIGHT);

			break;

		case HIDE_SWORD_LEFT:

			sprite->changeAnimation(STAND_LEFT);

			break;

		case HIDE_SWORD_RIGHT:

			sprite->changeAnimation(STAND_RIGHT);

			break;
		
		case DEATH_BY_ENEMY_RIGHT:
			sprite->changeAnimation(DEATH_BY_ENEMY_RIGHT_STOP);
			break;

			
		}
	}

	if (!map->collisionMoveDown(posPlayer.x, posPlayer.y + FALL_STEP, glm::ivec2(32, 64), direction) && !bJumping) {
		bFalling = true; posPlayer.y += FALL_STEP; fallenDistance += FALL_STEP; 
		if (sprite->animation() == STOP_JUMP_STAND_RIGHT || sprite->animation() == JUMP_STAND_RIGHT || sprite->animation() == JUMP_RUN_RIGHT || sprite->animation() == CHANGE_DIRECTION_TO_LEFT ||
			sprite->animation() == STOP_RUN_RIGHT || sprite->animation() == MOVE_RIGHT || sprite->animation() == SHIFT_RIGHT || sprite->animation() == START_RUN_RIGHT)sprite->changeAnimation(FALLING_RIGHT);
		else sprite->changeAnimation(FALLING_LEFT);

	}

	if (map->collisionMoveDown(posPlayer.x, posPlayer.y + FALL_STEP, glm::ivec2(32, 64), direction) && bFalling) {
		bFalling = false; posPlayer.y += FALL_STEP; 
		/*healthPoints -= fallenDistance / 64;*/ if (healthPoints < 0) healthPoints = 0;
		if (fallenDistance / 64 >= 1 && sprite->animation() == FALLING_RIGHT) { sprite->changeAnimation(TOUCH_FLOOR_RIGHT); lastDamageType = "fall_right"; }
		else if (sprite->animation() == FALLING_RIGHT) sprite->changeAnimation(WAKE_UP_RIGHT);
		else if ((fallenDistance) / 64 >= 1 && sprite->animation() == FALLING_LEFT) { sprite->changeAnimation(TOUCH_FLOOR_LEFT); lastDamageType = "fall_left"; }
		else if (sprite->animation() == FALLING_LEFT) sprite->changeAnimation(WAKE_UP_LEFT);
		fallenDistance = 0;
	}


	if (sprite->animation() == START_RUN_RIGHT  && !map->collisionMoveRight(posPlayer.x + 1.f, posPlayer.y, glm::ivec2(32, 64))) { posPlayer.x += 1.f; bJumping = false; }
	else if (sprite->animation() == SHIFT_RIGHT && !map->collisionMoveRight(posPlayer.x + 0.5f, posPlayer.y, glm::ivec2(32, 64))){ posPlayer.x += 0.5f; bJumping = false; }
	else if (sprite->animation() == MOVE_RIGHT && !map->collisionMoveRight(posPlayer.x + 1.f, posPlayer.y, glm::ivec2(32, 64))) { posPlayer.x += 1.f; bJumping = false; }
	else if (sprite->animation() == STOP_RUN_RIGHT && !map->collisionMoveRight(posPlayer.x + 1.f, posPlayer.y, glm::ivec2(32, 64))) { posPlayer.x += 1.f; bJumping = false; }
	else if (sprite->animation() == CHANGE_DIRECTION_TO_LEFT && !map->collisionMoveRight(posPlayer.x + 1.f, posPlayer.y, glm::ivec2(32, 64))) { posPlayer.x += 1.f;  bJumping = false; }
	else if (sprite->animation() == JUMP_RUN_RIGHT && !map->collisionMoveRight(posPlayer.x + 1.5f, posPlayer.y, glm::ivec2(32, 64))) { posPlayer.x += 1.5f; bJumping = true; }
	else if (sprite->animation() == JUMP_STAND_RIGHT && !map->collisionMoveRight(posPlayer.x + 1.5f, posPlayer.y, glm::ivec2(32, 64))) { posPlayer.x += 1.5f; bJumping = true; }
	else if (sprite->animation() == STOP_JUMP_STAND_RIGHT && !map->collisionMoveRight(posPlayer.x + 0.5f, posPlayer.y, glm::ivec2(32, 64))) { posPlayer.x += 0.5f; bJumping = false; }
	else if (Game::instance().getSpecialKey(GLUT_KEY_LEFT) && sprite->animation() == MOVE_SWORD_RIGHT && !map->collisionMoveLeft(posPlayer.x - 1.0f, posPlayer.y, glm::ivec2(32, 64))) { posPlayer.x -= 1.0f; bJumping = false; }
	else if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT) && sprite->animation() == MOVE_SWORD_RIGHT && !map->collisionMoveRight(posPlayer.x + 1.0f, posPlayer.y, glm::ivec2(32, 64))) { posPlayer.x += 1.0f; bJumping = false; }

	else if (sprite->animation() == STOP_JUMP_STAND_RIGHT || sprite->animation() == JUMP_STAND_RIGHT || sprite->animation() == JUMP_RUN_RIGHT || sprite->animation() == CHANGE_DIRECTION_TO_LEFT ||
		sprite->animation() == STOP_RUN_RIGHT || sprite->animation() == MOVE_RIGHT || sprite->animation() == SHIFT_RIGHT || sprite->animation() == START_RUN_RIGHT) { sprite->changeAnimation(STAND_RIGHT); bJumping = false; }

	else if (sprite->animation() == START_RUN_LEFT && !map->collisionMoveLeft(posPlayer.x - 1.f, posPlayer.y, glm::ivec2(32, 64))) { posPlayer.x -= 1.f; bJumping = false; }
	else if (sprite->animation() == MOVE_LEFT && !map->collisionMoveLeft(posPlayer.x - 1.f, posPlayer.y, glm::ivec2(32, 64))) { posPlayer.x -= 1.f; bJumping = false; }
	else if (sprite->animation() == STOP_RUN_LEFT && !map->collisionMoveLeft(posPlayer.x - 1.f, posPlayer.y, glm::ivec2(32, 64))) { posPlayer.x -= 1.f; bJumping = false; }
	else if (sprite->animation() == SHIFT_LEFT && !map->collisionMoveLeft(posPlayer.x - 0.5f, posPlayer.y, glm::ivec2(32, 64))) { posPlayer.x -= 0.5f; bJumping = false; }
	else if (sprite->animation() == CHANGE_DIRECTION_TO_RIGHT && !map->collisionMoveLeft(posPlayer.x - 1.f, posPlayer.y, glm::ivec2(32, 64))) { posPlayer.x -= 1.f; bJumping = false; }
	else if (sprite->animation() == JUMP_RUN_LEFT && !map->collisionMoveLeft(posPlayer.x - 1.5f, posPlayer.y, glm::ivec2(32, 64))) { posPlayer.x -= 1.5f; bJumping = true; }
	else if (sprite->animation() == JUMP_STAND_LEFT && !map->collisionMoveLeft(posPlayer.x - 1.5f, posPlayer.y, glm::ivec2(32, 64))) { posPlayer.x -= 1.5f; bJumping = true; }
	else if (sprite->animation() == STOP_JUMP_STAND_LEFT && !map->collisionMoveLeft(posPlayer.x - 0.5f, posPlayer.y, glm::ivec2(32, 64))) { posPlayer.x -= 0.5f; bJumping = false; }
	else if (Game::instance().getSpecialKey(GLUT_KEY_LEFT) && sprite->animation() == MOVE_SWORD_LEFT && !map->collisionMoveLeft(posPlayer.x - 1.0f, posPlayer.y, glm::ivec2(32, 64))) { posPlayer.x -= 1.0f; bJumping = false; }
	else if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT) && sprite->animation() == MOVE_SWORD_LEFT && !map->collisionMoveRight(posPlayer.x + 1.0f, posPlayer.y, glm::ivec2(32, 64))) {posPlayer.x += 1.0f; bJumping = false; }

	else if (sprite->animation() == STOP_JUMP_STAND_LEFT || sprite->animation() == JUMP_STAND_LEFT || sprite->animation() == JUMP_RUN_LEFT || sprite->animation() == CHANGE_DIRECTION_TO_RIGHT ||
		sprite->animation() == STOP_RUN_LEFT || sprite->animation() == MOVE_LEFT || sprite->animation() == SHIFT_LEFT || sprite->animation() == START_RUN_LEFT) { sprite->changeAnimation(STAND_LEFT);  bJumping = false; }

	if (sprite->animation() == JUMP_UP_LEFT || sprite->animation() == JUMP_UP_LEFT) jumped += 1; 
	if (sprite->animation() == GO_UP_LEFT || sprite->animation() == GO_UP_RIGHT) { 
		if (jumped + 0.75 <= 64) jumped += 0.75;
		else jumped = 64;
	}

	if (healthPoints == 0)  {
		if (lastDamageType == "fall_right") sprite->changeAnimation(DEATH_BY_FALLING_RIGHT);
		else if (lastDamageType == "fall_left") sprite->changeAnimation(DEATH_BY_FALLING_LEFT);
		else if (lastDamageType == "steelbars") sprite->changeAnimation(DEATH_BY_STEELBARS_RIGHT);
		else if (lastDamageType == "saw")  {
			sprite->changeAnimation(DEATH_BY_SAW);
			int xPlayer = (posPlayer.x + 32 - 1) / 32;
			int yPlayer = posPlayer.y / 64;
			posPlayer.x = xPlayer * 32 - 12;
			posPlayer.y = yPlayer * 64 + 5;
		} else if (lastDamageType == "enemy") sprite->changeAnimation(DEATH_BY_ENEMY_RIGHT);
	}

	if ((posPlayer.y / 64) == 4 && posPlayer.x >= 2680) {
		posPlayer.y = 640, posPlayer.x = 1450;
	}
	else if ((posPlayer.y / 64) == 10 && (posPlayer.x / 32) == 82) {
		posPlayer.y = 13 * 64; posPlayer.x = 73 * 32; sprite->changeAnimation(STAND_RIGHT);
	}

	//if (sultan->sultanIsAttacking())dealDamage(1, "enemy");

	for each (IA* sultan in sultans) sultan->setPlayerStats(this);

	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y - 8 - jumped)));

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

	return float(tileMapDispl.x + posPlayer.x +32) / widthScreen;

}

float Player::GetScreenY(int heightScreen) {

	return float(tileMapDispl.y + posPlayer.y ) / heightScreen;

}

void Player::dealDamage(int damage, string type) {

	if ((type == "saw" || type == "steelbars") && 
		(sprite->animation() != SHIFT_LEFT && sprite->animation() != SHIFT_RIGHT && 
		sprite->animation() != STAND_RIGHT && sprite->animation() != STAND_LEFT)) {
		if (healthPoints > damage) { healthPoints -= damage; lastDamageType = type; }
		else { healthPoints = 0; lastDamageType = type; }
	}
	
	else if ((type == "enemy") ) {
		if (!isSwordOut()) { healthPoints = 0; lastDamageType = type; }
		else { healthPoints -= damage; lastDamageType = type; }
	}

}

bool Player::isAttacking() {

	if (sprite->animation() != ATTACK_RIGHT
		&& sprite->animation() != ATTACK_LEFT
		) return false;

	else return true;
}




void Player::setSultans(vector<IA*>sultans)
{
	this->sultans = sultans;
}

bool Player::isBlocking() {

	if (sprite->animation() != BLOCK_RIGHT
		&& sprite->animation() != BLOCK_LEFT) return false;

	else return true;

}

bool Player::isSwordOut() {

	if (sprite->animation() != BLOCK_RIGHT
		&& sprite->animation() != BLOCK_LEFT
		&& sprite->animation() != ATTACK_LEFT
		&& sprite->animation() != ATTACK_RIGHT
		&& sprite->animation() != SWORD_OUT_LEFT
		&& sprite->animation() != SWORD_OUT_RIGHT
		&& sprite->animation() != BLOCK_RIGHT
		&& sprite->animation() != BLOCK_LEFT
		&& sprite->animation() != MOVE_SWORD_LEFT
		&& sprite->animation() != MOVE_SWORD_RIGHT
		&& sprite->animation() != STAND_SWORD_LEFT
		&& sprite->animation() != STAND_SWORD_RIGHT
		&& sprite->animation() != HIDE_SWORD_LEFT
		&& sprite->animation() != HIDE_SWORD_RIGHT) return false;

	else return true;

}