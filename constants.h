/* I prefer consts over defines, the compiler will optimize them out and there are less side effects that come with using defines.*/

const unsigned short kMAX_PORT_NUMBER = 65535;
const char INET_ADDR[] = "127.0.0.1";
const int MAX_BIND_RETRIES = 3;
const int INPUT_BUFFER_SIZE = 512;
const char TERMINATOR_CHAR = 10;