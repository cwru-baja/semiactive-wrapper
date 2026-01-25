#debug program for sending random zmq messages to a subscribed socket
import zmq
import random
import time

def main() -> None:
    i = 1
    context: zmq.Context = zmq.Context()
    socket: zmq.Socket = context.socket(zmq.PUB)
    socket.bind("ipc:///tmp/cyphal_out")
    while True:
        message = "{" + f"'subject_id': {random.randint(1, 2)}, " + f"'value': {random.randint(1, 10000) / 100.0}" + "}"
        socket.send_string(message)
        print(f"\rZMQ Sensor Outputs: {i}", end="", flush=True)
        i+=1
        time.sleep(.1)



if __name__ == "__main__":
    main()