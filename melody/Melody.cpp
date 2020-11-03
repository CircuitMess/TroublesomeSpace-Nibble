//
// Created by Domagoj on 03/11/2020.
//

#include "Melody.h"
#include "Melodies.h"
#include "StartUpMelody.h"
#include "VictoryMelody.h"
#include "GameOverMelody.h"
#include "LoopMelody.h"
#include "Note.h"


Melody::Melody(){

	Piezo.setMute(false);
}

void Melody::loop(uint t){

	play();

}

void Melody::playMelody(char *melody, bool loop){

	melodyType = melody;
	state = loop;

	case (melody){

		case START:

			for(auto &n : startUpMelody){

				Piezo.tone(n.note, n.duration);

				int pauseBetweenNotes = (int) (n.duration * 1.2);

				delay(pauseBetweenNotes);

				Piezo.noTone();
			}

		break;

		case WIN:

			for(auto &n : victoryMelody){

				Piezo.tone(n.note, n.duration);

				int pauseBetweenNotes = (int) (n.duration * 1.2);

				delay(pauseBetweenNotes);

				Piezo.noTone();
			}

		break;

		case LOSE:

			for(auto &n : gameOverMelody){

				Piezo.tone(n.note, n.duration);

				int pauseBetweenNotes = (int) (n.duration * 1.2);

				delay(pauseBetweenNotes);

				Piezo.noTone();
			}

		break;

		case LOOP:



		break;

		default:
			break;
	}

}

void Melody::play(){


	if(noteNum >= sizeof(Note))
		noteNum = 0;

	Piezo.tone(loopMelody[noteNum].note, loopMelody[noteNum].duration);

	unsigned long currentMillis = millis();

	if(currentMillis - previousMillis > loopMelody[noteNum].duration){

		previousMillis = currentMillis;
		noteNum++;
	}

}

void Melody::stop(){


}