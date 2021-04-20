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
#include "bitmaps/aliens/alien4.hpp"
#include "bitmaps/aliens/alien5.hpp"
#include "bitmaps/aliens/alien6.hpp"
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
#include "bitmaps/states/gameStateBackground.hpp"
#include "bitmaps/planets/planet1.hpp"
#include "bitmaps/planets/planet2.hpp"
#include "bitmaps/planets/planet3.hpp"
#include "bitmaps/planets/planet4.hpp"
#include "bitmaps/planets/planet5.hpp"
#include "bitmaps/planets/planet6.hpp"
#include "bitmaps/planets/planet7.hpp"
#include "bitmaps/planets/planet8.hpp"
#include "bitmaps/planets/planet9.hpp"
#include "bitmaps/tiltedPlayer/invisiblePlayer/invTiltLeft.hpp"
#include "bitmaps/tiltedPlayer/invisiblePlayer/invTiltRight.hpp"
#include "bitmaps/tiltedPlayer/player/playerTiltLeft1.hpp"
#include "bitmaps/tiltedPlayer/player/playerTiltLeft2.hpp"
#include "bitmaps/tiltedPlayer/player/playerTiltLeft3.hpp"
#include "bitmaps/tiltedPlayer/player/playerTiltRight1.hpp"
#include "bitmaps/tiltedPlayer/player/playerTiltRight2.hpp"
#include "bitmaps/tiltedPlayer/player/playerTiltRight3.hpp"
#include "bitmaps/tiltedPlayer/ignition/ignitionTiltLeft1.hpp"
#include "bitmaps/tiltedPlayer/ignition/ignitionTiltLeft2.hpp"
#include "bitmaps/tiltedPlayer/ignition/ignitionTiltLeft3.hpp"
#include "bitmaps/tiltedPlayer/ignition/ignitionTiltRight1.hpp"
#include "bitmaps/tiltedPlayer/ignition/ignitionTiltRight2.hpp"
#include "bitmaps/tiltedPlayer/ignition/ignitionTiltRight3.hpp"
#include "bitmaps/states/background.hpp"
#include "bitmaps/states/pauseScreen.hpp"

GameState *GameState::instance = nullptr;

GameState::GameState(){

	display = Nibble.getDisplay();
	baseSprite = display->getBaseSprite();

	instance = this;

	engine = new ParticleEngine(baseSprite);

	objects.push_back({(float) random(20, 100), 0, Object::FUEL});
	objects.push_back({(float) random(20, 100), 0, Object::ORE});

	playerX = 58;
	playerY = 90;

	ore = 0;
	fuelBar = {65, 122, 64, 5};
	invisibilityCounter = 3;

	lives = 30;
	dead = false;

	melodyTime = 0;
	loopPlaying = true;
	melodyPreviousMillis = 0;

	betweenLevelState = true;
	newLevel = true;
	level = 1;

	fuelCheck = false;
	previousFuelTime = 0;
	oreCheck = false;
	previousOreTime = 0;
	previousLevelTime = 0;

	fuelDx = 1;
}

void GameState::loop(uint time){

	if(pausedState){

		Melody.playMelody(STOP, false);

		baseSprite->clear(TFT_BLACK);

		draw();

		display->commit();

		states(time);

	}else if(betweenLevelState){

		//melodyTime = Melody.playMelody(START, false);

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
		states(time);

		display->commit();


	}
}

void GameState::enter(Game &_game){

	game = &_game;

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

}

void GameState::exit(){

	for(int i = 0; i < aliens.size(); i++)
		aliens.pop_back();
	for(int i = 0; i < objects.size(); i++)
		objects.pop_back();

	delete engine;

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

	else if(betweenLevelState){
		drawBackground();
		drawBetweenLevelState();

	}

	else{

		drawBackground();

		if(levelEnd)
			drawLevelEnd();

		//drawBackground();
		drawPlayer();
		drawObjects();
		drawAliens();

		drawCounterString();
		drawLivesString();
		drawInvisibilityCounter();
		drawFuelBar();
		engine->loop(0);

	}
}

