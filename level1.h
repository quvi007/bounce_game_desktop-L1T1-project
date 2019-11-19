int SCREEN_WIDTH = 1150, SCREEN_HEIGHT = 400, t_flag = 0, my_v = 10, game_over = 0, total_score = 1000, paused = 0;
int current_level=-2;
char text[1000], cheat_code[] = "787898";
void timers();

struct level {
    int levelID;
    double y;
    double water_acc_value;
    int brick_len, rad, grounded = 0, cheat_code_mode = 0, rings_crossed = 0;
    int obstacle_count, thorn_count, ring_count, xring_count, life_count, checkpoint_count, dyn_thorn_count, inclined_brick_count_1, inclined_brick_count_2, inverse_thorn_count;
    int remaining_life;
    int remaining_checkpoint;
    int inclined_plane_count_l, inclined_plane_count_r;
    int water_count, no_coll_l_obs_count, no_coll_r_obs_count, no_coll_t_obs_count, no_coll_b_obs_count, blue_obs_count, power_speed_count, pumper_count, deflater_count;
    double vy;
    double vx;
    double acc = -0.5;
    double acc_water = 0;
    int left_border;

    int current_stage = 1;

    int x_of_life[100];
    int y_of_life[100];

    int x_of_checkpoint[100];
    int y_of_checkpoint[100];

    int x_of_thorn[100];
    int y_of_thorn[100];

    int x_of_inverse_thorn[100];
    int y_of_inverse_thorn[100];


    int x_of_pumper[100];
    int y_of_pumper[100];

    int x_of_deflater[100];
    int y_of_deflater[100];

    int x_of_power_speed[100];
    int y_of_power_speed[100];

    int x_of_dyn_thorn[100];
    int y_of_dyn_thorn[100];

    int x_of_ring[100];
    int y_of_ring[100];

    int x_of_xring[100];
    int y_of_xring[100];

    int row_of_obs[100];
    int column_of_obs[100];
    int tile_count_from_left[100];
    int tile_count_from_bottom[100];

    int row_of_no_coll_obs[100];
    int column_of_no_coll_obs[100];
    int tile_count_from_left_no_coll[100];
    int tile_count_from_bottom_no_coll[100];

    int row_of_blue_obs[100];
    int column_of_blue_obs[100];
    int tile_count_from_left_blue_obs[100];
    int tile_count_from_bottom_blue_obs[100];

    int left_collision[100];
    int right_collision[100];
    int bottom_collision[100];
    int top_collision[100];

    struct line {
        int y1, x1, x2, y2;
    } grounds[100], ceilings[100], grounds_blue[100], ceilings_blue[100], grounds_thorn[100], ceilings_thorn[100], grounds_inverse_thorn[100], ceilings_inverse_thorn[100], grounds_dyn_thorn[100], ceilings_dyn_thorn[100], inclined_plane_l[100], inclined_plane_r[100], ceilings_water[100], grounds_pumper[100], grounds_deflater[100], grounds_power_speed[100], ceilings_pumper[100], ceilings_deflater[100], ceilings_power_speed[100];

    struct sq_brick {
        int x,y,side_len;
    } bottom_bricks[100], obs[100][100][100], top_bricks[100], blue_obs[100][100][100];


    struct triangular_brick {
        int x,y,side_len;
    } inclined_bricks_1[100], inclined_bricks_2[100], inclined_bricks_3[100], inclined_bricks_4[100];

    struct ring {
        int x, y;
        int crossed;
        int show_front;
        int show_back;
    } rings[100];

    struct xring {
        int x, y;
        int crossed;
        int show_front;
        int show_back;
    } xrings[100];



    struct thorn {
        int x, y;
    } thorns[100], inverse_thorns[100];

    struct pumper {
        int x, y;
    } pumpers[100];

    struct power_speed {
        int x, y;
    } power_speeds[100];

    struct deflater {
        int x, y;
    } deflaters[100];


    struct life {
        int x, y;
        int visible;
    } lifes[100];

    struct checkpoint {
        int x, y;
        int visible;
    } checkpoints[100], last_checkpoint;


    struct dyn_thorn {
        int x, y, vx, vy, left, right, up, down;
    } dyn_thorns[100];

    struct water {
        int x1, y1, x2, y2, x3, y3, x4, y4;
    } waters[100];


} level1, level2, level3;

void saveHighScore(){
    printf("%d\n", total_score);
    FILE *filePtr = fopen("resource/data/highscore.dat", "r");
    int saved_score;
    fscanf(filePtr, "%d", &saved_score);
    fclose(filePtr);
    if(total_score > saved_score){
        filePtr = fopen("resource/data/highscore.dat", "w");
        fprintf(filePtr, "%d", total_score);
        fclose(filePtr);
    }
}

void changeScore(){
    if(total_score>0)
        total_score--;
}

void firstInit(level* plevel) {
    plevel->grounded = 0, plevel->cheat_code_mode = 0, plevel->rings_crossed = 0;
    plevel->acc = -0.5;
    plevel->acc_water = 0;
    plevel->current_stage = 1;
    plevel->water_acc_value = 0;
    char fileLoc[100];
    sprintf(fileLoc,"resource/data/level%d.dat",plevel->levelID);
    FILE *fileptr = fopen(fileLoc,"r");
    double water_acc_value = 0;
    fscanf(fileptr,"%lf %d %d %d %d %d %d %d %d %d %d %d", &plevel->y, &plevel->brick_len, &plevel->rad, &plevel->obstacle_count, &plevel->thorn_count, &plevel->ring_count, &plevel->xring_count, &plevel->life_count, &plevel->checkpoint_count, &plevel->dyn_thorn_count, &plevel->remaining_life, &plevel->remaining_checkpoint);
    fscanf(fileptr,"%lf %lf", &plevel->vy, &plevel->vx);
    fscanf(fileptr, "%d", &plevel->left_border);
    for(int i = 0; i < plevel->life_count; ++i)
        fscanf(fileptr, "%d", &plevel->x_of_life[i]), plevel->lifes[i].visible = 1;
    for(int i = 0; i < plevel->life_count; ++i)
        fscanf(fileptr, "%d", &plevel->y_of_life[i]);

    for(int i = 0; i < plevel->checkpoint_count; ++i)
        fscanf(fileptr, "%d", &plevel->x_of_checkpoint[i]), plevel->checkpoints[i].visible = 1;
    for(int i = 0; i < plevel->checkpoint_count; ++i)
        fscanf(fileptr, "%d", &plevel->y_of_checkpoint[i]);

    for(int i = 0; i < plevel->thorn_count; ++i)
        fscanf(fileptr, "%d", &plevel->x_of_thorn[i]);
    for(int i = 0; i < plevel->thorn_count; ++i)
        fscanf(fileptr, "%d", &plevel->y_of_thorn[i]);

    for(int i = 0; i < plevel->dyn_thorn_count; ++i)
        fscanf(fileptr, "%d", &plevel->x_of_dyn_thorn[i]);
    for(int i = 0; i < plevel->dyn_thorn_count; ++i)
        fscanf(fileptr, "%d", &plevel->y_of_dyn_thorn[i]);

    for(int i = 0; i < plevel->ring_count; ++i)
        fscanf(fileptr, "%d", &plevel->x_of_ring[i]), plevel->rings[i].crossed = 0, plevel->rings[i].show_back = 0, plevel->rings[i].show_front = 0;
    for(int i = 0; i < plevel->ring_count; ++i)
        fscanf(fileptr, "%d", &plevel->y_of_ring[i]);
    for(int i = 0; i < plevel->xring_count; ++i)
        fscanf(fileptr, "%d", &plevel->x_of_xring[i]), plevel->xrings[i].crossed = 0, plevel->xrings[i].show_back = 0, plevel->xrings[i].show_front = 0;
    for(int i = 0; i < plevel->xring_count; ++i)
        fscanf(fileptr, "%d", &plevel->y_of_xring[i]);


    for(int i = 0; i < plevel->obstacle_count; ++i)
        fscanf(fileptr, "%d", &plevel->row_of_obs[i]);
    for(int i = 0; i < plevel->obstacle_count; ++i)
        fscanf(fileptr, "%d", &plevel->column_of_obs[i]);
    for(int i = 0; i < plevel->obstacle_count; ++i)
        fscanf(fileptr, "%d", &plevel->tile_count_from_left[i]);
    for(int i = 0; i < plevel->obstacle_count; ++i)
        fscanf(fileptr, "%d", &plevel->tile_count_from_bottom[i]);

    for(int i = 0; i < plevel->dyn_thorn_count; ++i)
        fscanf(fileptr, "%d", &plevel->dyn_thorns[i].vx);
    for(int i = 0; i < plevel->dyn_thorn_count; ++i)
        fscanf(fileptr, "%d", &plevel->dyn_thorns[i].vy);
    for(int i = 0; i < plevel->dyn_thorn_count; ++i)
        fscanf(fileptr, "%d", &plevel->dyn_thorns[i].left), plevel->dyn_thorns[i].left = plevel->dyn_thorns[i].left * plevel->brick_len + plevel->left_border;
    for(int i = 0; i < plevel->dyn_thorn_count; ++i)
        fscanf(fileptr, "%d", &plevel->dyn_thorns[i].right), plevel->dyn_thorns[i].right = plevel->dyn_thorns[i].right * plevel->brick_len + plevel->left_border;
    for(int i = 0; i < plevel->dyn_thorn_count; ++i)
        fscanf(fileptr, "%d", &plevel->dyn_thorns[i].up), plevel->dyn_thorns[i].up *= plevel->brick_len;
    for(int i = 0; i < plevel->dyn_thorn_count; ++i)
        fscanf(fileptr, "%d", &plevel->dyn_thorns[i].down), plevel->dyn_thorns[i].down *= plevel->brick_len;
    fscanf(fileptr, "%d", &plevel->inclined_brick_count_1);
    for(int i = 0; i < plevel->inclined_brick_count_1; ++i)
        fscanf(fileptr, "%d", &plevel->inclined_bricks_1[i].x), plevel->inclined_bricks_1[i].x = plevel->inclined_bricks_1[i].x * plevel->brick_len + plevel->left_border;
    for(int i = 0; i < plevel->inclined_brick_count_1; ++i)
        fscanf(fileptr, "%d", &plevel->inclined_bricks_1[i].y), plevel->inclined_bricks_1[i].y = plevel->inclined_bricks_1[i].y * plevel->brick_len;

    fscanf(fileptr, "%d", &plevel->inclined_brick_count_2);
    for(int i = 0; i < plevel->inclined_brick_count_2; ++i)
        fscanf(fileptr, "%d", &plevel->inclined_bricks_2[i].x), plevel->inclined_bricks_2[i].x = plevel->inclined_bricks_2[i].x * plevel->brick_len + plevel->left_border;
    for(int i = 0; i < plevel->inclined_brick_count_2; ++i)
        fscanf(fileptr, "%d", &plevel->inclined_bricks_2[i].y), plevel->inclined_bricks_2[i].y = plevel->inclined_bricks_2[i].y * plevel->brick_len;


    fscanf(fileptr, "%d", &plevel->inclined_plane_count_l);
    for(int i = 0; i < plevel->inclined_plane_count_l; ++i)
        fscanf(fileptr, "%d", &plevel->inclined_plane_l[i].x1), plevel->inclined_plane_l[i].x1 = plevel->inclined_plane_l[i].x1 * plevel->brick_len + plevel->left_border;
    for(int i = 0; i < plevel->inclined_plane_count_l; ++i)
        fscanf(fileptr, "%d", &plevel->inclined_plane_l[i].x2), plevel->inclined_plane_l[i].x2 = plevel->inclined_plane_l[i].x2 * plevel->brick_len + plevel->left_border;
    for(int i = 0; i < plevel->inclined_plane_count_l; ++i)
        fscanf(fileptr, "%d", &plevel->inclined_plane_l[i].y1), plevel->inclined_plane_l[i].y1 = plevel->inclined_plane_l[i].y1 * plevel->brick_len;
    for(int i = 0; i < plevel->inclined_plane_count_l; ++i)
        fscanf(fileptr, "%d", &plevel->inclined_plane_l[i].y2), plevel->inclined_plane_l[i].y2 = plevel->inclined_plane_l[i].y2 * plevel->brick_len;

    fscanf(fileptr, "%d", &plevel->inclined_plane_count_r);
    for(int i = 0; i < plevel->inclined_plane_count_r; ++i)
        fscanf(fileptr, "%d", &plevel->inclined_plane_r[i].x1), plevel->inclined_plane_r[i].x1 = plevel->inclined_plane_r[i].x1 * plevel->brick_len + plevel->left_border;
    for(int i = 0; i < plevel->inclined_plane_count_r; ++i)
        fscanf(fileptr, "%d", &plevel->inclined_plane_r[i].x2), plevel->inclined_plane_r[i].x2 = plevel->inclined_plane_r[i].x2 * plevel->brick_len + plevel->left_border;
    for(int i = 0; i < plevel->inclined_plane_count_r; ++i)
        fscanf(fileptr, "%d", &plevel->inclined_plane_r[i].y1), plevel->inclined_plane_r[i].y1 = plevel->inclined_plane_r[i].y1 * plevel->brick_len;
    for(int i = 0; i < plevel->inclined_plane_count_r; ++i)
        fscanf(fileptr, "%d", &plevel->inclined_plane_r[i].y2), plevel->inclined_plane_r[i].y2 = plevel->inclined_plane_r[i].y2 * plevel->brick_len;




    fscanf(fileptr, "%d", &plevel->water_count);
    for(int i = 0; i < plevel->water_count; ++i) {
        fscanf(fileptr, "%d %d %d %d %d %d %d %d", &plevel->waters[i].x1, &plevel->waters[i].y1, &plevel->waters[i].x2, &plevel->waters[i].y2, &plevel->waters[i].x3, &plevel->waters[i].y3, &plevel->waters[i].x4, &plevel->waters[i].y4);
        plevel->waters[i].x1 = plevel->waters[i].x1 * plevel->brick_len + plevel->left_border;
        plevel->waters[i].x2 = plevel->waters[i].x2 * plevel->brick_len + plevel->left_border;
        plevel->waters[i].x3 = plevel->waters[i].x3 * plevel->brick_len + plevel->left_border;
        plevel->waters[i].x4 = plevel->waters[i].x4 * plevel->brick_len + plevel->left_border;
        plevel->waters[i].y1 *= plevel->brick_len;
        plevel->waters[i].y2 *= plevel->brick_len;
        plevel->waters[i].y3 *= plevel->brick_len;
        plevel->waters[i].y4 *= plevel->brick_len;
    }
    fscanf(fileptr, "%d %d %d %d", &plevel->no_coll_l_obs_count, &plevel->no_coll_r_obs_count, &plevel->no_coll_t_obs_count, &plevel->no_coll_b_obs_count);
    for(int i = 0; i < plevel->no_coll_l_obs_count; ++i) {
        int x;
        fscanf(fileptr, "%d", &x);
        plevel->left_collision[x] = 1;
    }

    for(int i = 0; i < plevel->no_coll_r_obs_count; ++i) {
        int x;
        fscanf(fileptr, "%d", &x);
        plevel->right_collision[x] = 1;
    }

    for(int i = 0; i < plevel->no_coll_t_obs_count; ++i) {
        int x;
        fscanf(fileptr, "%d", &x);
        plevel->top_collision[x] = 1;
    }

    for(int i = 0; i < plevel->no_coll_b_obs_count; ++i) {
        int x;
        fscanf(fileptr, "%d", &x);
        plevel->bottom_collision[x] = 1;
    }

    fscanf(fileptr, "%d", &plevel->blue_obs_count);
    for(int i = 0; i < plevel->blue_obs_count; ++i)
        fscanf(fileptr, "%d", &plevel->row_of_blue_obs[i]);
    for(int i = 0; i < plevel->blue_obs_count; ++i)
        fscanf(fileptr, "%d", &plevel->column_of_blue_obs[i]);
    for(int i = 0; i < plevel->blue_obs_count; ++i)
        fscanf(fileptr, "%d", &plevel->tile_count_from_left_blue_obs[i]);
    for(int i = 0; i < plevel->blue_obs_count; ++i)
        fscanf(fileptr, "%d", &plevel->tile_count_from_bottom_blue_obs[i]);



    fscanf(fileptr, "%d", &plevel->pumper_count);
    for(int i = 0; i < plevel->pumper_count; ++i)
        fscanf(fileptr, "%d", &plevel->x_of_pumper[i]);
    for(int i = 0; i < plevel->pumper_count; ++i)
        fscanf(fileptr, "%d", &plevel->y_of_pumper[i]);

    fscanf(fileptr, "%d", &plevel->power_speed_count);
    for(int i = 0; i < plevel->power_speed_count; ++i)
        fscanf(fileptr, "%d", &plevel->x_of_power_speed[i]);
    for(int i = 0; i < plevel->power_speed_count; ++i)
        fscanf(fileptr, "%d", &plevel->y_of_power_speed[i]);

    fscanf(fileptr, "%d", &plevel->deflater_count);
    for(int i = 0; i < plevel->deflater_count; ++i)
        fscanf(fileptr, "%d", &plevel->x_of_deflater[i]);
    for(int i = 0; i < plevel->deflater_count; ++i)
        fscanf(fileptr, "%d", &plevel->y_of_deflater[i]);

    fscanf(fileptr, "%d", &plevel->inverse_thorn_count);
    for(int i = 0; i < plevel->inverse_thorn_count; ++i)
        fscanf(fileptr, "%d", &plevel->x_of_inverse_thorn[i]);
    for(int i = 0; i < plevel->inverse_thorn_count; ++i)
        fscanf(fileptr, "%d", &plevel->y_of_inverse_thorn[i]);


    fclose(fileptr);
}

