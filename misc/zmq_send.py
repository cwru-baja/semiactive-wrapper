#debug program for sending random zmq messages to a subscribed socket
import zmq

def main() -> None:
    context: zmq.Context = zmq.Context()
    socket: zmq.Socket = context.socket(zmq.PUB)
    socket.bind("ipc:///tmp/cyphal_out")
    while True:
        message = input("message to send:")
        socket.send_string(message)



if __name__ == "__main__":
    main()