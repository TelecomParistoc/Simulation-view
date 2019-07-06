#include "GL/freeglut.h"
#include "GL/gl.h"
#include <cstring>
#include <cmath>
#include <iostream>
#include <unistd.h>
#include <csignal>

#define angleDiff(a, b) (abs(a-b) % 360 <= 180 ? (abs(a-b) % 360) : 360 - (abs(a-b) % 360))
void keyboard(unsigned char key, int x, int y);
void reshape(int width, int height);
GLuint loadBMP_custom(const char * imagepath);
void timer (int value);
void signal_handler(int signum);
void move();
void turn();
void moveTo(int x, int y, int goalAngle);

using namespace std;

//Position of the robot
int position [2];

//Angle of the robot
int current_angle;

//Size of the robot
int size [2];

int distance_goal;
int goal_angle;
int number_of_movement;

GLuint fieldTex;
GLuint robotTex;

bool distance_callback_is_sent;
bool angle_callback_is_sent;
pid_t ppid;

void display()
{
    float x_size = size[0]/1000.0;
    float y_size = size[1]/1000.0;
    float x_pos = position[0]/1000.0;
    float y_pos = position[1]/1000.0;
    // Set Background Color
    glClearColor(0, 0, 0, 1.0);
    // Clear screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glBindTexture(GL_TEXTURE_2D, fieldTex);

    glBegin(GL_TRIANGLES);

    glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.5, 1.0, -3);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.5, -1.0, -3);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(1.5, -1, -3);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.5, 1.0, -3);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(1.5, -1, -3);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(1.5, 1, -3);

    glEnd();


    glBindTexture(GL_TEXTURE_2D, robotTex);
    glPushMatrix();
    glTranslatef(x_pos,y_pos,0);
    glRotatef(current_angle,0,0,1);

    glBegin(GL_TRIANGLES);

    glTexCoord2f(0.0f, 1.0f); glVertex3f(-x_size/2,y_size/2,-2);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-x_size/2,-y_size/2,-2);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(x_size/2,-y_size/2,-2);

    glTexCoord2f(0.0f, 1.0f); glVertex3f(-x_size/2,y_size/2,-2);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(x_size/2,y_size/2,-2);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(x_size/2,-y_size/2,-2);

    glEnd();
    /*
       glColor4f(1.0,1.0,1.0,0.3)
       glBegin(GL_TRIANGLE_FAN);
       int i;
       glVertex3f(0,0,0);
       for(i=0; i<100; i++) {
       glVertex3f(


       }


       glEnd();

*/
    glPopMatrix();



    glutSwapBuffers(); // Because GLUT_DOUBLE
}


int main(int argc, char **argv)
{
    /*
       sigset_t set;
       int sig;
       sigemptyset(&set);
       sigaddset(&set, SIGUSR1);
       sigwait(&set , &sig);
       cout << sig << endl;
       */
    distance_goal = 0;
    goal_angle = 0;
    distance_callback_is_sent = true;
    angle_callback_is_sent = true;
    signal(SIGUSR1, signal_handler);
    ppid = getppid();
    kill(ppid, SIGUSR1);
    if (argc == 6) {
        position[0] = stoi(argv[1]);
        position[1] = stoi(argv[2]);
        current_angle = stoi(argv[3],NULL);
        size[0] = stoi(argv[4]);
        size[1] = stoi(argv[5]);
    } else {
        cout << "Wrong number of arguments, ./view needs 5 arguments" << endl;
        return(1);
    }

    cout << "### Press c to close the Simulation view" << endl;
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

    glutInitWindowSize(900, 700);
    glutInitWindowPosition(100, 100);

    glutCreateWindow("OpenGL - Simulation view");
    glutDisplayFunc(display);

    glutReshapeFunc(reshape);

    //Enable Z-buffer depth test
    glEnable(GL_DEPTH_TEST);

    glEnable(GL_TEXTURE_2D);

    //Keyboard-events
    glutKeyboardFunc(keyboard);

    // after the closure of the window, the program keep going
    glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);

    // Projection transformation
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1.5, 1.5, -1.1667, 1.1667, 0, 4.0);

    fieldTex = loadBMP_custom("res/field.bmp");
    robotTex = loadBMP_custom("res/logo.bmp");


    glutTimerFunc(0,timer,0);
    glutMainLoop();

    return 0;
}

void timer(int value) {
   move();
   turn();
    glutPostRedisplay();
    glutTimerFunc(16, timer, 0);
}

void move() {
    if (distance_goal < -2) {
        distance_goal += 5;
        position[0] += (int)(5*sin(M_PI*current_angle/180));
        position[1] -= (int)(5*cos(M_PI*current_angle/180));
    }
    else if (distance_goal > 2) {
        distance_goal -= 5;
        position[0] -= (int)(5*sin(M_PI*current_angle/180));
        position[1] += (int)(5*cos(M_PI*current_angle/180));
    }
    else{
        distance_goal = 0;
        if (!distance_callback_is_sent) {
            distance_callback_is_sent = true;
            kill(ppid, SIGUSR1);
        }
    }
}

void turn() {
    if(goal_angle < 0) {
        goal_angle +=1;
        current_angle += 1;
    }
    else if(goal_angle > 0) {
        goal_angle -=1;
        current_angle -=1;
    }
    else {
        goal_angle = 0;
        if (!angle_callback_is_sent) {
            angle_callback_is_sent = true;
            kill(ppid, SIGUSR1);
        }
    }
}

