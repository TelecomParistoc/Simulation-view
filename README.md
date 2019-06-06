# Simulation view

The Simulation view is a graphic display done with OpenGL that shows the route of the robot.

## Instalation

Use the package manager apt to install libgl1-mesa-dev and freeglut3-dev.
```bash
sudo apt install libgl1-mesa-dev freeglut3-dev
```
Clone the git repositry.
Use `make` to compile the code.
## Usage

```bash
./view xInitialValue yInitialValue initialAngle xRobotSize yRobotSize
```
The lengths are in meter and the  angle is degree.

## Caution

The BMP files used as textures and loaded by the function:
```bash
GLuint loadBMP_custom(const char * imagepath);
```
need to be 24 bit bitmpas.

You can convert your image on [online-converting](https://online-converting.com/image/convert2bmp/).


