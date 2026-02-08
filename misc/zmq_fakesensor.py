#debug program for sending random zmq messages to a subscribed socket
import zmq
import random
import time

def main() -> None:
    i = 1

    msgs = [
        "{'subject_id': 578, 'type':'uavcan.primitive.String_1_0', 'value': 'sup dood'}",
        "{'subject_id': 908, 'type':'cwrubaja.suspension.vcm.Setpoint_0_2', 'priority': 2, 'current': {'ampere': 0.4}}"
    ]

    context: zmq.Context = zmq.Context()
    socket: zmq.Socket = context.socket(zmq.PUB)
    socket.bind("ipc:///tmp/cyphal_out")
    while True:
        message = msgs[random.randint(0, 1)]
        socket.send_string(message)
        print(f"\rZMQ Sensor Outputs: {i}", end="", flush=True)
        i+=1
        time.sleep(.1)



if __name__ == "__main__":
    main()