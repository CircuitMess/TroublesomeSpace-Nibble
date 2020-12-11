//
// Created by Domagoj on 11/11/2020.
//

#include "Menu.h"
#include "GameState.h"
#include "ShowHighscoreState.h"
#include "bitmaps/homescreen.hpp"

Menu *Menu::instance = nullptr;

Menu::Menu(){

	display = Nibble.getDisplay();
	baseSprite = display->getBaseSprite();

	instance = this;

	melodyTime = Melody.playMelody(MENU, false);

	baseSprite->clear(TFT_BLACK);
	drawMenu();
	display->commit();
}

void Menu::enter(Game &_game){

	game = &_game;

	upState = false;
	downState = false;
	aState = false;

	pointer = {30, 92, 55, 14};

	Input::getInstance()->setBtnPressCallback(BTN_UP, buttonUpPressed);
	Input::getInstance()->setBtnPressCallback(BTN_DOWN, buttonDownPressed);
	Input::getInstance()->setBtnPressCallback(BTN_A, buttonAPressed);
	Input::getInstance()->setBtnReleaseCallback(BTN_UP, buttonUpReleased);
	Input::getInstance()->setBtnReleaseCallback(BTN_DOWN, buttonDownReleased);
	Input::getInstance()->setBtnReleaseCallback(BTN_A, buttonAReleased);

}

void Menu::exit(){

	melodyTime = 0;
	Input::getInstance()->removeBtnPressCallback(BTN_UP);
	Input::getInstance()->removeBtnPressCallback(BTN_DOWN);
	Input::getInstance()->removeBtnPressCallback(BTN_A);
	Input::getInstance()->removeBtnReleaseCallback(BTN_UP);
	Input::getInstance()->removeBtnReleaseCallback(BTN_DOWN);
	Input::getInstance()->removeBtnReleaseCallback(BTN_A);
}


void Menu::loop(uint t){

	baseSprite->clear(TFT_BLACK);
	drawMenu();
	display->commit();

	states();
}

void Menu::states(){

	if(upState)
		pointer = {30, 92, 55, 14};
	else if(downState)
		pointer = {25, 107, 70, 14};

	if(aState && pointer.y == 92)
		game->changeState(new GameState());
	if(aState && pointer.y == 107)
		game->changeState(new ShowHighscoreState());
}

void Menu::drawMenu(){

	baseSprite->drawIcon(homescreen,0,0,128,128);

	baseSprite->setTextSize(1);
	baseSprite->setTextFont(1);
	baseSprite->setTextColor(TFT_WHITE);
	baseSprite->drawString(newGame, 35, 95);
	baseSprite->drawString(highScore, 30, 110);

	baseSprite->drawRect(pointer.x, pointer.y, pointer.width, pointer.height, TFT_GOLD);

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