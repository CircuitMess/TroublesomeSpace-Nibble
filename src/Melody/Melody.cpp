#include "Melody.h"
#include "StartUpMelody.h"
#include "GameOverMelody.h"
#include "LoopMelody.h"
#include "MenuMelody.h"


TroublesomeSpace::MelodyImpl TroublesomeSpace::Melody;


void TroublesomeSpace::MelodyImpl::loop(uint t){

	if(melodyPlaying){
		if(!loopCheck){

			play(false);

		}else{

			play(true);
		}
	}
}

uint TroublesomeSpace::MelodyImpl::playMelody(mel melody, bool loop){


	loopCheck = loop;
	melodyPlaying = true;

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

		case STOP:
			stop();
			break;

		default:
			return 0;
	}

	return melodyTime;

}


void TroublesomeSpace::MelodyImpl::play(bool loop){

	if(loop){

		if(loopNoteNum >= melodySize)
			loopNoteNum = 0;

		Piezo.tone((melodyNote + loopNoteNum)->note, (melodyNote + loopNoteNum)->duration);

		unsigned long currentMillis = millis();

		if(currentMillis - previousMillis > (melodyNote + loopNoteNum)->duration){

			previousMillis = currentMillis;
			loopNoteNum++;
		}
	}else{

		Piezo.tone((melodyNote + shortNoteNum)->note, (melodyNote + shortNoteNum)->duration);

		unsigned long currentMillis = millis();

		if(currentMillis - previousMillis > (melodyNote + shortNoteNum)->duration){

			previousMillis = currentMillis;
			shortNoteNum++;
			if(shortNoteNum >= melodySize){
				melodyPlaying = false;
			}
		}
	}
}


void TroublesomeSpace::MelodyImpl::stop(){

	melodySize = 0;
	melodyNote = nullptr;
	shortNoteNum = 0;
	loopNoteNum = 0;
	melodyPlaying = false;
	melodyTime = 0;
}
