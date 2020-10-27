//
// Created by Domagoj on 12-Oct-20.
//

#include <Arduino.h>
#include <Nibble.h>
#include "Pins.hpp"
#include "Game.h"


Game *Game::instance = nullptr;

Game::Game(){

	display = Nibble.getDisplay();
	baseSprite = display->getBaseSprite();
	Piezo.setMute(false);

	Input::getInstance()->setBtnPressCallback(BTN_UP, buttonUpPressed);
	Input::getInstance()->setBtnPressCallback(BTN_DOWN, buttonDownPressed);
	Input::getInstance()->setBtnPressCallback(BTN_LEFT, buttonLeftPressed);
	Input::getInstance()->setBtnPressCallback(BTN_RIGHT, buttonRightPressed);
	Input::getInstance()->setBtnPressCallback(BTN_A, buttonAPressed);
	Input::getInstance()->setBtnPressCallback(BTN_B, buttonBPressed);

	Input::getInstance()->setBtnReleaseCallback(BTN_UP, buttonUpReleased);
	Input::getInstance()->setBtnReleaseCallback(BTN_DOWN, buttonDownReleased);
	Input::getInstance()->setBtnReleaseCallback(BTN_LEFT, buttonLeftReleased);
	Input::getInstance()->setBtnReleaseCallback(BTN_RIGHT, buttonRightReleased);
	Input::getInstance()->setBtnReleaseCallback(BTN_A, buttonAReleased);
	Input::getInstance()->setBtnReleaseCallback(BTN_B, buttonBReleased);

	instance = this;

	triangleVector.push_back({0, (float) random(10, 50), 'H'}); // horizontal triangle [0]
	triangleVector.push_back({(float) random(10, 50), 'V', 0}); // vertical triangle [1]

	circleVector.push_back({(float) random(10, 80), (float) random(40, 117)});    // circle[0] = blue

}

void Game::loop(uint time){

	for(int i = 0; i<triangleVector.size(); i++)
		states(triangleVector[i],time);

	baseSprite->clear(TFT_BLACK);

	drawPlayer();
	drawCircles();
	drawTriangles();

	drawCounterString();
	drawSpawnPoint();
	drawLivesString();
	drawWarningMessage();

	display->commit();


}

void Game::drawPlayer(){
	baseSprite->fillCircle(playerX, playerY, radius, TFT_GOLD);
}

void Game::drawCircle(Circle &circle){

	baseSprite->fillCircle(circle.x, circle.y, radius, TFT_BLUE);

}

void Game::drawCircles(){
	for(int i = 0; i < circleVector.size(); i++){

		drawCircle(circleVector[i]);

	}
}

void Game::drawTriangle(Triangle &triangle){

	if(triangle.orientation == 'H'){

		baseSprite->fillTriangle(triangle.x, triangle.y - 5, 5 * sqrt(2) + triangle.x,
								 triangle.y, triangle.x, triangle.y + 5, TFT_RED);
	}
	if(triangle.orientation == 'V'){

		baseSprite->fillTriangle(triangle.x + 5, triangle.y, triangle.x,
								 triangle.y + 5 * sqrt(2), triangle.x - 5, triangle.y, TFT_RED);
	}


}

void Game::drawTriangles(){
	for(int i = 0; i < triangleVector.size(); i++){

		drawTriangle(triangleVector[i]);

	}
}

void Game::checkIfDead(Triangle &triangle){

	if((sqrt(pow(triangle.x + triangleSide * sqrt(3) / 6 - playerX, 2) +
			 pow(triangle.y - playerY, 2)) <
		(triangleSide * sqrt(3) / 6 + radius)) && ( triangle.orientation == 'H')){

		Piezo.tone(1000, 300);
		delay(500);

		playerX = 117;
		playerY = 10;

		lives--;

		if(lives == 0){

			cnt = 0;
			lives = 3;
			baseSprite->clear(TFT_BLACK);
			baseSprite->drawString(endMessage, 35, 55);
			display->commit();
			delay(2000);

		}

	}else if((sqrt(pow(triangle.x - playerX, 2) +
				   pow(triangle.y + triangleSide * sqrt(3) / 6 - playerY, 2)) <
			  (triangleSide * sqrt(3) / 6 + radius)) && (triangle.orientation == 'V')){

		Piezo.tone(1000, 300);
		delay(500);

		playerX = 117;
		playerY = 10;

		lives--;

		if(lives == 0){

			cnt = 0;
			lives = 3;
			baseSprite->clear(TFT_BLACK);
			baseSprite->drawString(endMessage, 35, 55);
			display->commit();
			delay(2000);

		}

	}
}

