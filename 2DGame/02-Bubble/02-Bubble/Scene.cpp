#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <GL/glew.h>
#include <GL/glut.h>
#include <glm/gtc/matrix_transform.hpp>
#include "Scene.h"
#include "Game.h"
#include "TrapSteelBars.h"
#include "Torch.h"
#include "TrapSaw.h"
#include "Princess.h"


#define SCREEN_X 0
#define SCREEN_Y 0

#define INIT_PLAYER_X_TILES 74
#define INIT_PLAYER_Y_TILES 10

#define SCREEN_X_SULTAN 0
#define SCREEN_Y_SULTAN 0

#define INIT_SULTAN_X_TILES 7
#define INIT_SULTAN_Y_TILES 4

Scene::Scene()
{
	map = NULL;
	player = NULL;
}

Scene::~Scene()
{
	if (map != NULL)
		delete map;
	if (player != NULL)
		delete player;
}

void Scene::init()
{

	initShaders();

	map = TileMap::createTileMap("levels/level01.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
	columns = TileMap::createTileMap(map->getColumnsFile(), glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
	initTorches(map->getTorchesFile());

	player = new Player();
	player->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize().first, INIT_PLAYER_Y_TILES * map->getTileSize().second));
	player->setTileMap(map);

	sultans = new IA();
	sultans->init(glm::ivec2(SCREEN_X_SULTAN, SCREEN_Y_SULTAN), texProgram);
	sultans->setPosition(glm::vec2(INIT_SULTAN_X_TILES * map->getTileSize().first, INIT_SULTAN_Y_TILES * map->getTileSize().second));
	sultans->setTileMap(map);
	sultans->setPlayer(player);

	player->setSultans(sultans);

	initTraps(map->getTrapsFile());
	initDoors("levels/level01doors.txt");

	healthInterface = HealthInterface::createHealthInterface(-1, glm::vec2(SCREEN_X, SCREEN_Y), texProgram);

	const float kOffsetX = static_cast<int>(player->GetScreenX(10 * map->getTileSize().first)) * 10 * map->getTileSize().first;
	const float kOffsetY = static_cast<int>(player->GetScreenY(3 * map->getTileSize().second)) * 3 * map->getTileSize().second;
	projection = glm::ortho(kOffsetX, SCREEN_WIDTH + kOffsetX, SCREEN_HEIGHT + kOffsetY, kOffsetY);
	glm::vec2 poscam; poscam.x = kOffsetX; poscam.y = kOffsetY;
	healthInterface->update(-1, poscam);

	currentTime = 0.0f;
	
	bPrincipalMenu = true; bStory1 = false; bStory2 = false; bInstructions = false; bCredits = false;

	princess = new Princess();
	princess->init("images/SpriteSheetPV.png", glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);

	coin1 = new CoinEasterEgg();
	coin1->init(glm::vec2(72 * 32, 10 * 64), glm::ivec2(SCREEN_X, SCREEN_Y), player, texProgram);
	coin2 = new CoinEasterEgg();
	coin2->init(glm::vec2(76 * 32, 10 * 64), glm::ivec2(SCREEN_X, SCREEN_Y), player, texProgram);

}

void Scene::restart() {


	player->restart();
	player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize().first, INIT_PLAYER_Y_TILES * map->getTileSize().second));
	restartTraps();
}

