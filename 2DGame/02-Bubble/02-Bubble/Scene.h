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
#include "Princess.h"

#include "TrapDoor.h"
#include "TrapSteelBars.h"
#include "TrapFallingFloor.h"
#include "TrapSaw.h"

#include "CoinEasterEgg.h"

// Scene contains all the entities of our game.
// It is responsible for updating and render them.

class Scene
{

public:
	Scene();
	~Scene();

	void init();
	void restart();
	void update(int deltaTime);
	void render();

private:
	void restartTraps();
	void initShaders();
	void initTraps(string TrapsFile);
	void initTorches(string TorchesFile);
	void initDoors(string doorsFile);
	void intiIA(string IAFile);

private:
	TileMap *map, *columns,*doors;

	Player *player;
	IA *sultans;
	Princess *princess;

	vector<TrapSteelBars*> trapsFloor;
	vector<TrapDoor *> trapsDoor;
	vector<TrapFallingFloor*> trapsFallingFloor;
	vector<TrapSaw *> trapsSaw;

	CoinEasterEgg *coin1;
	CoinEasterEgg *coin2;


	vector<Torch*> torches;

	HealthInterface *healthInterface;

	ShaderProgram texProgram;
	float currentTime;
	glm::mat4 projection;

	bool bPrincipalMenu, bInstructions, bStory1, bStory2, bCredits;
	int timer, lastHP;
	glm::ivec2 lastPos;

};


#endif // _SCENE_INCLUDE

