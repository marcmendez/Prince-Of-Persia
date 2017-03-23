#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "TileMap.h"


using namespace std;


TileMap *TileMap::createTileMap(const string &levelFile, const glm::vec2 &minCoords, ShaderProgram &program)
{
	TileMap *map = new TileMap(levelFile, minCoords, program);

	return map;
}


TileMap::TileMap(const string &levelFile, const glm::vec2 &minCoords, ShaderProgram &program)
{
	loadLevel(levelFile);
	prepareArrays(minCoords, program);
}

TileMap::~TileMap()
{
	if (map != NULL)
		map = NULL;
}


void TileMap::render() const
{
	glEnable(GL_TEXTURE_2D);
	tilesheet.use();
	glBindVertexArray(vao);
	glEnableVertexAttribArray(posLocation);
	glEnableVertexAttribArray(texCoordLocation);
	glDrawArrays(GL_TRIANGLES, 0, 6 * mapSize.x * mapSize.y);
	glDisable(GL_TEXTURE_2D);
}

void TileMap::free()
{
	glDeleteBuffers(1, &vbo);
}

bool TileMap::loadLevel(const string &levelFile)
{
	ifstream fin;
	string line, tilesheetFile;
	stringstream sstream;
	int tile;

	fin.open(levelFile.c_str());
	if (!fin.is_open())
		return false;
	getline(fin, line);
	if (line.compare(0, 7, "TILEMAP") != 0)
		return false;
	getline(fin, line);
	sstream.str(line);
	sstream >> mapSize.x >> mapSize.y;
	getline(fin, line);
	sstream.str(line);
	sstream >> tileSizeX >> tileSizeY >> blockSize;
	getline(fin, line);
	sstream.str(line);
	sstream >> tilesheetFile;
	tilesheet.loadFromFile(tilesheetFile, TEXTURE_PIXEL_FORMAT_RGBA);
	tilesheet.setWrapS(GL_CLAMP_TO_EDGE);
	tilesheet.setWrapT(GL_CLAMP_TO_EDGE);
	tilesheet.setMinFilter(GL_NEAREST);
	tilesheet.setMagFilter(GL_NEAREST);
	getline(fin, line);
	sstream.str(line);
	sstream >> tilesheetSize.x >> tilesheetSize.y;
	tileTexSize = glm::vec2(1.f / tilesheetSize.x, 1.f / tilesheetSize.y);

	map = new int[mapSize.x * mapSize.y];

	for (int j = 0; j<mapSize.y; j++) {

		int i = 0;
		while (getline(fin, line, ',')) {

			stringstream(line) >> tile;
			map[j*mapSize.x + i] = tile;
			i++;

		}

	}

	fin.close();

	return true;
}

void TileMap::prepareArrays(const glm::vec2 &minCoords, ShaderProgram &program)
{
	int tile, nTiles = 0;
	glm::vec2 posTile, texCoordTile[2], halfTexel;
	vector<float> vertices;

	halfTexel = glm::vec2(0.5f / tilesheet.width(), 0.5f / tilesheet.height());
	for (int j = mapSize.y - 1; j >= 0; j--)
	{
		for (int i = 0; i<mapSize.x; i++)
		{
			tile = map[j * mapSize.x + i];

			if (tile != -1)
			{
				// Non-empty tile
				nTiles++;
				posTile = glm::vec2(minCoords.x + i * tileSizeX, (minCoords.y - 63) + j * tileSizeY);
				texCoordTile[0] = glm::vec2(float((tile) % 10) / tilesheetSize.x, float((tile / 10) % 10) / tilesheetSize.y);
				texCoordTile[1] = texCoordTile[0] + tileTexSize;
				//texCoordTile[0] += halfTexel;
				texCoordTile[1] -= halfTexel;
				// First triangle
				vertices.push_back(posTile.x); vertices.push_back(posTile.y);
				vertices.push_back(texCoordTile[0].x); vertices.push_back(texCoordTile[0].y);
				vertices.push_back(posTile.x + blockSize); vertices.push_back(posTile.y);
				vertices.push_back(texCoordTile[1].x); vertices.push_back(texCoordTile[0].y);
				vertices.push_back(posTile.x + blockSize); vertices.push_back(posTile.y + blockSize);
				vertices.push_back(texCoordTile[1].x); vertices.push_back(texCoordTile[1].y);
				// Second triangle
				vertices.push_back(posTile.x); vertices.push_back(posTile.y);
				vertices.push_back(texCoordTile[0].x); vertices.push_back(texCoordTile[0].y);
				vertices.push_back(posTile.x + blockSize); vertices.push_back(posTile.y + blockSize);
				vertices.push_back(texCoordTile[1].x); vertices.push_back(texCoordTile[1].y);
				vertices.push_back(posTile.x); vertices.push_back(posTile.y + blockSize);
				vertices.push_back(texCoordTile[0].x); vertices.push_back(texCoordTile[1].y);
			}
		}
	}

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 24 * nTiles * sizeof(float), &vertices[0], GL_STATIC_DRAW);
	posLocation = program.bindVertexAttribute("position", 2, 4 * sizeof(float), 0);
	texCoordLocation = program.bindVertexAttribute("texCoord", 2, 4 * sizeof(float), (void *)(2 * sizeof(float)));
}

// Collision tests for axis aligned bounding boxes.
// Method collisionMoveDown also corrects Y coordinate if the box is
// already intersecting a tile below.

bool TileMap::collisionMoveLeft(int posx, int posy, const glm::ivec2 &size) const
{
	int x, y, p;

	// iniciar variables
	x = posx / tileSizeX;
	y = (posy / tileSizeY);
	p = (posx / (tileSizeX / 2)) % 2; /* ------0------ ------1------*/

	// Si no es background o cami i esta a la meitat dreta
	if (map[y*mapSize.x + x] / 10 % 10 != 4 && (map[y*mapSize.x + x] / 10) % 10 != 3 && p == 0) return true;

	else return false;

}

bool TileMap::collisionMoveRight(int posx, int posy, const glm::ivec2 &size) const
{
	int x, y, p;

	// iniciar variables
	x = posx / tileSizeX + 1;
	y = (posy / tileSizeY);
	p = (posx / (tileSizeX / 4)) % 4; /* ---0---  ---1--- ---2--- ---3---*/

	// Si no es background o cami i esta a la meitat dreta
	if (map[y*mapSize.x + x] / 10 % 10 != 4 && (map[y*mapSize.x + x] / 10) % 10 != 3 && p >= 1) return true;

	else return false;
}

bool TileMap::collisionMoveDown(int posx, int posy, const glm::ivec2 &size, char dir) const {
	
	// iniciar variables
	int x, y, p;
	x = posx / tileSizeX;
	y = posy / tileSizeY;

	// Si no es background retornar true
	if ((map[y*mapSize.x + x] / 10) % 10 != 4) return true;
	else return false;

}

bool TileMap:: canIMoveUp(const glm::ivec2 &pos, const glm::ivec2 &size, char dir) const {

	int x0, x1, y;
	x0 = pos.x / tileSizeX;
 	x1 = pos.x  / tileSizeX;
	y = pos.y / tileSizeY;

	if ((map[(y - 1)*mapSize.x + x0] / 10) % 10 != 4) return false;
	else if ( dir == 'r' && (map[(y - 1)*mapSize.x + x0 + 1] / 10) % 10 == 3) return true;
	else if (dir == 'l' && (map[(y - 1)*mapSize.x + x0 - 1] / 10) % 10 == 3) return true;
	else return false;

}



























































