//
// Created by Domagoj on 12-Oct-20.
//

#include <Arduino.h>
#include <Nibble.h>
#include "Pins.hpp"
#include "GameState.h"
#include "GameOverState.h"
#include "ShowHighscoreState.h"
#include "bitmaps/player.hpp"
#include "bitmaps/fuel.hpp"


GameState *GameState::instance = nullptr;

GameState::GameState(){

	display = Nibble.getDisplay();
	baseSprite = display->getBaseSprite();

	instance = this;

	triangles.push_back({(float) random(10, 150), 0, Triangle::V}); // vertical triangle

	circles.push_back({(float) random(10, 98), (float) random(30, 117)});    // circle = blue

	startUpMessage();

	melodyTime = Melody.playMelody(START, false);

}

void GameState::loop(uint time){

	unsigned int melodyCurrentMillis = millis();
	if(loopPlaying && melodyCurrentMillis - melodyPreviousMillis > melodyTime){
		melodyTime = Melody.playMelody(LOOP, true);
	}

	states(time);

	baseSprite->clear(TFT_BLACK);

	drawCounterString();
	drawLivesString();
	drawWarningMessage();
	drawInvisibilityCounter();

	drawPlayer();
	drawCircles();
	drawTriangles();

	display->commit();

}

void GameState::enter(Game &_game){

	game = &_game;

	player.x = 58;
	player.y = 90;

	score = 0;
	lives = 3;
	invisibilityCounter = 3;
	melodyTime = 0;
	loopPlaying = true;
	melodyPreviousMillis = 0;
	dead = false;

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

	Serial.println("game state enter passed");

}

void GameState::exit(){

	for(int i = 0; i < score / 10; i++)
		triangles.pop_back();

	loopPlaying = false;

	Input::getInstance()->removeBtnPressCallback(BTN_UP);
	Input::getInstance()->removeBtnPressCallback(BTN_DOWN);
	Input::getInstance()->removeBtnPressCallback(BTN_LEFT);
	Input::getInstance()->removeBtnPressCallback(BTN_RIGHT);
	Input::getInstance()->removeBtnPressCallback(BTN_A);
	Input::getInstance()->removeBtnPressCallback(BTN_B);

	Input::getInstance()->removeBtnReleaseCallback(BTN_UP);
	Input::getInstance()->removeBtnReleaseCallback(BTN_DOWN);
	Input::getInstance()->removeBtnReleaseCallback(BTN_LEFT);
	Input::getInstance()->removeBtnReleaseCallback(BTN_RIGHT);
	Input::getInstance()->removeBtnReleaseCallback(BTN_A);
	Input::getInstance()->removeBtnReleaseCallback(BTN_B);

}

void GameState::drawPlayer(){
	
	//playerColor = playerInvisible ? TFT_PURPLE : TFT_GOLD;
	//baseSprite->fillCircle(player.x, player.y, radius, playerColor);
	baseSprite->drawIcon(playerShip, player.x, player.y, 12,18,1,TFT_BLACK);

}

void GameState::drawCircle(Circle &circle){

	baseSprite->drawIcon(fuel, circle.x,circle.y,8,8,1,TFT_BLACK);

}

void GameState::drawCircles(){
	for(int i = 0; i < circles.size(); i++){

		drawCircle(circles[i]);

	}
}

void GameState::drawTriangle(Triangle &triangle){

	if(triangle.orientation == Triangle::H){

		baseSprite->fillTriangle(triangle.x, triangle.y - 5, 5 * sqrt(2) + triangle.x,
								 triangle.y, triangle.x, triangle.y + 5, TFT_RED);
	}
	if(triangle.orientation == Triangle::V){

		baseSprite->fillTriangle(triangle.x + 5, triangle.y, triangle.x,
								 triangle.y + 5 * sqrt(2), triangle.x - 5, triangle.y, TFT_RED);
	}

}

void GameState::drawTriangles(){
	for(int i = 0; i < triangles.size(); i++){

		drawTriangle(triangles[i]);

	}
}

