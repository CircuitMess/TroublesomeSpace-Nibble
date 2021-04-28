#include "EnterHighscoreState.h"
#include "ShowHighscoreState.h"
#include "Highscore/Highscore.h"

TroublesomeSpace::EnterHighscoreState *TroublesomeSpace::EnterHighscoreState::instance = nullptr;

TroublesomeSpace::EnterHighscoreState::EnterHighscoreState(Display &_display, uint _score){

	display = &_display;
	baseSprite = display->getBaseSprite();

	instance = this;

	score = _score;

	charCursor = 0;
	name = new char[5];

	strcpy(name, "AAAAA\0");
}

void TroublesomeSpace::EnterHighscoreState::enter(Game &_game){

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

void TroublesomeSpace::EnterHighscoreState::exit(){

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

void TroublesomeSpace::EnterHighscoreState::loop(uint){

	states();

	baseSprite->clear(TFT_BLACK);
	baseSprite->setTextFont(2);

	if(score > Highscore.get(0).score || Highscore.dataCount() <= 0){
		baseSprite->setCursor(25, 30);
		baseSprite->printCenter("NEW HIGHSCORE!");
		baseSprite->setCursor(25, 50);
		baseSprite->printCenter(score);
	}else{
		baseSprite->setCursor(25, 30);
		baseSprite->printCenter("SCORE");
		baseSprite->setCursor(25, 50);
		baseSprite->printCenter(score);
	}

	draw();

	display->commit();

}

void TroublesomeSpace::EnterHighscoreState::states(){

	if(upState){

		if(name[charCursor] != 'A' && (millis()-previousCursorTime)>100){
			name[charCursor]--;
			previousCursorTime = millis();
		}

		downState = false;
	}
	if(downState){

		if(name[charCursor] != 'Z' && (millis()-previousCursorTime)>100){
			name[charCursor]++;
			previousCursorTime = millis();
		}

		upState = false;
	}
	if(leftState){

		if(charCursor > 0)
			charCursor--;

		leftState = false;
	}

	if(aState){

		charCursor++;

		if(charCursor > 4){

			Score newScore;
			newScore.score = score;
			strcpy(newScore.name, name);

			Highscore.addData(newScore);
			game->changeState(new ShowHighscoreState(*display));
		}
		aState = false;
	}
}

void TroublesomeSpace::EnterHighscoreState::draw(){

	baseSprite->drawLine(50, 100, 82, 100, TFT_GOLD);

	baseSprite->setTextSize(1);
	baseSprite->setTextFont(1);
	baseSprite->setTextColor(TFT_LIGHTGREY);

	baseSprite->setCursor(10,90);
	baseSprite->drawString(dispName,10,92);

	if(charCursor >= 0){
		baseSprite->drawChar(name[0], 50, 90);
		baseSprite->drawLine(50, 98, 54, 98, TFT_LIGHTGREY);
	}
	if(charCursor >= 1){
		baseSprite->drawChar(name[1], 57, 90);
		baseSprite->drawLine(57, 98, 61, 98, TFT_LIGHTGREY);
	}
	if(charCursor >= 2){
		baseSprite->drawChar(name[2], 64, 90);
		baseSprite->drawLine(64, 98, 68, 98, TFT_LIGHTGREY);
	}
	if(charCursor >= 3){
		baseSprite->drawChar(name[3], 71, 90);
		baseSprite->drawLine(71, 98, 75, 98, TFT_LIGHTGREY);
	}
	if(charCursor >= 4){
		baseSprite->drawChar(name[4], 78, 90);
		baseSprite->drawLine(78, 98, 82, 98, TFT_LIGHTGREY);
	}

}

void TroublesomeSpace::EnterHighscoreState::buttonUpPressed(){

	instance->upState = true;
}

void TroublesomeSpace::EnterHighscoreState::buttonDownPressed(){

	instance->downState = true;
}

void TroublesomeSpace::EnterHighscoreState::buttonLeftPressed(){

	instance->leftState = true;
}

void TroublesomeSpace::EnterHighscoreState::buttonRightPressed(){

	instance->rightState = true;
}

void TroublesomeSpace::EnterHighscoreState::buttonAPressed(){

	instance->aState = true;
}

void TroublesomeSpace::EnterHighscoreState::buttonBPressed(){

	instance->bState = true;
}

void TroublesomeSpace::EnterHighscoreState::buttonUpReleased(){

	instance->upState = false;
}

void TroublesomeSpace::EnterHighscoreState::buttonDownReleased(){

	instance->downState = false;
}

void TroublesomeSpace::EnterHighscoreState::buttonLeftReleased(){

	instance->leftState = false;
}

void TroublesomeSpace::EnterHighscoreState::buttonRightReleased(){

	instance->rightState = false;
}

void TroublesomeSpace::EnterHighscoreState::buttonAReleased(){

	instance->aState = false;
}

void TroublesomeSpace::EnterHighscoreState::buttonBReleased(){

	instance->bState = false;
}