void GameState::drawPlayer(){

	if(playerInvisible){
		if(leftState || (leftState && upState) || (leftState && downState)){
			baseSprite->drawIcon(invTiltLeft, playerX, playerY, 12, 18, 1, TFT_BLACK);
		}
		else if(rightState || (rightState && upState) || (rightState && upState)){
			baseSprite->drawIcon(invTiltRight, playerX, playerY, 12, 18, 1, TFT_BLACK);
		}
		else
			baseSprite->drawIcon(invPlayer, playerX, playerY, 12, 18, 1, TFT_BLACK);
	}


	else if((leftState && upState)){
		switch(leanLeftIgnitionPlayerFrame){

			case 1:
				baseSprite->drawIcon(ignitionTiltLeft1, playerX, playerY, 12, 18, 1, TFT_BLACK);
				break;
			case 2:
				baseSprite->drawIcon(ignitionTiltLeft2, playerX, playerY, 11, 17, 1, TFT_BLACK);
				break;
			case 3:
				baseSprite->drawIcon(ignitionTiltLeft3, playerX, playerY, 10, 17, 1, TFT_BLACK);
				break;
			default:
				break;
		}
	}
	else if((leftState && downState)){
		switch(leanLeftPlayerFrame){

			case 1:
				baseSprite->drawIcon(playerTiltLeft1, playerX, playerY, 12, 18, 1, TFT_BLACK);
				break;
			case 2:
				baseSprite->drawIcon(playerTiltLeft2, playerX, playerY, 11, 14, 1, TFT_BLACK);
				break;
			case 3:
				baseSprite->drawIcon(playerTiltLeft3, playerX, playerY, 10, 14, 1, TFT_BLACK);
				break;
			default:
				break;
		}
	}
	else if (leftState){
		switch(leanLeftIgnitionPlayerFrame){

			case 1:
				baseSprite->drawIcon(ignitionTiltLeft1, playerX, playerY, 12, 18, 1, TFT_BLACK);
				break;
			case 2:
				baseSprite->drawIcon(ignitionTiltLeft2, playerX, playerY, 11, 17, 1, TFT_BLACK);
				break;
			case 3:
				baseSprite->drawIcon(ignitionTiltLeft3, playerX, playerY, 10, 17, 1, TFT_BLACK);
				break;
			default:
				break;
		}
	}

	else if((rightState && upState)){
		switch(leanRightIgnitionPlayerFrame){

			case 1:
				baseSprite->drawIcon(ignitionTiltRight1, playerX, playerY, 12, 18, 1, TFT_BLACK);
				break;
			case 2:
				baseSprite->drawIcon(ignitionTiltRight2, playerX, playerY, 11, 17, 1, TFT_BLACK);
				break;
			case 3:
				baseSprite->drawIcon(ignitionTiltRight3, playerX, playerY, 10, 17, 1, TFT_BLACK);
				break;
			default:
				break;
		}
	}
	else if((rightState && downState)){
		switch(leanRightPlayerFrame){

			case 1:
				baseSprite->drawIcon(playerTiltRight1, playerX, playerY, 12, 18, 1, TFT_BLACK);
				break;
			case 2:
				baseSprite->drawIcon(playerTiltRight2, playerX, playerY, 11, 14, 1, TFT_BLACK);
				break;
			case 3:
				baseSprite->drawIcon(playerTiltRight3, playerX, playerY, 10, 14, 1, TFT_BLACK);
				break;
			default:
				break;
		}
	}
	else if (rightState){
		switch(leanRightIgnitionPlayerFrame){

			case 1:
				baseSprite->drawIcon(ignitionTiltRight1, playerX, playerY, 12, 18, 1, TFT_BLACK);
				break;
			case 2:
				baseSprite->drawIcon(ignitionTiltRight2, playerX, playerY, 11, 17, 1, TFT_BLACK);
				break;
			case 3:
				baseSprite->drawIcon(ignitionTiltRight3, playerX, playerY, 10, 17, 1, TFT_BLACK);
				break;
			default:
				break;
		}
	}

	else if(upState){
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
	}
	else if(downState){

		baseSprite->drawIcon(playerShip, playerX, playerY, 12, 18, 1, TFT_BLACK);

	}
	else{
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
	if(alien.type == Alien::ALIEN3)
		baseSprite->drawIcon(alien3, alien.x, alien.y, 10, 10, 1, TFT_BLACK);
	if(alien.type == Alien::ALIEN4)
		baseSprite->drawIcon(alien4, alien.x, alien.y, 10, 10, 1, TFT_BLACK);
	if(alien.type == Alien::ALIEN5)
		baseSprite->drawIcon(alien5, alien.x, alien.y, 10, 10, 1, TFT_BLACK);
	if(alien.type == Alien::ALIEN6)
		baseSprite->drawIcon(alien6, alien.x, alien.y, 10, 10, 1, TFT_BLACK);

}

void GameState::drawAliens(){
	for(int i = 0; i < aliens.size(); i++){

		drawAlien(aliens[i]);

	}
}

void GameState::alienMovement(Alien &alien, uint t) const{

	if(alien.type == alien.ALIEN1){

		alien.y += speed * t / 15000;

		if(alien.y > 128){

			float pomX = alien.x;

			do {
				alien.x = (float) random(10, 98);
				alien.y = 0;
			} while(abs(pomX - alien.x) < 30);
		}
	}

	if(alien.type == alien.ALIEN2){

		alien.y += speed * t / 14000;

		if(alien.y > 128){

			float pomX = alien.x;

			do {
				alien.x = (float) random(10, 98);
				alien.y = 0;
			} while(abs(pomX - alien.x) < 30);
		}
	}

	if(alien.type == alien.ALIEN3){

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

			object.x = (float) random(20, 100);
			object.y = -10;

		}
	}

	if(object.type == Object::ORE && oreCheck){

		object.y += speed * t / 26000;

		if(object.y > 128){

			oreCheck = false;

			object.x = (float) random(20, 100);
			object.y = -10;

		}
	}

}

