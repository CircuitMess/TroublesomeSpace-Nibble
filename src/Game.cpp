//
// Created by Domagoj on 12-Oct-20.
//

#include <Arduino.h>
#include <Nibble.h>
#include "Pins.hpp"
#include "Game.h"


Game *Game::instance = nullptr;

Game::Game(){

	display = Nibble.getDisplay();
	baseSprite = display->getBaseSprite();
	Piezo.setMute(false);

	Input::getInstance()->setBtnPressCallback(BTN_UP, buttonUp_pressed);
	Input::getInstance()->setBtnPressCallback(BTN_DOWN, buttonDown_pressed);
	Input::getInstance()->setBtnPressCallback(BTN_LEFT, buttonLeft_pressed);
	Input::getInstance()->setBtnPressCallback(BTN_RIGHT, buttonRight_pressed);
	Input::getInstance()->setBtnPressCallback(BTN_A, buttonA_pressed);
	Input::getInstance()->setBtnPressCallback(BTN_B, buttonB_pressed);

	Input::getInstance()->setBtnReleaseCallback(BTN_UP, buttonUp_released);
	Input::getInstance()->setBtnReleaseCallback(BTN_DOWN, buttonDown_released);
	Input::getInstance()->setBtnReleaseCallback(BTN_LEFT, buttonLeft_released);
	Input::getInstance()->setBtnReleaseCallback(BTN_RIGHT, buttonRight_released);
	Input::getInstance()->setBtnReleaseCallback(BTN_A, buttonA_released);
	Input::getInstance()->setBtnReleaseCallback(BTN_B, buttonB_released);

	instance = this;

	triangle.push_back({0, 64}); // triangle[0]
	triangle.push_back({64, 0}); // triangle[1]
	circle.push_back({117, 10}); // circle[0] = green
	circle.push_back({(float) random(10, 117), (float) random(10, 117)});    // circle[1] = blue
	circle.push_back({117, 10}); // circle[2] = orientation_piece

}

void Game::loop(uint time){

	states(circle[0], time);

	baseSprite->clear(TFT_BLACK);

	circles();
	triangles();
	draw_counter_string();
	draw_spawn_point();
	draw_lives_string();
	warning_message();
	//lvl_surprise();

	draw_orientation_piece(circle[2]);

	display->commit();

}

void Game::drawCircles(fillCircles &dot, int i){
	if(i == 0){
		baseSprite->fillCircle(dot.dot_x, dot.dot_y, radius, TFT_GREEN);
		green_X = dot.dot_x;
		green_Y = dot.dot_y;
	}
	if(i == 1)
		baseSprite->fillCircle(dot.dot_x, dot.dot_y, radius, TFT_BLUE);

}

void Game::circles(){
	for(int i = 0; i < circle.size(); i++){

		drawCircles(circle[i], i);

	}

}

void Game::drawRed_Triangle(redTriangle &tri, int i){

	if(i == 0){

		if(tri.triangle_x > 127){
			tri.triangle_x = 0;
			tri.triangle_y = (float) random(20, 108);
		}
		baseSprite->fillTriangle(tri.triangle_x, tri.triangle_y - 5, 5 * sqrt(2) + tri.triangle_x,
								 tri.triangle_y, tri.triangle_x, tri.triangle_y + 5, TFT_RED);
		tri.triangle_x++;

	}
	if(i == 1){
		if(tri.triangle_y > 127){
			tri.triangle_x = (float) random(20, 108);
			tri.triangle_y = 0;
		}

		baseSprite->fillTriangle(tri.triangle_x + 5, tri.triangle_y, tri.triangle_x,
								 tri.triangle_y + 5 * sqrt(2), tri.triangle_x - 5, tri.triangle_y, TFT_RED);
		tri.triangle_y++;

	}

}

void Game::triangles(){
	for(int i = 0; i < triangle.size(); i++){

		drawRed_Triangle(triangle[i], i);
		check_if_dead(triangle[i], circle[0], circle[2]);
	}
}

