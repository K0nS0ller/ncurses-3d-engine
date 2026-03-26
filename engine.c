#include <stdio.h>
#include <curses.h>
#include <math.h>
#include <stdbool.h>

#define MAP_WIDTH  45
#define MAP_HEIGHT 65
#define FOV   (M_PI / 3.0)
#define DIST_MAX   20.0
#define WALL_HEIGHT 20

const char gradient[] = "$@#\\/|!;:  ";


const char map_inside[MAP_HEIGHT][MAP_WIDTH + 1] = {
    "#############################################",
    "#           2              2                #",
    "#           2              2                #",
    "#           2              2                #",
    "#           2              2                #",
    "#           2              2                #",
    "#           2              2                #",
    "#           2#####    #####2                #",
    "#            #            #                 #",
    "#            #            #                 #",
    "#            #            #                 #",
    "#            #            #                 #",
    "#            #            #                 #",
    "#            # #        # #                 #",
    "#            #            #                 #",
    "#            # #        # #                 #",
    "#            #            #                 #",
    "#            # #        # #                 #",
    "#            #            #                 #",
    "#            # #        # #                 #",
    "#            #            #                 #",
    "#            # #        # #                 #",
    "#            #            #                 #",
    "#            # #        # #                 #",
    "#            #            #                 #",
    "#            # #        # #                 #",
    "#            #            #                 #",
    "#            # #        # #                 #",
    "#            #            #                 #",
    "#            # #        # #                 #",
    "#            #            #                 #",
    "#            # #        # #                 #",
    "#            #111111111111#                 #",
    "#            # #        # #                 #",
    "#            #            #                 #",
    "#            #            #                 #",
    "#            # #        # #                 #",
    "#            #            #                 #",
    "#            # #        # #                 #",
    "#            #            #                 #",
    "#            # #        # #                 #",
    "#            # #        # #                 #",
    "#            #            #                 #",
    "#            # #        # #                 #",
    "#            #            #                 #",
    "#            # #        # #                 #",
    "#            #            #                 #",
    "#            # #        # #                 #",
    "#            #            #                 #",
    "#            # #        # #                 #",
    "#            #            #                 #",
    "#            # #        # #                 #",
    "#            #            #                 #",
    "#            # #        # #                 #",
    "#            #            #                 #",
    "#            #            #                 #",
    "#            # #        # #                 #",
    "#            #            #                 #",
    "#            #            #                 #",
    "#            # #        # #                 #",
    "#            #            #                 #",
    "#            #            #                 #",
    "#            ##############                 #",
    "#                                           #",
    "#############################################"
};


const char map_outside[MAP_HEIGHT][MAP_WIDTH + 1] = {
    "#############################################",
    "#                                           #",
    "#                                           #",
    "#            33333333333333                 #",
    "#            33333333333333                 #",
    "#            33333333333333                 #",
    "#            33333333333333                 #",
    "#            #####    #####                 #",
    "#            #            #                 #",
    "#            #            #                 #",
    "#            #            #                 #",
    "#            #            #                 #",
    "#            ##############                 #",
    "#                                           #",
    "#                                           #",
    "#                                           #",
    "#                                           #",
    "#                                           #",
    "#                                           #",
    "#                                           #",
    "#                                           #",
    "#                                           #",
    "#                                           #",
    "#                                           #",
    "#                                           #",
    "#                                           #",
    "#                                           #",
    "#                                           #",
    "#                                           #",
    "#                                           #",
    "#                                           #",
    "#                                           #",
    "#                                           #",
    "#                                           #",
    "#                                           #",
    "#                                           #",
    "#                                           #",
    "#                                           #",
    "#                                           #",
    "#                                           #",
    "#                                           #",
    "#                                           #",
    "#                                           #",
    "#                                           #",
    "#                                           #",
    "#                                           #",
    "#                                           #",
    "#                                           #",
    "#                                           #",
    "#                                           #",
    "#                                           #",
    "#                                           #",
    "#                                           #",
    "#                                           #",
    "#                                           #",
    "#                                           #",
    "#                                           #",
    "#                                           #",
    "#                                           #",
    "#                                           #",
    "#                                           #",
    "#                                           #",
    "#                                           #",
    "#                                           #",
    "#############################################"
};


char map[MAP_HEIGHT][MAP_WIDTH + 1] = {
    "#############################################",
    "#                                           #",
    "#                                           #",
    "#                                           #",
    "#                                           #",
    "#                                           #",
    "#                                           #",
    "#                                           #",
    "#                                           #",
    "#                                           #",
    "#                                           #",
    "#                                           #",
    "#                                           #",
    "#                                           #",
    "#                                           #",
    "#                                           #",
    "#                                           #",
    "#                                           #",
    "#                                           #",
    "#                                           #",
    "#                                           #",
    "#                                           #",
    "#                                           #",
    "#                                           #",
    "#                                           #",
    "#                                           #",
    "#                                           #",
    "#                                           #",
    "#                                           #",
    "#                                           #",
    "#                                           #",
    "#                                           #",
    "#                                           #",
    "#                                           #",
    "#                                           #",
    "#                                           #",
    "#                                           #",
    "#                                           #",
    "#                                           #",
    "#                                           #",
    "#                                           #",
    "#                                           #",
    "#                                           #",
    "#                                           #",
    "#                                           #",
    "#                                           #",
    "#                                           #",
    "#                                           #",
    "#                                           #",
    "#                                           #",
    "#                                           #",
    "#                                           #",
    "#                                           #",
    "#                                           #",
    "#                                           #",
    "#                                           #",
    "#                                           #",
    "#                                           #",
    "#                                           #",
    "#                                           #",
    "#                                           #",
    "#                                           #",
    "#                                           #",
    "#                                           #",
    "#############################################"
};


