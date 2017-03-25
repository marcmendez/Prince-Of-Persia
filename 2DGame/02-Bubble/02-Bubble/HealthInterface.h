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

private:

	bool loadInterface(int health);
	void prepareArrays(const glm::vec2 &minCoords, ShaderProgram &program);

private:
	GLuint vao;
	GLuint vbo;
	GLint posLocation, texCoordLocation;
	Texture interfacesheet;

};