void Game::check_if_dead(redTriangle &tri, fillCircles &greenDot, fillCircles &orientation){

	if(sqrt(pow(tri.triangle_x - greenDot.dot_x, 2) + pow(tri.triangle_y - greenDot.dot_y, 2)) <
	   (triangle_side * sqrt(3) / 6 + radius)){

		Piezo.tone(1000, 300);
		delay(500);

		greenDot.dot_x = 117;
		greenDot.dot_y = 10;

		orientation.dot_x = 117;
		orientation.dot_y = 10;

		lives--;

		if(lives == 0){

			cnt = 0;
			lives = 3;
			baseSprite->clear(TFT_BLACK);
			baseSprite->drawString(end_message, 35, 55);
			display->commit();
			delay(2000);

		}

	}
}

void Game::check_if_eaten(fillCircles &blueDot){

	if(abs(blueDot.dot_x - green_X) < (radius + 5) && abs(blueDot.dot_y - green_Y) < (radius + 5)){

		float pom_x = blueDot.dot_x;
		float pom_y = blueDot.dot_y;

		do {
			blueDot.dot_x = random(10, 117);
			blueDot.dot_y = random(10, 117);
		} while(abs(pom_x - blueDot.dot_x) < 20 && abs(pom_y - blueDot.dot_y));

		cnt++;
		Piezo.tone(500, 200);

	}

}

void Game::states(fillCircles &greenDot, uint t){

	if(instance->up_state == 1){
		greenDot.dot_y -= speed * t / 13000;
		if(greenDot.dot_y < 0)
			greenDot.dot_y = 127;

	}
	if(instance->left_state == 1){
		greenDot.dot_x -= speed * t / 13000;
		if(greenDot.dot_x < 0)
			greenDot.dot_x = 127;


	}
	if(instance->down_state == 1){
		greenDot.dot_y += speed * t / 13000;
		if(greenDot.dot_y > 127)
			greenDot.dot_y = 0;

	}
	if(instance->right_state == 1){
		greenDot.dot_x += speed * t / 13000;
		if(greenDot.dot_x > 127)
			greenDot.dot_x = 0;

	}
	if(instance->a_state == 1){
		if(instance->up_state == 1){
			greenDot.dot_y -= speed * t / 13000;
		}
		if(instance->down_state == 1){
			greenDot.dot_y += speed * t / 13000;
		}
		if(instance->right_state == 1){
			greenDot.dot_x += speed * t / 13000;
		}
		if(instance->left_state == 1){
			greenDot.dot_x -= speed * t / 13000;
		}


	}
	if(instance->b_state == 1){

	}

	check_if_eaten(circle[1]);

	orientation_piece(circle[2], circle[0]);

}

void Game::warning_message(){
	if((cnt + 1) % 20 == 0){
		lvl_mul = 20;
		baseSprite->setTextColor(TFT_RED);
		baseSprite->drawString(warning, 35, 55);
		baseSprite->drawLine(24, 0, 24, 128, TFT_RED);
		baseSprite->drawLine(104, 0, 104, 128, TFT_RED);
	}else if((cnt + 1) % 10 == 0){
		lvl_mul = 10;
		baseSprite->setTextColor(TFT_RED);
		baseSprite->drawString(warning, 35, 55);
		baseSprite->drawLine(0, 24, 128, 24, TFT_RED);
		baseSprite->drawLine(0, 104, 128, 104, TFT_RED);
	}
}