void getKeys(level* plevel) {
    if(GetAsyncKeyState(VK_UP))
        if(plevel->grounded)
            plevel->vy = 15 * plevel->grounded;
    if(GetAsyncKeyState(VK_RIGHT)) {
        plevel->vx = -my_v;
    } else if(GetAsyncKeyState(VK_LEFT)) {
        plevel->vx = my_v;
    } else if(!GetAsyncKeyState(VK_RIGHT) && !GetAsyncKeyState(VK_LEFT))
        plevel->vx = 0;
}



void draw_offset() {
    iSetColor(255,255,255);
    iFilledRectangle(0,0,100,SCREEN_HEIGHT);
    iFilledRectangle(SCREEN_WIDTH - 100,0,100,SCREEN_HEIGHT);
}

void move_obs(int new_vx, level *plevel, int new_vy) {
    for(int obs_no = 0; obs_no < plevel->obstacle_count; obs_no++) {
        for(int i = 0; i < plevel->row_of_obs[obs_no]; ++i) {
            for(int j = 0; j < plevel->column_of_obs[obs_no]; ++j) {
                plevel->obs[obs_no][i][j].x += new_vx;
                plevel->obs[obs_no][i][j].y += new_vy;
            }
        }

        plevel->grounds[obs_no].x1 += new_vx;
        plevel->ceilings[obs_no].x1 +=new_vx;
        plevel->grounds[obs_no].x2 +=new_vx;
        plevel->ceilings[obs_no].x2 += new_vx;

        plevel->grounds[obs_no].y1 += new_vy;
        plevel->ceilings[obs_no].y1 +=new_vy;
        plevel->grounds[obs_no].y2 +=new_vy;
        plevel->ceilings[obs_no].y2 += new_vy;

    }


    for(int blue_obs_no = 0; blue_obs_no < plevel->blue_obs_count; blue_obs_no++) {
        for(int i = 0; i < plevel->row_of_blue_obs[blue_obs_no]; ++i) {
            for(int j = 0; j < plevel->column_of_blue_obs[blue_obs_no]; ++j) {
                plevel->blue_obs[blue_obs_no][i][j].x += new_vx;
                plevel->blue_obs[blue_obs_no][i][j].y += new_vy;
            }
        }


        plevel->grounds_blue[blue_obs_no].x1 += new_vx;
        plevel->ceilings_blue[blue_obs_no].x1 +=new_vx;
        plevel->grounds_blue[blue_obs_no].x2 +=new_vx;
        plevel->ceilings_blue[blue_obs_no].x2 += new_vx;

        plevel->grounds_blue[blue_obs_no].y1 += new_vy;
        plevel->ceilings_blue[blue_obs_no].y1 +=new_vy;
        plevel->grounds_blue[blue_obs_no].y2 +=new_vy;
        plevel->ceilings_blue[blue_obs_no].y2 += new_vy;
    }

}

void move_ring(int new_vx, level *plevel, int new_vy) {
    for(int i = 0; i < plevel->ring_count; ++i) {
        plevel->rings[i].x += new_vx;
        plevel->rings[i].y += new_vy;
    }
}

void move_xring(int new_vx, level *plevel, int new_vy) {
    for(int i = 0; i < plevel->xring_count; ++i) {
        plevel->xrings[i].x += new_vx;
        plevel->xrings[i].y += new_vy;
    }
}


void move_thorn(int new_vx, level *plevel, int new_vy) {
    for(int thorn_no = 0; thorn_no < plevel->thorn_count; ++thorn_no) {
        plevel->thorns[thorn_no].x += new_vx;
        plevel->grounds_thorn[thorn_no].x1 += new_vx;
        plevel->ceilings_thorn[thorn_no].x1 +=new_vx;
        plevel->grounds_thorn[thorn_no].x2 +=new_vx;
        plevel->ceilings_thorn[thorn_no].x2 += new_vx;

        plevel->thorns[thorn_no].y += new_vy;
        plevel->grounds_thorn[thorn_no].y1 += new_vy;
        plevel->ceilings_thorn[thorn_no].y1 +=new_vy;
        plevel->grounds_thorn[thorn_no].y2 +=new_vy;
        plevel->ceilings_thorn[thorn_no].y2 += new_vy;
    }

    for(int inverse_thorn_no = 0; inverse_thorn_no < plevel->inverse_thorn_count; ++inverse_thorn_no) {
        plevel->inverse_thorns[inverse_thorn_no].x += new_vx;
        plevel->grounds_inverse_thorn[inverse_thorn_no].x1 += new_vx;
        plevel->ceilings_inverse_thorn[inverse_thorn_no].x1 +=new_vx;
        plevel->grounds_inverse_thorn[inverse_thorn_no].x2 +=new_vx;
        plevel->ceilings_inverse_thorn[inverse_thorn_no].x2 += new_vx;

        plevel->inverse_thorns[inverse_thorn_no].y += new_vy;
        plevel->grounds_inverse_thorn[inverse_thorn_no].y1 += new_vy;
        plevel->ceilings_inverse_thorn[inverse_thorn_no].y1 +=new_vy;
        plevel->grounds_inverse_thorn[inverse_thorn_no].y2 +=new_vy;
        plevel->ceilings_inverse_thorn[inverse_thorn_no].y2 += new_vy;
    }

}

void move_pumper(int new_vx, level *plevel, int new_vy) {
    for(int pumper_no = 0; pumper_no < plevel->pumper_count; ++pumper_no) {
        plevel->pumpers[pumper_no].x += new_vx;
        plevel->grounds_pumper[pumper_no].x1 += new_vx;
        plevel->ceilings_pumper[pumper_no].x1 +=new_vx;
        plevel->grounds_pumper[pumper_no].x2 +=new_vx;
        plevel->ceilings_pumper[pumper_no].x2 += new_vx;

        plevel->pumpers[pumper_no].y += new_vy;
        plevel->grounds_pumper[pumper_no].y1 += new_vy;
        plevel->ceilings_pumper[pumper_no].y1 +=new_vy;
        plevel->grounds_pumper[pumper_no].y2 +=new_vy;
        plevel->ceilings_pumper[pumper_no].y2 += new_vy;
    }
}

void move_power_speed(int new_vx, level *plevel, int new_vy) {
    for(int power_speed_no = 0; power_speed_no < plevel->power_speed_count; ++power_speed_no) {
        plevel->power_speeds[power_speed_no].x += new_vx;
        plevel->grounds_power_speed[power_speed_no].x1 += new_vx;
        plevel->ceilings_power_speed[power_speed_no].x1 +=new_vx;
        plevel->grounds_power_speed[power_speed_no].x2 +=new_vx;
        plevel->ceilings_power_speed[power_speed_no].x2 += new_vx;

        plevel->power_speeds[power_speed_no].y += new_vy;
        plevel->grounds_power_speed[power_speed_no].y1 += new_vy;
        plevel->ceilings_power_speed[power_speed_no].y1 +=new_vy;
        plevel->grounds_power_speed[power_speed_no].y2 +=new_vy;
        plevel->ceilings_power_speed[power_speed_no].y2 += new_vy;
    }
}

