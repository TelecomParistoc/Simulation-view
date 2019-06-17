from subprocess import Popen, PIPE
from shlex import split
from signal import SIGUSR1
from time import sleep
import robot


class Model:

    def _init_(self, xPos=0, yPos=0, angle=0, xSize=0.25, ySize=0.25)
        self.xPos = xPos
        self.yPos = yPos
        self.angle = angle
        self.xSize = xSize
        self.ySize = ySize
        self.robot = Robot(model=self)  #rajouter une réf à model dans robot
        args = split("./view 0 0 0 0.25 0.25")
view = Popen(args, stdout=None, stdin=PIPE)
sleep(2)
view.communicate(input=b'42\n'); #a bytes-like object is required
print('test_sig1')
view.send_signal(SIGUSR1)
print('test_sig2')