void GameState::checkIfDead(Alien &alien){

	uint dx = abs((alien.x + 5) - (playerX + 6));
	uint dy = abs((alien.y + 5) - (playerY + 9));

	if(sqrt(pow(dx, 2) + pow(dy, 2)) <
	   PLAYER_RADIUS + ALIEN_RADIUS){

		//Piezo.tone(1000, 300);

		dead = true;

		lives--;

		playerX = 58;
		playerY = 128;
		playerInvisible = true;
		previousInvisibilityTime = currentInvisibilityTime = millis();

		if(lives == 0){
			game->changeState(new GameOverState(ore));
		}
	}
}

void GameState::checkIfCollected(Object &object){

	if(object.type == Object::ORE){											// check if ore is collected

		uint dx = abs((object.x + 1.5) - (playerX + 6));
		uint dy = abs((object.y + 1.5) - (playerY + 9));

		if(sqrt(pow(dx, 2) + pow(dy, 2)) <
		   PLAYER_RADIUS + ORE_RADIUS){

			ore++;

			object.x = (float)random(20,100);
			object.y = 0;

			oreCheck = false;
			previousOreTime = millis();
		}
	}else if(object.type == Object::FUEL){									// check if fuel is collected

		uint dx = abs((object.x + 4) - (playerX + 6));
		uint dy = abs((object.y + 4) - (playerY + 9));

		if(sqrt(pow(dx, 2) + pow(dy, 2)) <
		   PLAYER_RADIUS + FUEL_RADIUS){


			if(fuelBar.width >= 64){
				fuelBar.x = 65;
				fuelBar.width = 64;
			}
			else{
				fuelBar.x -= 10;
				fuelBar.width += 10;
			}

			object.x = (float)random(20,100);
			object.y = 0;

			fuelCheck = false;
			previousFuelTime = millis();

		}
	}
}

