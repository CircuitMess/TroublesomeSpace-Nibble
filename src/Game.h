//
// Created by Domagoj on 12-Oct-20.
//

#ifndef NIBBLE_NEW_GAME_H
#define NIBBLE_NEW_GAME_H

#include <Arduino.h>
#include <CircuitOS.h>
#include <Nibble.h>
#include <Loop/LoopManager.h>
#include <Display/Display.h>
#include <Display/Sprite.h>
#include <Input/Input.h>
#include <Input/I2cExpander.h>
#include <Input/InputI2C.h>
#include "Pins.hpp"
#include <Audio/Piezo.h>

struct redTriangle{

	float triangle_x;
	float triangle_y;

};

struct fillCircles{

	float dot_x;
	float dot_y;
};


class Game : public LoopListener {

    public:

        Game();

        void loop(uint) override;

        static void buttonUp_pressed();
        static void buttonDown_pressed();
        static void buttonLeft_pressed();
        static void buttonRight_pressed();
        static void buttonA_pressed();
        static void buttonB_pressed();
        static void buttonUp_released();
        static void buttonDown_released();
        static void buttonLeft_released();
        static void buttonRight_released();
        static void buttonA_released();
        static void buttonB_released();

    private:

        static Game *instance;

        Display* display;
        Sprite* baseSprite;

        Vector<redTriangle> triangle;
        Vector<fillCircles> circle;

        void states(fillCircles &dot, uint t);

        void drawCircles(fillCircles &dot, int i);
        void circles();

        void triangles();
        void drawRed_Triangle(redTriangle &tri, int i);

        void check_if_eaten(fillCircles &dot);
        void check_if_dead(redTriangle &tri, fillCircles &dot, fillCircles &orientation);
        //void lvl_surprise();
        void warning_message();
        void draw_counter_string();
        void draw_spawn_point();
        void draw_lives_string();


        void draw_orientation_piece(fillCircles &dot);
        void orientation_piece(fillCircles &dot, fillCircles &greenDot);

        const float speed = 1;

        float green_X = 0;
        float green_Y = 0;

        int up_state = 0;
        int down_state = 0;
        int right_state = 0;
        int left_state = 0;
        int a_state = 0;
        int b_state = 0;

		const float radius = 5;
		const float triangle_side = 10;

        int lives = 3;
        int cnt = 0;
        int lvl_mul = 0;

        String end_message = "Game Over";
        String lives_rest = "Lives: ";
        String warning = "Warning!";

};

#endif //NIBBLE_NEW_GAME_H
