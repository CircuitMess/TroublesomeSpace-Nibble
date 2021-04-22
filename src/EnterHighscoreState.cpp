#include "EnterHighscoreState.h"
#include "ShowHighscoreState.h"
#include "Highscore/Highscore.h"

EnterHighscoreState *EnterHighscoreState::instance = nullptr;

EnterHighscoreState::EnterHighscoreState(uint _score){

	display = Nibble.getDisplay();
	baseSprite = display->getBaseSprite();

	instance = this;

	score = _score;
}

void EnterHighscoreState::enter(Game &_game){

	game = &_game;

	charCursor = 0;
	name = new char[5];

	strcpy(name, "AAAAA\0");

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

void EnterHighscoreState::exit(){

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

	free(name);
}

void EnterHighscoreState::loop(uint){

	states();

	baseSprite->clear(TFT_BLACK);

	if(score > Highscore.get(0).score){
		baseSprite->setCursor(25, 55);
		baseSprite->printf("New Highscore !");
		baseSprite->setCursor(60, 70);
		baseSprite->printf("%d", score);
	}else{
		baseSprite->setCursor(25, 50);
		baseSprite->printf("SCORE: %d", score);
	}

	draw();

	display->commit();

}

void EnterHighscoreState::states(){

	if(upState){

		name[charCursor]--;

		upState = false;

	}else if(downState){

		name[charCursor]++;

		downState = false;

	}else if(leftState){

		if(charCursor > 0)
			charCursor--;

		leftState = false;

	}else if(rightState){

		rightState = false;

	}else if(aState){

		charCursor++;

		if(charCursor > 4){

			Score newScore;
			newScore.score = score;
			strcpy(newScore.name, name);

			Highscore.addData(newScore);
			game->changeState(new ShowHighscoreState());
		}

		aState = false;
	}
}

void EnterHighscoreState::draw(){

	baseSprite->drawLine(40, 100, 70, 100, TFT_GOLD);
	baseSprite->drawLine(80, 100, 100, 100, TFT_GOLD);

	baseSprite->setTextSize(1);
	baseSprite->setTextFont(1);
	baseSprite->setTextColor(TFT_LIGHTGREY);

	if(charCursor >= 0)
		baseSprite->drawChar(name[0], 45, 90);
	if(charCursor >= 1)
		baseSprite->drawChar(name[1], 50, 90);
	if(charCursor >= 2)
		baseSprite->drawChar(name[2], 55, 90);
	if(charCursor >= 3)
		baseSprite->drawChar(name[3], 60, 90);
	if(charCursor >= 4)
		baseSprite->drawChar(name[4], 65, 90);

	baseSprite->drawNumber(score, 85, 90);


}

void EnterHighscoreState::buttonUpPressed(){

	instance->upState = true;
}

void EnterHighscoreState::buttonDownPressed(){

	instance->downState = true;
}

void EnterHighscoreState::buttonLeftPressed(){

	instance->leftState = true;
}

void EnterHighscoreState::buttonRightPressed(){

	instance->rightState = true;
}

void EnterHighscoreState::buttonAPressed(){

	instance->aState = true;
}

void EnterHighscoreState::buttonBPressed(){

	instance->bState = true;
}

void EnterHighscoreState::buttonUpReleased(){

	instance->upState = false;
}

void EnterHighscoreState::buttonDownReleased(){

	instance->downState = false;
}

void EnterHighscoreState::buttonLeftReleased(){

	instance->leftState = false;
}

void EnterHighscoreState::buttonRightReleased(){

	instance->rightState = false;
}

void EnterHighscoreState::buttonAReleased(){

	instance->aState = false;
}

void EnterHighscoreState::buttonBReleased(){

	instance->bState = false;
}