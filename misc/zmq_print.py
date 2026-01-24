#currently intended to use the venv cyphal-env
#intended to print the output of pycyphal
import zmq

def main() -> None:
    context = zmq.Context()
    receiver = context.socket(zmq.SUB)
    receiver.setsockopt_string(zmq.SUBSCRIBE, "")
    receiver.connect("ipc:///tmp/cyphal_out")
    
    while True:
        msg = receiver.recv()
        print(msg)
        

if __name__ == "__main__":
    main()