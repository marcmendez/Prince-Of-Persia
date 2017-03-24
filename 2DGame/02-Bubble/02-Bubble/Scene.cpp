#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "Scene.h"
#include "Game.h"
#include "TrapSteelBars.h"


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
	map = TileMap::createTileMap("levels/level1.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
	columns = TileMap::createTileMap(map->getColumnsFile(), glm::vec2(SCREEN_X, SCREEN_Y), texProgram);

	player = new Player();
	player->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize().first, INIT_PLAYER_Y_TILES * map->getTileSize().second));
	player->setTileMap(map);

	initTraps(map->getTrapsFile());

	const float kOffsetX = static_cast<int>(player->GetScreenX(10 * 32)) * 10 * 32;
	const float kOffsetY = static_cast<int>(player->GetScreenY(3 * 64)) * 3 * 64;
	projection = glm::ortho(kOffsetX, SCREEN_WIDTH + kOffsetX, SCREEN_HEIGHT + kOffsetY, kOffsetY);

	currentTime = 0.0f;
}

void Scene::update(int deltaTime)
{
	currentTime += deltaTime;
	player->update(deltaTime);
	for each (TrapSteelBars* trap in trapsFloor) 
		trap->update(deltaTime);
		
	const float kOffsetX = static_cast<int>(player->GetScreenX(10 * 32)) * 10 * 32;
	const float kOffsetY = static_cast<int>(player->GetScreenY(3 * 64)) * 3 * 64;

	projection = glm::ortho(kOffsetX, SCREEN_WIDTH + kOffsetX, SCREEN_HEIGHT + kOffsetY, kOffsetY);
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

	player->render();

	for each (TrapSteelBars* trap in trapsFloor)
		trap->render();


	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
	columns->render();

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

	int trap;
	fin.open(trapsFile.c_str());
	getline(fin, line);

	if (line.compare(0, 7, "TRAPSMAP!") != 0) {

		getline(fin, line);
		sstream.str(line);
		sstream >> mapSize.x >> mapSize.y;

		getline(fin, line);
		sstream.str(line);
		sstream >> spriteTrapsFile;

		for (int i = 0; i < mapSize.y; i++) {

			int j = 0;

			while (getline(fin, line, ',')) {

			stringstream(line) >> trap;

			if (trap == 1) {

				TrapSteelBars* trap = new TrapSteelBars();
				trap->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
				trap->setPosition(glm::vec2(j * map->getTileSize().first, i * map->getTileSize().second));
				trap->setPlayer(player);
				trapsFloor.push_back(trap);

			}

			j++;

			}
		}
	}

	fin.close();

}
