# Simulation view

The Simulation view is a graphic display done with OpenGL that shows the route of the robot.

## Instalation

You need to install two libraries: [freeglut](http://freeglut.sourceforge.net/) and [mesa](https://www.mesa3d.org/).
On Debian or Ubuntu:
```bash
sudo apt install libgl1-mesa-dev freeglut3-dev
```
On Arch:
```bash
sudo pacman -S mesa freeglut
```
Clone the git repositry.
Use `make` to compile the code.

## Usage

```bash
./view xInitialValue yInitialValue initialAngle xRobotSize yRobotSize
```
The lengths are in meter and the angle is in degree.

wip

## Caution

The BMP files used as textures and loaded by the function:
```bash
GLuint loadBMP_custom(const char * imagepath);
```
need to be 24 bit bitmpas.

You can convert your image with the command line `convert` or on [online-converting](https://online-converting.com/image/convert2bmp/).


