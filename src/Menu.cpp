//
// Created by Domagoj on 11/11/2020.
//

#include "Menu.h"
#include "GameState.h"
#include "ShowHighscoreState.h"
#include "EraseHighscoreState.h"
#include "bitmaps/states/homescreen.hpp"

Menu *Menu::instance = nullptr;

Menu::Menu(){

	display = Nibble.getDisplay();
	baseSprite = display->getBaseSprite();

	instance = this;

	melodyTime = Melody.playMelody(MENU, true);

	baseSprite->clear(TFT_BLACK);
	draw();
	display->commit();
}

void Menu::enter(Game &_game){

	game = &_game;

	upState = false;
	downState = false;
	aState = false;

	menuOrder = 1;

	Input::getInstance()->setBtnPressCallback(BTN_UP, buttonUpPressed);
	Input::getInstance()->setBtnPressCallback(BTN_DOWN, buttonDownPressed);
	Input::getInstance()->setBtnPressCallback(BTN_A, buttonAPressed);
	Input::getInstance()->setBtnReleaseCallback(BTN_UP, buttonUpReleased);
	Input::getInstance()->setBtnReleaseCallback(BTN_DOWN, buttonDownReleased);
	Input::getInstance()->setBtnReleaseCallback(BTN_A, buttonAReleased);

}

void Menu::exit(){

	Input::getInstance()->removeBtnPressCallback(BTN_UP);
	Input::getInstance()->removeBtnPressCallback(BTN_DOWN);
	Input::getInstance()->removeBtnPressCallback(BTN_A);
	Input::getInstance()->removeBtnReleaseCallback(BTN_UP);
	Input::getInstance()->removeBtnReleaseCallback(BTN_DOWN);
	Input::getInstance()->removeBtnReleaseCallback(BTN_A);
}

void Menu::loop(uint t){

	baseSprite->clear(TFT_BLACK);
	draw();
	display->commit();

	states();
}

void Menu::states(){

	if(instance->upState){
		if(menuOrder == 2)
			menuOrder = 1;
		else if (menuOrder == 3)
			menuOrder = 2;
		else if (menuOrder == 4)
			menuOrder = 3;

		step = 0;
		previousArrowTime = millis();
		instance->upState = false;
	}
	if(instance->downState){
		if(menuOrder == 1)
			menuOrder = 2;
		else if (menuOrder == 2)
			menuOrder = 3;
		else if (menuOrder == 3)
			menuOrder = 4;

		step = 0;
		previousArrowTime = millis();
		instance->downState =false;
	}

	if(millis() - previousArrowTime > 200){
		step++;
		if(step > 2)
			step = 0;
		previousArrowTime = millis();
	}

	if(aState && menuOrder == 1){
		game->changeState(new GameState());
	}
	if(aState && menuOrder == 2){
		Melody.playMelody(STOP,false);
		game->changeState(new ShowHighscoreState());
	}
	if(aState && menuOrder == 3){
		game->changeState(new EraseHighscoreState());
	}
	if(aState && menuOrder == 4){

	}
}

void Menu::draw(){

	baseSprite->drawIcon(homescreen, 0, 0, 128, 128);

	baseSprite->setTextSize(1);
	baseSprite->setTextFont(1);
	baseSprite->setTextColor(TFT_WHITE);

	if(menuOrder == 1){
		baseSprite->drawString(newGame, 40, 105);
	}
	if(menuOrder == 2){
		baseSprite->drawString(highScore, 35, 105);
	}
	if(menuOrder == 3){
		baseSprite->drawString(eraseHighscore, 20, 105);
	}
	if(menuOrder == 4){
		baseSprite->drawString(info, 52, 105);
	}

	drawTriangleArrows();
}

void Menu::drawTriangleArrows(){

	if(menuOrder == 1){
		baseSprite->fillTriangle(64, 120 + step, 59, 115 + step, 69, 115 + step, TFT_LIGHTGREY);
	}
	if(menuOrder == 2){
		baseSprite->fillTriangle(64, 120 + step, 59, 115 + step, 69, 115 + step, TFT_LIGHTGREY);
		baseSprite->fillTriangle(64, 95 - step, 59, 100 - step, 69, 100 - step, TFT_LIGHTGREY);
	}
	if(menuOrder == 3){
		baseSprite->fillTriangle(64, 120 + step, 59, 115 + step, 69, 115 + step, TFT_LIGHTGREY);
		baseSprite->fillTriangle(64, 95 - step, 59, 100 - step, 69, 100 - step, TFT_LIGHTGREY);
	}
	if(menuOrder == 4){
		baseSprite->fillTriangle(64, 95 - step, 59, 100 - step, 69, 100 - step, TFT_LIGHTGREY);
	}
}

void Menu::buttonUpPressed(){
	instance->upState = true;
}

void Menu::buttonDownPressed(){
	instance->downState = true;
}

void Menu::buttonAPressed(){
	instance->aState = true;
}

void Menu::buttonUpReleased(){
	instance->upState = false;
}

void Menu::buttonDownReleased(){
	instance->downState = false;
}

void Menu::buttonAReleased(){
	instance->aState = false;
}

