from subprocess import Popen, PIPE
from shlex import split
from signal import SIGUSR1
from time import sleep

args = split("./view 0 0 0 0.25 0.25")
view = Popen(args, stdout=None, stdin=PIPE)
sleep(2)
view.communicate(input=b'42\n'); #a bytes-like object is required
print('test_sig1')
view.send_signal(SIGUSR1)
print('test_sig2')