void move_deflater(int new_vx, level *plevel, int new_vy) {
    for(int deflater_no = 0; deflater_no < plevel->deflater_count; ++deflater_no) {
        plevel->deflaters[deflater_no].x += new_vx;
        plevel->grounds_deflater[deflater_no].x1 += new_vx;
        plevel->ceilings_deflater[deflater_no].x1 +=new_vx;
        plevel->grounds_deflater[deflater_no].x2 +=new_vx;
        plevel->ceilings_deflater[deflater_no].x2 += new_vx;

        plevel->deflaters[deflater_no].y += new_vy;
        plevel->grounds_deflater[deflater_no].y1 += new_vy;
        plevel->ceilings_deflater[deflater_no].y1 +=new_vy;
        plevel->grounds_deflater[deflater_no].y2 +=new_vy;
        plevel->ceilings_deflater[deflater_no].y2 += new_vy;
    }
}


void move_life(int new_vx, level *plevel, int new_vy) {
    for(int i = 0; i < plevel->life_count; ++i) {
        plevel->lifes[i].x += new_vx;
        plevel->lifes[i].y += new_vy;
    }
}

void move_inclined_bricks(int new_vx, level *plevel, int new_vy) {
    for(int i = 0; i < plevel->inclined_brick_count_1; ++i) {
        plevel->inclined_bricks_1[i].x += new_vx;
        plevel->inclined_bricks_1[i].y += new_vy;
    }

    for(int i = 0; i < plevel->inclined_brick_count_2; ++i) {
        plevel->inclined_bricks_2[i].x += new_vx;
        plevel->inclined_bricks_2[i].y += new_vy;
    }

}

void move_checkpoint(int new_vx, level *plevel, int new_vy) {
    for(int i = 0; i < plevel->checkpoint_count; ++i) {
        plevel->checkpoints[i].x += new_vx;
        plevel->checkpoints[i].y += new_vy;
    }
    plevel->last_checkpoint.x += new_vx;
    plevel->last_checkpoint.y += new_vy;
}

void move_inclined_plane(int new_vx, level *plevel, int new_vy) {
    for(int i = 0; i < plevel->inclined_plane_count_l; ++i) {
        plevel->inclined_plane_l[i].x1 += new_vx;
        plevel->inclined_plane_l[i].x2 += new_vx;
        plevel->inclined_plane_l[i].y1 += new_vy;
        plevel->inclined_plane_l[i].y2 += new_vy;
    }
    for(int i = 0; i < plevel->inclined_plane_count_r; ++i) {
        plevel->inclined_plane_r[i].x1 += new_vx;
        plevel->inclined_plane_r[i].x2 += new_vx;
        plevel->inclined_plane_r[i].y1 += new_vy;
        plevel->inclined_plane_r[i].y2 += new_vy;
    }

}

void move_dyn_thorn(int new_vx, level *plevel, int new_vy) {
    for(int dyn_thorn_no = 0; dyn_thorn_no < plevel->dyn_thorn_count; ++dyn_thorn_no) {
        plevel->dyn_thorns[dyn_thorn_no].x += new_vx;
        plevel->grounds_dyn_thorn[dyn_thorn_no].x1 += new_vx;
        plevel->ceilings_dyn_thorn[dyn_thorn_no].x1 +=new_vx;
        plevel->grounds_dyn_thorn[dyn_thorn_no].x2 +=new_vx;
        plevel->ceilings_dyn_thorn[dyn_thorn_no].x2 += new_vx;
        plevel->dyn_thorns[dyn_thorn_no].left += new_vx;
        plevel->dyn_thorns[dyn_thorn_no].right += new_vx;

        plevel->dyn_thorns[dyn_thorn_no].y += new_vy;
        plevel->grounds_dyn_thorn[dyn_thorn_no].y1 += new_vy;
        plevel->ceilings_dyn_thorn[dyn_thorn_no].y1 +=new_vy;
        plevel->grounds_dyn_thorn[dyn_thorn_no].y2 +=new_vy;
        plevel->ceilings_dyn_thorn[dyn_thorn_no].y2 += new_vy;
        plevel->dyn_thorns[dyn_thorn_no].up += new_vy;
        plevel->dyn_thorns[dyn_thorn_no].down += new_vy;
    }
}

void move_water(int new_vx, level *plevel, int new_vy) {
    for(int water_no = 0; water_no < plevel->water_count; ++water_no) {
        plevel->waters[water_no].x1 += new_vx;
        plevel->waters[water_no].x2 += new_vx;
        plevel->waters[water_no].x3 += new_vx;
        plevel->waters[water_no].x4 += new_vx;
        plevel->waters[water_no].y1 += new_vy;
        plevel->waters[water_no].y2 += new_vy;
        plevel->waters[water_no].y3 += new_vy;
        plevel->waters[water_no].y4 += new_vy;
        plevel->ceilings_water[water_no].x1 += new_vx;
        plevel->ceilings_water[water_no].x2 += new_vx;
        plevel->ceilings_water[water_no].y1 += new_vy;
        plevel->ceilings_water[water_no].y2 += new_vy;
    }
}

void dyn_thorn_move(level *plevel) {

    for(int dyn_thorn_no = 0; dyn_thorn_no < plevel->dyn_thorn_count; ++dyn_thorn_no) {
        if(plevel->dyn_thorns[dyn_thorn_no].vx > 0 && plevel->dyn_thorns[dyn_thorn_no].x + plevel->brick_len * 2 > plevel->dyn_thorns[dyn_thorn_no].right) {
            plevel->dyn_thorns[dyn_thorn_no].vx *= -1;
        } else if (plevel->dyn_thorns[dyn_thorn_no].vx < 0 && plevel->dyn_thorns[dyn_thorn_no].x < plevel->dyn_thorns[dyn_thorn_no].left) {
            plevel->dyn_thorns[dyn_thorn_no].vx *= -1;
        } else if(plevel->dyn_thorns[dyn_thorn_no].vy > 0 && plevel->dyn_thorns[dyn_thorn_no].y + plevel->brick_len * 2 > plevel->dyn_thorns[dyn_thorn_no].up)
            plevel->dyn_thorns[dyn_thorn_no].vy *= -1;
        else if(plevel->dyn_thorns[dyn_thorn_no].vy < 0 && plevel->dyn_thorns[dyn_thorn_no].y < plevel->dyn_thorns[dyn_thorn_no].down)
            plevel->dyn_thorns[dyn_thorn_no].vy *= -1;
        plevel->dyn_thorns[dyn_thorn_no].x += plevel->dyn_thorns[dyn_thorn_no].vx;
        plevel->grounds_dyn_thorn[dyn_thorn_no].x1 += plevel->dyn_thorns[dyn_thorn_no].vx;
        plevel->ceilings_dyn_thorn[dyn_thorn_no].x1 += plevel->dyn_thorns[dyn_thorn_no].vx;
        plevel->grounds_dyn_thorn[dyn_thorn_no].x2 += plevel->dyn_thorns[dyn_thorn_no].vx;
        plevel->ceilings_dyn_thorn[dyn_thorn_no].x2 += plevel->dyn_thorns[dyn_thorn_no].vx;
        plevel->dyn_thorns[dyn_thorn_no].y += plevel->dyn_thorns[dyn_thorn_no].vy;
        plevel->grounds_dyn_thorn[dyn_thorn_no].y1 += plevel->dyn_thorns[dyn_thorn_no].vy;
        plevel->ceilings_dyn_thorn[dyn_thorn_no].y1 += plevel->dyn_thorns[dyn_thorn_no].vy;
        plevel->grounds_dyn_thorn[dyn_thorn_no].y2 += plevel->dyn_thorns[dyn_thorn_no].vy;
        plevel->ceilings_dyn_thorn[dyn_thorn_no].y2 += plevel->dyn_thorns[dyn_thorn_no].vy;
    }

}

void moveAll(int new_vx, level *plevel, int new_vy) {
    move_obs(new_vx, plevel, new_vy);
    move_ring(new_vx, plevel, new_vy);
    move_xring(new_vx, plevel, new_vy);
    move_thorn(new_vx, plevel, new_vy);
    move_dyn_thorn(new_vx, plevel, new_vy);
    move_life(new_vx, plevel, new_vy);
    move_checkpoint(new_vx, plevel, new_vy);
    move_inclined_bricks(new_vx, plevel, new_vy);
    move_inclined_plane(new_vx, plevel, new_vy);
    move_water(new_vx, plevel, new_vy);
    move_power_speed(new_vx, plevel, new_vy);
    move_pumper(new_vx, plevel, new_vy);
    move_deflater(new_vx, plevel, new_vy);
}

