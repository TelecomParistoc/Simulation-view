from subprocess import Popen, PIPE
from shlex import split
from signal import SIGUSR1, pause, signal, SIGUSR2
from time import sleep
from os import remove

#could have some issues if there are different callbacks at the same time
xPos = 0
yPos = 0
angle = 0
xSize = 250
ySize = 250
callback = lambda: None

def callback_handler(signum, stf) :
    callback()

def read_direction(signum, stf):
    direction_file=open("tmp.txt", "r")
    return int(direction_file.read())

signal(SIGUSR1, callback_handler)
signal(SIGUSR2, read_direction)

view = Popen(["./view",str(xPos), str(yPos), str(angle), str(xSize), str(ySize)], stdout=None, stdin=PIPE)
pause() #Doesn't work on Windows


def check_number(x):
    assert(isinstance(x, int) or isinstance(x, float))

def move(distance, callback = lambda: None):
    check_number(distance)
    view.send_signal(SIGUSR1)
    view.stdin.write(str.encode('m'+str(distance)+'\n'))
    view.stdin.flush()
    callback = callback

def turn(heading, callback = lambda: None):
    check_number(heading)
    #Put a condition on the heading >=0 && <360
    view.send_signal(SIGUSR1)
    view.stdin.write(str.encode('t'+str(heading)+'\n'))
    view.stdin.flush()
    callback = callback

def moveTo(x, y, goalAngle, callback = lambda: None):
    check_number(x)
    check_number(y)
    check_number(goalAngle)
    view.send_signal(SIGUSR1)
    view.stdin.write(str.encode('o'+str(x)+'/'+str(y)+'/'+str(goalAngle)+'\n'))
    view.stdin.flush()
    callback = callback

def close_simulation(secs = 0):
    sleep(secs)
    remove("tmp.txt")
    view.terminate()

def getDirection():
    view.send_signal(SIGUSR1)
    view.stdin.write(str.encode('d\n'))
    view.stdin.flush()

def setPosition(x, y):
    check_number(x)
    check_number(y)
    view.send_signal(SIGUSR1)
    view.stdin.write(str.encode('s'+str(x)+'/'+str(y)+'\n'))
    view.stdin.flush()
