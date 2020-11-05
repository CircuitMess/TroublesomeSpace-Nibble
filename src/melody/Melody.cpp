//
// Created by Domagoj on 03/11/2020.
//

#include "Melody.h"
#include "StartUpMelody.h"
#include "VictoryMelody.h"
#include "GameOverMelody.h"
#include "LoopMelody.h"


Melody::Melody(){

	Piezo.setMute(false);

	loopMelodySize = sizeof(loopMelody) / sizeof(loopMelody[0]);
	loopMelodyNote = loopMelody;
}

void Melody::loop(uint t){


	if(!loopCheck){

		oneTimePLay();
		stop();

	}
	else{
		loopPlay();
	}

}

void Melody::playMelody(mel melody, bool loop){

	loopCheck = loop;

	switch(melody){

		case START:
			melodySize = sizeof(startUpMelody) / sizeof(startUpMelody[0]);
			melodyNote = startUpMelody;
			break;

		case VICTORY:
			melodySize = sizeof(victoryMelody) / sizeof(victoryMelody[0]);
			melodyNote = victoryMelody;
			break;

		case LOSE:
			melodySize = sizeof(gameOverMelody) / sizeof(gameOverMelody[0]);
			melodyNote = gameOverMelody;
			break;

		case LOOP:
			melodySize = sizeof(loopMelody) / sizeof(loopMelody[0]);
			melodyNote = loopMelody;
			break;

		default:
			return;
	}

}

void Melody::oneTimePLay(){

	for(int i = 0; i < melodySize; i++){

		Piezo.tone((melodyNote + i)->note, (melodyNote + i)->duration);

		delay((melodyNote + i)->duration);

		Piezo.noTone();
	}
}

void Melody::loopPlay(){

	if(noteNum >= loopMelodySize)
		noteNum = 0;

	Piezo.tone((loopMelodyNote + noteNum)->note, (loopMelodyNote + noteNum)->duration);

	unsigned long currentMillis = millis();

	if(currentMillis - previousMillis > (loopMelodyNote + noteNum)->duration){

		previousMillis = currentMillis;
		noteNum++;
	}

}

void Melody::stop(){

	melodySize = 0;
	melodyNote = NULL;
	noteNum = 0;
	loopCheck = true;

}

