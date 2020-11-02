//
// Created by Domagoj on 28/10/2020.
//

#ifndef NIBBLE_NEW_MELODY_H
#define NIBBLE_NEW_MELODY_H

#include "Pitches.h"
#include "Audio.h"

Note startUpMelody[] = {

		{NOTE_C4, 250},
		{NOTE_G3, 125},
		{NOTE_G3, 125},
		{NOTE_A3, 250},
		{NOTE_G3, 250},
		{0,       250},
		{NOTE_B3, 250},
		{NOTE_C4, 250}

};

Note inGameMelody[] = {

		{NOTE_E5, 250},
		{NOTE_C5, 250},
		{NOTE_A4, 250},
		{NOTE_E5, 250},
		{NOTE_C5, 250},
		{NOTE_A4, 250},
		{NOTE_E5, 250},
		{NOTE_C5, 250},
		{NOTE_E5, 250},
		{NOTE_C5, 250},
		{NOTE_A4, 250},
		{NOTE_E5, 250},
		{NOTE_C5, 250},
		{NOTE_A4, 250},
		{NOTE_E5, 250},
		{NOTE_C5, 250},


		{NOTE_D5, 250},
		{NOTE_B4, 250},
		{NOTE_F4, 250},
		{NOTE_D5, 250},
		{NOTE_B4, 250},
		{NOTE_F4, 250},
		{NOTE_D5, 250},
		{NOTE_B4, 250},
		{NOTE_D5, 250},
		{NOTE_B4, 250},
		{NOTE_F4, 250},
		{NOTE_D5, 250},
		{NOTE_B4, 250},
		{NOTE_F4, 250},
		{NOTE_D5, 250},
		{NOTE_B4, 250}
};

Note victoryMelody[] = {

		{NOTE_G3,  125},
		{NOTE_G3,  125},
		{NOTE_G3,  125},
		{NOTE_G3,  125},
		{0,        125},
		{NOTE_FS3, 125},
		{NOTE_F3,  125},
		{0,500},

		{NOTE_G3,  125},
		{NOTE_G3,  125},
		{NOTE_G3,  125},
		{NOTE_G3,  125},
		{0,        125},
		{NOTE_FS3, 125},
		{NOTE_F3,  125},
		{0,500}

};

Note gameOverMelody[] = {

		{NOTE_A3, 250},
		{NOTE_F3, 250},
		{NOTE_D3, 250},
		{NOTE_B2, 250},
		{NOTE_G2, 1000}

};

#endif //NIBBLE_NEW_MELODY_H