void collision_detection(level *plevel) {

    for(int i = 0; i < plevel->obstacle_count; ++i) {
        if(!plevel->left_collision[i] && plevel->ceilings[i].y1 < plevel->y + plevel->rad && plevel->y - plevel->rad < plevel->grounds[i].y1 && plevel->vx < 0 && plevel->ceilings[i].x1 < (SCREEN_WIDTH/2 + plevel->rad) && (SCREEN_WIDTH/2 + plevel->rad) - plevel->ceilings[i].x1 <=plevel->rad) {
            int diff = (SCREEN_WIDTH/2 + plevel->rad) - plevel->ceilings[i].x1;
            moveAll(diff, plevel, 0);

        }

        if(!plevel->right_collision[i] && plevel->ceilings[i].y1 < plevel->y + plevel->rad && plevel->y - plevel->rad < plevel->grounds[i].y1 && plevel->vx > 0 && plevel->ceilings[i].x2 > (SCREEN_WIDTH/2 - plevel->rad) && plevel->ceilings[i].x2 - (SCREEN_WIDTH/2 - plevel->rad) <=plevel->rad) {

            int diff = plevel->ceilings[i].x2 - (SCREEN_WIDTH/2 - plevel->rad);
            moveAll(-diff, plevel, 0);

        }

        if(!plevel->bottom_collision[i] && plevel->ceilings[i].x1 < (SCREEN_WIDTH/2 + plevel->rad) && plevel->ceilings[i].x2 > (SCREEN_WIDTH/2 - plevel->rad) && plevel->vy>0 && plevel->y + plevel->rad > plevel->ceilings[i].y1 && (plevel->y + plevel->rad) - plevel->ceilings[i].y1 <=plevel->rad) {
            plevel->y = plevel->ceilings[i].y1 - plevel->rad;
            plevel->vy = 0;
        }
        if(!plevel->top_collision[i] && plevel->grounds[i].x1 < (SCREEN_WIDTH/2 + plevel->rad) && plevel->grounds[i].x2 > (SCREEN_WIDTH/2 - plevel->rad) && plevel->vy<0 && (plevel->grounds[i].y1 > (plevel->y - plevel->rad)) && plevel->grounds[i].y1 - (plevel->y - plevel->rad) <=plevel->rad) {

            plevel->vy = 0;
            plevel->y = plevel->grounds[i].y1 + plevel->rad;
            plevel->grounded = 1;
        }
    }


    for(int i = 0; i < plevel->blue_obs_count; ++i) {
        if(plevel->ceilings_blue[i].y1 < plevel->y + plevel->rad && plevel->y - plevel->rad < plevel->grounds_blue[i].y1 && plevel->vx < 0 && plevel->ceilings_blue[i].x1 < (SCREEN_WIDTH/2 + plevel->rad) && (SCREEN_WIDTH/2 + plevel->rad) - plevel->ceilings_blue[i].x1 <=plevel->rad) {
            int diff = (SCREEN_WIDTH/2 + plevel->rad) - plevel->ceilings_blue[i].x1;
            moveAll(diff, plevel, 0);
            //printf("%d\n", i);
        }

        if(plevel->ceilings_blue[i].y1 < plevel->y + plevel->rad && plevel->y - plevel->rad < plevel->grounds_blue[i].y1 && plevel->vx > 0 && plevel->ceilings_blue[i].x2 > (SCREEN_WIDTH/2 - plevel->rad) && plevel->ceilings_blue[i].x2 - (SCREEN_WIDTH/2 - plevel->rad) <=plevel->rad) {
            int diff = plevel->ceilings_blue[i].x2 - (SCREEN_WIDTH/2 - plevel->rad);
            moveAll(-diff, plevel, 0);

        }

        if(plevel->ceilings_blue[i].x1 < (SCREEN_WIDTH/2 + plevel->rad) && plevel->ceilings_blue[i].x2 > (SCREEN_WIDTH/2 - plevel->rad) && plevel->vy>0 && plevel->y + plevel->rad > plevel->ceilings_blue[i].y1 && (plevel->y + plevel->rad) - plevel->ceilings_blue[i].y1 <=plevel->rad) {
            plevel->y = plevel->ceilings_blue[i].y1 - plevel->rad;
            plevel->vy = 0;
        }
        if(plevel->grounds_blue[i].x1 < (SCREEN_WIDTH/2 + plevel->rad) && plevel->grounds_blue[i].x2 > (SCREEN_WIDTH/2 - plevel->rad) && plevel->vy<0 && (plevel->grounds_blue[i].y1 > (plevel->y - plevel->rad)) && plevel->grounds_blue[i].y1 - (plevel->y - plevel->rad) <=plevel->rad) {

            plevel->vy = 0;
            plevel->y = plevel->grounds_blue[i].y1 + plevel->rad;
            plevel->grounded = 2;
        }
    }



    for(int i = 0; i < plevel->life_count; ++i) {
        if(!plevel->lifes[i].visible)
            continue;

        int c_l_x, c_l_y, c_b_x, c_b_y;
        c_l_x = plevel->lifes[i].x + plevel->brick_len/2;
        c_l_y = plevel->lifes[i].y + plevel->brick_len/2;
        c_b_x = SCREEN_WIDTH/2;
        c_b_y = plevel->y;

        if (c_l_x < 0 or c_l_x > SCREEN_WIDTH)
            continue;
        int sq_dist = (c_l_x - c_b_x) * (c_l_x - c_b_x) + (c_l_y - c_b_y) * (c_l_y - c_b_y);
        if(sq_dist <= (plevel->rad+plevel->brick_len/2) * (plevel->rad+plevel->brick_len/2)) {
            plevel->lifes[i].visible = 0;
            plevel->remaining_life++;
            total_score += 10;
            //printf("%d\n",plevel->remaining_life);
        }
    }

    for(int i = 0; i < plevel->checkpoint_count; ++i) {
        if(!plevel->checkpoints[i].visible)
            continue;

        int c_l_x, c_l_y, c_b_x, c_b_y;
        c_l_x = plevel->checkpoints[i].x + plevel->brick_len/2;
        c_l_y = plevel->checkpoints[i].y + plevel->brick_len/2;
        c_b_x = SCREEN_WIDTH/2;
        c_b_y = plevel->y;

        if (c_l_x < 0 or c_l_x > SCREEN_WIDTH)
            continue;
        int sq_dist = (c_l_x - c_b_x) * (c_l_x - c_b_x) + (c_l_y - c_b_y) * (c_l_y - c_b_y);
        if(sq_dist <= (plevel->rad+plevel->brick_len/2) * (plevel->rad+plevel->brick_len/2)) {
            plevel->checkpoints[i].visible = 0;
            plevel->last_checkpoint = plevel->checkpoints[i];
            plevel->remaining_checkpoint++;
            total_score += 5;
            //printf("%d",plevel->remaining_checkpoint);
        }
    }
    for (int i=0; i<plevel->ring_count; ++i) {
        if (plevel->rings[i].crossed)
            continue;
        if (plevel->rings[i].x < 0 || plevel->rings[i].x > SCREEN_WIDTH)
            continue;

        if (((plevel->vx > 0 && abs(SCREEN_WIDTH/2 - plevel->rings[i].x) <= plevel->rad) || (plevel->vx < 0 && abs(plevel->rings[i].x - SCREEN_WIDTH/2) <= 20)) && plevel->y > plevel->rings[i].y && plevel->y -plevel->rings[i].y < 100) {
            plevel->rings[i].crossed = 1;
            plevel->rings_crossed+=1;
            //printf("%d\n", plevel->rings_crossed);
        }
    }

    for (int i=0; i<plevel->xring_count; ++i) {
        if (plevel->xrings[i].crossed)
            continue;
        if (plevel->xrings[i].x < 0 || plevel->xrings[i].x > SCREEN_WIDTH)
            continue;

        if (plevel->xrings[i].x <= SCREEN_WIDTH/2 && plevel->xrings[i].x + 100 >= SCREEN_WIDTH/2 && ((plevel->vy > 0 && abs(plevel->xrings[i].y - plevel->y) <= 20) || (plevel->vy <0 && abs(plevel->y - plevel->xrings[i].y) <= plevel->rad))) {
            plevel->xrings[i].crossed = 1;
            plevel->rings_crossed+=1;
            //printf("%d\n", plevel->rings_crossed);
        }
    }


    for(int i = 0; i < plevel->thorn_count; ++i) {

        //printf("%d\n", ( (abs) ((int)plevel->y - plevel->last_checkpoint.y) / SCREEN_HEIGHT - plevel->brick_len) * SCREEN_HEIGHT - plevel->brick_len);

        if(plevel->ceilings_thorn[i].y1 < plevel->y + plevel->rad && plevel->y - plevel->rad < plevel->grounds_thorn[i].y1 && plevel->vx < 0 && plevel->ceilings_thorn[i].x1 < (SCREEN_WIDTH/2 + plevel->rad) && (SCREEN_WIDTH/2 + plevel->rad) - plevel->ceilings_thorn[i].x1 <plevel->rad) {
            int diff = (SCREEN_WIDTH/2 + plevel->rad) - plevel->ceilings_thorn[i].x1;
            moveAll(diff, plevel, 0);

            if(plevel->cheat_code_mode)
                continue;
            while (plevel->last_checkpoint.y <0)
                moveAll(0, plevel, SCREEN_HEIGHT - plevel->brick_len);
            while (plevel->last_checkpoint.y >SCREEN_HEIGHT - plevel->brick_len)
                moveAll(0, plevel, -(SCREEN_HEIGHT - plevel->brick_len));
            moveAll((SCREEN_WIDTH/2 - plevel->last_checkpoint.x - 25), plevel, 0);
            plevel->y = 300;
            plevel->remaining_life--;
            if(total_score > 10) total_score -= 10;
        }

        if(plevel->ceilings_thorn[i].y1 < plevel->y + plevel->rad && plevel->y - plevel->rad < plevel->grounds_thorn[i].y1 && plevel->vx > 0 && plevel->ceilings_thorn[i].x2 > (SCREEN_WIDTH/2 - plevel->rad) && plevel->ceilings_thorn[i].x2 - (SCREEN_WIDTH/2 - plevel->rad) <=plevel->rad) {
            int diff = plevel->ceilings_thorn[i].x2 - (SCREEN_WIDTH/2 - plevel->rad);
            moveAll(-diff, plevel, 0);
            if(plevel->cheat_code_mode)
                continue;
            while (plevel->last_checkpoint.y <0)
                moveAll(0, plevel, SCREEN_HEIGHT - plevel->brick_len);
            while (plevel->last_checkpoint.y >SCREEN_HEIGHT - plevel->brick_len)
                moveAll(0, plevel, -(SCREEN_HEIGHT - plevel->brick_len));
            moveAll((SCREEN_WIDTH/2 - plevel->last_checkpoint.x - 25), plevel, 0);
            plevel->y = 300;
            plevel->remaining_life--;
            if(total_score > 10) total_score -= 10;
        }

        if(plevel->ceilings_thorn[i].x1 < (SCREEN_WIDTH/2 + plevel->rad) && plevel->ceilings_thorn[i].x2 > (SCREEN_WIDTH/2 - plevel->rad) && plevel->vy>0 && plevel->y + plevel->rad > plevel->ceilings_thorn[i].y1 && (plevel->y + plevel->rad) - plevel->ceilings_thorn[i].y1 <=plevel->rad) {
            plevel->y = plevel->ceilings_thorn[i].y1 - plevel->rad;
            plevel->vy = 0;
            if(plevel->cheat_code_mode)
                continue;
            while (plevel->last_checkpoint.y <0)
                moveAll(0, plevel, SCREEN_HEIGHT - plevel->brick_len);
            while (plevel->last_checkpoint.y >SCREEN_HEIGHT - plevel->brick_len)
                moveAll(0, plevel, -(SCREEN_HEIGHT - plevel->brick_len));
            moveAll((SCREEN_WIDTH/2 - plevel->last_checkpoint.x - 25), plevel, 0);
            plevel->y = 300;
            plevel->remaining_life--;
            if(total_score > 10) total_score -= 10;
        }
        if(plevel->grounds_thorn[i].x1 < (SCREEN_WIDTH/2 + plevel->rad) && plevel->grounds_thorn[i].x2 > (SCREEN_WIDTH/2 - plevel->rad) && plevel->vy<0 && (plevel->grounds_thorn[i].y1 > (plevel->y - plevel->rad)) && plevel->grounds_thorn[i].y1 - (plevel->y - plevel->rad) <=plevel->rad) {
            plevel->vy = 0;
            plevel->y = plevel->grounds_thorn[i].y1 + plevel->rad;
            plevel->grounded = 1;
            if(plevel->cheat_code_mode)
                continue;
            while (plevel->last_checkpoint.y <0)
                moveAll(0, plevel, SCREEN_HEIGHT - plevel->brick_len);
            while (plevel->last_checkpoint.y >SCREEN_HEIGHT - plevel->brick_len)
                moveAll(0, plevel, -(SCREEN_HEIGHT - plevel->brick_len));
            moveAll((SCREEN_WIDTH/2 - plevel->last_checkpoint.x - 25), plevel, 0);
            plevel->y = 300;
            plevel->remaining_life--;
            if(total_score > 10) total_score -= 10;
        }

    }

    for(int i = 0; i < plevel->inverse_thorn_count; ++i) {

        //printf("%d\n", ( (abs) ((int)plevel->y - plevel->last_checkpoint.y) / SCREEN_HEIGHT - plevel->brick_len) * SCREEN_HEIGHT - plevel->brick_len);

        if(plevel->ceilings_inverse_thorn[i].y1 < plevel->y + plevel->rad && plevel->y - plevel->rad < plevel->grounds_inverse_thorn[i].y1 && plevel->vx < 0 && plevel->ceilings_inverse_thorn[i].x1 < (SCREEN_WIDTH/2 + plevel->rad) && (SCREEN_WIDTH/2 + plevel->rad) - plevel->ceilings_inverse_thorn[i].x1 <plevel->rad) {
            int diff = (SCREEN_WIDTH/2 + plevel->rad) - plevel->ceilings_inverse_thorn[i].x1;
            moveAll(diff, plevel, 0);
            if(plevel->cheat_code_mode)
                continue;
            while (plevel->last_checkpoint.y <0)
                moveAll(0, plevel, SCREEN_HEIGHT - plevel->brick_len);
            while (plevel->last_checkpoint.y >SCREEN_HEIGHT - plevel->brick_len)
                moveAll(0, plevel, -(SCREEN_HEIGHT - plevel->brick_len));
            moveAll((SCREEN_WIDTH/2 - plevel->last_checkpoint.x - 25), plevel, 0);
            plevel->y = 300;
            plevel->remaining_life--;
            if(total_score > 10) total_score -= 10;
        }

        if(plevel->ceilings_inverse_thorn[i].y1 < plevel->y + plevel->rad && plevel->y - plevel->rad < plevel->grounds_inverse_thorn[i].y1 && plevel->vx > 0 && plevel->ceilings_inverse_thorn[i].x2 > (SCREEN_WIDTH/2 - plevel->rad) && plevel->ceilings_inverse_thorn[i].x2 - (SCREEN_WIDTH/2 - plevel->rad) <=plevel->rad) {
            int diff = plevel->ceilings_inverse_thorn[i].x2 - (SCREEN_WIDTH/2 - plevel->rad);
            moveAll(-diff, plevel, 0);
            if(plevel->cheat_code_mode)
                continue;
            while (plevel->last_checkpoint.y <0)
                moveAll(0, plevel, SCREEN_HEIGHT - plevel->brick_len);
            while (plevel->last_checkpoint.y >SCREEN_HEIGHT - plevel->brick_len)
                moveAll(0, plevel, -(SCREEN_HEIGHT - plevel->brick_len));
            moveAll((SCREEN_WIDTH/2 - plevel->last_checkpoint.x - 25), plevel, 0);
            plevel->y = 300;
            plevel->remaining_life--;
            if(total_score > 10) total_score -= 10;
        }

        if(plevel->ceilings_inverse_thorn[i].x1 < (SCREEN_WIDTH/2 + plevel->rad) && plevel->ceilings_inverse_thorn[i].x2 > (SCREEN_WIDTH/2 - plevel->rad) && plevel->vy>0 && plevel->y + plevel->rad > plevel->ceilings_inverse_thorn[i].y1 && (plevel->y + plevel->rad) - plevel->ceilings_inverse_thorn[i].y1 <=plevel->rad) {
            plevel->y = plevel->ceilings_inverse_thorn[i].y1 - plevel->rad;
            plevel->vy = 0;
            if(plevel->cheat_code_mode)
                continue;
            while (plevel->last_checkpoint.y <0)
                moveAll(0, plevel, SCREEN_HEIGHT - plevel->brick_len);
            while (plevel->last_checkpoint.y >SCREEN_HEIGHT - plevel->brick_len)
                moveAll(0, plevel, -(SCREEN_HEIGHT - plevel->brick_len));
            moveAll((SCREEN_WIDTH/2 - plevel->last_checkpoint.x - 25), plevel, 0);
            plevel->y = 300;
            plevel->remaining_life--;
            if(total_score > 10) total_score -= 10;
        }
        if(plevel->grounds_inverse_thorn[i].x1 < (SCREEN_WIDTH/2 + plevel->rad) && plevel->grounds_inverse_thorn[i].x2 > (SCREEN_WIDTH/2 - plevel->rad) && plevel->vy<0 && (plevel->grounds_inverse_thorn[i].y1 > (plevel->y - plevel->rad)) && plevel->grounds_inverse_thorn[i].y1 - (plevel->y - plevel->rad) <=plevel->rad) {
            plevel->vy = 0;
            plevel->y = plevel->grounds_inverse_thorn[i].y1 + plevel->rad;
            if(plevel->cheat_code_mode)
                continue;
            while (plevel->last_checkpoint.y <0)
                moveAll(0, plevel, SCREEN_HEIGHT - plevel->brick_len);
            while (plevel->last_checkpoint.y >SCREEN_HEIGHT - plevel->brick_len)
                moveAll(0, plevel, -(SCREEN_HEIGHT - plevel->brick_len));
            moveAll((SCREEN_WIDTH/2 - plevel->last_checkpoint.x - 25), plevel, 0);
            plevel->y = 300;
            plevel->remaining_life--;
            if(total_score > 10) total_score -= 10;
        }

    }


    for(int i = 0; i < plevel->deflater_count; ++i) {


        if(plevel->ceilings_deflater[i].y1 < plevel->y + plevel->rad && plevel->y - plevel->rad < plevel->grounds_deflater[i].y1 && plevel->vx < 0 && plevel->ceilings_deflater[i].x1 < (SCREEN_WIDTH/2 + plevel->rad) && (SCREEN_WIDTH/2 + plevel->rad) - plevel->ceilings_deflater[i].x1 <plevel->rad) {
            int diff = (SCREEN_WIDTH/2 + plevel->rad) - plevel->ceilings_deflater[i].x1;
            moveAll(diff, plevel, 0);
            plevel->water_acc_value = 0;
        }

        if(plevel->ceilings_deflater[i].y1 < plevel->y + plevel->rad && plevel->y - plevel->rad < plevel->grounds_deflater[i].y1 && plevel->vx > 0 && plevel->ceilings_deflater[i].x2 > (SCREEN_WIDTH/2 - plevel->rad) && plevel->ceilings_deflater[i].x2 - (SCREEN_WIDTH/2 - plevel->rad) <=plevel->rad) {
            int diff = plevel->ceilings_deflater[i].x2 - (SCREEN_WIDTH/2 - plevel->rad);
            moveAll(-diff, plevel, 0);
            plevel->water_acc_value = 0;
        }

        if(plevel->ceilings_deflater[i].x1 < (SCREEN_WIDTH/2 + plevel->rad) && plevel->ceilings_deflater[i].x2 > (SCREEN_WIDTH/2 - plevel->rad) && plevel->vy>0 && plevel->y + plevel->rad > plevel->ceilings_deflater[i].y1 && (plevel->y + plevel->rad) - plevel->ceilings_deflater[i].y1 <=plevel->rad) {
            plevel->y = plevel->ceilings_deflater[i].y1 - plevel->rad;
            plevel->vy = 0;
            plevel->water_acc_value = 0;
        }
        if(plevel->grounds_deflater[i].x1 < (SCREEN_WIDTH/2 + plevel->rad) && plevel->grounds_deflater[i].x2 > (SCREEN_WIDTH/2 - plevel->rad) && plevel->vy<0 && (plevel->grounds_deflater[i].y1 > (plevel->y - plevel->rad)) && plevel->grounds_deflater[i].y1 - (plevel->y - plevel->rad) <=plevel->rad) {
            plevel->vy = 0;
            plevel->y = plevel->grounds_deflater[i].y1 + plevel->rad;
            plevel->grounded = 1;
            plevel->water_acc_value = 0;
        }


    }

    for(int i = 0; i < plevel->power_speed_count; ++i) {


        if(plevel->ceilings_power_speed[i].y1 < plevel->y + plevel->rad && plevel->y - plevel->rad < plevel->grounds_power_speed[i].y1 && plevel->vx < 0 && plevel->ceilings_power_speed[i].x1 < (SCREEN_WIDTH/2 + plevel->rad) && (SCREEN_WIDTH/2 + plevel->rad) - plevel->ceilings_power_speed[i].x1 <plevel->rad) {
            int diff = (SCREEN_WIDTH/2 + plevel->rad) - plevel->ceilings_power_speed[i].x1;
            moveAll(diff, plevel, 0);
            t_flag = 0;
            my_v = 13;
            iResumeTimer(3);
        }

        if(plevel->ceilings_power_speed[i].y1 < plevel->y + plevel->rad && plevel->y - plevel->rad < plevel->grounds_power_speed[i].y1 && plevel->vx > 0 && plevel->ceilings_power_speed[i].x2 > (SCREEN_WIDTH/2 - plevel->rad) && plevel->ceilings_power_speed[i].x2 - (SCREEN_WIDTH/2 - plevel->rad) <=plevel->rad) {
            int diff = plevel->ceilings_power_speed[i].x2 - (SCREEN_WIDTH/2 - plevel->rad);
            moveAll(-diff, plevel, 0);
            t_flag = 0;
            my_v = 13;
            iResumeTimer(3);
        }

        if(plevel->ceilings_power_speed[i].x1 < (SCREEN_WIDTH/2 + plevel->rad) && plevel->ceilings_power_speed[i].x2 > (SCREEN_WIDTH/2 - plevel->rad) && plevel->vy>0 && plevel->y + plevel->rad > plevel->ceilings_power_speed[i].y1 && (plevel->y + plevel->rad) - plevel->ceilings_power_speed[i].y1 <=plevel->rad) {
            plevel->y = plevel->ceilings_power_speed[i].y1 - plevel->rad;
            plevel->vy = 0;
            t_flag = 0;
            my_v = 13;
            iResumeTimer(3);

        }
        if(plevel->grounds_power_speed[i].x1 < (SCREEN_WIDTH/2 + plevel->rad) && plevel->grounds_power_speed[i].x2 > (SCREEN_WIDTH/2 - plevel->rad) && plevel->vy<0 && (plevel->grounds_power_speed[i].y1 > (plevel->y - plevel->rad)) && plevel->grounds_power_speed[i].y1 - (plevel->y - plevel->rad) <=plevel->rad) {
            plevel->vy = 0;
            plevel->y = plevel->grounds_power_speed[i].y1 + plevel->rad;
            plevel->grounded = 1;
            t_flag = 0;
            my_v = 13;
            iResumeTimer(3);

        }


    }

    for(int i = 0; i < plevel->pumper_count; ++i) {


        if(plevel->ceilings_pumper[i].y1 < plevel->y + plevel->rad && plevel->y - plevel->rad < plevel->grounds_pumper[i].y1 && plevel->vx < 0 && plevel->ceilings_pumper[i].x1 < (SCREEN_WIDTH/2 + plevel->rad) && (SCREEN_WIDTH/2 + plevel->rad) - plevel->ceilings_pumper[i].x1 <plevel->rad) {
            int diff = (SCREEN_WIDTH/2 + plevel->rad) - plevel->ceilings_pumper[i].x1;
            moveAll(diff, plevel, 0);
            plevel->water_acc_value = 0.9;
        }

        if(plevel->ceilings_pumper[i].y1 < plevel->y + plevel->rad && plevel->y - plevel->rad < plevel->grounds_pumper[i].y1 && plevel->vx > 0 && plevel->ceilings_pumper[i].x2 > (SCREEN_WIDTH/2 - plevel->rad) && plevel->ceilings_pumper[i].x2 - (SCREEN_WIDTH/2 - plevel->rad) <=plevel->rad) {
            int diff = plevel->ceilings_pumper[i].x2 - (SCREEN_WIDTH/2 - plevel->rad);
            moveAll(-diff, plevel, 0);
            plevel->water_acc_value = 0.9;
        }

        if(plevel->ceilings_pumper[i].x1 < (SCREEN_WIDTH/2 + plevel->rad) && plevel->ceilings_pumper[i].x2 > (SCREEN_WIDTH/2 - plevel->rad) && plevel->vy>0 && plevel->y + plevel->rad > plevel->ceilings_pumper[i].y1 && (plevel->y + plevel->rad) - plevel->ceilings_pumper[i].y1 <=plevel->rad) {
            plevel->y = plevel->ceilings_pumper[i].y1 - plevel->rad;
            plevel->vy = 0;
            plevel->water_acc_value = 0.9;
        }
        if(plevel->grounds_pumper[i].x1 < (SCREEN_WIDTH/2 + plevel->rad) && plevel->grounds_pumper[i].x2 > (SCREEN_WIDTH/2 - plevel->rad) && plevel->vy<0 && (plevel->grounds_pumper[i].y1 > (plevel->y - plevel->rad)) && plevel->grounds_pumper[i].y1 - (plevel->y - plevel->rad) <=plevel->rad) {
            plevel->vy = 0;
            plevel->y = plevel->grounds_pumper[i].y1 + plevel->rad;
            plevel->grounded = 1;
            plevel->water_acc_value = 0.9;
        }


    }


    for(int i = 0; i < plevel->dyn_thorn_count - 1; ++i) {

        if(plevel->ceilings_dyn_thorn[i].y1 < plevel->y + plevel->rad && plevel->y - plevel->rad < plevel->grounds_dyn_thorn[i].y1 && plevel->ceilings_dyn_thorn[i].x1 < (SCREEN_WIDTH/2 + plevel->rad) && (SCREEN_WIDTH/2 + plevel->rad) - plevel->ceilings_dyn_thorn[i].x1 <plevel->rad) {
            if(plevel->cheat_code_mode)
                continue;
            while (plevel->last_checkpoint.y <0)
                moveAll(0, plevel, SCREEN_HEIGHT - plevel->brick_len);
            while (plevel->last_checkpoint.y >SCREEN_HEIGHT - plevel->brick_len)
                moveAll(0, plevel, -(SCREEN_HEIGHT - plevel->brick_len));
            moveAll((SCREEN_WIDTH/2 - plevel->last_checkpoint.x - 25), plevel, 0);
            plevel->y = 300;
            plevel->remaining_life--;
            if(total_score > 50) total_score -= 50;
        }

        if(plevel->ceilings_dyn_thorn[i].y1 < plevel->y + plevel->rad && plevel->y - plevel->rad < plevel->grounds_dyn_thorn[i].y1  && plevel->ceilings_dyn_thorn[i].x2 > (SCREEN_WIDTH/2 - plevel->rad) && plevel->ceilings_dyn_thorn[i].x2 - (SCREEN_WIDTH/2 - plevel->rad) <=plevel->rad) {
            if(plevel->cheat_code_mode)
                continue;
            while (plevel->last_checkpoint.y <0)
                moveAll(0, plevel, SCREEN_HEIGHT - plevel->brick_len);
            while (plevel->last_checkpoint.y >SCREEN_HEIGHT - plevel->brick_len)
                moveAll(0, plevel, -(SCREEN_HEIGHT - plevel->brick_len));
            moveAll((SCREEN_WIDTH/2 - plevel->last_checkpoint.x - 25), plevel, 0);
            plevel->y = 300;
            plevel->remaining_life--;
            if(total_score > 50) total_score -= 50;
        }

        if(plevel->ceilings_dyn_thorn[i].x1 < (SCREEN_WIDTH/2 + plevel->rad) && plevel->ceilings_dyn_thorn[i].x2 > (SCREEN_WIDTH/2 - plevel->rad) && plevel->y + plevel->rad > plevel->ceilings_dyn_thorn[i].y1 && (plevel->y + plevel->rad) - plevel->ceilings_dyn_thorn[i].y1 <=plevel->rad) {
            if(plevel->cheat_code_mode)
                continue;
            plevel->y = plevel->ceilings_dyn_thorn[i].y1 - plevel->rad;
            plevel->vy = 0;
            while (plevel->last_checkpoint.y <0)
                moveAll(0, plevel, SCREEN_HEIGHT - plevel->brick_len);
            while (plevel->last_checkpoint.y >SCREEN_HEIGHT - plevel->brick_len)
                moveAll(0, plevel, -(SCREEN_HEIGHT - plevel->brick_len));
            moveAll((SCREEN_WIDTH/2 - plevel->last_checkpoint.x - 25), plevel, 0);
            plevel->y = 300;
            plevel->remaining_life--;
            if(total_score > 50) total_score -= 50;
        }
        if(plevel->grounds_dyn_thorn[i].x1 < (SCREEN_WIDTH/2 + plevel->rad) && plevel->grounds_dyn_thorn[i].x2 > (SCREEN_WIDTH/2 - plevel->rad) && (plevel->grounds_dyn_thorn[i].y1 > (plevel->y - plevel->rad)) && plevel->grounds_dyn_thorn[i].y1 - (plevel->y - plevel->rad) <=plevel->rad) {
            if(plevel->cheat_code_mode)
                continue;
            plevel->vy = 0;
            plevel->y = plevel->grounds_dyn_thorn[i].y1 + plevel->rad;
            while (plevel->last_checkpoint.y <0)
                moveAll(0, plevel, SCREEN_HEIGHT - plevel->brick_len);
            while (plevel->last_checkpoint.y >SCREEN_HEIGHT - plevel->brick_len)
                moveAll(0, plevel, -(SCREEN_HEIGHT - plevel->brick_len));
            moveAll((SCREEN_WIDTH/2 - plevel->last_checkpoint.x - 25), plevel, 0);
            plevel->y = 300;
            plevel->remaining_life--;
            if(total_score > 50) total_score -= 50;
        }
    }



    if(plevel->ceilings_dyn_thorn[plevel->dyn_thorn_count - 1].y1 < plevel->y + plevel->rad && plevel->y - plevel->rad < plevel->grounds_dyn_thorn[plevel->dyn_thorn_count - 1].y1 && plevel->ceilings_dyn_thorn[plevel->dyn_thorn_count - 1].x1 < (SCREEN_WIDTH/2 + plevel->rad) && (SCREEN_WIDTH/2 + plevel->rad) - plevel->ceilings_dyn_thorn[plevel->dyn_thorn_count - 1].x1 <plevel->rad) {
        if(plevel->rings_crossed == plevel->ring_count + plevel->xring_count) {
            current_level++;
            total_score += 1000;
            saveHighScore();
            timers();
        }
        int diff = (SCREEN_WIDTH/2 + plevel->rad) - plevel->ceilings_dyn_thorn[plevel->dyn_thorn_count - 1].x1;
        moveAll(diff, plevel, 0);
    }

    if(plevel->ceilings_dyn_thorn[plevel->dyn_thorn_count - 1].y1 < plevel->y + plevel->rad && plevel->y - plevel->rad < plevel->grounds_dyn_thorn[plevel->dyn_thorn_count - 1].y1  && plevel->ceilings_dyn_thorn[plevel->dyn_thorn_count - 1].x2 > (SCREEN_WIDTH/2 - plevel->rad) && plevel->ceilings_dyn_thorn[plevel->dyn_thorn_count - 1].x2 - (SCREEN_WIDTH/2 - plevel->rad) <=plevel->rad) {
        if(plevel->rings_crossed == plevel->ring_count + plevel->xring_count) {
            current_level++;
            total_score += 1000;
            saveHighScore();
            timers();
        }
        int diff = plevel->ceilings_dyn_thorn[plevel->dyn_thorn_count - 1].x2 - (SCREEN_WIDTH/2 - plevel->rad);
        moveAll(-diff, plevel, 0);
    }

    for(int inclined_plane_no_l = 0; inclined_plane_no_l < plevel->inclined_plane_count_l; ++inclined_plane_no_l) {
        int y1,y2,x1,x2, x, y;
        x = SCREEN_WIDTH/2;
        y = plevel->y;
        y1 = plevel->inclined_plane_l[inclined_plane_no_l].y1;
        y2 = plevel->inclined_plane_l[inclined_plane_no_l].y2;

        x1 = plevel->inclined_plane_l[inclined_plane_no_l].x1;
        x2 = plevel->inclined_plane_l[inclined_plane_no_l].x2;
        if(!(x + plevel->rad >= x1 && x <= x2 && y - plevel->rad >= y1 && y-plevel->rad <= y2))
            continue;
        double m = (y1 - y2) * 1.0 / (x1 - x2);
        double d = abs(m*x - y - m*x1 + y1) / sqrt(m*m + 1);
        if((d - plevel->rad) <= 1e-9) {
            plevel->grounded = 1;
            plevel->vy = 0;
            plevel->y += (plevel->rad - d) / sqrt(2);
            moveAll((y1 < y2) ? 1 : -1, plevel, 0);
        }
    }

    for(int inclined_plane_no_r = 0; inclined_plane_no_r < plevel->inclined_plane_count_r; ++inclined_plane_no_r) {
        int y1,y2,x1,x2, x, y;
        x = SCREEN_WIDTH/2;
        y = plevel->y;
        y1 = plevel->inclined_plane_r[inclined_plane_no_r].y1;
        y2 = plevel->inclined_plane_r[inclined_plane_no_r].y2;

        x1 = plevel->inclined_plane_r[inclined_plane_no_r].x1;
        x2 = plevel->inclined_plane_r[inclined_plane_no_r].x2;
        if(!(x >= x1 && x - plevel->rad <= x2 && y - plevel->rad <= y1 && y-plevel->rad >= y2))
            continue;
        double m = (y1 - y2) * 1.0 / (x1 - x2);
        double d = abs(m*x - y - m*x1 + y1) / sqrt(m*m + 1);
        if((d - plevel->rad) <= 1e-9) {
            plevel->grounded = 1;
            plevel->vy = 0;
            plevel->y += (plevel->rad - d) / sqrt(2);
            moveAll((y1 < y2) ? 1 : -1, plevel, 0);
        }
    }



    int c = 1;
    for(int water_no = 0; water_no < plevel->water_count; ++water_no) {
        if(SCREEN_WIDTH/2 >= plevel->waters[water_no].x2 && SCREEN_WIDTH/2 <= plevel->waters[water_no].x3 && plevel->y-plevel->rad >= plevel->waters[water_no].y2 && plevel->y + plevel->rad < plevel->waters[water_no].y1) {
            c = 0;
            break;
        }
        if(plevel->ceilings_water[water_no].x1 < (SCREEN_WIDTH/2 + plevel->rad) && plevel->ceilings_water[water_no].x2 > (SCREEN_WIDTH/2 - plevel->rad) && plevel->vy>0 && plevel->y + plevel->rad >= plevel->ceilings_water[water_no].y1 && plevel->y + plevel->rad - plevel->ceilings_water[water_no].y1 <=plevel->rad) {
            plevel->y = plevel->ceilings_water[water_no].y1 - plevel->rad + 0.5;
            //plevel->vy = 0;
            plevel->grounded = 1;

        }
    }
    if(c)
        plevel->acc_water = 0, global_flag = 0;
    else
        plevel->acc_water = plevel->water_acc_value, global_flag = 1;

    //plevel->acc_water = 0;


}
void init(level *plevel) {

    int x = plevel->left_border - plevel->brick_len, y=0;
    for(int ring_no = 0; ring_no < plevel->ring_count; ++ring_no) {
        plevel->rings[ring_no].x = plevel->x_of_ring[ring_no] * plevel->brick_len + plevel->left_border;
        plevel->rings[ring_no].y = plevel->y_of_ring[ring_no] * plevel->brick_len;
    }

    for(int xring_no = 0; xring_no < plevel->xring_count; ++xring_no) {
        plevel->xrings[xring_no].x = plevel->x_of_xring[xring_no] * plevel->brick_len + plevel->left_border;
        plevel->xrings[xring_no].y = plevel->y_of_xring[xring_no] * plevel->brick_len;
    }

    for(int checkpoint_no = 0; checkpoint_no < plevel->checkpoint_count; ++checkpoint_no) {
        plevel->checkpoints[checkpoint_no].x = plevel->x_of_checkpoint[checkpoint_no] * plevel->brick_len + plevel->left_border;
        plevel->checkpoints[checkpoint_no].y = plevel->y_of_checkpoint[checkpoint_no] * plevel->brick_len;
    }

    for(int life_no = 0; life_no < plevel->life_count; ++life_no) {
        plevel->lifes[life_no].x = plevel->x_of_life[life_no] * plevel->brick_len + plevel->left_border;
        plevel->lifes[life_no].y = plevel->y_of_life[life_no] * plevel->brick_len;
    }

    for(int thorn_no = 0; thorn_no < plevel->thorn_count; ++thorn_no) {
        plevel->thorns[thorn_no].x = plevel->x_of_thorn[thorn_no] * plevel->brick_len + plevel->left_border;
        plevel->thorns[thorn_no].y = plevel->y_of_thorn[thorn_no] * plevel->brick_len;

        plevel->grounds_thorn[thorn_no].x1 = plevel->thorns[thorn_no].x + 10;
        plevel->grounds_thorn[thorn_no].x2 = plevel->thorns[thorn_no].x + plevel->brick_len - 10;
        plevel->grounds_thorn[thorn_no].y1 = plevel->thorns[thorn_no].y + plevel->brick_len;
        plevel->grounds_thorn[thorn_no].y2 = plevel->thorns[thorn_no].y + plevel->brick_len;

        plevel->ceilings_thorn[thorn_no].x1 = plevel->thorns[thorn_no].x + 10;
        plevel->ceilings_thorn[thorn_no].x2 = plevel->thorns[thorn_no].x + plevel->brick_len - 10;
        plevel->ceilings_thorn[thorn_no].y1 = plevel->thorns[thorn_no].y;
        plevel->ceilings_thorn[thorn_no].y2 = plevel->thorns[thorn_no].y;
    }


    for(int inverse_thorn_no = 0; inverse_thorn_no < plevel->inverse_thorn_count; ++inverse_thorn_no) {
        plevel->inverse_thorns[inverse_thorn_no].x = plevel->x_of_inverse_thorn[inverse_thorn_no] * plevel->brick_len + plevel->left_border;
        plevel->inverse_thorns[inverse_thorn_no].y = plevel->y_of_inverse_thorn[inverse_thorn_no] * plevel->brick_len;

        plevel->grounds_inverse_thorn[inverse_thorn_no].x1 = plevel->inverse_thorns[inverse_thorn_no].x + 10;
        plevel->grounds_inverse_thorn[inverse_thorn_no].x2 = plevel->inverse_thorns[inverse_thorn_no].x + plevel->brick_len - 10;
        plevel->grounds_inverse_thorn[inverse_thorn_no].y1 = plevel->inverse_thorns[inverse_thorn_no].y + plevel->brick_len;
        plevel->grounds_inverse_thorn[inverse_thorn_no].y2 = plevel->inverse_thorns[inverse_thorn_no].y + plevel->brick_len;

        plevel->ceilings_inverse_thorn[inverse_thorn_no].x1 = plevel->inverse_thorns[inverse_thorn_no].x + 10;
        plevel->ceilings_inverse_thorn[inverse_thorn_no].x2 = plevel->inverse_thorns[inverse_thorn_no].x + plevel->brick_len - 10;
        plevel->ceilings_inverse_thorn[inverse_thorn_no].y1 = plevel->inverse_thorns[inverse_thorn_no].y;
        plevel->ceilings_inverse_thorn[inverse_thorn_no].y2 = plevel->inverse_thorns[inverse_thorn_no].y;
    }

    for(int pumper_no = 0; pumper_no < plevel->pumper_count; ++pumper_no) {
        plevel->pumpers[pumper_no].x = plevel->x_of_pumper[pumper_no] * plevel->brick_len + plevel->left_border;
        plevel->pumpers[pumper_no].y = plevel->y_of_pumper[pumper_no] * plevel->brick_len;

        plevel->grounds_pumper[pumper_no].x1 = plevel->pumpers[pumper_no].x + 10;
        plevel->grounds_pumper[pumper_no].x2 = plevel->pumpers[pumper_no].x + plevel->brick_len - 10;
        plevel->grounds_pumper[pumper_no].y1 = plevel->pumpers[pumper_no].y + plevel->brick_len*2;
        plevel->grounds_pumper[pumper_no].y2 = plevel->pumpers[pumper_no].y + plevel->brick_len*2;

        plevel->ceilings_pumper[pumper_no].x1 = plevel->pumpers[pumper_no].x + 10;
        plevel->ceilings_pumper[pumper_no].x2 = plevel->pumpers[pumper_no].x + plevel->brick_len - 10;
        plevel->ceilings_pumper[pumper_no].y1 = plevel->pumpers[pumper_no].y;
        plevel->ceilings_pumper[pumper_no].y2 = plevel->pumpers[pumper_no].y;
    }


    for(int deflater_no = 0; deflater_no < plevel->deflater_count; ++deflater_no) {
        plevel->deflaters[deflater_no].x = plevel->x_of_deflater[deflater_no] * plevel->brick_len + plevel->left_border;
        plevel->deflaters[deflater_no].y = plevel->y_of_deflater[deflater_no] * plevel->brick_len;

        plevel->grounds_deflater[deflater_no].x1 = plevel->deflaters[deflater_no].x;
        plevel->grounds_deflater[deflater_no].x2 = plevel->deflaters[deflater_no].x + plevel->brick_len;
        plevel->grounds_deflater[deflater_no].y1 = plevel->deflaters[deflater_no].y + plevel->brick_len;
        plevel->grounds_deflater[deflater_no].y2 = plevel->deflaters[deflater_no].y + plevel->brick_len;

        plevel->ceilings_deflater[deflater_no].x1 = plevel->deflaters[deflater_no].x;
        plevel->ceilings_deflater[deflater_no].x2 = plevel->deflaters[deflater_no].x + plevel->brick_len;
        plevel->ceilings_deflater[deflater_no].y1 = plevel->deflaters[deflater_no].y;
        plevel->ceilings_deflater[deflater_no].y2 = plevel->deflaters[deflater_no].y;
    }


    for(int power_speed_no = 0; power_speed_no < plevel->power_speed_count; ++power_speed_no) {
        plevel->power_speeds[power_speed_no].x = plevel->x_of_power_speed[power_speed_no] * plevel->brick_len + plevel->left_border;
        plevel->power_speeds[power_speed_no].y = plevel->y_of_power_speed[power_speed_no] * plevel->brick_len;

        plevel->grounds_power_speed[power_speed_no].x1 = plevel->power_speeds[power_speed_no].x;
        plevel->grounds_power_speed[power_speed_no].x2 = plevel->power_speeds[power_speed_no].x + plevel->brick_len;
        plevel->grounds_power_speed[power_speed_no].y1 = plevel->power_speeds[power_speed_no].y + plevel->brick_len;
        plevel->grounds_power_speed[power_speed_no].y2 = plevel->power_speeds[power_speed_no].y + plevel->brick_len;

        plevel->ceilings_power_speed[power_speed_no].x1 = plevel->power_speeds[power_speed_no].x;
        plevel->ceilings_power_speed[power_speed_no].x2 = plevel->power_speeds[power_speed_no].x + plevel->brick_len;
        plevel->ceilings_power_speed[power_speed_no].y1 = plevel->power_speeds[power_speed_no].y;
        plevel->ceilings_power_speed[power_speed_no].y2 = plevel->power_speeds[power_speed_no].y;
    }

    for(int dyn_thorn_no = 0; dyn_thorn_no < plevel->dyn_thorn_count; ++dyn_thorn_no) {
        plevel->dyn_thorns[dyn_thorn_no].x = plevel->x_of_dyn_thorn[dyn_thorn_no] * plevel->brick_len + plevel->left_border;
        plevel->dyn_thorns[dyn_thorn_no].y = plevel->y_of_dyn_thorn[dyn_thorn_no] * plevel->brick_len;

        plevel->grounds_dyn_thorn[dyn_thorn_no].x1 = plevel->dyn_thorns[dyn_thorn_no].x;
        plevel->grounds_dyn_thorn[dyn_thorn_no].x2 = plevel->dyn_thorns[dyn_thorn_no].x + plevel->brick_len * 2;
        plevel->grounds_dyn_thorn[dyn_thorn_no].y1 = plevel->dyn_thorns[dyn_thorn_no].y + plevel->brick_len * 2;
        plevel->grounds_dyn_thorn[dyn_thorn_no].y2 = plevel->dyn_thorns[dyn_thorn_no].y + plevel->brick_len * 2;
        plevel->ceilings_dyn_thorn[dyn_thorn_no].x1 = plevel->dyn_thorns[dyn_thorn_no].x;
        plevel->ceilings_dyn_thorn[dyn_thorn_no].x2 = plevel->dyn_thorns[dyn_thorn_no].x + plevel->brick_len * 2;
        plevel->ceilings_dyn_thorn[dyn_thorn_no].y1 = plevel->dyn_thorns[dyn_thorn_no].y;
        plevel->ceilings_dyn_thorn[dyn_thorn_no].y2 = plevel->dyn_thorns[dyn_thorn_no].y;
    }

    for(int obs_no = 0; obs_no < plevel->obstacle_count; ++obs_no) {
        x = plevel->tile_count_from_left[obs_no] * plevel->brick_len + plevel->left_border, y = plevel->tile_count_from_bottom[obs_no] * plevel->brick_len;
        for(int i = 0; i < plevel->row_of_obs[obs_no]; ++i) {
            for(int j = 0; j < plevel->column_of_obs[obs_no]; ++j) {
                plevel->obs[obs_no][i][j].side_len = plevel->brick_len;
                plevel->obs[obs_no][i][j].x = x + j * plevel->obs[obs_no][i][j].side_len;
                plevel->obs[obs_no][i][j].y = y + i * plevel->obs[obs_no][i][j].side_len;
            }
        }
        plevel->grounds[obs_no].x1 = plevel->obs[obs_no][plevel->row_of_obs[obs_no]-1][0].x;
        plevel->grounds[obs_no].x2 = plevel->obs[obs_no][plevel->row_of_obs[obs_no]-1][plevel->column_of_obs[obs_no]-1].x + plevel->brick_len;
        plevel->grounds[obs_no].y1 = plevel->obs[obs_no][plevel->row_of_obs[obs_no]-1][0].y + plevel->brick_len ;
        plevel->grounds[obs_no].y2 = plevel->obs[obs_no][plevel->row_of_obs[obs_no]-1][plevel->column_of_obs[obs_no]-1].y + plevel->brick_len;

        plevel->ceilings[obs_no].x1 = plevel->obs[obs_no][plevel->row_of_obs[obs_no]-1][0].x;
        plevel->ceilings[obs_no].x2 = plevel->obs[obs_no][plevel->row_of_obs[obs_no]-1][plevel->column_of_obs[obs_no]-1].x + plevel->brick_len;
        plevel->ceilings[obs_no].y1 = plevel->obs[obs_no][0][0].y;
        plevel->ceilings[obs_no].y2 = plevel->obs[obs_no][0][plevel->column_of_obs[obs_no]-1].y;

    }

    for(int blue_obs_no = 0; blue_obs_no < plevel->blue_obs_count; ++blue_obs_no) {
        x = plevel->tile_count_from_left_blue_obs[blue_obs_no] * plevel->brick_len + plevel->left_border, y = plevel->tile_count_from_bottom_blue_obs[blue_obs_no] * plevel->brick_len;
        for(int i = 0; i < plevel->row_of_blue_obs[blue_obs_no]; ++i) {
            for(int j = 0; j < plevel->column_of_blue_obs[blue_obs_no]; ++j) {
                plevel->blue_obs[blue_obs_no][i][j].side_len = plevel->brick_len;
                plevel->blue_obs[blue_obs_no][i][j].x = x + j * plevel->blue_obs[blue_obs_no][i][j].side_len;
                plevel->blue_obs[blue_obs_no][i][j].y = y + i * plevel->blue_obs[blue_obs_no][i][j].side_len;
            }
        }

        plevel->grounds_blue[blue_obs_no].x1 = plevel->blue_obs[blue_obs_no][plevel->row_of_blue_obs[blue_obs_no]-1][0].x;
        plevel->grounds_blue[blue_obs_no].x2 = plevel->blue_obs[blue_obs_no][plevel->row_of_blue_obs[blue_obs_no]-1][plevel->column_of_blue_obs[blue_obs_no]-1].x + plevel->brick_len;
        plevel->grounds_blue[blue_obs_no].y1 = plevel->blue_obs[blue_obs_no][plevel->row_of_blue_obs[blue_obs_no]-1][0].y + plevel->brick_len ;
        plevel->grounds_blue[blue_obs_no].y2 = plevel->blue_obs[blue_obs_no][plevel->row_of_blue_obs[blue_obs_no]-1][plevel->column_of_blue_obs[blue_obs_no]-1].y + plevel->brick_len;

        plevel->ceilings_blue[blue_obs_no].x1 = plevel->blue_obs[blue_obs_no][plevel->row_of_blue_obs[blue_obs_no]-1][0].x;
        plevel->ceilings_blue[blue_obs_no].x2 = plevel->blue_obs[blue_obs_no][plevel->row_of_blue_obs[blue_obs_no]-1][plevel->column_of_blue_obs[blue_obs_no]-1].x + plevel->brick_len;
        plevel->ceilings_blue[blue_obs_no].y1 = plevel->blue_obs[blue_obs_no][0][0].y;
        plevel->ceilings_blue[blue_obs_no].y2 = plevel->blue_obs[blue_obs_no][0][plevel->column_of_blue_obs[blue_obs_no]-1].y;

    }

    //printf("ok");
    plevel->last_checkpoint.x = SCREEN_WIDTH/2 - plevel->rad;
    plevel->last_checkpoint.y = plevel->y;

    for(int water_no = 0; water_no < plevel->water_count; ++water_no) {

        plevel->ceilings_water[water_no].x1 = plevel->waters[water_no].x1;
        plevel->ceilings_water[water_no].x2 = plevel->waters[water_no].x4;
        plevel->ceilings_water[water_no].y1 = plevel->waters[water_no].y1;
        plevel->ceilings_water[water_no].y2 = plevel->waters[water_no].y4;

    }

}
void gravity_function(level *plevel) {
    //printf("%d\n", total_score);
    if (plevel->remaining_life == 0) {
        if(!game_over)
            saveHighScore();
        game_over = 1;
        iPauseTimer(4);

    }
    int tt_flag = 1;
    for(int i = strlen(cheat_code) - 1, j = strlen(text) - 1; i >= 0; --i, j--) {
        if(cheat_code[i] != text[j]) {
            tt_flag = 0;
            break;
        }
    }
    if(tt_flag)
        plevel->cheat_code_mode = 1;
    //printf("%d %d\n", plevel->rings_crossed, plevel->ring_count + plevel->xring_count);
    if(abs(plevel->vy) <= 20)
        plevel->vy+=plevel->acc + plevel->acc_water;
    plevel->grounded = 0;
    plevel->y+=plevel->vy;
    moveAll(plevel->vx, plevel, 0);
    collision_detection(plevel);

    if(plevel->y + plevel->rad >= 8 * plevel->brick_len) {
        plevel->current_stage++;
        plevel->y = plevel->brick_len;
        moveAll(plevel->vx, plevel, -(SCREEN_HEIGHT - plevel->brick_len));

    } else if(plevel->y - plevel->rad <= 0) {
        plevel->y = SCREEN_HEIGHT - plevel->brick_len;
        plevel->current_stage--;
        moveAll(plevel->vx, plevel, SCREEN_HEIGHT - plevel->brick_len);
    }

}