void Scene::update(int deltaTime)
{

	if (healthInterface->isMenu()) {

		const float kOffsetX = static_cast<int>(player->GetScreenX(10 * 32)) * 10 * 32;
		const float kOffsetY = static_cast<int>(player->GetScreenY(3 * 64)) * 3 * 64;
		glm::vec2 poscam; poscam.x = kOffsetX; poscam.y = kOffsetY;
		if (!bCredits) projection = glm::ortho(kOffsetX, SCREEN_WIDTH + kOffsetX, SCREEN_HEIGHT + kOffsetY, kOffsetY);
		
		if (bPrincipalMenu) {
			if (Game::instance().getSpecialKey(GLUT_KEY_LEFT)) { healthInterface->update(-3, poscam); bPrincipalMenu = false; bStory1 = true; timer = 0; }
			else if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT)) { healthInterface->update(-2, poscam); bPrincipalMenu = false; bInstructions = true; }
		}

		else if (bInstructions) {
			if (Game::instance().getSpecialKey(GLUT_KEY_UP)) { healthInterface->update(-1, poscam); bPrincipalMenu = true; bInstructions = false; }
		}

		else if (bStory1) {
			timer += deltaTime;
			projection = glm::ortho(kOffsetX, 1295 + kOffsetX, 816 + kOffsetY, kOffsetY);
			glm::vec2 poscam; poscam.x = kOffsetX; poscam.y = kOffsetY;
			if (timer >= 1000) { healthInterface->update(-4, poscam); bStory1 = false; bStory2 = true; }

		}

		else if (bStory2) {
			timer += deltaTime;
			projection = glm::ortho(kOffsetX, 1295 + kOffsetX, 816 + kOffsetY, kOffsetY);
			glm::vec2 poscam; poscam.x = kOffsetX; poscam.y = kOffsetY;
			if (timer >= 2000) { healthInterface->update(player->getHealth(), poscam); bStory2 = false; timer = 0; }

		}

		else if (bCredits) {
			if (Game::instance().getSpecialKey(GLUT_KEY_UP)) { healthInterface->update(-1, poscam); bPrincipalMenu = true; bCredits = false; }

		}
	}

	else if ((player->getPosition().x / 32) == 73 && (player->getPosition().y) / 64 == 13) {

		if (!princess->finished()) {

			const float kOffsetX = static_cast<int>(player->GetScreenX(10 * 32)) * 10 * 32;
			const float kOffsetY = static_cast<int>(player->GetScreenY(3 * 64)) * 3 * 64;
			projection = glm::ortho(kOffsetX, SCREEN_WIDTH + kOffsetX, SCREEN_HEIGHT + kOffsetY, kOffsetY);
			glm::vec2 poscam; poscam.x = kOffsetX; poscam.y = kOffsetY;

			if (player->getEasterEggPoints() >= 2 && !princess->peach()) princess->init("images/SpriteSheetPeach.png", glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
			else if (player->getEasterEggPoints() < 2 && princess->peach()) princess->init("images/SpriteSheetPV.png", glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
			princess->setStarted(true); princess->update(deltaTime);

		}

		else if (princess->finished()) {

			restart();

			const float kOffsetX = static_cast<int>(player->GetScreenX(10 * 32)) * 10 * 32;
			const float kOffsetY = static_cast<int>(player->GetScreenY(3 * 64)) * 3 * 64;
			projection = glm::ortho(kOffsetX, 1295 + kOffsetX, 816 + kOffsetY, kOffsetY);
			glm::vec2 poscam; poscam.x = kOffsetX; poscam.y = kOffsetY;

			healthInterface->update(-5, poscam); bCredits = true;

		}
	}

	else if (!healthInterface->isMenu()) {

		currentTime += deltaTime;

		if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT) && player->getHealth() <= 0) restart();

		for each (Torch* torch in torches)
			torch->update(deltaTime);

		player->update(deltaTime);
		//sultans->update(deltaTime);

		for each (TrapSteelBars* trap in trapsFloor)
			trap->update(deltaTime);

		for each (TrapFallingFloor* trap in trapsFallingFloor)
			trap->update(deltaTime);

		for each (TrapDoor* trap in trapsDoor)
			trap->update(deltaTime);

		for each (TrapSaw* trap in trapsSaw)
			trap->update(deltaTime);

		const float kOffsetX = static_cast<int>(player->GetScreenX(10 * 32)) * 10 * 32;
		const float kOffsetY = static_cast<int>(player->GetScreenY(3 * 64)) * 3 * 64;
		projection = glm::ortho(kOffsetX, SCREEN_WIDTH + kOffsetX, SCREEN_HEIGHT + kOffsetY, kOffsetY);
		glm::vec2 poscam; poscam.x = kOffsetX; poscam.y = kOffsetY;

		if (lastHP != player->getHealth() || lastPos.x != kOffsetX || lastPos.y != kOffsetY) healthInterface->update(player->getHealth(), poscam);
		lastHP = player->getHealth(); lastPos = glm::ivec2(kOffsetX, kOffsetY);
		coin1->update(deltaTime); coin2->update(deltaTime);

	}
}

void Scene::render()
{
	glm::mat4 modelview;

	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
	map->render();

	for each (Torch* torch in torches) torch->render();
	for each (TrapFallingFloor* trap in trapsFallingFloor) trap->render();
	for each (TrapSaw* trap in trapsSaw) trap->render();
	for each (TrapDoor* trap in trapsDoor) trap->render();

	player->render();
	//sultans->render();

	for each (TrapSteelBars* trap in trapsFloor) trap->render();

	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
	columns->render();

	princess->render();
	coin1->render(); coin2->render();

	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
	healthInterface->render();

	
	

}

