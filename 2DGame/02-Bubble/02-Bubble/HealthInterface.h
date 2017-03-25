#include <iostream>
#include <glm/glm.hpp>
#include "Texture.h"
#include "ShaderProgram.h"

class HealthInterface
{

public:
	// Tile maps can only be created inside an OpenGL context
	static HealthInterface *createHealthInterface(int health, const glm::vec2 &minCoords, ShaderProgram &program);
	HealthInterface(int health, const glm::vec2 &minCoords, ShaderProgram &program);

	void render() const;
	void free();
	void update(int health, glm::vec2 poscam);

private:

	bool loadInterface(int health);
	void prepareArrays(glm::vec2 poscam);

private:
	GLuint vao;
	GLuint vbo;
	GLint posLocation, texCoordLocation;
	Texture interfacesheet;
	glm::vec2 minCoords;
	ShaderProgram program;

};
