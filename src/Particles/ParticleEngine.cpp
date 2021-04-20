#include "ParticleEngine.h"

ParticleEngine::ParticleEngine(Sprite *_baseSprite){

	baseSprite = _baseSprite;

	engineX = 0;
	engineY = 0;
	previousCreationTime = 0;

}

ParticleEngine::~ParticleEngine() noexcept{
	removeAll();
}

void ParticleEngine::loop(uint t){

	if(millis()- previousCreationTime > creationFrequency){
		if(particles.size() < 5){
			createParticle();
			previousCreationTime = millis();
		}else{
			removeParticle(particles.at(0));
		}
	}

	for(int i = 0; i < particles.size(); ++i){
		particles[i]->loop(0);
	}
}

void ParticleEngine::update(uint8_t x, uint8_t y){

	engineX = x;
	engineY = y;
}

void ParticleEngine::createParticle(){

	auto* newParticle = new Particle(this, baseSprite, engineX, engineY);
	particles.push_back(newParticle);
}

void ParticleEngine::removeParticle(Particle *particleToRemove){

	int indexToRemove = particles.indexOf(particleToRemove);

	particles.remove(indexToRemove);
	delete particleToRemove;

}

void ParticleEngine::removeAll(){

	particles.clear();
}
