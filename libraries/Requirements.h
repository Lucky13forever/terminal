// used to include all the libraries


#ifndef TERMINAL_REQUIREMENTS_H
#define TERMINAL_REQUIREMENTS_H

#include <bits/stdc++.h>
#include <ncurses.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <dirent.h>
#include <algorithm>
#include <cctype>
#include <exception>
#include "Scanner.h"
#include "Constants.h"
#include "Status.h"
#include "Display.h"
#include "Errors.h"
#include "Exceptions.h"
#include "Utils.h"

//Terminal needs to be before all the rest of commands
//Commands can't exist without the terminal, commands in the future might alter the state of the terminal
#include "../commands/Terminal.h"
#include "../commands/History.h"
#include "../commands/Exit.h"
#include "../commands/Pwd.h"
#include "../commands/Clear.h"
#include "../commands/Cd.h"
#include "../commands/Ls.h"

//user input might use terminal stuff and commands aswell
#include "UserInput.h"
//Application will have access to everything, this is for control, the new main
#include "Application.h"

#endif //TERMINAL_REQUIREMENTS_H