void GameState::states(uint t){

	if(newLevel){
		levelHandler();                                        // level control function
		lvlEndPlanetY = -45;
		fuelBar = {65, 122, 64, 5};        // restoring fuel bar on every level
	}                                                                    // new level states


	if(pausedState){

		if(instance->aState){

			pausedState = false;
			aState = false;

		}                // resume game
		else if(instance->bState){

			game->changeState(new Menu());
		}            // quit game
	}                                                                // pause states

	else if(betweenLevelState){

		if(instance->aState){															// start new lvl
			betweenLevelState = false;
			previousLevelTime = millis();

			previousFuelTime = millis();												// update previous fuel time
			previousOreTime = millis();													// update previous ore time

			instance->aState = false;
		}

		backgroundY1 += (float) speed * t / 100000;            							// controlling background
		if(backgroundY1 > 127)
			backgroundY1 = 0;
		backgroundY2 += (float) speed * t / 100000;
		if(backgroundY2 > 0)
			backgroundY2 = -127;


	}                                                        							// between level states

	else{                                                           					// in game states

		if(instance->upState){
			playerY -= speed * t / 13000;
			if(playerY <= 0)
				playerY = 0;

		}                           							// controlling movement
		if(instance->leftState){
			playerX -= speed * t / 13000;
			if(playerX <= 0)
				playerX = 0;


		}                        							// controlling movement
		if(instance->downState){
			playerY += speed * t / 13000;
			if(playerY >= 103)
				playerY = 103;

		}                        							// controlling movement
		if(instance->rightState){
			playerX += speed * t / 13000;
			if(playerX >= 116)
				playerX = 116;

		}                        							// controlling movement
		if(instance->aState && !playerInvisible){

			if(invisibilityCounter > 0){

				playerInvisible = true;
				previousInvisibilityTime = currentInvisibilityTime = millis();

			}
			instance->aState = false;
		}                            		// invisibility
		if(instance->bState){

			pausedState = true;
			instance->bState = false;
		}                            							// paused state


		if(fuelBar.width <= 0)															// fuel bar states
			game->changeState(new GameOverState(ore));
		else{
			fuelBar.x += speed * t * fuelDx/ 1000000;
			fuelBar.width -= speed * t * fuelDx/ 1000000;
		}

		invisibility();

		for(int i = 0; i < aliens.size(); ++i){											// alien movement

			alienMovement(aliens[i], t);
			if(!playerInvisible)
				checkIfDead(aliens[i]);
			if(dead)
				break;
		}

		for(int i = 0; i < objects.size(); ++i){										// object movement & interaction

			objectMovement(objects[i], t);
			checkIfCollected(objects[i]);
		}


		if(millis() - previousOreTime > oreTime){

			oreCheck = true;
			previousOreTime = millis();
		}                                                                        // new ore every oreTime
		if(millis() - previousFuelTime > fuelTime){

			fuelCheck = true;
			previousFuelTime = millis();
		}                                                                        // new fuel every fuelTime

		if(millis() - previousLowIgnitionPlayerTime > lowIgnitionTimePerFrame){
			lowIgnitionPlayerFrame++;
			if(lowIgnitionPlayerFrame > 3)
				lowIgnitionPlayerFrame = 1;
			previousLowIgnitionPlayerTime = millis();
		}                                            //low ignition .gif
		if(millis() - previousHighIgnitionPlayerTime > highIgnitionTimePerFrame){
			highIgnitionPlayerFrame++;
			if(highIgnitionPlayerFrame > 4)
				highIgnitionPlayerFrame = 1;
			previousHighIgnitionPlayerTime = millis();
		}                                        //high ignition .gif

		if(millis() - previousLeanLeftPlayerTime > leanLeftTimePerFrame){
			leanLeftPlayerFrame++;
			if(leanLeftPlayerFrame > 2)
				leanLeftPlayerFrame = 2;
			previousLeanLeftPlayerTime = millis();
		}                                                //lean left no gas .gif
		if(millis() - previousLeanRightPlayerTime > leanRightTimePerFrame){
			leanRightPlayerFrame++;
			if(leanRightPlayerFrame > 2)
				leanRightPlayerFrame = 2;
			previousLeanRightPlayerTime = millis();
		}                                                //lean right no gas .gif

		if(millis() - previousLeanLeftIgnitionPlayerTime > leanLeftIgnitionTimePerFrame){
			leanLeftIgnitionPlayerFrame++;
			if(leanLeftIgnitionPlayerFrame > 2)
				leanLeftIgnitionPlayerFrame = 2;
			previousLeanLeftIgnitionPlayerTime = millis();
		}                                //lean left ignition .gif
		if(millis() - previousLeanRightIgnitionPlayerTime > leanRightIgnitionTimePerFrame){
			leanRightIgnitionPlayerFrame++;
			if(leanRightIgnitionPlayerFrame > 2)
				leanRightIgnitionPlayerFrame = 2;
			previousLeanRightIgnitionPlayerTime = millis();
		}                                //lean right ignition .gif

		if(dead){
			if(playerY <= 90)
				playerY = 90;
			else
				playerY -= speed * t / 13000;

		}                                            					// spawning when dead

		if(millis() - previousLevelTime > levelTime){								// lvl ending

			levelEnd = true;
			if(lvlEndPlanetY >= 20)
				lvlEndPlanetY = 20;
			else
				lvlEndPlanetY += speed * t / 26000;

			fuelCheck = false;														// stop fuel & ore when lvl end
			previousFuelTime = 0;
			oreCheck = false;
			previousOreTime = 0;

			if(millis() - (previousLevelTime + 5000) > levelTime){					// 5 sec after lvl end -> change lvl
				newLevel = true;
				level++;
				betweenLevelState = true;
				levelEnd = false;

				previousLevelTime = millis();
			}
		}

		backgroundY1 += (float) speed * t / 50000;                    				// controlling background
		if(backgroundY1 > 127)
			backgroundY1 = 0;
		backgroundY2 += (float) speed * t / 50000;
		if(backgroundY2 > 0)
			backgroundY2 = -127;

		engine->update(playerX + 6, playerY + 18);					// particle engine
	}
}

