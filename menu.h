float cloud_x = 0;
float cloud_x2 = 1000;
char frame[100] = "frames/frame(1).bmp";
int frame_count = 45;
int cur_frame = 1;
int frames_displayed = 0;
void show_count_cursive(int num, int x, int y);
void show_count_brown_four(int num, int x, int y);
void show_count_brown_two(int num, int x, int y);
int cur_screen = 0;
show_menu() {
    if (cur_screen == 0)
    {
        FILE* filePtr = fopen("resource/data/highscore.dat", "r");
        int highscore;
        fscanf(filePtr, "%d", &highscore);
        fclose(filePtr);
        iShowBMP(1,1,"resource/menu_background.fle");
        iShowBMP(cloud_x,285, "images/cloud2.bmp");
        iShowBMP(cloud_x2, 320, "images/clouds.bmp");
        show_count_brown_four(highscore, 980, 150);
    }
    if (cur_screen == 1) {
        iShowBMP(0,0, "resource/HowToPlay.fle");
    }
    if (cur_screen == 2) {
        iShowBMP(0,0, "resource/credits2.fle");
    }
}

void move_cloud () {
    cloud_x+= 0.4;
    cloud_x2 -= 0.4;
    if (cloud_x > 1200) cloud_x = 0;
    if (cloud_x2 < 0) cloud_x2 = 1200;
}

void loading_screen() {
    sprintf(frame, "frames/frame(%d).bmp", cur_frame);
    cur_frame %= frame_count;
    cur_frame++;
    frames_displayed++;
}

void animation() {
    iSetColor(26, 106, 175);
    iFilledRectangle(0,0,1200, 500);
    iShowBMP(350, 100, frame);
}

void show_count_cursive(int num, int x, int y) {
    int first_digit = num / 10;
    int second_digit = num % 10;
    iShowImg(x, y, first_digit + 27);
    iShowImg(x+25, y, second_digit+27);
}
void show_count_brown_four(int num, int x, int y) {
    int first_digit = num % 10;
    num /= 10;
    int second_digit = num % 10;
    num /= 10;
    int third_digit = num % 10;
    num /= 10;
    int fourth_digit = num % 10;
    iShowImg(x, y, fourth_digit + 37);
    iShowImg(x+25, y, third_digit+37);
    iShowImg(x+50, y, second_digit+37);
    iShowImg(x+75, y, first_digit+37);
}

void show_count_brown_two(int num, int x, int y) {
    int first_digit = num % 10;
    num /= 10;
    int second_digit = num % 10;
    num /= 10;
    iShowImg(x, y, second_digit+37);
    iShowImg(x+25, y, first_digit+37);
}
