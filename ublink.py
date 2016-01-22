

import ublink
from websocket import create_connection
import struct


ws = create_connection("ws://blinken.eecs.umich.edu/ws/stream")
print 'connected'
while True:
    frame = ws.recv()

    #print 'py: %d' % len(frame)
    ublink.show(frame)


