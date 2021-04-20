#include "Particle.h"
#include "ParticleEngine.h"

Particle::Particle(ParticleEngine *_particle, Sprite *_baseSprite, uint8_t engineX, uint8_t engineY){

	baseSprite = _baseSprite;

	// engine = _particle;

	particleX = random(engineX-(delta/2), engineX+(delta/2));
	particleY = engineY;

}

void Particle::loop(uint){

	movementModifier++;
	if(movementModifier > 255){
		movementModifier = 0;
	}

	float deltaY = 7.0f*(movementModifier/255.0f)*sin((float)movementModifier/255.0f * TWO_PI);;

	if(millis()-particleMovementTime > 10){

		particleY += deltaY;
		particleMovementTime = millis();
	}

	if(particleX < 0){
		particleX = 0;
	}
	else if(particleX > 128){
		particleX = 128;
	}

	draw();
}

void Particle::draw(){
	baseSprite->drawPixel((uint8_t)particleX, (uint8_t)particleY, TFT_ORANGE);
}