void Scene::initShaders()
{
	Shader vShader, fShader;

	vShader.initFromFile(VERTEX_SHADER, "shaders/texture.vert");
	if (!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/texture.frag");
	if (!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	texProgram.init();
	texProgram.addShader(vShader);
	texProgram.addShader(fShader);
	texProgram.link();
	if (!texProgram.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << texProgram.log() << endl << endl;
	}
	texProgram.bindFragmentOutput("outColor");
	vShader.free();
	fShader.free();
}

void Scene::initTraps(string trapsFile) {

	ifstream fin;
	string line, spriteTrapsFile;
	stringstream sstream;
	glm::ivec2 mapSize;
	char trap[10];

	fin.open(trapsFile.c_str());
	getline(fin, line);

	if (line.compare(0, 7, "TRAPSMAP!") != 0) {

		getline(fin, line);
		sstream.str(line);
		sstream >> mapSize.x >> mapSize.y;

		getline(fin, line);
		sstream.str(line);
		sstream >> spriteTrapsFile;

		for (int j = 0; j<mapSize.y; j++) {
			for (int i = 0; i < mapSize.x; i++) {

				fin.getline(trap, 10, ',');

				if (strcmp(trap, "0") == 0) {

					TrapFallingFloor* trap = new TrapFallingFloor();
					trap->init(glm::vec2(i * map->getTileSize().first, j * map->getTileSize().second), glm::ivec2(SCREEN_X, SCREEN_Y), player, map, texProgram);
					trapsFallingFloor.push_back(trap);

				}

				if (strcmp(trap, "1") == 0) {

				TrapSteelBars* trap = new TrapSteelBars();
				trap->init(glm::vec2(i * map->getTileSize().first, j * map->getTileSize().second), glm::ivec2(SCREEN_X, SCREEN_Y), player, texProgram);
				trapsFloor.push_back(trap);

				}

				if (strcmp(trap, "2") == 0) {

					TrapSaw* trap = new TrapSaw();
					trap->init(glm::vec2(i * map->getTileSize().first, j * map->getTileSize().second), glm::ivec2(SCREEN_X, SCREEN_Y), player, texProgram);
					trapsSaw.push_back(trap);

				}
			}
		}
	}

	fin.close();

}

void Scene::initDoors(string doorsFile) {

	ifstream fin;
	string line, spriteTrapsFile;
	stringstream sstream;
	glm::ivec2 mapSize;
	int numberOfTraps;
	int doorx, doory, pplatex, pplatey;

	fin.open(doorsFile.c_str());
	getline(fin, line);

	if (line.compare(0, 7, "DOORMAP!") != 0) {

		getline(fin, line);
		sstream.str(line);
		sstream >> numberOfTraps;

		getline(fin, line);
		sstream.str(line);
		sstream >> spriteTrapsFile;

		for (int i = 0; i < numberOfTraps; ++i) {
			
			fin >> doorx >> doory >> pplatex >> pplatey;
			TrapDoor* door = new TrapDoor();
			door->init(glm::vec2(doorx*32, doory*64), glm::vec2(pplatex*32, pplatey*64), glm::ivec2(SCREEN_X, SCREEN_Y), player, map, texProgram);
			trapsDoor.push_back(door);

		}

		
	}

	fin.close();

}

void Scene::initTorches(string torchesFile) {

	ifstream fin;
	string line, spriteTrapsFile;
	stringstream sstream;
	glm::ivec2 mapSize;
	char trap[10];

	fin.open(torchesFile.c_str());
	getline(fin, line);

	if (line.compare(0, 7, "TORCHESMAP!") != 0) {

		getline(fin, line);
		sstream.str(line);
		sstream >> mapSize.x >> mapSize.y;

		getline(fin, line);
		sstream.str(line);
		sstream >> spriteTrapsFile;

		for (int j = 0; j<mapSize.y; j++) {
			for (int i = 0; i < mapSize.x; i++) {

				fin.getline(trap, 10, ',');

				if (strcmp(trap, "0") == 0) {

					Torch* torch = new Torch();
					torch->init(glm::vec2(i * map->getTileSize().first, j * map->getTileSize().second), glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
					torches.push_back(torch);

				}
			}
		}
	}

	fin.close();

}

void Scene::restartTraps() {

	for each (TrapSteelBars* trap in trapsFloor)
		trap->restart();

	for each (TrapFallingFloor* trap in trapsFallingFloor)
		trap->restart();

	for each (TrapDoor* trap in trapsDoor)
		trap->restart();

	for each (TrapSaw* trap in trapsSaw)
		trap->restart();

	princess->restart(); 

	coin1->restart();
	coin2->restart();

}
