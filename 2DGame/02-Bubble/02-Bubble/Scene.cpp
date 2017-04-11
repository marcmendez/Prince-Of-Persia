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

#define INIT_PLAYER_X_TILES 81
#define INIT_PLAYER_Y_TILES 10

#define NUMBER_ENEMIES 2

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
	doors = TileMap::createTileMap("levels/level01stairs.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
	initTorches(map->getTorchesFile());

	player = new Player();
	player->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize().first, INIT_PLAYER_Y_TILES * map->getTileSize().second));
	player->setTileMap(map);

	
	initIA("levels/level01IA.txt");
	player->setSultans(sultans);

	initTraps(map->getTrapsFile());
	initDoors("levels/level01doors.txt");

	healthInterface = HealthInterface::createHealthInterface(-1, glm::vec2(SCREEN_X, SCREEN_Y), texProgram);

	const float kOffsetX = static_cast<int>(player->GetScreenX(10 * map->getTileSize().first)) * 10 * map->getTileSize().first;
	const float kOffsetY = static_cast<int>(player->GetScreenY(3 * map->getTileSize().second)) * 3 * map->getTileSize().second;
	projection = glm::ortho(kOffsetX, SCREEN_WIDTH + kOffsetX, SCREEN_HEIGHT + kOffsetY, kOffsetY);
	glm::vec2 poscam; poscam.x = kOffsetX; poscam.y = kOffsetY;
	healthInterface->update(-1, false, 0, poscam);

	currentTime = 0.0f;
	
	bPrincipalMenu = true; bStory1 = false; bStory2 = false; bInstructions = false; bCredits = false; lastEnemy = false; releaseCtrl = true;

	princess = new Princess();
	princess->init("images/SpriteSheetPV.png", glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);


	// 7. 0
	coin1 = new CoinEasterEgg();
	coin1->init(glm::vec2(7 * 32, 0 * 64), glm::ivec2(SCREEN_X, SCREEN_Y), player, texProgram);

	//25 . 8
	coin2 = new CoinEasterEgg();
	coin2->init(glm::vec2(25 * 32, 8 * 64), glm::ivec2(SCREEN_X, SCREEN_Y), player, texProgram);

	//6 . 9
	coin3 = new CoinEasterEgg();
	coin3->init(glm::vec2(6 * 32, 9 * 64), glm::ivec2(SCREEN_X, SCREEN_Y), player, texProgram);

	// 87  12
	coin4 = new CoinEasterEgg();
	coin4->init(glm::vec2(87 * 32, 12 * 64), glm::ivec2(SCREEN_X, SCREEN_Y), player, texProgram);

}

void Scene::restart() {


	player->restart();
	player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize().first, INIT_PLAYER_Y_TILES * map->getTileSize().second));
	

	restartTraps();
}

