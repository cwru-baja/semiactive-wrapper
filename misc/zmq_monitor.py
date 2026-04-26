
import argparse
import json
import zmq

MAX_IN_BATCH_PER_TICK = 200

def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(description="Monitor ZMQ traffic with optional subject_id filtering")
    parser.add_argument(
        "-s",
        "--subject-id",
        type=int,
        action="append",
        dest="subject_ids",
        help="Only print messages with this subject_id. Repeat to pass multiple IDs.",
    )
    return parser.parse_args()


def should_print(message: str, subject_filter: set[int] | None) -> bool:
    if subject_filter is None:
        return True

    try:
        payload = json.loads(message)
    except json.JSONDecodeError:
        return False

    subject_id = payload.get("subject_id")
    return isinstance(subject_id, int) and subject_id in subject_filter


def drain_messages(
    receiver: zmq.Socket,
    label: str,
    subject_filter: set[int] | None,
    max_messages: int | None = None,
) -> int:
    count = 0
    while max_messages is None or count < max_messages:
        try:
            msg = receiver.recv_string(flags=zmq.NOBLOCK)
        except zmq.Again:
            break

        if should_print(msg, subject_filter):
            print(f"{label}: {msg}", flush=True)
        count += 1

    return count


def main() -> None:
    args = parse_args()
    subject_filter = set(args.subject_ids) if args.subject_ids else None

    context = zmq.Context()

    # monitor incoming messages from pycyphal
    receiver_in = context.socket(zmq.SUB)
    receiver_in.setsockopt_string(zmq.SUBSCRIBE, "")
    receiver_in.connect("ipc:///tmp/cyphal_in")

    # monitor outgoing messages to pycyphal
    receiver_out = context.socket(zmq.SUB)
    receiver_out.setsockopt_string(zmq.SUBSCRIBE, "")
    receiver_out.connect("ipc:///tmp/cyphal_out")

    poller = zmq.Poller()
    poller.register(receiver_in, zmq.POLLIN)
    poller.register(receiver_out, zmq.POLLIN)

    try:
        while True:
            events = dict(poller.poll(10))

            # Service low-rate outbound traffic first so it is not starved by heavy IN traffic.
            if receiver_out in events:
                drain_messages(receiver_out, "OUT", subject_filter)

            # Drain only a bounded number of input messages per loop for fairness.
            if receiver_in in events:
                drain_messages(
                    receiver_in,
                    "IN",
                    subject_filter,
                    max_messages=MAX_IN_BATCH_PER_TICK,
                )

    except KeyboardInterrupt:
        pass
    finally:
        receiver_in.close(0)
        receiver_out.close(0)
        context.term()
        

if __name__ == "__main__":
    main()