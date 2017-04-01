#include "HealthInterface.h"
#include <vector>
#include <string>

using namespace std;

HealthInterface *HealthInterface::createHealthInterface(const int health, const glm::vec2 &minCoords, ShaderProgram &program) {
	HealthInterface *healthInterface = new HealthInterface(health, minCoords, program);
	return healthInterface;
}

HealthInterface::HealthInterface(const int health, const glm::vec2 &minCoords, ShaderProgram &program) {
	loadInterface(health,0);
	this->minCoords = minCoords;
	this->program = program;
	glm::vec2 poscam; poscam.x = 0; poscam.y = 0;
	prepareArrays(poscam);
	bMenu = true;
}

void HealthInterface::render() const {
	glEnable(GL_TEXTURE_2D);
	interfacesheet.use();
	glBindVertexArray(vao);
	glEnableVertexAttribArray(posLocation);
	glEnableVertexAttribArray(texCoordLocation);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glDisable(GL_TEXTURE_2D);
}

void HealthInterface::free() {
	glDeleteBuffers(1, &vbo);
}

void HealthInterface::update(int health, glm::vec2 poscam) {
	loadInterface(health, poscam.y);
	prepareArrays(poscam);
}

bool HealthInterface::loadInterface(int health, int poscamY) {

	if (health >= 0) { interfacesheet.loadFromFile("images/interface/HealthInterfaceSheet" + to_string(health) + ".png", TEXTURE_PIXEL_FORMAT_RGBA); bMenu = false; }
	else if (health == -1) { interfacesheet.loadFromFile("images/menu/Menu.png", TEXTURE_PIXEL_FORMAT_RGBA); bMenu = true; }
	else if (health == -2) { interfacesheet.loadFromFile("images/menu/instructions.png", TEXTURE_PIXEL_FORMAT_RGBA); bMenu = true; }
	else if (health == -3) { interfacesheet.loadFromFile("images/menu/story1.png", TEXTURE_PIXEL_FORMAT_RGBA); bMenu = true; }
	else if (health == -4) { interfacesheet.loadFromFile("images/menu/story2.png", TEXTURE_PIXEL_FORMAT_RGBA); bMenu = true; }
	else if (health == -5) { interfacesheet.loadFromFile("images/menu/credits.png", TEXTURE_PIXEL_FORMAT_RGBA); bMenu = true; }
	else { interfacesheet.loadFromFile("images/menu/credits.png", TEXTURE_PIXEL_FORMAT_RGBA); bMenu = true; }

	interfacesheet.setWrapS(GL_CLAMP_TO_EDGE);
	interfacesheet.setWrapT(GL_CLAMP_TO_EDGE);
	interfacesheet.setMinFilter(GL_NEAREST);
	interfacesheet.setMagFilter(GL_NEAREST);
	
	return true;
}

void HealthInterface::prepareArrays(glm::vec2 poscam)
{
	vector<float> vertices;

	//First Triangle
	vertices.push_back(0 + poscam.x); vertices.push_back(0 + poscam.y);
	vertices.push_back(0); vertices.push_back(0);

	vertices.push_back(interfacesheet.width() + poscam.x); vertices.push_back(0 + poscam.y);
	vertices.push_back(1); vertices.push_back(0);

	vertices.push_back(interfacesheet.width() + poscam.x); vertices.push_back(interfacesheet.height() + poscam.y);
	vertices.push_back(1); vertices.push_back(1);

	//Second Triangle

	vertices.push_back(0 + poscam.x); vertices.push_back(0 + poscam.y);
	vertices.push_back(0); vertices.push_back(0);

	vertices.push_back(0 + poscam.x); vertices.push_back(interfacesheet.height() + poscam.y);
	vertices.push_back(0); vertices.push_back(1);

	vertices.push_back(interfacesheet.width() + poscam.x); vertices.push_back(interfacesheet.height() + poscam.y);
	vertices.push_back(1); vertices.push_back(1);

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(float), &vertices[0], GL_STATIC_DRAW);
	posLocation = program.bindVertexAttribute("position", 2, 4 * sizeof(float), 0);
	texCoordLocation = program.bindVertexAttribute("texCoord", 2, 4 * sizeof(float), (void *)(2 * sizeof(float)));
}
