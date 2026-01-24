#debug program for sending random zmq messages to a subscribed socket
import zmq
import random
import time

def main() -> None:
    context: zmq.Context = zmq.Context()
    socket: zmq.Socket = context.socket(zmq.PUB)
    socket.bind("ipc:///tmp/cyphal_out")
    while True:
        message = F"{'subject_id': 0001, 'value': {random.random()}}"
        socket.send_string(message)
        time.sleep(.5)



if __name__ == "__main__":
    main()