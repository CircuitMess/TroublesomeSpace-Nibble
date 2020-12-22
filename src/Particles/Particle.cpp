//
// Created by Domagoj on 21/12/2020.
//

#include "Particle.h"
#include "ParticleEngine.h"



Particle::Particle(ParticleEngine *_particle){

	display = Nibble.getDisplay();
	baseSprite = display->getBaseSprite();

	engine = _particle;

}

void Particle::loop(uint){

	draw();

	if(millis()-previousDestructionTime > destructionTime){

		engine->removeParticle();
		Serial.println("remove");

		previousDestructionTime = millis();
	}

}

void Particle::draw(){

	for(int i = 0; i < particles.size(); i++){
		Serial.println("drawParticles");

		drawParticles(particles[i]);
	}
}

void Particle::drawParticles(Particles &_particle){

	baseSprite->drawPixel(_particle.posX,_particle.posY,TFT_RED);
}