void GameState::triangleMovement(Triangle &triangle, uint t) const{

	if(triangle.orientation == Triangle::H){

		triangle.x += speed * t / 13000;
		if(triangle.x > 128){

			float pomY = triangle.y;

			do {
				triangle.x = 0;
				triangle.y = (float) random(30, 118);
			} while(abs(pomY - triangle.y) < 30);
		}
	}

	if(triangle.orientation == Triangle::V){

		triangle.y += speed * t / 13000;

		if(triangle.y > 128){

			float pomX = triangle.x;

			do {
				triangle.x = (float) random(10, 98);
				triangle.y = 0;
			} while(abs(pomX - triangle.x) < 30);
		}
	}
}

void GameState::checkIfDead(Triangle &triangle){

	if((sqrt(pow(triangle.x + triangleSide * sqrt(3) / 6 - player.x, 2) +
			 pow(triangle.y - player.y, 2)) <
		(triangleSide * sqrt(3) / 6 + radius)) && (triangle.orientation == Triangle::H)){

		Piezo.tone(1000, 300);

		lives--;

		if(lives == 0){
			dead = true;
			gameOver();
		}

		player.x = 58;
		player.y = 90;

	}else if((sqrt(pow(triangle.x - player.x, 2) +
				   pow(triangle.y + triangleSide * sqrt(3) / 6 - player.y, 2)) <
			  (triangleSide * sqrt(3) / 6 + radius)) && (triangle.orientation == Triangle::V)){

		Piezo.tone(1000, 300);

		lives--;

		if(lives == 0){
			dead = true;
			gameOver();

		}

		player.x = 58;
		player.y = 90;

	}

/*
	float firstEdgePX = player.x - triangle.x;
	float firstEdgePY = player.y - (triangle.y - 5);
	float firstEdgeKX = 0;
	float firstEdgeKY = 10;

	float firstK = firstEdgePX * firstEdgeKX + firstEdgePY * firstEdgeKY;

	if(firstK > 0){
		float firstLen = sqrt(firstEdgeKX * firstEdgeKX + firstEdgeKY * firstEdgeKY);
		firstK = firstK / firstLen;

		if(firstK < firstLen){
			if(sqrt(firstEdgePX * firstEdgePX + firstEdgePY * firstEdgePY - firstK * firstK) < radius){

				Piezo.tone(1000, 300);
				delay(500);

				player.x = 117;
				player.y = 10;

				lives--;

				if(lives == 0){

					for(int i = 0; i < score / 10; i++)
						triangles.pop_back();

					baseSprite->clear(TFT_BLACK);
					baseSprite->setTextSize(1);
					baseSprite->setTextFont(2);
					baseSprite->setTextColor(TFT_WHITE);
					baseSprite->drawString(endMessage, 35, 50);
					baseSprite->drawString(finalScore, 40, 80);
					baseSprite->drawNumber(score, 90, 80);
					display->commit();

					score = 0;
					lives = 3;
					noteNum = 0;

					gameOverTones();
					delay(500);

				}
			}
		}
	}

	float secondEdgePX = player.x - triangle.x;
	float secondEdgePY = player.y - (triangle.y + 5);
	float secondEdgeKX = 10 * sqrt(3) / 2;
	float secondEdgeKY = 5;

	float secondK = secondEdgePX * secondEdgeKX + secondEdgePY * secondEdgeKY;

	if(secondK > 0){
		float secondLen = sqrt(secondEdgeKX * secondEdgeKX + secondEdgeKY * secondEdgeKY);
		secondK = secondK / secondLen;

		if(secondK < secondLen){
			if(sqrt(secondEdgePX * secondEdgePX + secondEdgePY * secondEdgePY - secondK * secondK) < radius){

				Piezo.tone(1000, 300);
				delay(500);

				player.x = 117;
				player.y = 10;

				lives--;

				if(lives == 0){

					for(int i = 0; i < score / 10; i++)
						triangles.pop_back();

					baseSprite->clear(TFT_BLACK);
					baseSprite->setTextSize(1);
					baseSprite->setTextFont(2);
					baseSprite->setTextColor(TFT_WHITE);
					baseSprite->drawString(endMessage, 35, 50);
					baseSprite->drawString(finalScore, 40, 80);
					baseSprite->drawNumber(score, 90, 80);
					display->commit();

					score = 0;
					lives = 3;
					noteNum = 0;

					gameOverTones();
					delay(500);

				}
			}
		}
	}

	float thirdEdgePX = player.x - (triangle.x + 10 * sqrt(3) / 2);
	float thirdEdgePY = player.y - triangle.y;
	float thirdEdgeKX = -10 * sqrt(3) / 2;
	float thirdEdgeKY = -5;

	float thirdK = thirdEdgePX * thirdEdgeKX + thirdEdgePY * thirdEdgeKY;

	if(thirdK > 0){
		float thirdLen = sqrt(thirdEdgeKX * thirdEdgeKX + thirdEdgeKY * thirdEdgeKY);
		thirdK = thirdK / thirdLen;

		if(thirdK < thirdLen){
			if(sqrt(thirdEdgePX * thirdEdgePX + thirdEdgePY * thirdEdgePY - thirdK * thirdK) < radius){

				Piezo.tone(1000, 300);
				delay(500);

				player.x = 117;
				player.y = 10;

				lives--;

				if(lives == 0){

					for(int i = 0; i < score / 10; i++)
						triangles.pop_back();

					baseSprite->clear(TFT_BLACK);
					baseSprite->setTextSize(1);
					baseSprite->setTextFont(2);
					baseSprite->setTextColor(TFT_WHITE);
					baseSprite->drawString(endMessage, 35, 50);
					baseSprite->drawString(finalScore, 40, 80);
					baseSprite->drawNumber(score, 90, 80);
					display->commit();

					score = 0;
					lives = 3;
					noteNum = 0;

					gameOverTones();
					delay(500);

				}
			}
		}
	}
*/

/*
	if((sqrt(pow(triangle.x + triangleSide * sqrt(3) / 6 - player.x, 2) +
			 pow(triangle.y - player.y, 2)) <
		(triangleSide * sqrt(3) / 6 + radius)) && (triangle.orientation == Triangle::H)){

		Piezo.tone(1000, 300);
		delay(500);

		player.x = 117;
		player.y = 10;

		lives--;

		if(lives == 0){

			for(int i = 0; i < score / 10; i++)
				triangles.pop_back();
			score = 0;
			lives = 3;
			noteNum = 0;
			baseSprite->clear(TFT_BLACK);
			baseSprite->setTextSize(1);
			baseSprite->setTextFont(2);
			baseSprite->setTextColor(TFT_WHITE);
			baseSprite->drawString(endMessage, 35, 55);
			display->commit();
			delay(2000);

		}

	}else if((sqrt(pow(triangle.x - player.x, 2) +
				   pow(triangle.y + triangleSide * sqrt(3) / 6 - player.y, 2)) <
			  (triangleSide * sqrt(3) / 6 + radius)) && (triangle.orientation == Triangle::V)){

		Piezo.tone(1000, 300);
		delay(500);

		player.x = 117;
		player.y = 10;

		lives--;

		if(lives == 0){

			for(int i = 0; i < score / 10; i++)
				triangles.pop_back();
			score = 0;
			lives = 3;
			noteNum = 0;
			baseSprite->clear(TFT_BLACK);
			baseSprite->setTextSize(1);
			baseSprite->setTextFont(2);
			baseSprite->setTextColor(TFT_WHITE);
			baseSprite->drawString(endMessage, 35, 55);
			display->commit();
			delay(2000);

		}

	}
*/
}

