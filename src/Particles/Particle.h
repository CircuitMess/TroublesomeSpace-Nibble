#ifndef NIBBLE_NEW_PARTICLE_H
#define NIBBLE_NEW_PARTICLE_H

#include <Arduino.h>
#include <CircuitOS.h>
#include <Nibble.h>
#include <Loop/LoopManager.h>
#include <Display/Display.h>
#include <Display/Sprite.h>

class ParticleEngine;

class Particle : public LoopListener {

public:

	Particle(ParticleEngine *particle, Sprite *baseSprite, uint8_t engineX, uint8_t engineY);

	void draw();

	void loop(uint) override;

private:

	Display *display;
	Sprite *baseSprite;

	ParticleEngine *engine;

	const uint8_t delta = 10;

	float particleX;
	float particleY;

	uint32_t particleMovementTime = millis();
	uint8_t movementModifier = 0;
};

#endif //NIBBLE_NEW_PARTICLE_H