// moveTo is almost the dame function as the function in: libmotors/src/motion.c

void moveTo(int x, int y, int goalAngle) {
    cout <<"Moving to (" << x <<", " << y << ")"<<endl;
    // compute coordinates of the start to end vector
    int deltaX = x - position[0], deltaY = y - position[1];

    // compute heading the robot should have to go to its destination forward
    int angle = atan2(deltaY, deltaX)*180.0/M_PI;
    while(angle >= 360) angle -= 360;
    while(angle < 0) angle += 360;

    //we want to minimize the total rotation (ie the rotation before and after the translation)
    int current_heading = current_angle;
    float rotation_if_forward = abs(angleDiff(angle, current_heading));
    float rotation_if_backward = abs(angleDiff(angle + 180, current_heading));
    if (goalAngle != -1) {
        rotation_if_forward += abs(angleDiff(angle, goalAngle));
        rotation_if_backward += abs(angleDiff(angle + 180, goalAngle));
    }
    int forward = (rotation_if_backward < rotation_if_forward ? -1 : +1);

    // save distance and end angle then start to movement
    if (forward == -1) angle = angle + 180;
    if (angle < 0) angle += 360;
    if (angle >= 360) angle -= 360;

    distance_goal = forward * sqrt(deltaX * deltaX + deltaY * deltaY);
    goal_angle = goalAngle;
    //turn(angle, startRotationDone);
}

void signal_handler(int signum) {
    char data_in[9];
    cin >> data_in;
    switch(*data_in) {
        case 'm': {
                      distance_callback_is_sent = false;
                      distance_goal = stoi(data_in +1,NULL);
                      if (distance_goal >= 0)
                          cout << "The robot is moving "<< distance_goal <<" millimeter(s) forward"<< endl;
                      else
                          cout << "The robot is moving "<< -distance_goal <<" millimeter(s) backward"<< endl;
                  } break;
        case 't': {
                      angle_callback_is_sent = false;
                      goal_angle = stoi(data_in+1,NULL);
                      cout << "The robot is rotating by " << goal_angle << " degree(s)" << endl;
                  } break;
        case 'o': {
                      int x = stoi(strtok(data_in + 1, " "), NULL);
                      int y = stoi(strtok(NULL," "), NULL);
                      int goalAngle = stoi(strtok(NULL, " "), NULL);
                  } break;
        default:
                  cout << "view: the instruction '" <<data_in << "' is not recognized"<< endl;
                  break;
    }

}

void keyboard(unsigned char key, int x, int y) {
    if (key == 'c') {
        glutLeaveMainLoop();
    }
    else if (key == 'z') {
        position[0] -= (int)(50*sin(M_PI*current_angle/180));
        position[1] += (int)(50*cos(M_PI*current_angle/180));
    }
    else if (key == 's') {
        position[0] += (int)(50*sin(M_PI*current_angle/180));
        position[1] -= (int)(50*cos(M_PI*current_angle/180));
    }
    else if (key == 'q') {
        current_angle += 2;
    }
    else if (key == 'd') {
        current_angle -= 2;
    }
    glutPostRedisplay();
}

void reshape(int width, int height) {
    glViewport(0,0,width,height);
    float x;
    float y;
    if (3*height<2*width) {
        x = (float)(width)/height;
        y =  1;
    } else {
        x = 1.5;
        y = 1.5*(float)(height)/width;
    }
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-x, x, -y, y, 0, 4.0);
}



GLuint loadBMP_custom(const char * imagepath) {
    // Data read from the header of the BMP file
    unsigned char header[54]; // Each BMP file begins by a 54-bytes header
    unsigned int dataPos;     // Position in the file where the actual data begins
    unsigned int width, height;
    unsigned int imageSize;   // = width*height*3
    // Actual RGB data
    unsigned char * data;

    // Open the file
    FILE * file = fopen(imagepath,"rb");
    if (!file){cout << "Image could not be opened" << endl; return 0;}

    if ( fread(header, 1, 54, file)!=54 ){ // If not 54 bytes read : problem
        cout << "Not a correct BMP file" << endl;
        return 0;
    }
    if ( header[0]!='B' || header[1]!='M' ){
        cout << "Not a correct BMP file" << endl;
        return 0;
    }

    // Read ints from the byte array
    dataPos    = *(int*)&(header[0x0A]);
    imageSize  = *(int*)&(header[0x22]);
    width      = *(int*)&(header[0x12]);
    height     = *(int*)&(header[0x16]);

    // Some BMP files are misformatted, guess missing information
    if (imageSize==0)    imageSize=width*height*3; // 3 : one byte for each Red, Green and Blue component
    if (dataPos==0)      dataPos=54; // The BMP header is done that way

    // Create a buffer
    data = new unsigned char [imageSize];

    // Read the actual data from the file into the buffer
    fread(data,1,imageSize,file);

    //Everything is in memory now, the file can be closed
    fclose(file);

    GLuint texture;

    // Create one OpenGL texture
    glGenTextures(1, &texture);

    // "Bind" the newly created texture : all future texture functions will modify this texture
    glBindTexture(GL_TEXTURE_2D, texture);

    // Give the image to OpenGL
    glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR); //scale linearly when image bigger than texture
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); //scale linearly when image smalled than texture

    return texture;
}
