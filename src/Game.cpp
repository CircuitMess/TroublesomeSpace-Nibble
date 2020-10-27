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

	triangleVector.push_back({0, 64}); // triangle_horizontal[0]
	triangleVector.push_back({64, 0}); // triangle_vertical[1]
	triangleVector.push_back({0, (float) random(10, 50)}); // triangle_horizontal[2]
	triangleVector.push_back({(float) random(10, 50), 0}); // triangle_vertical[3]

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

	baseSprite->fillCircle(circle.x, circle.y, radius, TFT_BLUE);

}

void Game::drawCircles(){
	for(int i = 0; i < circleVector.size(); i++){

		drawCircle(circleVector[i]);

	}
}

void Game::drawTriangle(Triangle &tri){

	if(i == 0){

		horizontal_triangle_y = tri.triangle_y;

		if(tri.triangle_x > 127){
			tri.triangle_x = 0;
			tri.triangle_y = (float) random(20, 108);
		}
		baseSprite->fillTriangle(tri.triangle_x, tri.triangle_y - 5, 5 * sqrt(2) + tri.triangle_x,
								 tri.triangle_y, tri.triangle_x, tri.triangle_y + 5, TFT_RED);
		tri.triangle_x++;

	}
	if(i == 1){

		vertical_triangle_x = tri.triangle_x;

		if(tri.triangle_y > 127){
			tri.triangle_x = (float) random(20, 108);
			tri.triangle_y = 0;
		}

		baseSprite->fillTriangle(tri.triangle_x + 5, tri.triangle_y, tri.triangle_x,
								 tri.triangle_y + 5 * sqrt(2), tri.triangle_x - 5, tri.triangle_y, TFT_RED);
		tri.triangle_y++;

	}

	if(i == 2 && (cnt > 9)){
		if(tri.triangle_x > 127){
			do {
				tri.triangle_x = 0;
				tri.triangle_y = (float) random(20, 108);
			} while(abs(horizontal_triangle_y - tri.triangle_y) < 40);
		}
		baseSprite->fillTriangle(tri.triangle_x, tri.triangle_y - 5, 5 * sqrt(2) + tri.triangle_x,
								 tri.triangle_y, tri.triangle_x, tri.triangle_y + 5, TFT_RED);
		tri.triangle_x++;

	}

	if(i == 3 && (cnt > 19)){
		if(tri.triangle_y > 127){
			do {
				tri.triangle_x = (float) random(20, 108);
				tri.triangle_y = 0;
			} while(abs(vertical_triangle_x - tri.triangle_x) < 40);
		}

		baseSprite->fillTriangle(tri.triangle_x + 5, tri.triangle_y, tri.triangle_x,
								 tri.triangle_y + 5 * sqrt(2), tri.triangle_x - 5, tri.triangle_y, TFT_RED);
		tri.triangle_y++;

	}

}

void Game::drawTriangles(){
	for(int i = 0; i < triangleVector.size(); i++){

		drawTriangle(triangleVector[i]);
		checkIfDead(triangleVector[i]);
	}
}

void Game::checkIfDead(Triangle &triangle){

	if((sqrt(pow(tri.triangle_x + triangle_side * sqrt(3) / 6 - greenDot.dot_x, 2) +
			 pow(tri.triangle_y - greenDot.dot_y, 2)) <
		(triangle_side * sqrt(3) / 6 + radius)) && (i % 2 == 0)){

		Piezo.tone(1000, 300);
		delay(500);

		greenDot.dot_x = 117;
		greenDot.dot_y = 10;

		lives--;

		if(lives == 0){

			cnt = 0;
			lives = 3;
			baseSprite->clear(TFT_BLACK);
			baseSprite->drawString(end_message, 35, 55);
			display->commit();
			delay(2000);

		}

	}else if((sqrt(pow(tri.triangle_x - greenDot.dot_x, 2) +
				   pow(tri.triangle_y + triangle_side * sqrt(3) / 6 - greenDot.dot_y, 2)) <
			  (triangle_side * sqrt(3) / 6 + radius)) && (i % 2 == 1)){

		Piezo.tone(1000, 300);
		delay(500);

		greenDot.dot_x = 117;
		greenDot.dot_y = 10;

		lives--;

		if(lives == 0){

			cnt = 0;
			lives = 3;
			baseSprite->clear(TFT_BLACK);
			baseSprite->drawString(end_message, 35, 55);
			display->commit();
			delay(2000);

		}

	}
}

void Game::checkIfEaten(Circle &blue){

	if(abs(blueDot.dot_x - green_X) < (radius + 5) && abs(blueDot.dot_y - green_Y) < (radius + 5)){

		float pom_x = blueDot.dot_x;
		float pom_y = blueDot.dot_y;

		do {
			blueDot.dot_x = (float) random(10, 117);
			blueDot.dot_y = (float) random(10, 117);
		} while(abs(pom_x - blueDot.dot_x) < 30 && abs(pom_y - blueDot.dot_y) < 30);

		cnt++;

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
		if(playerX<= 5)
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

	checkIfEaten(circleVector[0]);

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

