//
// Created by Domagoj on 03/11/2020.
//

#ifndef NIBBLE_NEW_MELODY_H
#define NIBBLE_NEW_MELODY_H

#include <Arduino.h>
#include <CircuitOS.h>
#include <Nibble.h>
#include <Loop/LoopManager.h>
#include "Melodies.h"
#include "Note.h"


class Melody : public LoopListener {

public:

	Melody();

	void loop(uint) override;
	uint playMelody(mel melody , bool loop);

private:

	void play(bool loop);
	void stop();

	int melodySize = 0;
	Note *melodyNote;

	int noteNum = 0;
	unsigned long previousMillis = 0;

	bool loopCheck = false;

	unsigned int melodyTime = 0;
};


#endif //NIBBLE_NEW_MELODY_H