void draw_level(level *plevel) {

    for(int water_no = 0; water_no < plevel->water_count; ++water_no) {
        double xs[4], ys[4];
        xs[0] = plevel->waters[water_no].x1;
        xs[1] = plevel->waters[water_no].x2;
        xs[2] = plevel->waters[water_no].x3;
        xs[3] = plevel->waters[water_no].x4;

        ys[0] = plevel->waters[water_no].y1;
        ys[1] = plevel->waters[water_no].y2;
        ys[2] = plevel->waters[water_no].y3;
        ys[3] = plevel->waters[water_no].y4;
        iSetColor(102, 224, 255);
        iFilledPolygon(xs, ys, 4);
    }

    for(int checkpoint_no = 0; checkpoint_no < plevel->checkpoint_count; ++checkpoint_no) {
        if(!(plevel->checkpoints[checkpoint_no].x >= 0 && plevel->checkpoints[checkpoint_no].x <=SCREEN_WIDTH && plevel->checkpoints[checkpoint_no].y >= 0 && plevel->checkpoints[checkpoint_no].y <= SCREEN_HEIGHT))
            continue;
        if (plevel->checkpoints[checkpoint_no].visible)
            iShowImg(plevel->checkpoints[checkpoint_no].x, plevel->checkpoints[checkpoint_no].y, 16);
        else
            iShowImg(plevel->checkpoints[checkpoint_no].x, plevel->checkpoints[checkpoint_no].y, 17);
    }

    for(int life_no = 0; life_no < plevel->life_count; ++life_no) {
        if(!(plevel->lifes[life_no].x >= 0 && plevel->lifes[life_no].x <=SCREEN_WIDTH && plevel->lifes[life_no].y >= 0 && plevel->lifes[life_no].y <= SCREEN_HEIGHT))
            continue;
        if(plevel->lifes[life_no].visible)
            iShowImg(plevel->lifes[life_no].x, plevel->lifes[life_no].y, 1);
    }
    for(int ring_no = 0; ring_no < plevel->ring_count; ++ring_no) {
        if(!(plevel->rings[ring_no].x >= 0 && plevel->rings[ring_no].x <=SCREEN_WIDTH && plevel->rings[ring_no].y >= 0 && plevel->rings[ring_no].y <= SCREEN_HEIGHT))
            continue;
        if (plevel->rings[ring_no].crossed)
            iShowImg(plevel->rings[ring_no].x+17, plevel->rings[ring_no].y, 5);
        else
            iShowImg(plevel->rings[ring_no].x+17, plevel->rings[ring_no].y, 4);
    }

    for(int xring_no = 0; xring_no < plevel->xring_count; ++xring_no) {
        if(!(plevel->xrings[xring_no].x >= 0 && plevel->xrings[xring_no].x <=SCREEN_WIDTH && plevel->xrings[xring_no].y >= 0 && plevel->xrings[xring_no].y <= SCREEN_HEIGHT))
            continue;
        if (plevel->xrings[xring_no].crossed)
            iShowImg(plevel->xrings[xring_no].x, plevel->xrings[xring_no].y + 17, 13);
        else
            iShowImg(plevel->xrings[xring_no].x, plevel->xrings[xring_no].y + 17, 12);
    }

    for(int thorn_no = 0; thorn_no < plevel->thorn_count; ++thorn_no) {
        if(!(plevel->thorns[thorn_no].x >= 0 && plevel->thorns[thorn_no].x <=SCREEN_WIDTH && plevel->thorns[thorn_no].y >= 0 && plevel->thorns[thorn_no].y <= SCREEN_HEIGHT))
            continue;
        iShowImg(plevel->thorns[thorn_no].x, plevel->thorns[thorn_no].y, 8);
    }

    for(int inverse_thorn_no = 0; inverse_thorn_no < plevel->inverse_thorn_count; ++inverse_thorn_no) {
        if(!(plevel->inverse_thorns[inverse_thorn_no].x >= 0 && plevel->inverse_thorns[inverse_thorn_no].x <=SCREEN_WIDTH && plevel->inverse_thorns[inverse_thorn_no].y >= 0 && plevel->inverse_thorns[inverse_thorn_no].y <= SCREEN_HEIGHT))
            continue;
        iShowImg(plevel->inverse_thorns[inverse_thorn_no].x, plevel->inverse_thorns[inverse_thorn_no].y, 48);
    }

    for(int power_speed_no = 0; power_speed_no < plevel->power_speed_count; ++power_speed_no) {
        if(!(plevel->power_speeds[power_speed_no].x >= 0 && plevel->power_speeds[power_speed_no].x <=SCREEN_WIDTH && plevel->power_speeds[power_speed_no].y >= 0 && plevel->power_speeds[power_speed_no].y <= SCREEN_HEIGHT))
            continue;
        iShowImg(plevel->power_speeds[power_speed_no].x, plevel->power_speeds[power_speed_no].y, 22);
    }

    for(int deflater_no = 0; deflater_no < plevel->deflater_count; ++deflater_no) {
        if(!(plevel->deflaters[deflater_no].x >= 0 && plevel->deflaters[deflater_no].x <=SCREEN_WIDTH && plevel->deflaters[deflater_no].y >= 0 && plevel->deflaters[deflater_no].y <= SCREEN_HEIGHT))
            continue;
        iShowImg(plevel->deflaters[deflater_no].x, plevel->deflaters[deflater_no].y, 23);
    }

    for(int pumper_no = 0; pumper_no < plevel->pumper_count; ++pumper_no) {
        if(!(plevel->pumpers[pumper_no].x >= 0 && plevel->pumpers[pumper_no].x <=SCREEN_WIDTH && plevel->pumpers[pumper_no].y >= 0 && plevel->pumpers[pumper_no].y <= SCREEN_HEIGHT))
            continue;
        iShowImg(plevel->pumpers[pumper_no].x, plevel->pumpers[pumper_no].y, 24);
    }

    for(int inclined_brick_no_1 = 0; inclined_brick_no_1 < plevel->inclined_brick_count_1; ++inclined_brick_no_1) {
        if(!(plevel->inclined_bricks_1[inclined_brick_no_1].x >= 0 && plevel->inclined_bricks_1[inclined_brick_no_1].x <=SCREEN_WIDTH && plevel->inclined_bricks_1[inclined_brick_no_1].y >= 0 && plevel->inclined_bricks_1[inclined_brick_no_1].y <= SCREEN_HEIGHT))
            continue;
        iShowImg(plevel->inclined_bricks_1[inclined_brick_no_1].x, plevel->inclined_bricks_1[inclined_brick_no_1].y, 19);
    }

    for(int inclined_brick_no_2 = 0; inclined_brick_no_2 < plevel->inclined_brick_count_2; ++inclined_brick_no_2) {
        if(!(plevel->inclined_bricks_2[inclined_brick_no_2].x >= 0 && plevel->inclined_bricks_2[inclined_brick_no_2].x <=SCREEN_WIDTH && plevel->inclined_bricks_2[inclined_brick_no_2].y >= 0 && plevel->inclined_bricks_2[inclined_brick_no_2].y <= SCREEN_HEIGHT))
            continue;
        iShowImg(plevel->inclined_bricks_2[inclined_brick_no_2].x, plevel->inclined_bricks_2[inclined_brick_no_2].y, 2);
    }

    int ball_idx = 15;
    if(plevel->water_acc_value == 0.9)
        ball_idx = 47;
    iShowImg(SCREEN_WIDTH/2 - plevel->rad,(int) plevel->y-plevel->rad, ball_idx);

    for(int ring_no = 0; ring_no < plevel->ring_count; ++ring_no) {
        if(!(plevel->rings[ring_no].x >= 0 && plevel->rings[ring_no].x <=SCREEN_WIDTH && plevel->rings[ring_no].y >= 0 && plevel->rings[ring_no].y <= SCREEN_HEIGHT))
            continue;
        if (plevel->rings[ring_no].crossed)
            iShowImg(plevel->rings[ring_no].x, plevel->rings[ring_no].y, 18);
        else
            iShowImg(plevel->rings[ring_no].x, plevel->rings[ring_no].y, 3);
    }

    for(int xring_no = 0; xring_no < plevel->xring_count; ++xring_no) {
        if(!(plevel->xrings[xring_no].x >= 0 && plevel->xrings[xring_no].x <=SCREEN_WIDTH && plevel->xrings[xring_no].y >= 0 && plevel->xrings[xring_no].y <= SCREEN_HEIGHT))
            continue;
        if (plevel->xrings[xring_no].crossed)
            iShowImg(plevel->xrings[xring_no].x, plevel->xrings[xring_no].y, 11);
        else
            iShowImg(plevel->xrings[xring_no].x, plevel->xrings[xring_no].y, 14);
    }

    for(int obs_no = 0; obs_no < plevel->obstacle_count; obs_no++) {
        for(int i = 0; i < plevel->row_of_obs[obs_no]; ++i) {
            for(int j = 0; j < plevel->column_of_obs[obs_no]; ++j) {
                if(plevel->obs[obs_no][i][j].x >= 0 && plevel->obs[obs_no][i][j].x <= SCREEN_WIDTH && plevel->obs[obs_no][i][j].y >= 0 && plevel->obs[obs_no][i][j].y <= SCREEN_HEIGHT)
                    iShowImg(plevel->obs[obs_no][i][j].x, plevel->obs[obs_no][i][j].y, 20);
            }
        }
    }
    for(int blue_obs_no = 0; blue_obs_no < plevel->blue_obs_count; blue_obs_no++) {
        for(int i = 0; i < plevel->row_of_blue_obs[blue_obs_no]; ++i) {
            for(int j = 0; j < plevel->column_of_blue_obs[blue_obs_no]; ++j) {
                if(plevel->blue_obs[blue_obs_no][i][j].x >= 0 && plevel->blue_obs[blue_obs_no][i][j].x <= SCREEN_WIDTH && plevel->blue_obs[blue_obs_no][i][j].y >= 0 && plevel->blue_obs[blue_obs_no][i][j].y <= SCREEN_HEIGHT)
                    iShowImg(plevel->blue_obs[blue_obs_no][i][j].x, plevel->blue_obs[blue_obs_no][i][j].y, 21);
            }
        }
    }


    for(int dyn_thorn_no = 0; dyn_thorn_no < plevel->dyn_thorn_count; ++dyn_thorn_no) {
        if(!(plevel->dyn_thorns[dyn_thorn_no].x >= 0 && plevel->dyn_thorns[dyn_thorn_no].x <=SCREEN_WIDTH && plevel->dyn_thorns[dyn_thorn_no].y >= 0 && plevel->dyn_thorns[dyn_thorn_no].y <= SCREEN_HEIGHT))
            continue;
        int idx = 0;
        if(dyn_thorn_no == plevel->dyn_thorn_count - 1) {
            if(plevel->rings_crossed == plevel->ring_count + plevel->xring_count) {
                idx = 26;
            } else {
                idx = 25;
            }
        }
        iShowImg(plevel->dyn_thorns[dyn_thorn_no].x, plevel->dyn_thorns[dyn_thorn_no].y, idx);


    }

    draw_offset();
    if(paused) iShowBMP(1055, 150, "resource/resume.fle");
    else iShowBMP(1055, 150, "resource/pause.fle");
    iShowBMP(2, 350, "resource/score.fle");
    show_count_brown_four(total_score, 2, 325);
    iShowBMP(1055, 350, "resource/lifes_count.fle");
    show_count_brown_two(plevel->remaining_life, 1070, 325);
    if(game_over){
        iShowBMP(0, 0, "resource/GameOver.fle");
        show_count_brown_four(total_score, 630, 230);
    }
}




void timers() {
    iResumeTimer(4);
    if (current_level == -1) {
        iResumeTimer(1);
    } else {
        iPauseTimer(1);
    }
    if (current_level == -2) {
        iResumeTimer(2);
    } else {
        iPauseTimer(2);
    }
    if(current_level == 2)
        firstInit(&level2), init(&level2);
    else if(current_level == 1) {
        total_score = 1000;
        firstInit(&level1), init(&level1);
    } else if(current_level == 3) {
        firstInit(&level3), init(&level3);
    }
}
