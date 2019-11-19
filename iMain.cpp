//#define _CRT_SECURE_NO_WARNINGS

int global_flag = 0;
#include "iGraphics.h"
#include "iShowImg.h"
#include "menu.h"
#include<math.h>
#include "level1.h"
int loading_done = 0;
void timers();

void get_keys() {
    if (current_level == 1) getKeys(&level1), gravity_function(&level1), dyn_thorn_move(&level1);
    else if (current_level == 2) getKeys(&level2), gravity_function(&level2), dyn_thorn_move(&level2);
    else if (current_level == 3) getKeys(&level3), gravity_function(&level3), dyn_thorn_move(&level3);
}
void iDraw() {
	//place your drawing codes here
	iClear();
	iSetColor(255,255,255);
    iFilledRectangle(0,0,2000,2000);

    if (frames_displayed >= 100 and !loading_done)
    {
        current_level = -1;
        loading_done = 1;
        timers();
    }
    if (current_level == -2) animation();
    else if (current_level == -1) show_menu();
    else if (current_level==1) draw_level(&level1);
    else if (current_level==2) draw_level(&level2);
    else if(current_level ==3 ) draw_level(&level3);
    else if (current_level == 4) iPauseTimer(4), iShowBMP(0, 0, "resource/congrats.fle"), show_count_brown_four(total_score, 600, 183);
    //if (current_level==3) draw_level(&level);
}

void iMouseMove(int mx, int my) {

}

void iMouse(int button, int state, int mx, int my) {
    if (current_level == -1 && button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        if (mx > 40 && mx < 95 && my > 55 &&  my < 105) {
            cur_screen = 2;
        }

        else if (mx > 40 && mx < 95 && my > 130 && my < 180) {
            cur_screen = 1;
        }
        else {
            if (cur_screen == 0)
            {
                current_level = 1;
                timers();
            }
            else cur_screen = 0;
        }
    } else{
        if(current_level >= 1 && current_level <= 3 && !game_over){
            if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN){
                if(mx >= 1056 && mx <= 1102 && my >= 154 && my <= 203){
                    if(paused){
                        paused = 0;
                        iResumeTimer(0);
                        iResumeTimer(1);
                        iResumeTimer(2);
                        iResumeTimer(3);
                        iResumeTimer(4);

                    } else{
                        paused = 1;
                        iPauseTimer(0);
                        iPauseTimer(1);
                        iPauseTimer(2);
                        iPauseTimer(3);
                        iPauseTimer(4);
                    }
                }
            }
        }
        else if(game_over && current_level >= 1 && button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
            game_over = 0;
            current_level = -2;
            loading_done = 0;
            frames_displayed = 0;
            timers();
        }
        else if(current_level == 4 && button == GLUT_LEFT_BUTTON && state == GLUT_DOWN){
            game_over = 0;
            current_level = -2;
            loading_done = 0;
            frames_displayed = 0;
            timers();
        }
    }
}

void iKeyboard(unsigned char key) {
	static int c=0;
	if (key == 'q') {
		exit(0);
	}
	 else {
        char t[2];
        t[0] = key;
        t[1] = 0;
        strcat(text, t);
	}
}

void iSpecialKeyboard(unsigned char key) {
	if (key == GLUT_KEY_END) {
		exit(0);
	}
}


void power_speed_timer() {
    if(t_flag == 0){
        t_flag = 1;
        return;
    }
    if(t_flag == 1){
        my_v = 10;
        iPauseTimer(3);
        return;
    }

}

int main() {
    text[0] = 0;
    level1.levelID = 1;
    level2.levelID = 2;
    level3.levelID = 3;
    image_init();
    for(int i = 0; i < image_count; ++i){
        iLoadImg(i, 0xFFFFFF);
    }

    iSetTimer(5, get_keys);
    iSetTimer(10, move_cloud);
    iPauseTimer(1);
    iSetTimer(30, loading_screen);
    iPauseTimer(2);
    iSetTimer(10000, power_speed_timer);
    iPauseTimer(3);
    iSetTimer(1000, changeScore);
    iPauseTimer(4);
    timers();

	iInitialize(SCREEN_WIDTH, SCREEN_HEIGHT, "Bounce Game");
	return 0;
}


