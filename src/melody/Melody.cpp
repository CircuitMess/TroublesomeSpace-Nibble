//
// Created by Domagoj on 03/11/2020.
//

#include "Melody.h"
#include "StartUpMelody.h"
#include "VictoryMelody.h"
#include "GameOverMelody.h"
#include "LoopMelody.h"
#include "MenuMelody.h"


MelodyImpl Melody;

void MelodyImpl::loop(uint t){

	if(!loopCheck){

		play(false);
		stop();

	}else{

		play(true);
	}

}

uint MelodyImpl::playMelody(mel melody, bool loop){

	loopCheck = loop;

	switch(melody){

		case MENU:
			melodySize = sizeof(menuMelody) / sizeof(menuMelody[0]);
			melodyNote = menuMelody;
			for(int i = 0; i < melodySize; ++i)
				melodyTime += menuMelody[i].duration;
			break;

		case START:
			melodySize = sizeof(startUpMelody) / sizeof(startUpMelody[0]);
			melodyNote = startUpMelody;
			for(int i = 0; i < melodySize; ++i)
				melodyTime += startUpMelody[i].duration;
			break;

		case VICTORY:
			melodySize = sizeof(victoryMelody) / sizeof(victoryMelody[0]);
			melodyNote = victoryMelody;
			for(int i = 0; i < melodySize; ++i)
				melodyTime += victoryMelody[i].duration;
			break;

		case LOSE:
			melodySize = sizeof(gameOverMelody) / sizeof(gameOverMelody[0]);
			melodyNote = gameOverMelody;
			for(int i = 0; i < melodySize; ++i)
				melodyTime += gameOverMelody[i].duration;

			break;

		case LOOP:
			melodySize = sizeof(loopMelody) / sizeof(loopMelody[0]);
			melodyNote = loopMelody;
			break;

		default:
			return 0;
	}

	return melodyTime;

}

void MelodyImpl::play(bool loop){
	if(loop){

		if(noteNum >= melodySize)
			noteNum = 0;

		Piezo.tone((melodyNote + noteNum)->note, (melodyNote + noteNum)->duration);

		unsigned long currentMillis = millis();

		if(currentMillis - previousMillis > (melodyNote + noteNum)->duration){

			previousMillis = currentMillis;
			noteNum++;
		}
	}else{

		for(int i = 0; i < melodySize; i++){

			Piezo.tone((melodyNote + i)->note, (melodyNote + i)->duration);

			delay((melodyNote + i)->duration);

			Piezo.noTone();
		}
	}
}


void MelodyImpl::stop(){

	melodySize = 0;
	melodyNote = nullptr;
	noteNum = 0;

}

