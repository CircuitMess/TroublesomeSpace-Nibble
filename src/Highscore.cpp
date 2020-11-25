
//
// Created by Domagoj on 13/11/2020.
//

#include "Highscore.h"
#include "GameState.h"
#include "GameOverState.h"
#include <FS.h>
#include <LittleFS.h>

Highscore *Highscore::instance = nullptr;

Highscore::Highscore(uint score, bool _previousState){

	display = Nibble.getDisplay();
	baseSprite = display->getBaseSprite();

	instance = this;

	previousState = _previousState;

	_score.score = score;
	charCursor = 0;
}

void Highscore::enter(Game &_game){

	game = &_game;

	upState = false;
	downState = false;
	aState = false;
	bState = false;

	for(char &i : _score.name)
		i = 'A';

	loadData();

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

void Highscore::exit(){

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

void Highscore::loop(uint){

	if(previousState){                // Game over -> Highscore

		baseSprite->clear(TFT_BLACK);

		draw();

		baseSprite->setTextSize(1);
		baseSprite->setTextFont(1);
		baseSprite->setTextColor(TFT_LIGHTGREY);
		baseSprite->drawString(confirfmMessage, 1, 120);

		display->commit();
	}else{                            // Menu -> Highscore

		baseSprite->clear(TFT_BLACK);

		draw();

		baseSprite->setTextSize(1);
		baseSprite->setTextFont(1);
		baseSprite->setTextColor(TFT_LIGHTGREY);
		baseSprite->drawString(deleteMessage, 1, 120);

		display->commit();
	}

	states();
}

void Highscore::states(){

	if(previousState){    // previous = game over

		if(upState){

			(_score.name[charCursor])--;

			upState = false;

		}else if(downState){

			_score.name[charCursor]++;

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

				if(addScore && previousState){
					addData(_score);
					addScore = false;
				}

			}

			aState = false;

		}else if(bState){

			baseSprite->clear(TFT_BLACK);
			game->changeState(new GameOverState(L, _score.score, true));
			display->commit();

		}

	}else{                // previous = menu

		if(aState){

			clearData();
			aState = false;
		}

		if(bState){

			baseSprite->clear(TFT_BLACK);
			game->changeState(new Menu());
			display->commit();

		}
	}
}

void Highscore::draw(){

	baseSprite->drawLine(40, 40, 70, 40, TFT_GOLD);
	baseSprite->drawLine(80, 40, 100, 40, TFT_GOLD);

	baseSprite->drawLine(40, 55, 70, 55, TFT_GOLD);
	baseSprite->drawLine(80, 55, 100, 55, TFT_GOLD);

	baseSprite->drawLine(40, 70, 70, 70, TFT_GOLD);
	baseSprite->drawLine(80, 70, 100, 70, TFT_GOLD);

	//baseSprite->drawLine(40, 85, 70, 85, TFT_GOLD);
	//baseSprite->drawLine(80, 85, 100, 85, TFT_GOLD);

	baseSprite->setTextSize(1);
	baseSprite->setTextFont(2);
	baseSprite->setTextColor(TFT_WHITE);
	baseSprite->drawString(highscore, 30, 10);

	baseSprite->setTextFont(1);
	baseSprite->drawNumber(1, 30, 30);
	baseSprite->drawNumber(2, 30, 45);
	baseSprite->drawNumber(3, 30, 60);

	baseSprite->setTextSize(1);
	baseSprite->setTextFont(1);
	baseSprite->setTextColor(TFT_LIGHTGREY);
	baseSprite->drawString(backMessage, 80, 120);

	if(previousState){

		if(charCursor >= 0)
			baseSprite->drawChar(_score.name[0], 40, 90);
		if(charCursor >= 1)
			baseSprite->drawChar(_score.name[1], 45, 90);
		if(charCursor >= 2)
			baseSprite->drawChar(_score.name[2], 50, 90);
		if(charCursor >= 3)
			baseSprite->drawChar(_score.name[3], 55, 90);
		if(charCursor >= 4)
			baseSprite->drawChar(_score.name[4], 60, 90);
		baseSprite->drawNumber(_score.score, 85, 90);

		baseSprite->drawLine(40, 100, 70, 100, TFT_GOLD);
		baseSprite->drawLine(80, 100, 100, 100, TFT_GOLD);
	}

	baseSprite->drawString(data.scores[0].name, 40, 30);
	baseSprite->drawNumber(data.scores[0].score, 85, 30);

	baseSprite->drawString(data.scores[1].name, 40, 45);
	baseSprite->drawNumber(data.scores[1].score, 85, 45);

	baseSprite->drawString(data.scores[2].name, 40, 60);
	baseSprite->drawNumber(data.scores[2].score, 85, 60);


}

void Highscore::begin(){

	bool exist = LittleFS.exists(FILENAME);
	if(!exist){
		data.count = 0;
		saveData();
	}else{
		loadData();
	}
}

void Highscore::addData(Score &score){

	if(data.count == MAX_SCORE_PLAYERS && score.score < data.scores[data.count - 1].score)
		return;

	uint8_t i;

	for(i = 0; i < data.count; i++){

		if(_score.score <= data.scores[i].score)
			continue;

		uint8_t firstToMove = i;
		uint8_t numberOfDataToMove = data.count - i;
		if(data.count == MAX_SCORE_PLAYERS)
			numberOfDataToMove--;

		for(int j = firstToMove + numberOfDataToMove; j > firstToMove; j--)
			memcpy(&data.scores[j], &data.scores[j - 1], sizeof(Score));

		break;
	}

	if(i == MAX_SCORE_PLAYERS)
		return;

	memcpy(&data.scores[i], &score, sizeof(Score));
	data.count = min(MAX_SCORE_PLAYERS, data.count + 1);
	saveData();

}

void Highscore::clearData(){

	data.count = 0;
	saveData();
}

void Highscore::saveData(){

	File file = LittleFS.open(FILENAME, "w");
	file.write((byte *) &data, sizeof(Data));
	file.close();
}

void Highscore::loadData(){

	File file = LittleFS.open(FILENAME, "r");
	file.readBytes((char *) &data, sizeof(Data));
	file.close();
}

const Score &Highscore::get(uint8_t i){

	return data.scores[i];
}

uint8_t Highscore::dataCount(){

	return data.count;
}

void Highscore::buttonUpPressed(){

	instance->upState = true;
}

void Highscore::buttonDownPressed(){

	instance->downState = true;
}

void Highscore::buttonLeftPressed(){

	instance->leftState = true;
}

void Highscore::buttonRightPressed(){

	instance->rightState = true;
}

void Highscore::buttonAPressed(){

	instance->aState = true;
}

void Highscore::buttonBPressed(){

	instance->bState = true;
}

void Highscore::buttonUpReleased(){

	instance->upState = false;
}

void Highscore::buttonDownReleased(){

	instance->downState = false;
}

void Highscore::buttonLeftReleased(){

	instance->leftState = false;
}

void Highscore::buttonRightReleased(){

	instance->rightState = false;
}

void Highscore::buttonAReleased(){

	instance->aState = false;
}

void Highscore::buttonBReleased(){

	instance->bState = false;
}

