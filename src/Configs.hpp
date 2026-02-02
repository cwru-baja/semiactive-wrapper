#include <string>


const auto CONFIG_FL_SUBJECT_ID = 3100;
const auto CONFIG_FL_UNIQUE_NAME = "FL_Wheel";

const auto CONFIG_FR_SUBJECT_ID = 3101;
const auto CONFIG_FR_UNIQUE_NAME = "FR_Wheel";

const auto CONFIG_BR_SUBJECT_ID = 3102;
const auto CONFIG_BR_UNIQUE_NAME = "BR_Wheel";

const auto CONFIG_BL_SUBJECT_ID = 3103;
const auto CONFIG_BL_UNIQUE_NAME = "BL_Wheel";

const int CONFIG_UPDATE_HZ = 100;
const int CONFIG_MAX_UPDATE_DUMPS = 25;

const std::string CONFIG_ZMQ_IN = "ipc:///tmp/cyphal_out"; // zmq inproc for testing
const std::string CONFIG_ZMQ_OUT = "ipc:///tmp/wrapper_out"; // zmq inproc for testing