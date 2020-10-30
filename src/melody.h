//
// Created by Domagoj on 28/10/2020.
//

#ifndef NIBBLE_NEW_MELODY_H
#define NIBBLE_NEW_MELODY_H

// notes in the melody:
int startUpMelody[] = {

		NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 0, NOTE_B3, NOTE_C4

};

// note durations: 4 = quarter note, 8 = eighth note, etc.:
int startUpNoteDurations[] = {

		4, 8, 8, 4, 4, 4, 4, 4

};


int loopMelody[] = {

		NOTE_E5, NOTE_C5, NOTE_A4,
		NOTE_E5, NOTE_C5, NOTE_A4,
		NOTE_E5, NOTE_C5,
		NOTE_E5, NOTE_C5, NOTE_A4,
		NOTE_E5, NOTE_C5, NOTE_A4,
		NOTE_E5, NOTE_C5,

		NOTE_D5 , NOTE_B4, NOTE_F4,
		NOTE_D5 , NOTE_B4, NOTE_F4,
		NOTE_D5 , NOTE_B4,
		NOTE_D5 , NOTE_B4, NOTE_F4,
		NOTE_D5 , NOTE_B4, NOTE_F4,
		NOTE_D5 , NOTE_B4, 0

};

int loopNoteDuration[] = {

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
		250, 250, 250

};

#endif //NIBBLE_NEW_MELODY_H