void GameState::checkIfEaten(Circle &blue){

	if(sqrt(pow(blue.x - player.x, 2) + pow(blue.y - player.y, 2)) < (radius * 2)){

		float pomX = blue.x;
		float pomY = blue.y;

		do {
			blue.x = (float) random(10, 117);
			blue.y = (float) random(10, 117);
		} while(sqrt(pow(pomX - blue.x, 2) + pow(pomY - blue.y, 2)) < 30);

		score++;
/*
		if(score > 0 && score % 20 == 0){
			triangles.push_back({0, (float) random(10, 50), Triangle::H}); // horizontal triangle
			lives++;
		}else if(score > 0 && score % 10 == 0){
			triangles.push_back({(float) random(10, 50), 0, Triangle::V}); // vertical triangle
			lives++;
		}
*/
		if(score > 0 && score % 10 == 0){
			triangles.push_back({(float) random(10, 150), 0, Triangle::V}); // vertical triangle
			lives++;
		}
		//Piezo.tone(500, 200);

	}

}

void GameState::states(uint t){

	if(instance->upState){
		player.y -= speed * t / 13000;
		if(player.y <= 5)
			player.y = 5;

	}
	if(instance->leftState){
		player.x -= speed * t / 13000;
		if(player.x <= 5)
			player.x = 5;


	}
	if(instance->downState){
		player.y += speed * t / 13000;
		if(player.y >= 122)
			player.y = 122;

	}
	if(instance->rightState){
		player.x += speed * t / 13000;
		if(player.x >= 122)
			player.x = 122;

	}
	if(instance->aState){
		if(!playerInvisible){

			if(instance->upState && player.y > 5){
				player.y -= speed * t / 13000;
			}
			if(instance->downState && player.y < 122){
				player.y += speed * t / 13000;
			}
			if(instance->rightState && player.x < 122){
				player.x += speed * t / 13000;
			}
			if(instance->leftState && player.y > 5){
				player.x -= speed * t / 13000;
			}
		}

	}
	if(instance->bState){

		if(invisibilityCounter > 0){

			playerInvisible = true;
			previousInvisibilityTime = currentInvisibilityTime = millis();

			instance->aState = false; // disable faster movement while invisible
		}

		Input::getInstance()->removeBtnPressCallback(BTN_B);
	}

	invisibility();


	for(int i = 0; i < triangles.size(); ++i){

		triangleMovement(triangles[i], t);
		if(!playerInvisible)
			checkIfDead(triangles[i]);
		if(dead)
			break;
	}

	for(int i = 0; i < circles.size(); ++i){

		checkIfEaten(circles[i]);
	}

}

