//
// Created by Domagoj on 12-Oct-20.
//

#ifndef NIBBLE_NEW_GAME_H
#define NIBBLE_NEW_GAME_H

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

struct Triangle {

	float x;
	float y;
	enum { H, V } orientation; // H-horizontal, V-vertical

};

struct Circle {

	float x;
	float y;
};


class Game : public LoopListener {

public:

	Game();

	void loop(uint t) override;

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

private:

	static Game *instance;

	Display *display;
	Sprite *baseSprite;

	Vector<Triangle> triangles;
	Vector<Circle> circles;

	void states(uint time);

	void drawPlayer();

	void drawCircle(Circle &circle);

	void drawCircles();

	void drawTriangle(Triangle &triangle);

	void drawTriangles();

	void triangleMovement(Triangle &triangle, uint t);

	void checkIfEaten(Circle &circle);

	void checkIfDead(Triangle &triangle);

	void drawWarningMessage();

	void startUpMessage();

	void drawCounterString();

	void drawSpawnPoint();

	void drawLivesString();

	void startUpTones();

	void inGameTones();


	const float speed = 1;

	Circle player;
	uint playerColor = TFT_GOLD;

	bool upState = false;
	bool downState = false;
	bool rightState = false;
	bool leftState = false;
	bool aState = false;
	bool bState = false;

	const float radius = 5;
	const float triangleSide = 10;

	int lives = 3;
	int score = 0;

	bool playerInvisible = false;

	const char* endMessage = "Game Over";
	const char* livesRest = "Lives: ";
	const char* warning = "Warning!";
	const char* startGame = "Starting game";

	int note = 0;

	int toneCnt = 0;
	unsigned long previousMillis = 0;

};

#endif //NIBBLE_NEW_GAME_H
