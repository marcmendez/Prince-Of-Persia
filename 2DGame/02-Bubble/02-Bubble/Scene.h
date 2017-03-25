#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE

#include <glm/glm.hpp>
#include <vector>

#include "ShaderProgram.h"
#include "HealthInterface.h"

#include "TileMap.h"
#include "Torch.h"

#include "Player.h"
#include "IA.h"

#include "TrapDoor.h"
#include "TrapSteelBars.h"
#include "TrapFallingFloor.h"
#include "TrapSaw.h"

// Scene contains all the entities of our game.
// It is responsible for updating and render them.

class Scene
{

public:
	Scene();
	~Scene();

	void init();
	void update(int deltaTime);
	void render();

private:
	void initShaders();
	void initTraps(string TrapsFile);
	void initTorches(string TorchesFile);
	void initDoors(string doorsFile);
	void intiIA(string IAFile);

private:
	TileMap *map, *columns;

	Player *player;
	IA *sultans;

	vector<TrapSteelBars*> trapsFloor;
	vector<TrapDoor *> trapsDoor;
	vector<TrapFallingFloor*> trapsFallingFloor;
	vector<TrapSaw *> trapsSaw;

	vector<Torch*> torches;

	HealthInterface *healthInterface;

	ShaderProgram texProgram;
	float currentTime;
	glm::mat4 projection;

};


#endif // _SCENE_INCLUDE