bool is_wall(int x, int y) {
    if (x < 0 || x >= MAP_WIDTH || y < 0 || y >= MAP_HEIGHT)
        return true;
    return map[y][x] == '#';
}

bool is_trigger_one(int x, int y) {
    if (x < 0 || x >= MAP_WIDTH || y < 0 || y >= MAP_HEIGHT)
        return true;
    return map[y][x] == '1';
}

bool is_trigger_two(int x, int y) {
    if (x < 0 || x >= MAP_WIDTH || y < 0 || y >= MAP_HEIGHT)
        return true;
    return map[y][x] == '2';
}

bool is_trigger_three(int x, int y) {
    if (x < 0 || x >= MAP_WIDTH || y < 0 || y >= MAP_HEIGHT)
        return true;
    return map[y][x] == '3';
}

bool is_inside = false;

int main() {
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);
    curs_set(0);

    int maxx, maxy;
    getmaxyx(stdscr, maxy, maxx);

    for(int w = 0; w < MAP_WIDTH; w++){
        for(int h=0; h< MAP_HEIGHT; h++){
            map[h][w] = map_inside[h][w];
        }
    }

    int ch;

    for(int row = 1; row < maxy; row++){
        for(int col = 0; col < maxx; col++){
            mvprintw(row, col, ".");
            refresh();
            if(ch == 'q'){
                endwin();
                return 0;
            }
        }
    }

    float px = MAP_WIDTH / 2;
    float py = 1;
    float angle = 0.0f;


    while (1) {
        ch = getch();
        if (ch == 'q') break;

        if (ch == 'd') angle += 5.0f;
        if (ch == 'a') angle -= 5.0f;
        if (angle < 0) angle += 360;
        if (angle >= 360) angle -= 360;

        float rad = angle * M_PI / 180.0f;
        float dx = cosf(rad);
        float dy = sinf(rad);
        float new_x = px, new_y = py;

        if (ch == 'w') {
            new_x = px + dx * 0.1f;
            new_y = py + dy * 0.1f;
        }
        if (ch == 's') {
            new_x = px - dx * 0.1f;
            new_y = py - dy * 0.1f;
        }



        if (!is_wall((int)new_x, (int)new_y)) {
            px = new_x;
            py = new_y;
        }

        if (is_trigger_one((int)new_x, (int)new_y)) {
            px = new_x;
            py -= 4;
        }


        if(is_trigger_three((int) new_x, (int) new_y)){
            for(int w = 0; w < MAP_WIDTH; w++){
                for(int h=0; h< MAP_HEIGHT; h++){
                    map[h][w] = map_inside[h][w];
                }
            }
        }

        if(is_trigger_two((int)new_x, (int)new_y)){
            for(int w = 0; w < MAP_WIDTH; w++){
                for(int h=0; h< MAP_HEIGHT; h++){
                    map[h][w] = map_outside[h][w];
                }
            }
        }

        for (int col = 0; col < maxx; col++) {
            float ray_angle = rad - FOV/2.0f + (float)col / maxx * FOV;
            float ray_dx = cosf(ray_angle);
            float ray_dy = sinf(ray_angle);

            float distance = 0.0f;
            int hit_x = 0, hit_y = 0;
            while (distance < DIST_MAX) {
                distance += 0.05f;
                hit_x = (int)(px + ray_dx * distance);
                hit_y = (int)(py + ray_dy * distance);
                if (is_wall(hit_x, hit_y)) break;
            }

            float corrected_dist = distance * cosf(ray_angle - rad);
            int wall_height = (int)(WALL_HEIGHT * DIST_MAX / corrected_dist);
            if (wall_height > maxy) wall_height = maxy;

            int gradient_index = (int)((corrected_dist / DIST_MAX) * (sizeof(gradient) - 2));
            if (gradient_index < 0) gradient_index = 0;
            if (gradient_index >= sizeof(gradient) - 1) gradient_index = sizeof(gradient) - 2;
            char wall_char = gradient[gradient_index];

            int start_y = (maxy - wall_height) / 2;
            int end_y = start_y + wall_height;
            for (int row = 0; row < maxy; row++) {
                if (row >= start_y && row < end_y) {
                    mvprintw(row, col, "%c", wall_char);
                } else {
                    mvprintw(row, col, " ");
                }
            }
        }
        //mvprintw(0,0,"player pos: x %f y %f", px, py);
        refresh();

    }

    endwin();
    return 0;
}