void GameState::gameOver(){

	game->changeState(new GameOverState(score));

}

void GameState::invisibility(){

	if(playerInvisible)
		currentInvisibilityTime = millis();

	if(currentInvisibilityTime - previousInvisibilityTime > invisibilityTime){

		playerInvisible = false;
		invisibilityCounter--;
		currentInvisibilityTime = previousInvisibilityTime = millis();

		Input::getInstance()->setBtnPressCallback(BTN_B, buttonBPressed);
	}
}

void GameState::drawWarningMessage(){

	if((score + 1) % 10 == 0){
		baseSprite->setTextColor(TFT_RED);
		baseSprite->drawString(warning, 45, 60);

	}
}

void GameState::startUpMessage(){

	baseSprite->clear(TFT_BLACK);
	baseSprite->setTextSize(1);
	baseSprite->setTextFont(2);
	baseSprite->setTextColor(TFT_WHITE);
	baseSprite->drawString(startGame, 25, 55);
	display->commit();

}

void GameState::drawCounterString(){

	baseSprite->setTextSize(1);
	baseSprite->setTextFont(1);
	baseSprite->setTextColor(TFT_LIGHTGREY);
	baseSprite->drawNumber(score, 110, 120);

}

void GameState::drawLivesString(){

	baseSprite->setTextSize(1);
	baseSprite->setTextFont(1);
	baseSprite->setTextColor(TFT_LIGHTGREY);
	baseSprite->drawString(livesRest, 2, 3);
	baseSprite->drawNumber(lives, 42, 3);
}

void GameState::drawInvisibilityCounter(){

	baseSprite->setTextSize(1);
	baseSprite->setTextFont(1);
	baseSprite->setTextColor(TFT_LIGHTGREY);
	baseSprite->drawString(invisibleTimes, 1, 120);
	baseSprite->drawNumber(invisibilityCounter, 85, 120);
}

void GameState::buttonUpPressed(){

	instance->upState = true;

}

void GameState::buttonDownPressed(){

	instance->downState = true;

}

void GameState::buttonLeftPressed(){

	instance->leftState = true;

}

void GameState::buttonRightPressed(){

	instance->rightState = true;

}

void GameState::buttonAPressed(){

	instance->aState = true;

}

void GameState::buttonBPressed(){

	instance->bState = true;

}

void GameState::buttonUpReleased(){

	instance->upState = false;

}

void GameState::buttonDownReleased(){

	instance->downState = false;

}

void GameState::buttonLeftReleased(){

	instance->leftState = false;

}

void GameState::buttonRightReleased(){

	instance->rightState = false;


}

void GameState::buttonAReleased(){

	instance->aState = false;

}

void GameState::buttonBReleased(){

	instance->bState = false;
}
