#debug program for sending random zmq messages to a subscribed socket
import zmq
import random
import time

def main() -> None:
    context: zmq.Context = zmq.Context()
    socket: zmq.Socket = context.socket(zmq.PUB)
    socket.bind("ipc:///tmp/cyphal_out")
    while True:
        message = "{" + f"'subject_id': {random.randint(0, 1000)}, " + f"'value': {random.randint(1, 10000) / 100.0}" + "}"
        socket.send_string(message)
        time.sleep(.1)



if __name__ == "__main__":
    main()