/*
void Game::lvl_surprise() {

	if (cnt > 0 && cnt % 20 == 0) {
		baseSprite->fillTriangle(ver_tri_x1 + 40, ver_tri_y1, ver_tri_x2 + 40, ver_tri_y2, ver_tri_x3 + 40, ver_tri_y3, TFT_RED); //ty++ for faster movement
		baseSprite->fillTriangle(ver_tri_x1 - 40, ver_tri_y1, ver_tri_x2 - 40, ver_tri_y2, ver_tri_x3 - 40, ver_tri_y3, TFT_RED);
	}
	else if (cnt > 0 && cnt % 10 == 0) {
		baseSprite->fillTriangle(hor_tri_x1, hor_tri_y1 + 40, hor_tri_x2, hor_tri_y2 + 40, hor_tri_x3, hor_tri_y3 + 40, TFT_RED); //tx++ for faster movement
		baseSprite->fillTriangle(hor_tri_x1, hor_tri_y1 - 40, hor_tri_x2, hor_tri_y2 - 40, hor_tri_x3, hor_tri_y3 - 40, TFT_RED);
	}
}
*/
void Game::draw_counter_string(){

	baseSprite->setTextSize(1);
	baseSprite->setTextFont(2);
	baseSprite->setTextColor(TFT_WHITE);
	baseSprite->drawNumber(cnt, 5, 3);

}

void Game::draw_spawn_point(){

	baseSprite->drawLine(127, 0, 107, 0, TFT_WHITE);
	baseSprite->drawLine(127, 0, 127, 20, TFT_WHITE);
	baseSprite->drawLine(107, 0, 107, 20, TFT_WHITE);
	baseSprite->drawLine(107, 20, 127, 20, TFT_WHITE);

}

void Game::draw_lives_string(){

	baseSprite->drawString(lives_rest, 1, 110);
	baseSprite->drawNumber(lives, 40, 110);
}


void Game::buttonUp_pressed(){

	instance->up_state = 1;

}

void Game::buttonDown_pressed(){

	instance->down_state = 1;

}

void Game::buttonLeft_pressed(){

	instance->left_state = 1;

}

void Game::buttonRight_pressed(){

	instance->right_state = 1;

}

void Game::buttonA_pressed(){

	instance->a_state = 1;

}

void Game::buttonB_pressed(){

	instance->b_state = 1;

}

void Game::buttonUp_released(){

	instance->up_state = 0;

}

void Game::buttonDown_released(){

	instance->down_state = 0;

}

void Game::buttonLeft_released(){

	instance->left_state = 0;

}

void Game::buttonRight_released(){

	instance->right_state = 0;


}

void Game::buttonA_released(){

	instance->a_state = 0;

}

void Game::buttonB_released(){

	instance->b_state = 0;
}


void Game::draw_orientation_piece(fillCircles &dot){
	baseSprite->fillCircle(dot.dot_x, dot.dot_y, 2, TFT_GREEN);
}

void Game::orientation_piece(fillCircles &orientation, fillCircles &greenDot){

	if(instance->up_state == 1 && instance->left_state == 1){
		orientation.dot_x = greenDot.dot_x - 6 * cos(45);
		orientation.dot_y = greenDot.dot_y - 6 * sin(45);
	}else if(instance->up_state == 1 && instance->right_state == 1){
		orientation.dot_x = greenDot.dot_x + 6 * cos(45);
		orientation.dot_y = greenDot.dot_y - 6 * sin(45);
	}else if(instance->down_state == 1 && instance->left_state == 1){
		orientation.dot_x = greenDot.dot_x - 6 * cos(45);
		orientation.dot_y = greenDot.dot_y + 6 * sin(45);
	}else if(instance->down_state == 1 && instance->right_state == 1){
		orientation.dot_x = greenDot.dot_x + 6 * cos(45);
		orientation.dot_y = greenDot.dot_y + 6 * sin(45);
	}else if(instance->up_state == 1){

		orientation.dot_x = greenDot.dot_x;
		orientation.dot_y = greenDot.dot_y - 6;
	}else if(instance->left_state == 1){

		orientation.dot_x = greenDot.dot_x - 6;
		orientation.dot_y = greenDot.dot_y;
	}else if(instance->down_state == 1){

		orientation.dot_x = greenDot.dot_x;
		orientation.dot_y = greenDot.dot_y + 6;
	}else if(instance->right_state == 1){

		orientation.dot_x = greenDot.dot_x + 6;
		orientation.dot_y = greenDot.dot_y;
	}

}
