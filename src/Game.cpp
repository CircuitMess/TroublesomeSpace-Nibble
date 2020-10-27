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
	Piezo.setMute(true);

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

	triangleVector.push_back({0, (float) random(30, 98), 'H'}); // horizontal triangle [0]
	triangleVector.push_back({(float) random(30, 98), 'V', 0}); // vertical triangle [1]

	circleVector.push_back({(float) random(10, 80), (float) random(40, 117)});    // circle[0] = blue

}

void Game::loop(uint time){

	states(time);

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

	baseSprite->fillCircle(circle.x, circle.y, radius - 1, TFT_BLUE);

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

void Game::triangleMovement(Triangle &triangle, uint t){

	if(triangle.orientation == 'H'){

		triangle.x += speed * t / 13000;

		if(triangle.x > 127){

			float pomY = triangle.y;

			do {
				triangle.x = 0;
				triangle.y = (float) random(30, 118);
			} while(abs(pomY - triangle.y) < 30);
		}
	}

	if(triangle.orientation == 'V'){

		triangle.y += speed * t / 13000;

		if(triangle.y > 127){

			float pomX = triangle.x;

			do {
				triangle.x = (float) random(10, 98);
				triangle.y = 0;
			} while(abs(pomX - triangle.x) < 30);
		}
	}
}

void Game::checkIfDead(Triangle &triangle){

	if((sqrt(pow(triangle.x + triangleSide * sqrt(3) / 6 - playerX, 2) +
			 pow(triangle.y - playerY, 2)) <
		(triangleSide * sqrt(3) / 6 + radius)) && (triangle.orientation == 'H')){

		Piezo.tone(1000, 300);
		delay(500);

		playerX = 117;
		playerY = 10;

		lives--;

		if(lives == 0){

			for(int i = 0; i < cnt / 10; i++)
				triangleVector.pop_back();
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

			for(int i = 0; i < cnt / 10; i++)
				triangleVector.pop_back();
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

	if(sqrt(pow(blue.x - playerX, 2) + pow(blue.y - playerY, 2)) < (radius + radius - 1)){

		float pomX = blue.x;
		float pomY = blue.y;

		do {
			blue.x = (float) random(10, 117);
			blue.y = (float) random(10, 117);
		} while(sqrt(pow(pomX - blue.x, 2) + pow(pomY - blue.y, 2)) < 30);

		cnt++;
		newTriangle = true;

		Piezo.tone(500, 200);

	}

}

void Game::states(uint t){


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

	if(newTriangle){
		if(cnt > 0 && cnt % 20 == 0)
			triangleVector.push_back({0, (float) random(10, 50), 'H'}); // horizontal triangle

		else if(cnt > 0 && cnt % 10 == 0)
			triangleVector.push_back({(float) random(10, 50), 0, 'V'}); // vertical triangle

		newTriangle = false;
	}

	for(int i = 0; i < triangleVector.size(); ++i){

		triangleMovement(triangleVector[i], t);
		checkIfDead(triangleVector[i]);
	}

	checkIfEaten(circleVector[0]);

}

void Game::drawWarningMessage(){
	if((cnt + 1) % 20 == 0){
		baseSprite->setTextColor(TFT_RED);
		baseSprite->drawString(warning, 35, 55);

	}else if((cnt + 1) % 10 == 0){
		baseSprite->setTextColor(TFT_RED);
		baseSprite->drawString(warning, 35, 55);

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

