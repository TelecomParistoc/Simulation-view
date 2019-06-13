#define PI 3.14159265
#include "GL/freeglut.h"
#include "GL/gl.h"
#include <cstring>
#include <cmath>
#include <iostream>
#include <unistd.h>
#include <csignal>

void keyboard(unsigned char key, int x, int y);
void reshape(int width, int height);
GLuint loadBMP_custom(const char * imagepath);

using namespace std;

//Position of the robot
float position [2];

//Angle of the robot
float angle;

//Size of the robot
float size [2];

GLuint fieldTex;
GLuint robotTex;
void display()
{
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
    glTranslatef(position[0],position[1],0);
    glRotatef(angle,0,0,1);

    glBegin(GL_TRIANGLES);

    glTexCoord2f(0.0f, 1.0f); glVertex3f(-size[0]/2,size[1]/2,-2);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-size[0]/2,-size[1]/2,-2);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(size[0]/2,-size[1]/2,-2);

    glTexCoord2f(0.0f, 1.0f); glVertex3f(-size[0]/2,size[1]/2,-2);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(size[0]/2,size[1]/2,-2);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(size[0]/2,-size[1]/2,-2);

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
    int var ;
    /*
    sigset_t set;
    int sig;
    sigemptyset(&set);
    sigaddset(&set, SIGUSR1);
    sigwait(&set , &sig);
    cout << sig << endl;
    */
    cin >> var;
    cout << var << endl;
    if (argc == 6) {
        position[0] = strtof(argv[1],NULL);
        position[1] = strtof(argv[2],NULL);
        angle = strtof(argv[3],NULL);
        size[0] = strtof(argv[4],NULL);
        size[1] = strtof(argv[5],NULL);
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

    glutMainLoop();

    return 0;
}

void keyboard(unsigned char key, int x, int y) {
    if (key == 'c') {
        glutLeaveMainLoop();
    }
    else if (key == 'z') {
        position[0] -= 0.05*sin(PI*angle/180);
        position[1] += 0.05*cos(PI*angle/180);
    }
    else if (key == 's') {
        position[0] += 0.05*sin(PI*angle/180);
        position[1] -= 0.05*cos(PI*angle/180);
    }
    else if (key == 'q') {
        angle += 2;
    }
    else if (key == 'd') {
        angle -= 2;
    }
    glutPostRedisplay();
    glutPostRedisplay();
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

