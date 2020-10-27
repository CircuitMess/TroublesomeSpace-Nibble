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
	char orientation;

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

	Vector<Triangle> triangleVector;
	Vector<Circle> circleVector;

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

	void drawCounterString();

	void drawSpawnPoint();

	void drawLivesString();


	const float speed = 1;

	Circle player;
	uint32_t playerColor = TFT_GOLD;


	int upState = 0;
	int downState = 0;
	int rightState = 0;
	int leftState = 0;
	int aState = 0;
	int bState = 0;

	const float radius = 5;
	const float triangleSide = 10;

	int lives = 3;
	int cnt = 0;

	bool newTriangle = true;
	bool invisibilityButtonStatus = false;


	String endMessage = "Game Over";
	String livesRest = "Lives: ";
	String warning = "Warning!";

};

#endif //NIBBLE_NEW_GAME_H
