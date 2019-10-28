#include "view.hpp"
#include <cstring>
#include <cmath>
#include <unistd.h>
#include <csignal>
#include <cstdlib>
#include <iostream>

pid_t ppid;
ActionQueue currentActionQueue;
float position [2];
int current_angle;
int size [2];
FILE * info_file;
static GLuint fieldTex;
static GLuint robotTex;
static int currentDirection = DIR_NONE;

int main(int argc, char **argv)
{
    signal(SIGUSR1, signal_handler);
    ppid = getppid();
    currentActionQueue.setPpid(ppid);

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

    cout << "### Press space to start the simulation" << endl;
    cout << "### Press c to close the Simulation view" << endl;

    glutTimerFunc(0,timer,0);
    glutMainLoop();

    return 0;
}

void display()
{
    float x_size = size[0]/1000.0;
    float y_size = size[1]/1000.0;
    float x_pos = position[0]/1000.0-1.5;
    float y_pos = position[1]/1000.0-1;
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
    glRotatef(current_angle-90,0,0,1);

    glBegin(GL_TRIANGLES);

    glTexCoord2f(0.0f, 1.0f); glVertex3f(-x_size/2,y_size/2,-2);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-x_size/2,-y_size/2,-2);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(x_size/2,-y_size/2,-2);

    glTexCoord2f(0.0f, 1.0f); glVertex3f(-x_size/2,y_size/2,-2);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(x_size/2,y_size/2,-2);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(x_size/2,-y_size/2,-2);

    glEnd();

    glPopMatrix();

    glutSwapBuffers(); // Because GLUT_DOUBLE
}

void timer(int value) {
    switch(currentActionQueue.getCurrentMovement()) {
        case('m'):
            move();
            glutPostRedisplay();
            break;
        case('t'):
            turn();
            glutPostRedisplay();
            break;
        case('n'):
            break;
    }
    glutTimerFunc(16, timer, 0);
}

void move() {
    int goal_value = currentActionQueue.getGoalValue();
    float real_angle = M_PI*(current_angle-90)/180;
    if (goal_value < -2) {
        currentActionQueue.setGoalValue(goal_value+5);
        position[0] += 5*sin(real_angle);
        position[1] -= 5*cos(real_angle);
        currentDirection = DIR_BACKWARD;
    }
    else if (goal_value > 2) {
        currentActionQueue.setGoalValue(goal_value-5);
        position[0] -= 5*sin(real_angle);
        position[1] += 5*cos(real_angle);
        currentDirection = DIR_FORWARD;
    }
    else {
        currentActionQueue.popMovement();
        currentDirection = DIR_NONE;
    }

}

void turn() {
    int goal_value = currentActionQueue.getGoalValue();
    if(goal_value < 0) {
        currentActionQueue.setGoalValue(goal_value+1);
        current_angle -= 1;
    }
    else if(goal_value > 0) {
        currentActionQueue.setGoalValue(goal_value-1);
        current_angle +=1;
    }
    else
        currentActionQueue.popMovement();
}

// moveTo is almost the same function as the function in: libmotors/src/motion.c

void moveTo(int x, int y, int goalAngle) {
    cout <<"Moving to (" << x <<", " << y << ")"<<endl;
    // compute coordinates of the start to end vector
    float deltaX = x - position[0], deltaY = y - position[1];

    // compute heading the robot should have to go to its destination forward
    int angle = (int)(atan2(deltaY, deltaX)*180.0/M_PI);
    while(angle >= 360) angle -= 360;
    while(angle < 0) angle += 360;

    //we want to minimize the total rotation (ie the rotation before and after the translation)
    float rotation_if_forward = abs(angleDiff(angle, current_angle));
    float rotation_if_backward = abs(angleDiff(angle + 180, current_angle));
    if (goalAngle != -1) {
        rotation_if_forward += abs(angleDiff(angle, goalAngle));
        rotation_if_backward += abs(angleDiff(angle + 180, goalAngle));
    }
    int forward = (rotation_if_backward < rotation_if_forward ? -1 : +1);

    // save distance and end angle then start to movement
    if (forward == -1) angle = angle + 180;
    if (angle < 0) angle += 360;
    if (angle >= 360) angle -= 360;
    currentActionQueue.clear();
    currentActionQueue.pushMovement('t',angle-current_angle);
    currentActionQueue.pushMovement('m',(int)(forward * sqrt(deltaX * deltaX + deltaY * deltaY)));
    currentActionQueue.pushMovement('t',goalAngle-angle);
}

void writeDirection() {
    info_file = fopen("tmp.txt","w");
    if (info_file == NULL) {
        perror("Error: can't open tmp.txt");
        exit(-1);
    }
    fprintf(info_file, "%d\n",currentDirection);
    fclose(info_file);
    kill(ppid, SIGUSR2);
}


void signal_handler(int signum) {
    char data_in[30];
    cin >> data_in;
    switch(*data_in) {
        case 'm':
            {
                int goal_distance = stoi(data_in + 1, NULL);
                currentActionQueue.clear();
                currentActionQueue.pushMovement('m', goal_distance);
                if ( goal_distance >= 0)
                    cout << "The robot is moving "<< goal_distance <<" millimeter(s) forward"<< endl;
                else
                    cout << "The robot is moving "<< goal_distance <<" millimeter(s) backward"<< endl;
                break;
            }
        case 't':
            {
                int goal_angle = stoi(data_in + 1, NULL) - current_angle;
                currentActionQueue.clear();
                currentActionQueue.pushMovement('t', goal_angle);
                cout << "The robot is rotating by " << goal_angle << " degree(s)" << endl;
                break;
            }

        case 'o':
            {
                int x = stoi(strtok(data_in + 1, "/"), NULL);
                int y = stoi(strtok(NULL,"/"), NULL);
                int goalAngle = stoi(strtok(NULL, "/"), NULL);
                moveTo(x,y,goalAngle);
                break;
            }
        case 'd':
            writeDirection();
            break;


        default:
            cout << "view: the instruction '" <<data_in << "' is not recognized"<< endl;
            break;
    }
}

void keyboard(unsigned char key, int x, int y) {
    switch(key) {
        case ' ':
            kill(ppid, SIGUSR1);
            glutPostRedisplay();
            break;
        case 'c':
            glutLeaveMainLoop();
            remove("tmp.txt");
            kill(ppid, SIGTERM);
            break;
    }
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