void Game::checkIfEaten(Circle &blue){

	if(sqrt(pow(blue.x - playerX,2) + pow(blue.y - playerY,2)) < (radius * 2)){

		float pomX = blue.x;
		float pomY = blue.y;

		do {
			blue.x = (float) random(10, 117);
			blue.y = (float) random(10, 117);
		} while(abs(pomX - blue.x) < 30 && abs(pomY - blue.y) < 30);

		cnt++;

		Piezo.tone(500, 200);

	}

}

void Game::states(Triangle &triangle, uint t){


	if(instance->upState == 1){
		playerY -= speed * t / 13000;
		if(playerY <= 5)
			playerY = 5;

	}
	if(instance->leftState == 1){
		playerX -= speed * t / 13000;
		if(playerX <= 5)
			playerX = 5;


	}
	if(instance->downState == 1){
		playerY += speed * t / 13000;
		if(playerY >= 122)
			playerY = 122;

	}
	if(instance->rightState == 1){
		playerX += speed * t / 13000;
		if(playerX >= 122)
			playerX = 122;

	}
	if(instance->aState == 1){
		if(instance->upState == 1 && playerY > 5){
			playerY -= speed * t / 13000;
		}
		if(instance->downState == 1 && playerY < 122){
			playerY += speed * t / 13000;
		}
		if(instance->rightState == 1 && playerX < 122){
			playerX += speed * t / 13000;
		}
		if(instance->leftState == 1 && playerY > 5){
			playerX -= speed * t / 13000;
		}


	}
	if(instance->bState == 1){

	}

	if(cnt % 20 == 0)
		triangleVector.push_back({(float) random(10, 50), 0, 'V'}); // vertical triangle
	else if(cnt % 10 == 0)
		triangleVector.push_back({0, (float) random(10, 50), 'H'}); // horizontal triangle

/*
	for(int i = 0; i < triangleVector.size(); i++){

		if(i % 2 == 0)
			triangle.horizontalNumber++;
		else
			triangle.verticalNumber++;
	}
*/

	if(triangle.x > 127){
		triangle.x = 0;
		triangle.y = (float) random(20, 108);
	}


	if(triangle.y > 127){
		triangle.x = (float) random(20, 108);
		triangle.y = 0;
	}

	checkIfEaten(circleVector[0]);

	for(int i = 0; i< triangleVector.size(); i++)
		checkIfDead(triangleVector[i]);

}

void Game::drawWarningMessage(){
	if((cnt + 1) % 20 == 0){
		baseSprite->setTextColor(TFT_RED);
		baseSprite->drawString(warning, 35, 55);
		baseSprite->drawLine(24, 0, 24, 128, TFT_RED);
		baseSprite->drawLine(104, 0, 104, 128, TFT_RED);
	}else if((cnt + 1) % 10 == 0){
		baseSprite->setTextColor(TFT_RED);
		baseSprite->drawString(warning, 35, 55);
		baseSprite->drawLine(0, 24, 128, 24, TFT_RED);
		baseSprite->drawLine(0, 104, 128, 104, TFT_RED);
	}
}

void Game::drawCounterString(){

	baseSprite->setTextSize(1);
	baseSprite->setTextFont(2);
	baseSprite->setTextColor(TFT_WHITE);
	baseSprite->drawNumber(cnt, 5, 3);

}

void Game::drawSpawnPoint(){

	baseSprite->drawLine(127, 0, 107, 0, TFT_WHITE);
	baseSprite->drawLine(127, 0, 127, 20, TFT_WHITE);
	baseSprite->drawLine(107, 0, 107, 20, TFT_WHITE);
	baseSprite->drawLine(107, 20, 127, 20, TFT_WHITE);

}

void Game::drawLivesString(){

	baseSprite->drawString(livesRest, 1, 110);
	baseSprite->drawNumber(lives, 40, 110);
}


void Game::buttonUpPressed(){

	instance->upState = 1;

}

void Game::buttonDownPressed(){

	instance->downState = 1;

}

void Game::buttonLeftPressed(){

	instance->leftState = 1;

}

void Game::buttonRightPressed(){

	instance->rightState = 1;

}

void Game::buttonAPressed(){

	instance->aState = 1;

}

void Game::buttonBPressed(){

	instance->bState = 1;

}

void Game::buttonUpReleased(){

	instance->upState = 0;

}

void Game::buttonDownReleased(){

	instance->downState = 0;

}

void Game::buttonLeftReleased(){

	instance->leftState = 0;

}

void Game::buttonRightReleased(){

	instance->rightState = 0;


}

void Game::buttonAReleased(){

	instance->aState = 0;

}

void Game::buttonBReleased(){

	instance->bState = 0;
}

