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
#include "melody/Melody.h"
#include "State.h"
#include "Game.h"

struct Triangle {

	float x;
	float y;
	enum {
		H, V
	} orientation; // H-horizontal, V-vertical

};

struct Circle {

	float x;
	float y;
};


class GameState : public State {

public:

	GameState();

	void loop(uint t) override;

	void enter(Game& game) override;

	void exit() override;

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
	int lives{};
	int invisibilityCounter{};

	uint melodyTime = 0;
	bool loopPlaying = false;

private:

	static GameState *instance;

	Vector<Triangle> triangles;
	Vector<Circle> circles;

	void states(uint time);

	void drawPlayer();

	void drawCircle(Circle &circle);

	void drawCircles();

	void drawTriangle(Triangle &triangle);

	void drawTriangles();

	void triangleMovement(Triangle &triangle, uint t) const;

	void checkIfEaten(Circle &circle);

	void checkIfDead(Triangle &triangle);

	void invisibility();

	void gameOver();

	void drawWarningMessage();

	void startUpMessage();

	void drawCounterString();

	void drawSpawnPoint();

	void drawLivesString();

	void drawInvisibilityCounter();


	const float speed = 1;

	Circle player{117, 10};
	uint playerColor = TFT_GOLD;

	bool upState = false;
	bool downState = false;
	bool rightState = false;
	bool leftState = false;
	bool aState = false;
	bool bState = false;

	const float radius = 4;
	const float triangleSide = 10;

	bool dead = false;

	bool playerInvisible = false;

	unsigned long invisibilityTime = 4000; // t = 4000 ms
	unsigned long currentInvisibilityTime = 0;
	unsigned long previousInvisibilityTime = 0;

	const char *livesRest = "Lives: ";
	const char *warning = "Warning!";
	const char *startGame = "Starting game";
	const char *invisibleTimes = "Invisibility: ";

	unsigned long melodyPreviousMillis = 0;


};

#endif //NIBBLE_NEW_GAMESTATE_H
