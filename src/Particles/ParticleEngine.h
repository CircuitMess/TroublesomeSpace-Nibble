#ifndef NIBBLE_NEW_PARTICLEENGINE_H
#define NIBBLE_NEW_PARTICLEENGINE_H

#include <Arduino.h>
#include <CircuitOS.h>
#include <Nibble.h>
#include <Loop/LoopManager.h>
#include <Display/Display.h>
#include <Display/Sprite.h>
#include "Particle.h"

namespace TroublesomeSpace {

	class Particle;

	class ParticleEngine : public LoopListener {

	public:

		explicit ParticleEngine(Sprite *baseSprite);

		~ParticleEngine();

		void loop(uint) override;

		void update(uint8_t engineX, uint8_t engineY);

		void createParticle();

		void removeParticle(Particle *particleToRemove);

		void removeAll();


	private:

		Sprite *baseSprite;

		Vector<Particle *> particles;

		uint16_t creationFrequency = 50;
		uint previousCreationTime = 0;

		uint8_t engineX = 0;
		uint8_t engineY = 0;

	};

}

#endif //NIBBLE_NEW_PARTICLEENGINE_H
