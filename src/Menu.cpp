//
// Created by Domagoj on 11/11/2020.
//

#include "Menu.h"
#include "GameState.h"

Menu *Menu::instance = nullptr;

Menu::Menu(Melody *mel){

	display = Nibble.getDisplay();
	baseSprite = display->getBaseSprite();

	instance = this;

	melody = mel;
	melodyTime = melody->playMelody(MENU, false);
}

void Menu::loop(uint t){

	baseSprite->clear(TFT_BLACK);

	drawMenu();
	states();

	display->commit();
}

void Menu::enter(Game &_game){

	game = &_game;

	pointer = {30, 65, 70, 30};

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

void Menu::states(){

	if(upState)
		pointer = {30, 70, 70, 20};
	else if(downState)
		pointer = {25, 90, 80, 20};

	if(aState)
		game->changeState(new GameState(melody));

}

void Menu::drawMenu(){

	baseSprite->setTextSize(1);
	baseSprite->setTextFont(2);
	baseSprite->setTextColor(TFT_GOLD);
	baseSprite->drawString(title, 10, 20);

	baseSprite->setTextSize(1);
	baseSprite->setTextFont(2);
	baseSprite->setTextColor(TFT_WHITE);
	baseSprite->drawString(newGame, 35, 70);
	baseSprite->drawString(highScore, 30, 90);

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