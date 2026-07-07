#ifndef GLOBALS_H
#define GLOBALS_H

#define MAP_WIDTH  45
#define MAP_HEIGHT 65
#define FOV   (M_PI / 3.0)
#define DIST_MAX 30
#define WALL_HEIGHT 25
#define SPEED 0.1f
#define ROTATION_STEP 5.0f
#define TELEPORTATION_STEP 4

int maxx, maxy;

struct player{
    float x, y, angle;
    int tunnel_pass_iterations;
};

struct player p;

#endif
