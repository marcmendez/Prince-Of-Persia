#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "Scene.h"
#include "Game.h"
#include "TrapSteelBars.h"
#include "Torch.h"


#define SCREEN_X 0
#define SCREEN_Y 0

#define INIT_PLAYER_X_TILES	5
#define INIT_PLAYER_Y_TILES 0

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

	initTraps(map->getTrapsFile());

	healthInterface = HealthInterface::createHealthInterface(1, glm::vec2(SCREEN_X, SCREEN_Y), texProgram);

	const float kOffsetX = static_cast<int>(player->GetScreenX(10 * map->getTileSize().first)) * 10 * map->getTileSize().first;
	const float kOffsetY = static_cast<int>(player->GetScreenY(3 * map->getTileSize().second)) * 3 * map->getTileSize().second;
	projection = glm::ortho(kOffsetX, SCREEN_WIDTH + kOffsetX, SCREEN_HEIGHT + kOffsetY, kOffsetY);

	currentTime = 0.0f;

}

void Scene::update(int deltaTime)
{
	currentTime += deltaTime;

	for each (Torch* torch in torches)
		torch->update(deltaTime);

	player->update(deltaTime);

	for each (TrapSteelBars* trap in trapsFloor) 
		trap->update(deltaTime);

	for each (TrapFallingFloor* trap in trapsFallingFloor)
		trap->update(deltaTime);
	
	const float kOffsetX = static_cast<int>(player->GetScreenX(10 * 32)) * 10 * 32;
	const float kOffsetY = static_cast<int>(player->GetScreenY(3 * 64)) * 3 * 64;
	projection = glm::ortho(kOffsetX, SCREEN_WIDTH + kOffsetX, SCREEN_HEIGHT + kOffsetY, kOffsetY);

	glm::vec2 poscam; poscam.x = kOffsetX; poscam.y = kOffsetY;
	healthInterface->update(player->getHealth(), poscam);


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

	player->render();

	for each (TrapSteelBars* trap in trapsFloor) trap->render();

	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
	columns->render();

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

				if (strcmp(trap, "2") == 0) {

					TrapFallingFloor* trap = new TrapFallingFloor();
					trap->setPlayer(player);
					trap->setTileMap(map);
					trap->init(glm::vec2(i * map->getTileSize().first, j * map->getTileSize().second), glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
					trapsFallingFloor.push_back(trap);

				}

				if (strcmp(trap, "3") == 0) {

				TrapSteelBars* trap = new TrapSteelBars();
				trap->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
				trap->setPosition(glm::vec2(i * map->getTileSize().first, j * map->getTileSize().second));
				trap->setPlayer(player);
				trapsFloor.push_back(trap);

				}
			}
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

				if (strcmp(trap, "1") == 0) {

					Torch* torch = new Torch();
					torch->init(glm::vec2(i * map->getTileSize().first, j * map->getTileSize().second), glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
					torches.push_back(torch);

				}
			}
		}
	}

	fin.close();

}