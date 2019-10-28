from subprocess import Popen, PIPE
from shlex import split
from signal import SIGUSR1, pause, signal
from time import sleep

#could have some issues if there are different callbacks at the same time
xPos = 0
yPos = 0
angle = 0
xSize = 250
ySize = 250
callback = lambda: None

def callback_handler(signum, stf) :
    callback()

signal(SIGUSR1, callback_handler)

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
    view.terminate()

# def getDirection():