void Scene::update(int deltaTime)
{
	if (!Game::instance().getSpecialKey(114))releaseCtrl = true;
	if (healthInterface->isMenu()) {

		const float kOffsetX = static_cast<int>(player->GetScreenX(10 * 32)) * 10 * 32;
		const float kOffsetY = static_cast<int>(player->GetScreenY(3 * 64)) * 3 * 64;
		glm::vec2 poscam; poscam.x = kOffsetX; poscam.y = kOffsetY;
		if (!bCredits) projection = glm::ortho(kOffsetX, SCREEN_WIDTH + kOffsetX, SCREEN_HEIGHT + kOffsetY, kOffsetY);
		
		if (bPrincipalMenu) {
			if (Game::instance().getSpecialKey(114) && releaseCtrl) { healthInterface->update(-3, false, 0, poscam); bPrincipalMenu = false; bStory1 = true; timer = 0; }
			else if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT)) { healthInterface->update(-2, false, 0, poscam); bPrincipalMenu = false; bInstructions = true; }
		}

		else if (bInstructions) {
			if (Game::instance().getSpecialKey(GLUT_KEY_LEFT)) { healthInterface->update(-1, false, 0, poscam); bPrincipalMenu = true; bInstructions = false; }
		}

		else if (bStory1) {
			timer += deltaTime;
			projection = glm::ortho(kOffsetX, 1295 + kOffsetX, 816 + kOffsetY, kOffsetY);
			glm::vec2 poscam; poscam.x = kOffsetX; poscam.y = kOffsetY;
			if (timer >= 1000) { healthInterface->update(-4, false, 0, poscam); bStory1 = false; bStory2 = true; }

		}

		else if (bStory2) {
			timer += deltaTime;
			projection = glm::ortho(kOffsetX, 1295 + kOffsetX, 816 + kOffsetY, kOffsetY);
			glm::vec2 poscam; poscam.x = kOffsetX; poscam.y = kOffsetY;
			if (timer >= 2000) { healthInterface->update(player->getHealth(), false, 0, poscam); bStory2 = false; timer = 0; }

		}	

		else if (bCredits) {
			if (Game::instance().getSpecialKey(114)) { healthInterface->update(-1, false, 0, poscam); bPrincipalMenu = true; bCredits = false; releaseCtrl = false; }

		}
	}

	else if ((player->getPosition().x / 32) >= 73 && (player->getPosition().y) / 64 == 13) {

		if (!princess->finished()) {

			const float kOffsetX = static_cast<int>(player->GetScreenX(10 * 32)) * 10 * 32;
			const float kOffsetY = static_cast<int>(player->GetScreenY(3 * 64)) * 3 * 64;
			projection = glm::ortho(kOffsetX, SCREEN_WIDTH + kOffsetX, SCREEN_HEIGHT + kOffsetY, kOffsetY);
			glm::vec2 poscam; poscam.x = kOffsetX; poscam.y = kOffsetY;

			if (player->getEasterEggPoints() >= 4 && !princess->peach()) princess->init("images/SpriteSheetPeach.png", glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
			else if (player->getEasterEggPoints() < 4 && princess->peach()) princess->init("images/SpriteSheetPV.png", glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
			princess->setStarted(true); princess->update(deltaTime);

		}

		else if (princess->finished()) {

			restart();

			const float kOffsetX = static_cast<int>(player->GetScreenX(10 * 32)) * 10 * 32;
			const float kOffsetY = static_cast<int>(player->GetScreenY(3 * 64)) * 3 * 64;
			projection = glm::ortho(kOffsetX, 1295 + kOffsetX, 816 + kOffsetY, kOffsetY);
			glm::vec2 poscam; poscam.x = kOffsetX; poscam.y = kOffsetY;

			healthInterface->update(-5, false,0, poscam); bCredits = true;

		}
	}

	else if (!healthInterface->isMenu()) {

		currentTime += deltaTime;

		if (Game::instance().getSpecialKey(114) && player->getHealth() <= 0) restart();

		for each (IA* sultan in sultans)
			sultan->update(deltaTime);

		player->update(deltaTime);

		for each (Torch* torch in torches)
			torch->update(deltaTime);		

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

		if (lastEnemy != player->playerSeesEnemy() || lastHPEnemy != player->getHealthEnemy() || lastHP != player->getHealth() || lastPos.x != kOffsetX || lastPos.y != kOffsetY) healthInterface->update(player->getHealth(), player->playerSeesEnemy(), player->getHealthEnemy(), poscam);
		lastHP = player->getHealth(); lastPos = glm::ivec2(kOffsetX, kOffsetY); lastEnemy = player->playerSeesEnemy(); lastHPEnemy = player->getHealthEnemy();
		coin1->update(deltaTime); coin2->update(deltaTime); coin3->update(deltaTime); coin4->update(deltaTime);

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
	doors->render();

	for each (Torch* torch in torches) torch->render();
	for each (TrapFallingFloor* trap in trapsFallingFloor) trap->render();
	for each (TrapSaw* trap in trapsSaw) trap->render();
	for each (TrapDoor* trap in trapsDoor) trap->render();

	player->render();
	for each (IA* sultan in sultans) sultan->render();

	for each (TrapSteelBars* trap in trapsFloor) trap->render();

	coin1->render(); coin2->render();
	coin3->render(); coin4->render();

	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
	columns->render();

	princess->render();

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

void Scene::initIA(string IAFile) {

	ifstream fin;
	string line;
	stringstream sstream;
	glm::ivec2 mapSize;
	int numberOfEnemies;
	int sultanX, sultanY ,maxPos, minPos;

	fin.open(IAFile.c_str());
	getline(fin, line);

	if (line.compare(0, 7, "IAMAP!") != 0) {

		getline(fin, line);
		sstream.str(line);
		sstream >> numberOfEnemies;


		for (int i = 0; i < numberOfEnemies; i++)
		{
			fin >> sultanX >> sultanY >> minPos >> maxPos;
			IA* sultan = new IA();
			sultan->init(glm::ivec2(32*sultanX, 64*sultanY), texProgram, player, minPos*32, maxPos * 32);
			sultan->setTileMap(map);
			sultan->setPosition(glm::ivec2(sultanX * map->getTileSize().first, sultanY * map->getTileSize().second));
			sultans.push_back(sultan);

		}

		}

	fin.close();
	}

	


