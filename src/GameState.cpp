//
// Created by Domagoj on 12-Oct-20.
//

#include <Arduino.h>
#include <Nibble.h>
#include "Pins.hpp"
#include "GameState.h"
#include "GameOverState.h"
#include "bitmaps/player/player.hpp"
#include "bitmaps/objects/fuel.hpp"
#include "bitmaps/objects/ore.hpp"
#include "bitmaps/aliens/alien1.hpp"
#include "bitmaps/aliens/alien2.hpp"
#include "bitmaps/aliens/alien3.hpp"
#include "bitmaps/player/invisiblePlayer.hpp"
#include "bitmaps/igniteLowFrames/igniteLowPlayer1.hpp"
#include "bitmaps/igniteLowFrames/igniteLowPlayer2.hpp"
#include "bitmaps/igniteLowFrames/igniteLowPlayer3.hpp"
#include "bitmaps/igniteLowFrames/igniteLowPlayer4.hpp"
#include "bitmaps/igniteHighFrames/igniteHighPlayer1.hpp"
#include "bitmaps/igniteHighFrames/igniteHighPlayer2.hpp"
#include "bitmaps/igniteHighFrames/igniteHighPlayer3.hpp"
#include "bitmaps/igniteHighFrames/igniteHighPlayer4.hpp"
#include "bitmaps/igniteHighFrames/igniteHighPlayer5.hpp"
#include "bitmaps/redFlag.hpp"


GameState *GameState::instance = nullptr;

GameState::GameState(){

	display = Nibble.getDisplay();
	baseSprite = display->getBaseSprite();

	instance = this;

	aliens.push_back({(float) random(10, 150), 0, Alien::ALIEN1});
	objects.push_back({(float) random(10, 150), 0, Object::FUEL});
	objects.push_back({(float) random(10, 150), 0, Object::ORE});

	melodyTime = Melody.playMelody(START, false);

}

void GameState::loop(uint time){

	if(pausedState){

		baseSprite->clear(TFT_BLACK);

		draw();

		display->commit();

		states(time);
	}else{

		unsigned int melodyCurrentMillis = millis();
		if(loopPlaying && melodyCurrentMillis - melodyPreviousMillis > melodyTime){
			melodyTime = Melody.playMelody(LOOP, true);
		}

		baseSprite->clear(TFT_BLACK);

		draw();

		display->commit();

		states(time);

	}
}

