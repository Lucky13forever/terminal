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

class FlagHasNoValue : public std::exception {
private:
    string flag;
public:
    explicit FlagHasNoValue(const string &flag) : flag(flag) {};

    const char * what () {
        return "Flag has no value";
    }

    const string &getFlag() const {
        return flag;
    }
};

class SyntaxError : public std::exception {
private:
    string sign;
public:
    explicit SyntaxError(const string &sign) : sign(sign) {};

    const char * what () {
        return "Flag has no value";
    }

    const string &getSign() const {
        return sign;
    }
};




#endif //TERMINAL_ERRORS_H
