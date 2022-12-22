//
// Created by area51 on 22.12.22.
//

#ifndef TERMINAL_CD_H
#define TERMINAL_CD_H
class Cd{
public:
    void run(string);
    bool is_valid(string command);
}cd_command;

bool Cd::is_valid(string command){

    // i should only find 1 space inside. It's ok to have more, but only if there's something after them
    return true;

}

void Cd::run(string command) {
    //only one argument
    return;

}
#endif //TERMINAL_CD_H
