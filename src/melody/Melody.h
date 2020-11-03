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


class Melody : public LoopListener {

public:

	Melody();

	void loop(uint) override;
	static void playMelody(mel melody , bool loop);

private:

	void play();
	void stop();

	int noteNum = 0;
	unsigned long previousMillis = 0;
};


#endif //NIBBLE_NEW_MELODY_H
