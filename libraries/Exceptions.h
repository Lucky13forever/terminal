//
// Created by area51 on 10.12.22.
//

#ifndef TERMINAL_EXCEPTIONS_H
#define TERMINAL_EXCEPTIONS_H

using namespace std;
class NoSuchFileOrDirectory : public std::exception {
    string real_path;
public:
    const char * what() {
        return "No such file or directory found";
    }

    explicit NoSuchFileOrDirectory(const string &realPath) : real_path(realPath) {}
};

class InvalidFlag : public std::exception {
private:
    string flag;
public:
    explicit InvalidFlag(const string &flag) : flag(flag) {};

    const char * what () {
        return "Invalid flag provided";
    }

    const string &getFlag() const {
        return flag;
    }
};

class MissingArguments : public std::exception {
public:
    const char * what () {
        return "No arguments received!";
    }
};

#endif //TERMINAL_ERRORS_H
