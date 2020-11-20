//
// Created by Domagoj on 13/11/2020.
//

#include "Highscore.h"
#include "GameOverState.h"
#include <FS.h>
#include <LittleFS.h>

Highscore *Highscore::instance = nullptr;

Highscore::Highscore(int _score, bool _previousState, Melody *mel){

	display = Nibble.getDisplay();
	baseSprite = display->getBaseSprite();

	instance = this;

	melody = mel;

	previousState = _previousState;

	begin();

	score.score = _score;

	if(score.score == 0 && previousState){
		addData(score);
	}
}

void Highscore::enter(Game &_game){

	game = &_game;

	upState = false;
	downState = false;
	aState = false;
	bState = false;

	Input::getInstance()->setBtnPressCallback(BTN_UP, buttonUpPressed);
	Input::getInstance()->setBtnPressCallback(BTN_DOWN, buttonDownPressed);
	Input::getInstance()->setBtnPressCallback(BTN_A, buttonAPressed);
	Input::getInstance()->setBtnPressCallback(BTN_B, buttonBPressed);

	Input::getInstance()->setBtnReleaseCallback(BTN_UP, buttonUpReleased);
	Input::getInstance()->setBtnReleaseCallback(BTN_DOWN, buttonDownReleased);
	Input::getInstance()->setBtnReleaseCallback(BTN_A, buttonAReleased);
	Input::getInstance()->setBtnReleaseCallback(BTN_B, buttonBReleased);

}

void Highscore::exit(){


	Input::getInstance()->removeBtnPressCallback(BTN_UP);
	Input::getInstance()->removeBtnPressCallback(BTN_DOWN);
	Input::getInstance()->removeBtnPressCallback(BTN_A);
	Input::getInstance()->removeBtnPressCallback(BTN_B);

	Input::getInstance()->removeBtnReleaseCallback(BTN_UP);
	Input::getInstance()->removeBtnReleaseCallback(BTN_DOWN);
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

	if(upState){

	}else if(downState){

	}else if(aState){

		clearData();

	}else if(bState){
		if(previousState){

			baseSprite->clear(TFT_BLACK);
			game->changeState(new GameOverState(L, melody, score.score, true));
			display->commit();

		}else{

			baseSprite->clear(TFT_BLACK);
			game->changeState(new Menu(melody));
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

	baseSprite->drawLine(40, 85, 70, 85, TFT_GOLD);
	baseSprite->drawLine(80, 85, 100, 85, TFT_GOLD);

	baseSprite->drawLine(40, 100, 70, 100, TFT_GOLD);
	baseSprite->drawLine(80, 100, 100, 100, TFT_GOLD);

	baseSprite->setTextSize(1);
	baseSprite->setTextFont(2);
	baseSprite->setTextColor(TFT_WHITE);
	baseSprite->drawString(highscore, 30, 10);

	baseSprite->setTextFont(1);
	baseSprite->drawNumber(1, 30, 35);
	baseSprite->drawNumber(2, 30, 50);
	baseSprite->drawNumber(3, 30, 65);
	baseSprite->drawNumber(4, 30, 80);
	baseSprite->drawNumber(5, 30, 95);

	baseSprite->setTextSize(1);
	baseSprite->setTextFont(1);
	baseSprite->setTextColor(TFT_LIGHTGREY);
	baseSprite->drawString(backMessage, 80, 120);
}

void Highscore::begin(){

	if(!LittleFS.exists(FILENAME)){
		data.count = 0;
		saveData();
	}else
		loadData();
}

void Highscore::addData(Score &_score){

	if(data.count == MAX_SCORE_PLAYERS && score.score < data.scores[data.count - 1].score)
		return;

	uint8_t i;

	for(i = 0; i < data.count; i++){

		if(score.score <= data.scores[i].score)
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

void Highscore::buttonAReleased(){

	instance->aState = false;
}

void Highscore::buttonBReleased(){

	instance->bState = false;
}

