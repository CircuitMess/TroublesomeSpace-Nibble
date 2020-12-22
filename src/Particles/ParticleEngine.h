//
// Created by Domagoj on 21/12/2020.
//

#ifndef NIBBLE_NEW_PARTICLEENGINE_H
#define NIBBLE_NEW_PARTICLEENGINE_H

#include <Arduino.h>
#include <CircuitOS.h>
#include <Nibble.h>
#include <Loop/LoopManager.h>
#include <Display/Display.h>
#include <Display/Sprite.h>
#include "Particle.h"

class Particle;

class ParticleEngine : public LoopListener {

public:

	ParticleEngine();

	void loop(uint);

	void update(float engineX, float engineY);

	void createParticle();

	void removeParticle();

	void removeAll();

private:

	Vector<Particle*> particles;

	uint creationFrequency = 1000;

	uint previousCreationTime = 0;

	float engineX = 0;
	float engineY = 0;

	float delta = 18;

};


#endif //NIBBLE_NEW_PARTICLEENGINE_H
