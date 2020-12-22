//
// Created by Domagoj on 21/12/2020.
//

#include "ParticleEngine.h"
#include "Particle.h"

ParticleEngine::ParticleEngine(){


}

void ParticleEngine::loop(uint){

	if(millis()- previousCreationTime > creationFrequency){

		createParticle();

		Serial.println("create");

		previousCreationTime = millis();
	}
}

void ParticleEngine::update(float x, float y){

	engineX = x;
	engineY = y;
	Serial.println("update");

}

void ParticleEngine::createParticle(){

	Particle* newParticle = new Particle(this);
	LoopManager::addListener(newParticle);
	particles.push_back(newParticle);

	Particles* newPt = new Particles;
	newPt->posX = (float)random(engineX-delta/2,engineX+delta/2);
	newPt->posY = engineY;

}

void ParticleEngine::removeParticle(){

	particles.remove(0);

}

void ParticleEngine::removeAll(){

	for(int i = 0; i < particles.size(); ++i){
		particles.pop_back();
	}
}
