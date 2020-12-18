//
// Created by Domagoj on 11/11/2020.
//

#include "Menu.h"
#include "GameState.h"
#include "ShowHighscoreState.h"
#include "bitmaps/states/homescreen.hpp"

Menu *Menu::instance = nullptr;

Menu::Menu(){

	display = Nibble.getDisplay();
	baseSprite = display->getBaseSprite();

	instance = this;

	melodyTime = Melody.playMelody(MENU, false);

	baseSprite->clear(TFT_BLACK);
	draw();
	display->commit();
}

void Menu::enter(Game &_game){

	game = &_game;

	upState = false;
	downState = false;
	aState = false;

	Input::getInstance()->setBtnPressCallback(BTN_UP, buttonUpPressed);
	Input::getInstance()->setBtnPressCallback(BTN_DOWN, buttonDownPressed);
	Input::getInstance()->setBtnPressCallback(BTN_A, buttonAPressed);
	Input::getInstance()->setBtnReleaseCallback(BTN_UP, buttonUpReleased);
	Input::getInstance()->setBtnReleaseCallback(BTN_DOWN, buttonDownReleased);
	Input::getInstance()->setBtnReleaseCallback(BTN_A, buttonAReleased);

}

void Menu::exit(){

	newGameOption = true;
	highscoreOption = false;

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

	if(upState){
		newGameOption = true;
		highscoreOption = false;
	}
	if(downState){
		newGameOption = false;
		highscoreOption = true;
	}


	if(aState && newGameOption)
		game->changeState(new GameState());
	if(aState && highscoreOption)
		game->changeState(new ShowHighscoreState());
}

void Menu::draw(){

	baseSprite->drawIcon(homescreen, 0, 0, 128, 128);

	baseSprite->setTextSize(1);
	baseSprite->setTextFont(1);
	baseSprite->setTextColor(TFT_WHITE);

	if(newGameOption){
		baseSprite->drawString(newGame, 40, 105);
		baseSprite->drawTriangle(64, 120, 59, 115, 69, 115, TFT_LIGHTGREY);
	}
	if(highscoreOption){
		baseSprite->drawString(highScore, 35, 105);
		baseSprite->drawTriangle(64, 95, 59, 100, 69, 100, TFT_LIGHTGREY);
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