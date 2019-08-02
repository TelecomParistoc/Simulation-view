#ifndef VIEW_H
#define VIEW_H

#define angleDiff(a, b) (abs(a-b) % 360 <= 180 ? (abs(a-b) % 360) : 360 - (abs(a-b) % 360))

using namespace std;

//Position of the robot
int position [2];

//Angle of the robot
int current_angle;

//Size of the robot
int size [2];


int goal_value;
char current_movement;
int number_of_movement;
queue<char> movements;
queue<int> values;

FILE * info_file;

GLuint fieldTex;
GLuint robotTex;

pid_t ppid;

void keyboard(unsigned char key, int x, int y);
void reshape(int width, int height);
GLuint loadBMP_custom(const char * imagepath);
void timer (int value);
void signal_handler(int signum);
void move();
void turn();
void moveTo(int x, int y, int goalAngle);
void display();


#endif