void GameState::enter(Game &_game){

	game = &_game;

	playerX = 58;
	playerY = 90;

	score = 0;
	lives = 3;
	invisibilityCounter = 3;
	melodyTime = 0;
	loopPlaying = true;
	melodyPreviousMillis = 0;
	dead = false;

	fuelBar = {65, 122, 62, 5};

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

	for(int i = 0; i < aliens.size(); i++)
		aliens.pop_back();
	for(int i = 0; i < objects.size(); i++)
		objects.pop_back();

	oreCheck = false;
	fuelCheck = false;
	previousOreTime = 0;
	previousFuelTime = 0;

	pausedState = false;

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

void GameState::draw(){

	if(pausedState)
		drawPausedState();

	else{

		drawPlayer();
		drawObjects();
		drawAliens();

		drawCounterString();
		drawLivesString();
		drawInvisibilityCounter();
		drawFuelBar();
	}
}

void GameState::drawPlayer(){

	if(playerInvisible)
		baseSprite->drawIcon(invPlayer, playerX, playerY, 12, 18, 1, TFT_BLACK);

	else if(upState && !downState){
		switch(highIgnitionPlayerFrame){

			case 1:
				baseSprite->drawIcon(igniteHighPlayer2, playerX, playerY, 12, 17, 1, TFT_BLACK);
				break;
			case 2:
				baseSprite->drawIcon(igniteHighPlayer3, playerX, playerY, 12, 17, 1, TFT_BLACK);
				break;
			case 3:
				baseSprite->drawIcon(igniteHighPlayer4, playerX, playerY, 12, 16, 1, TFT_BLACK);
				break;
			case 4:
				baseSprite->drawIcon(igniteHighPlayer5, playerX, playerY, 12, 15, 1, TFT_BLACK);
				break;

			default:
				break;
		}
	}else if(!upState && downState){

		baseSprite->drawIcon(playerShip, playerX, playerY, 12, 18, 1, TFT_BLACK);

	}else{
		switch(lowIgnitionPlayerFrame){

			case 1:
				baseSprite->drawIcon(igniteLowPlayer2, playerX, playerY, 12, 16, 1, TFT_BLACK);
				break;
			case 2:
				baseSprite->drawIcon(igniteLowPlayer3, playerX, playerY, 12, 16, 1, TFT_BLACK);
				break;
			case 3:
				baseSprite->drawIcon(igniteLowPlayer4, playerX, playerY, 12, 15, 1, TFT_BLACK);
				break;

			default:
				break;
		}
	}

}

void GameState::drawObject(Object &object){

	if(object.type == Object::FUEL && fuelCheck)
		baseSprite->drawIcon(fuelIcon, object.x, object.y, 8, 8, 1, TFT_BLACK);
	if(object.type == Object::ORE && oreCheck)
		baseSprite->drawIcon(oreIcon, object.x, object.y, 3, 3, 1, TFT_BLACK);

}

void GameState::drawObjects(){
	for(int i = 0; i < objects.size(); i++){

		drawObject(objects[i]);

	}
}

void GameState::drawAlien(Alien &alien){

	if(alien.type == Alien::ALIEN1)
		baseSprite->drawIcon(alien1, alien.x, alien.y, 10, 10, 1, TFT_BLACK);
	if(alien.type == Alien::ALIEN2)
		baseSprite->drawIcon(alien2, alien.x, alien.y, 10, 10, 1, TFT_BLACK);
	if(alien.type == Alien::ALIEN2)
		baseSprite->drawIcon(alien3, alien.x, alien.y, 10, 10, 1, TFT_BLACK);

}

void GameState::drawAliens(){
	for(int i = 0; i < aliens.size(); i++){

		drawAlien(aliens[i]);

	}
}

void GameState::alienMovement(Alien &alien, uint t) const{

	if(alien.type == alien.ALIEN1){

		alien.y += speed * t / 13000;

		if(alien.y > 128){

			float pomX = alien.x;

			do {
				alien.x = (float) random(10, 98);
				alien.y = 0;
			} while(abs(pomX - alien.x) < 30);
		}
	}
}

void GameState::objectMovement(Object &object, uint t){

	if(object.type == Object::FUEL && fuelCheck){

		object.y += speed * t / 26000;

		if(object.y > 128){

			fuelCheck = false;

			float pomX = object.x;

			do {
				object.x = (float) random(10, 98);
				object.y = 0;
			} while(abs(pomX - object.x) < 30);

		}
	}

	if(object.type == Object::ORE && oreCheck){

		object.y += speed * t / 26000;

		if(object.y > 128){

			oreCheck = false;

			float pomX = object.x;

			do {
				object.x = (float) random(10, 98);
				object.y = 0;
			} while(abs(pomX - object.x) < 30);
		}
	}

}

void GameState::checkIfDead(Alien &alien){

	uint dx = abs((alien.x + 5) - (playerX + 6));
	uint dy = abs((alien.y + 5) - (playerY + 9));

	if(sqrt(pow(dx, 2) + pow(dy, 2)) <
	   sqrt(pow(playerX + 6, 2) + pow(playerY + 9, 2)) + sqrt(pow(alien.x + 5, 2) + pow(alien.y + 5, 2))){

		Piezo.tone(1000, 300);

		lives--;

		if(lives == 0){
			dead = true;
			gameOver();

		}

		playerX = 58;
		playerY = 90;

	}

}

void GameState::checkIfCollected(Object &object){

	if(object.type == Object::ORE){

		uint dx = abs((object.x + 1.5) - (playerX + 6));
		uint dy = abs((object.y + 1.5) - (playerY + 9));

		if(sqrt(pow(dx, 2) + pow(dy, 2)) <
		   sqrt(pow(playerX + 6, 2) + pow(playerY + 9, 2)) + sqrt(pow(object.x + 1.5, 2) + pow(object.y + 1.5, 2))){

			ore++;

		}
	}else if(object.type == Object::FUEL){

		uint dx = abs((object.x + 4) - (playerX + 6));
		uint dy = abs((object.y + 4) - (playerY + 9));

		if(sqrt(pow(dx, 2) + pow(dy, 2)) <
		   sqrt(pow(playerX + 6, 2) + pow(playerY + 9, 2)) + sqrt(pow(object.x + 4, 2) + pow(object.y + 4, 2))){

			fuel++;

		}

	}

}


void GameState::states(uint t){

	if(pausedState){

		if(instance->aState){

			pausedState = false;
			aState = false;

		}else if(instance->bState){

			game->changeState(new Menu());
		}
	}else{

		if(instance->upState){
			playerY -= speed * t / 13000;
			if(playerY <= 9)
				playerY = 9;

		}
		if(instance->leftState){
			playerX -= speed * t / 13000;
			if(playerX <= 6)
				playerX = 6;


		}
		if(instance->downState){
			playerY += speed * t / 13000;
			if(playerY >= 119)
				playerY = 119;

		}
		if(instance->rightState){
			playerX += speed * t / 13000;
			if(playerX >= 122)
				playerX = 122;

		}
		if(instance->aState){

			if(invisibilityCounter > 0){

				playerInvisible = true;
				previousInvisibilityTime = currentInvisibilityTime = millis();

				//instance->aState = false; // disable faster movement while invisible
			}

			Input::getInstance()->removeBtnPressCallback(BTN_A);
		}
		if(instance->bState){

			pausedState = true;
			bState = false;
		}

		invisibility();

		for(int i = 0; i < aliens.size(); ++i){

			alienMovement(aliens[i], t);
			if(!playerInvisible)
				//checkIfDead(aliens[i]);
				if(dead)
					break;
		}


		for(int i = 0; i < objects.size(); ++i){

			objectMovement(objects[i], t);
			checkIfCollected(objects[i]);
		}


		if(millis() - previousOreTime > oreTime){

			oreCheck = true;
			previousOreTime = millis();
		}
		if(millis() - previousFuelTime > fuelTime){

			fuelCheck = true;
			previousFuelTime = millis();
		}

		if(millis() - previousLowIgnitionPlayerTime > lowIgnitionTimePerFrame){
			lowIgnitionPlayerFrame++;
			if(lowIgnitionPlayerFrame > 3)
				lowIgnitionPlayerFrame = 1;
			previousLowIgnitionPlayerTime = millis();
		}
		if(millis() - previousHighIgnitionPlayerTime > highIgnitionTimePerFrame){
			highIgnitionPlayerFrame++;
			if(highIgnitionPlayerFrame > 4)
				highIgnitionPlayerFrame = 1;
			previousHighIgnitionPlayerTime = millis();
		}
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


void GameState::drawCounterString(){

	baseSprite->setTextSize(1);
	baseSprite->setTextFont(1);
	baseSprite->setTextColor(TFT_LIGHTGREY);
	baseSprite->drawNumber(score, 110, 3);

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
	baseSprite->drawNumber(invisibilityCounter, 30, 120);
}

void GameState::drawFuelBar(){

	baseSprite->drawRect(64, 121, 64, 7, TFT_WHITE);
	baseSprite->fillRect(fuelBar.x, fuelBar.y, fuelBar.width, fuelBar.height, TFT_GREEN);
}

void GameState::drawPausedState(){

	baseSprite->drawIcon(redFlag, 59, 20, 10, 10, 1, TFT_BLACK);

	baseSprite->setTextSize(1);
	baseSprite->setTextFont(2);
	baseSprite->setTextColor(TFT_LIGHTGREY);
	baseSprite->drawString(pause, 40, 64);

	baseSprite->setTextSize(1);
	baseSprite->setTextFont(1);
	baseSprite->setTextColor(TFT_LIGHTGREY);
	baseSprite->drawString(resume, 1, 120);
	baseSprite->drawString(quit, 80, 120);
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
