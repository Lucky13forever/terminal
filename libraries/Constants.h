#ifndef PROJECT_CONSTANTS_H
#define PROJECT_CONSTANTS_H

using namespace std;
const int ENTER = 10;
const int BACKSPACE = 263;
const char * EXIT = "exit";
const char * HISTORY = "history";
const char * PWD = "pwd";
const char * CD = "cd";
const char * LS = "ls";
const int RUNNING = 0;
const int TERMINATED = 1;
const int DEBUG = 2; // 0 is nothing, 1 is for the display.debug_message, 2 is for debug_file, 3 is for both
const char * CLEAR = "clear";
const int COLOR_WHITE_CODE = 1;
const int COLOR_GREEN_CODE = 2;
const int COLOR_CYAN_CODE = 3;
const int COLOR_BLUE_CODE = 4;
const int COLOR_RED_CODE = 5;
const int COLOR_YELLOW_CODE = 6;
const int COLOR_MAGENTA_CODE = 7;
const int BUFFER_SIZE = 1024;
const string GO_UP = "up";
const string GO_DOWN = "down";

#endif //PROJECT_CONSTANTS_H
