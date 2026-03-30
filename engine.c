#include <stdio.h>
#include <ncurses.h>
#include <math.h>
#include <stdbool.h>

#define MAP_WIDTH  45
#define MAP_HEIGHT 65
#define FOV   (M_PI / 3.0)
#define DIST_MAX 30.0
#define WALL_HEIGHT 20
#define SPEED 0.1f

// const char gradient[] = "$@#\\/|!;:  ";

const char gradient[] = "@%#*+=_-   ";

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
    "#            #            #                 #",
    "#            # #        # #                 #",
    "#            #            #                 #",
    "#            #4#44444444#4#                 #",
    "#            #            #                 #",
    "#            # #        # #                 #",
    "#            #            #                 #",
    "#            # #        # #                 #",
    "#            #111111111111#                 #",
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
    "#            #            #                 #",
    "#            #            #                 #",
    "#            # #        # #                 #",
    "#            #            #                 #",
    "#############################################"
};


const char map_outside[MAP_HEIGHT][MAP_WIDTH + 1] = {
    "#############################################",
    "#                                           #",
    "#            33333333333333                 #",
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

bool is_trigger_four(int x, int y) {
    if (x < 0 || x >= MAP_WIDTH || y < 0 || y >= MAP_HEIGHT)
        return true;
    return map[y][x] == '4';
}

int tunnel_pass_iterations = 0;

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


    float px = MAP_WIDTH / 2;
    float py = 1;
    float angle = 0.0f;


    while (1) {
        int ch = getch();
        if (ch == 'q') break;

        if (ch == 'l') angle += 5.0f;
        if (ch == 'j') angle -= 5.0f;
        if (angle < 0) angle += 360;
        if (angle >= 360) angle -= 360;

        float rad = angle * M_PI / 180.0f;
        float dx = cosf(rad);
        float dy = sinf(rad);
        float new_x = px, new_y = py;

        if (ch == 'w') {
            new_x = px + dx * SPEED;
            new_y = py + dy * SPEED;
        }
        if (ch == 's') {
            new_x = px - dx * SPEED;
            new_y = py - dy * SPEED;
        }

        if (ch == 'd') {
            float new_rad = rad + M_PI / 2.0f;
            float ndx = cosf(new_rad);
            float ndy = sinf(new_rad);
            new_x = px + ndx * SPEED;
            new_y = py + ndy * SPEED;
        }

        if (ch == 'a') {
            float new_rad = rad - M_PI / 2.0f;
            float ndx = cosf(new_rad);
            float ndy = sinf(new_rad);
            new_x = px + ndx * SPEED;
            new_y = py + ndy * SPEED;
        }


        if (!is_wall((int)new_x, (int)new_y)) {
            px = new_x;
            py = new_y;
        }

        if (is_trigger_one((int)new_x, (int)new_y)) {
            px = new_x;
            py -= 3;
            tunnel_pass_iterations++;
        }

        if (is_trigger_four((int)new_x, (int)new_y)) {
            if(tunnel_pass_iterations > 0){
                px = new_x;
                py += 3;
                tunnel_pass_iterations--;
            }
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
        //mvprintw(0,0,"player pos: x %f y %f; angle: %f", px, py, angle);
        refresh();

    }

    endwin();
    return 0;
}
