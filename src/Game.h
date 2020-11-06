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
#include "melody/Melody.h"

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


class Game : public LoopListener {

public:

	explicit Game(Melody *m);

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
	Melody *melody;

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

	void triangleMovement(Triangle &triangle, uint t) const;

	void checkIfEaten(Circle &circle);

	void checkIfDead(Triangle &triangle);

	void invisibility();

	void victory();

	void gameOver();

	void victoryMessage();

	void gameOverMessage();

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

	int lives = 3;
	int score = 0;

	bool playerInvisible = false;
	int invisibilityCounter = 3;
	unsigned long invisibilityTime = 4000; // t = 4000 ms
	unsigned long currentInvisibilityTime = 0;
	unsigned long previousInvisibilityTime = 0;

	const char *endMessage = "Game Over";
	const char *livesRest = "Lives: ";
	const char *warning = "Warning!";
	const char *startGame = "Starting game";
	const char *invisibleTimes = "Invisibility: ";
	const char *winMessage = "Victory!";
	const char *finalScore = "Score: ";

	unsigned long melodyPreviousMillis = 0;
	uint melodyTime = 0;

	unsigned long gameOverMillis = 0;
	unsigned long victoryMillis = 0;

};

#endif //NIBBLE_NEW_GAME_H
