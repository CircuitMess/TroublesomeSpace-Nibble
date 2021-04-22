#include "GameOverState.h"
#include "GameState.h"
#include "EnterHighscoreState.h"
#include "ShowHighscoreState.h"
#include "Highscore/Highscore.h"
#include "bitmaps/states/gameover.hpp"


GameOverState::GameOverState(uint _score){

	display = Nibble.getDisplay();
	baseSprite = display->getBaseSprite();

	score = _score;

	melodyTime = Melody.playMelody(LOSE, false);

	baseSprite->clear(TFT_BLACK);

	draw();
}

void GameOverState::enter(Game &_game){

	game = &_game;

}

void GameOverState::exit(){

}

void GameOverState::loop(uint){

	baseSprite->clear(TFT_BLACK);

	draw();

	display->commit();

	if(millis() - gameOverMillis > melodyTime){

		if(score > Highscore.get(4).score){
			game->changeState(new EnterHighscoreState(score));
		}
		else{
			game->changeState(new ShowHighscoreState());
		}
	}

}

void GameOverState::draw(){

	baseSprite->drawIcon(gameover, 0, 0, 128, 128);

}
