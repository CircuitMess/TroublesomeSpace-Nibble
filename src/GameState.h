#ifndef NIBBLE_NEW_GAMESTATE_H
#define NIBBLE_NEW_GAMESTATE_H

#include <Arduino.h>
#include <CircuitOS.h>
#include <Nibble.h>
#include <Loop/LoopManager.h>
#include <Display/Display.h>
#include <Display/Sprite.h>
#include <Input/Input.h>
#include <Input/I2cExpander.h>
#include <Input/InputI2C.h>
#include "Pins.hpp"
#include <Audio/Piezo.h>
#include "State.h"
#include "Game.h"
#include "Particles/ParticleEngine.h"
#include "Particles/Particle.h"

#define		PLAYER_RADIUS	6
#define		ALIEN_RADIUS	5
#define		ORE_RADIUS		sqrt(pow(1.5, 2) + pow(1.5, 2))
#define		FUEL_RADIUS		sqrt(pow(4, 2) + pow(4, 2))

struct Alien {

	float x;
	float y;
	enum alienType{
		ALIEN1, ALIEN2, ALIEN3, ALIEN4, ALIEN5, ALIEN6,
		ALIEN_SHIP1, ALIEN_SHIP2, ALIEN_SHIP3, ALIEN_SHIP4, ALIEN_SHIP5
	}type;
};

struct Object {

	float x;
	float y;
	enum objectType{
		FUEL,ORE
	}type;
};

struct FuelBar{

	float x;
	float y;
	float width;
	float height;

};

struct Star{
	float x;
	float y;
};

class GameState : public State {

public:

	GameState();

	void loop(uint t) override;

	void enter(Game& game) override;

	void exit() override;

	void draw();

	static void buttonUpPressed();

	static void buttonDownPressed();

	static void buttonLeftPressed();

	static void buttonRightPressed();

	static void buttonAPressed();

	static void buttonBPressed();

	static void buttonUpReleased();

	static void buttonDownReleased();

	static void buttonLeftReleased();

	static void buttonRightReleased();

	static void buttonAReleased();

	static void buttonBReleased();

	uint lives{};
	uint level{};
	uint ore{};
	int invisibilityCounter{};

	uint melodyTime = 0;
	bool loopPlaying = false;

private:

	static GameState *instance;

	Vector<Alien> aliens;
	Vector<Object> objects;
	Vector<Star> stars;

	ParticleEngine *engine;

	FuelBar fuelBar;

	void states(uint time);

	void levelHandler();

	void drawPlayer();

	void drawObject(Object &object);

	void drawObjects();

	void drawAlien(Alien &alien);

	void drawAliens();

	void alienMovement(Alien &alien, uint t) const;

	void objectMovement(Object &object, uint t);

	void checkIfCollected(Object &object);

	void checkIfDead(Alien &alien);

	void invisibility();

	void drawBackground();

	void drawCounterString();

	void drawLivesString();

	void drawInvisibilityCounter();

	void drawFuelBar();

	void drawPausedState();

	void drawBetweenLevelState();

	void drawLevelEnd();


	float playerX = 58;
	float playerY = 90;

	uint lowIgnitionPlayerFrame = 1;
	uint previousLowIgnitionPlayerTime = 0;
	uint lowIgnitionTimePerFrame = 300;
	uint highIgnitionPlayerFrame = 1;
	uint previousHighIgnitionPlayerTime = 0;
	uint highIgnitionTimePerFrame = 300;

	uint leanLeftPlayerFrame = 1;
	uint previousLeanLeftPlayerTime = 0;
	uint leanLeftTimePerFrame = 300;
	uint leanRightPlayerFrame = 1;
	uint previousLeanRightPlayerTime = 0;
	uint leanRightTimePerFrame = 300;

	uint leanLeftIgnitionPlayerFrame = 1;
	uint previousLeanLeftIgnitionPlayerTime = 0;
	uint leanLeftIgnitionTimePerFrame = 300;
	uint leanRightIgnitionPlayerFrame = 1;
	uint previousLeanRightIgnitionPlayerTime = 0;
	uint leanRightIgnitionTimePerFrame = 300;

	const float speed = 1;

	bool upState = false;
	bool downState = false;
	bool rightState = false;
	bool leftState = false;
	bool aState = false;
	bool bState = false;

	bool dead = false;

	bool playerInvisible = false;

	unsigned long invisibilityTime = 5000;
	unsigned long currentInvisibilityTime = 0;
	unsigned long previousInvisibilityTime = 0;

	const char *livesRest = "Lives: ";
	const char *invisibleTimes = "Inv: ";
	const char *pause = "Paused";
	const char *resume = "Resume: A";
	const char *quit = "Quit: B";
	const char *continueString = "Press A to continue";
	const char *levelNumber = "Level";

	unsigned long melodyPreviousMillis = millis();

	bool oreCheck = false;
	uint oreTime = 8000;
	unsigned long previousOreTime = millis();

	bool fuelCheck = false;
	uint fuelTime = 14000;
	unsigned long previousFuelTime = millis();
	float fuelDx = 1;

	bool pausedState = false;
	bool betweenLevelState = false;

	bool newLevel = false;

	uint levelTime = 60000;
	unsigned long previousLevelTime = millis();

	bool levelEnd = false;
	float lvlEndPlanetX = 42;
	float lvlEndPlanetY = -45;


};

#endif //NIBBLE_NEW_GAMESTATE_H
