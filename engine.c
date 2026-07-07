#include <ncurses.h>
#include <math.h>
#include <stdbool.h>

#include "maps.h"
#include "globals.h"

#define SHOW_PLAYER_PARAMETERS

const char gradient[] = "@#%*+=_-   ";


char *mapptr[MAP_HEIGHT][MAP_WIDTH + 1];

bool is_wall(int x, int y) {
    return *mapptr[y][x] == '#';
}

bool is_trigger_one(int x, int y) {
    return *mapptr[y][x] == '1';
}

bool is_trigger_two(int x, int y) {
    return *mapptr[y][x] == '2';
}

bool is_trigger_three(int x, int y) {
    return *mapptr[y][x] == '3';
}

bool is_trigger_four(int x, int y) {
    return *mapptr[y][x] == '4';
}


int main() {
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);
    curs_set(0);

    extern int maxx, maxy;
    getmaxyx(stdscr, maxy, maxx);
    char framebuffer[maxx][maxy];

    for (int h = 0; h != MAP_HEIGHT; h++){
        for (int w = 0; w != MAP_WIDTH + 1; w++){
            mapptr[h][w] = &map_inside[h][w];
        }
    }

    p.x = MAP_WIDTH / 2;
    p.y = 1;
    p.angle = 0.0f;


    while (1) {
        int ch = getch();
        if (ch == 'q') break;

        if (ch == 'l') p.angle += ROTATION_STEP;
        if (ch == 'j') p.angle -= ROTATION_STEP;
        if (p.angle < 0) p.angle += 360;
        if (p.angle >= 360) p.angle -= 360;

        float rad = p.angle * M_PI / 180.0f;
        float dx = cosf(rad);
        float dy = sinf(rad);
        float new_x = p.x, new_y = p.y;

        if (ch == 'w') {
            new_x = p.x + dx * SPEED;
            new_y = p.y + dy * SPEED;
        }
        if (ch == 's') {
            new_x = p.x - dx * SPEED;
            new_y = p.y - dy * SPEED;
        }

        if (ch == 'd') {
            float new_rad = rad + M_PI / 2.0f;
            float ndx = cosf(new_rad);
            float ndy = sinf(new_rad);
            new_x = p.x + ndx * SPEED;
            new_y = p.y + ndy * SPEED;
        }

        if (ch == 'a') {
            float new_rad = rad - M_PI / 2.0f;
            float ndx = cosf(new_rad);
            float ndy = sinf(new_rad);
            new_x = p.x + ndx * SPEED;
            new_y = p.y + ndy * SPEED;
        }


        if (!is_wall((int)new_x, (int)new_y)) {
            p.x = new_x;
            p.y = new_y;
        }

        if (is_trigger_one((int)new_x, (int)new_y)) {
            p.x = new_x;
            p.y -= TELEPORTATION_STEP;
            p.tunnel_pass_iterations++;
        }

        if(is_trigger_two((int)new_x, (int)new_y)){
            for(int w = 0; w < MAP_WIDTH; w++){
                for(int h=0; h< MAP_HEIGHT; h++){
                    mapptr[h][w] = &map_outside[h][w];
                }
            }
        }
        if(is_trigger_three((int) new_x, (int) new_y)){
            for(int w = 0; w < MAP_WIDTH; w++){
                for(int h=0; h< MAP_HEIGHT; h++){
                    mapptr[h][w] = &map_inside[h][w];
                }
            }
        }

        if (is_trigger_four((int)new_x, (int)new_y)) {
            if(p.tunnel_pass_iterations > 0){
                p.x = new_x;
                p.y += TELEPORTATION_STEP;
                p.tunnel_pass_iterations--;
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
                hit_x = (int)(p.x + ray_dx * distance);
                hit_y = (int)(p.y + ray_dy * distance);
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
                    framebuffer[col][row] = wall_char;
                } else {
                    framebuffer[col][row] = ' ';
                }
            }
        }

        for(int x = 0; x <= maxx; x++){
            for(int y = 0; y <= maxy; y++){
                mvaddch(y,x, framebuffer[x][y]);
            }
        }
        #ifdef SHOW_PLAYER_PARAMETERS
            mvprintw(0,0,"player pos: x: %f y: %f; angle: %f", p.x, p.y, p.angle);
        #endif
        refresh();
    }

    endwin();
    return 0;
}
