from subprocess import Popen, PIPE
from shlex import split
from signal import SIGUSR1, pause, signal
from time import sleep
#import robot


class Model:

    def __init__(self, xPos=0, yPos=0, angle=0, xSize=250, ySize=250):
        self.xPos = xPos
        self.yPos = yPos
        self.angle = angle
        self.xSize = xSize
        self.ySize = ySize
        #self.robot = Robot(model=self)  #rajouter une réf à model dans robot
        signal(SIGUSR1, lambda signum, stf:None)
        self.view = Popen(["./view",str(xPos), str(yPos), str(angle), str(xSize), str(ySize)], stdout=None, stdin=PIPE)
        pause() #Not on Windows
        self.callback = lambda: None
        signal(SIGUSR1, self.callback_handler)

    def check_number(self, x):
        assert(isinstance(x, int) or isinstance(x, float))

    def move(self, distance, callback = lambda: None):
        self.check_number(distance) #checker si c'est la bonne unité
        self.view.send_signal(SIGUSR1)
        self.view.stdin.write(str.encode('m'+str(distance)+'\n'))
        self.view.stdin.flush()
        self.callback = callback

    def turn(self, heading, callback = lambda: None):
        self.check_number(heading)
        #Put a condition on the heading >=0 && <360
        self.view.send_signal(SIGUSR1)
        self.view.stdin.write(str.encode('t'+str(heading)+'\n'))
        self.view.stdin.flush()
        self.callback = callback

    def moveTo(self, x, y, goalAngle, callback = lambda: None):
        self.check_number(x)
        self.check_number(y)
        self.check_number(goalAngle)
        self.view.send_signal(SIGUSR1)
        self.view.stdin.write(str.encode('o'+str(x)+'/'+str(y)+'/'+str(goalAngle)+'\n'))
        self.view.stdin.flush()
        self.callback = callback

    def callback_handler(self, signum, stf) :
        self.callback()



def test_callback():
    print("test callback")

test = Model(xPos = 1000, yPos = 1000, angle=0)
sleep(1)
test.turn(30,test_callback)
sleep(3)
test.turn(180,test_callback)
sleep(3)
test.moveTo(2000,1000,180)
sleep(1);
test.moveTo(0,0,180)

"""
test.move(600)
sleep(2)
test.turn(600)
"""
