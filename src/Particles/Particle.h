#ifndef NIBBLE_NEW_PARTICLE_H
#define NIBBLE_NEW_PARTICLE_H

#include <Arduino.h>
#include <CircuitOS.h>
#include <Nibble.h>
#include <Loop/LoopManager.h>
#include <Display/Display.h>
#include <Display/Sprite.h>

struct Particles{

	float posX;
	float posY;
};

class ParticleEngine;

class Particle : public LoopListener {

public:

	Particle(ParticleEngine *particle);

	void draw();

	void loop(uint) override;

private:

	void drawParticles(Particles &particle);

	Display *display;
	Sprite *baseSprite;

	Vector<Particles> particles;

	ParticleEngine *engine;

	uint destructionTime = 2000;

	uint previousDestructionTime = 0;


};

#endif //NIBBLE_NEW_PARTICLE_H
