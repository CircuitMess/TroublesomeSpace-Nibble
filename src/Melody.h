//
// Created by Domagoj on 28/10/2020.
//

#ifndef NIBBLE_NEW_MELODY_H
#define NIBBLE_NEW_MELODY_H

#include "Pitches.h"

struct StartUpMelody {

	int note[8] = {

			NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 0, NOTE_B3, NOTE_C4

	};

	int duration[8] = {

			4, 8, 8, 4, 4, 4, 4, 4

	};
};

struct InGameMelody {

	int note[40] = {

			NOTE_E5, NOTE_C5, NOTE_A4,
			NOTE_E5, NOTE_C5, NOTE_A4,
			NOTE_E5, NOTE_C5,
			NOTE_E5, NOTE_C5, NOTE_A4,
			NOTE_E5, NOTE_C5, NOTE_A4,
			NOTE_E5, NOTE_C5,

			NOTE_D5, NOTE_B4, NOTE_F4,
			NOTE_D5, NOTE_B4, NOTE_F4,
			NOTE_D5, NOTE_B4,
			NOTE_D5, NOTE_B4, NOTE_F4,
			NOTE_D5, NOTE_B4, NOTE_F4,
			NOTE_D5, NOTE_B4,

	};

	int duration[40] = {

			250, 250, 250,
			250, 250, 250,
			250, 250,
			250, 250, 250,
			250, 250, 250,
			250, 250,

			250, 250, 250,
			250, 250, 250,
			250, 250,
			250, 250, 250,
			250, 250, 250,
			250, 250

	};
};


#endif //NIBBLE_NEW_MELODY_H
