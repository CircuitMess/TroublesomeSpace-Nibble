//
// Created by Domagoj on 12-Oct-20.
//

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


struct Alien {

	float x;
	float y;
	enum alienType{
		ALIEN1,ALIEN2,ALIEN3
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

	uint score{};
	uint lives{};
	uint level{};
	uint fuel{};
	uint ore{};
	int invisibilityCounter{};

	uint melodyTime = 0;
	bool loopPlaying = false;

private:

	static GameState *instance;

	Vector<Alien> aliens;
	Vector<Object> objects;

	FuelBar fuelBar;

	void states(uint time);

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

	void gameOver();

	void drawCounterString();

	void drawLivesString();

	void drawInvisibilityCounter();

	void drawFuelBar();


	float playerX = 58;
	float playerY = 90;

	const float speed = 1;

	bool upState = false;
	bool downState = false;
	bool rightState = false;
	bool leftState = false;
	bool aState = false;
	bool bState = false;

	bool dead = false;

	bool playerInvisible = false;

	unsigned long invisibilityTime = 4000; // t = 4000 ms
	unsigned long currentInvisibilityTime = 0;
	unsigned long previousInvisibilityTime = 0;

	const char *livesRest = "Lives: ";
	const char *invisibleTimes = "Inv: ";

	unsigned long melodyPreviousMillis = 0;

	bool oreCheck = false;
	uint oreTime = 15000; // 15 sec
	unsigned long previousOreTime = 0;

	bool fuelCheck = false;
	uint fuelTime = 25000; // 25 sec
	unsigned long previousFuelTime = 0;


};

#endif //NIBBLE_NEW_GAMESTATE_H