void GameState::levelHandler(){

	switch(level){

		case 1:
			aliens.push_back({(float) random(10, 150), 0, Alien::ALIEN1});
			fuelDx += 0.2;
			break;

		case 2:
			aliens.pop_back();
			aliens.push_back({(float) random(10, 150), 0, Alien::ALIEN2});
			fuelDx += 0.2;
			break;

		case 3:
			aliens.pop_back();
			aliens.push_back({(float) random(10, 150), 0, Alien::ALIEN3});
			fuelDx += 0.2;
			break;

		case 4:
			aliens.pop_back();
			aliens.push_back({(float) random(10, 150), 0, Alien::ALIEN4});
			fuelDx += 0.2;
			break;
		case 5:
			aliens.pop_back();
			aliens.push_back({(float) random(10, 150), 0, Alien::ALIEN5});
			fuelDx += 0.2;
			break;
		case 6:
			aliens.pop_back();
			aliens.push_back({(float) random(10, 150), 0, Alien::ALIEN6});
			fuelDx += 0.2;
			break;
		case 7:
			aliens.pop_back();
			aliens.push_back({(float) random(10, 150), 0, Alien::ALIEN3});
			fuelDx += 0.2;
			break;
		case 8:
			aliens.pop_back();
			aliens.push_back({(float) random(10, 150), 0, Alien::ALIEN3});
			fuelDx += 0.2;
			break;
		case 9:
			aliens.pop_back();
			aliens.push_back({(float) random(10, 150), 0, Alien::ALIEN3});
			fuelDx += 0.2;
			break;
		default:
			break;

	}

	newLevel = false;
}

void GameState::invisibility(){

	if(playerInvisible){
		currentInvisibilityTime = millis();

		if(currentInvisibilityTime - previousInvisibilityTime > invisibilityTime){

			playerInvisible = false;
			if(!dead)
				invisibilityCounter--;
			else
				dead = false;
			currentInvisibilityTime = previousInvisibilityTime = millis();

			Input::getInstance()->setBtnPressCallback(BTN_A, buttonAPressed);
		}
	}
}


void GameState::drawBackground(){

	//baseSprite->drawIcon(gameStateBackground,backgroundX1,backgroundY1,128,128);
	//baseSprite->drawIcon(gameStateBackground,backgroundX2,backgroundY2,128,128);
	//baseSprite->drawMovingBackground(gameStateBackground,0,0,128,128,128,128,1);
	//baseSprite->drawIcon(background,0,0,128,128);
	baseSprite->clear(TFT_BLACK);

}

void GameState::drawCounterString(){

	baseSprite->setTextSize(1);
	baseSprite->setTextFont(1);
	baseSprite->setTextColor(TFT_GOLD);
	baseSprite->drawNumber(ore, 5, 3);
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

	baseSprite->drawRect(64, 121, 64, 7, TFT_PURPLE);
	baseSprite->fillRect(fuelBar.x, fuelBar.y, fuelBar.width, fuelBar.height, TFT_BLUE);
}

