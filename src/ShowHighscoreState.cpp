#include "ShowHighscoreState.h"
#include "GameState.h"
#include "Highscore/Highscore.h"
#include "bitmaps/states/highscore.hpp"


ShowHighscoreState *ShowHighscoreState::instance = nullptr;

ShowHighscoreState::ShowHighscoreState(){

	display = Nibble.getDisplay();
	baseSprite = display->getBaseSprite();

	instance = this;

}

void ShowHighscoreState::enter(Game &_game){

	game = &_game;

	bState = false;

	baseSprite->clear(TFT_BLACK);

	Input::getInstance()->setBtnPressCallback(BTN_B, buttonBPressed);
	Input::getInstance()->setBtnReleaseCallback(BTN_B, buttonBReleased);
}

void ShowHighscoreState::exit(){

	Input::getInstance()->removeBtnPressCallback(BTN_B);
	Input::getInstance()->removeBtnReleaseCallback(BTN_B);
}

void ShowHighscoreState::loop(uint){

	draw();

	display->commit();

	baseSprite->clear(TFT_BLACK);

	if(bState)
		game->changeState(new Menu());

	if(millis() - previousArrowTime > 200){
		step++;
		if(step > 2)
			step = 0;
		previousArrowTime = millis();
	}

}

void ShowHighscoreState::draw(){

	baseSprite->drawIcon(highscoreScreen,0,0,128,128);

	baseSprite->setTextSize(1);
	baseSprite->setTextFont(1);
	baseSprite->setTextColor(TFT_LIGHTGREY);
	baseSprite->drawString(back, 10, 2);

	baseSprite->fillTriangle(3-step, 5, 8-step, 2, 8-step, 8, TFT_LIGHTGREY);

	for(int i = 0; i < 5; i++){

		baseSprite->setCursor(30, 37+i*16);

		if((i+1) <= Highscore.dataCount()){
			baseSprite->printf(" %s   %d", Highscore.get(i).name, Highscore.get(i).score);
		}else{
			baseSprite->printf("   -     -  ");
		}
	}
}

void ShowHighscoreState::buttonBPressed(){

	instance->bState = true;
}

void ShowHighscoreState::buttonBReleased(){

	instance->bState = false;
}
