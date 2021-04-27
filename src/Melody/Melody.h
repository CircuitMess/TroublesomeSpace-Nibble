#ifndef NIBBLE_NEW_MELODY_H
#define NIBBLE_NEW_MELODY_H

#include <Arduino.h>
#include <CircuitOS.h>
#include <Nibble.h>
#include <Loop/LoopManager.h>
#include "Melodies.h"
#include "Note.h"


class MelodyImpl : public LoopListener {

public:

	void loop(uint) override;
	uint playMelody(mel melody , bool loop);

private:

	void play(bool loop);
	void stop();

	int melodySize = 0;
	Note *melodyNote{};

	int shortNoteNum = 0;
	int loopNoteNum = 0;
	unsigned long previousMillis = 0;

	bool loopCheck = false;
	bool melodyPlaying = false;

	unsigned int melodyTime = 0;
};

extern MelodyImpl Melody;

#endif //NIBBLE_NEW_MELODY_H