void GameState::drawPausedState(){

	baseSprite->drawIcon(pauseScreen, 0, 0, 128, 128);

	baseSprite->setTextSize(1);
	baseSprite->setTextFont(1);
	baseSprite->setTextColor(TFT_LIGHTGREY);
	baseSprite->drawString(resume, 1, 118);
	baseSprite->drawString(quit, 80, 118);
}

void GameState::drawBetweenLevelState(){

	baseSprite->setTextSize(1);
	baseSprite->setTextFont(1);
	baseSprite->setTextColor(TFT_LIGHTGREY);
	baseSprite->drawString(continueString, 1, 120);

	switch(level){

		case 1:
			baseSprite->drawIcon(planet1, 42, 20, 45, 45, 1, TFT_BLACK);
			baseSprite->drawString(levelNumber, 42, 80);
			baseSprite->drawNumber(level, 80, 80);

			break;
		case 2:
			baseSprite->drawIcon(planet2, 42, 20, 45, 45, 1, TFT_BLACK);
			baseSprite->drawString(levelNumber, 42, 80);
			baseSprite->drawNumber(level, 80, 80);
			break;
		case 3:
			baseSprite->drawIcon(planet3, 42, 20, 45, 45, 1, TFT_BLACK);
			baseSprite->drawString(levelNumber, 42, 80);
			baseSprite->drawNumber(level, 80, 80);
			break;
		case 4:
			baseSprite->drawIcon(planet4, 42, 20, 45, 45, 1, TFT_BLACK);
			baseSprite->drawString(levelNumber, 42, 80);
			baseSprite->drawNumber(level, 80, 80);
			break;
		case 5:
			baseSprite->drawIcon(planet5, 42, 20, 45, 45, 1, TFT_BLACK);
			baseSprite->drawString(levelNumber, 42, 80);
			baseSprite->drawNumber(level, 80, 80);
			break;
		case 6:
			baseSprite->drawIcon(planet6, 42, 20, 45, 45, 1, TFT_BLACK);
			baseSprite->drawString(levelNumber, 42, 80);
			baseSprite->drawNumber(level, 80, 80);
			break;
		case 7:
			baseSprite->drawIcon(planet7, 42, 20, 45, 45, 1, TFT_BLACK);
			baseSprite->drawString(levelNumber, 42, 80);
			baseSprite->drawNumber(level, 80, 80);
			break;
		case 8:
			baseSprite->drawIcon(planet8, 42, 20, 45, 45, 1, TFT_BLACK);
			baseSprite->drawString(levelNumber, 42, 80);
			baseSprite->drawNumber(level, 80, 80);
			break;
		case 9:
			baseSprite->drawIcon(planet9, 42, 20, 45, 45, 1, TFT_BLACK);
			baseSprite->drawString(levelNumber, 42, 80);
			baseSprite->drawNumber(level, 80, 80);
			break;
		case 10:
			break;
		default:
			break;

	}

}

void GameState::drawLevelEnd(){

	switch(level){

		case 1:
			baseSprite->drawIcon(planet2, lvlEndPlanetX, lvlEndPlanetY, 45, 45, 1, TFT_BLACK);
			break;
		case 2:
			baseSprite->drawIcon(planet3, lvlEndPlanetX, lvlEndPlanetY, 45, 45, 1, TFT_BLACK);
			break;
		case 3:
			baseSprite->drawIcon(planet4, lvlEndPlanetX, lvlEndPlanetY, 45, 45, 1, TFT_BLACK);
			break;
		case 4:
			baseSprite->drawIcon(planet5, lvlEndPlanetX, lvlEndPlanetY, 45, 45, 1, TFT_BLACK);
			break;
		case 5:
			baseSprite->drawIcon(planet6, lvlEndPlanetX, lvlEndPlanetY, 45, 45, 1, TFT_BLACK);
			break;
		case 6:
			baseSprite->drawIcon(planet7, lvlEndPlanetX, lvlEndPlanetY, 45, 45, 1, TFT_BLACK);
			break;
		case 7:
			baseSprite->drawIcon(planet8, lvlEndPlanetX, lvlEndPlanetY, 45, 45, 1, TFT_BLACK);
			break;
		case 8:
			baseSprite->drawIcon(planet9, lvlEndPlanetX, lvlEndPlanetY, 45, 45, 1, TFT_BLACK);
			break;
		case 9:
			break;

		default:
			break;

	}

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
