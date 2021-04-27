#include "GameOverState.h"
#include "GameState.h"
#include "EnterHighscoreState.h"
#include "ShowHighscoreState.h"
#include "Highscore/Highscore.h"
#include "bitmaps/states/gameover.hpp"
#include "Melody/Melody.h"


TroublesomeSpace::GameOverState::GameOverState(Display &_display, uint _score){

	display = &_display;
	baseSprite = display->getBaseSprite();

	score = _score;

	melodyTime = Melody.playMelody(LOSE, false);

	baseSprite->clear(TFT_BLACK);

	draw();
}

void TroublesomeSpace::GameOverState::enter(Game &_game){

	game = &_game;

}

void TroublesomeSpace::GameOverState::exit(){

}

void TroublesomeSpace::GameOverState::loop(uint){

	baseSprite->clear(TFT_BLACK);

	draw();

	display->commit();

	if(millis() - gameOverMillis > melodyTime){

		if(score > Highscore.get(4).score){
			game->changeState(new EnterHighscoreState(*display, score));
		}
		else{
			game->changeState(new ShowHighscoreState(*display));
		}
	}

}

void TroublesomeSpace::GameOverState::draw(){

	baseSprite->drawIcon(gameover, 0, 0, 128, 128);

}
