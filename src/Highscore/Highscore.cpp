#include "Highscore.h"
#include <FS.h>
#include <LittleFS.h>

HighscoreImpl Highscore;

HighscoreImpl::HighscoreImpl(){

}

void HighscoreImpl::begin(){

	if(!LittleFS.begin()){
		Serial.println("LittleFS begin error");
	}

	bool exist = LittleFS.exists(FILENAME);
	if(!exist){
		data.count = 0;
		saveData();
	}else{
		loadData();
	}
}

void HighscoreImpl::addData(Score &score){

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

void HighscoreImpl::clearData(){

	data.count = 0;
	saveData();
}

void HighscoreImpl::saveData(){

	File file = LittleFS.open(FILENAME, "w");
	file.write((byte *) &data, sizeof(Data));
	file.close();
}

void HighscoreImpl::loadData(){

	File file = LittleFS.open(FILENAME, "r");
	file.readBytes((char *) &data, sizeof(Data));
	file.close();
}

const Score &HighscoreImpl::get(uint8_t i){

	return data.scores[i];
}

uint8_t HighscoreImpl::dataCount(){

	return data.count;
}