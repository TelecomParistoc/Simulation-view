#ifndef VIEW_H
#define VIEW_H
#include "ActionQueue.hpp"
#include <GL/freeglut.h>
#include <GL/gl.h>
#define angleDiff(a, b) (abs(a-b) % 360 <= 180 ? (abs(a-b) % 360) : 360 - (abs(a-b) % 360))


#define DIR_NONE 0
#define DIR_FORWARD 1
#define DIR_BACKWARD 2


void keyboard(unsigned char key, int x, int y);
void reshape(int width, int height);
GLuint loadBMP_custom(const char * imagepath);
void timer (int value);
void signal_handler(int signum);
void move();
void turn();
void moveTo(int x, int y, int goalAngle);
void display();
void writeDirection();


#endif
