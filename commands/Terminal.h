//
// Created by area51 on 10.12.22.
//
#ifndef TERMINAL_TERMINAL_H
#define TERMINAL_TERMINAL_H


class Terminal{
    Scanner terminal_scanner = *new Scanner();
    int state = RUNNING;
public:
    const string &getPath() const;

private:

    string path;

    vector<string>prefix = {"area51", ":", "~", "$ "};
    vector<int>color_pairs = {COLOR_GREEN_CODE, COLOR_WHITE_CODE, COLOR_CYAN_CODE, COLOR_WHITE_CODE};
public:
    const vector<string> &getPrefix() const;

    const vector<int> &getColorPairs() const;

    void initialize_path();
    void update_path(string);



public:
    const int get_state() {return state;};
    void terminate() {state = TERMINATED;};
    bool running() {return state == RUNNING;};
    Terminal();
    void configure();
    void check_if_external_command_exists(string command, string prev);
    int execute_external_command(string command, string previous_command_result, string &result_after_run,
                                 bool keep_on_screen);
    bool this_command_exists(string name);
    string extract_name_of_command(string);
    string trim_path_received(string path);
    char **return_char_pointer_from_vector_of_strings(vector<string> argv_vector);
}terminal;

Terminal::Terminal()
{
    initialize_path();
}

void Terminal::configure()
{
    initialize_path();
}

void Terminal::initialize_path(){
    char buffer[1024];
    path = "";
    if (getcwd(buffer, sizeof(buffer)) != nullptr)
    {
        path.append(buffer);
    }
    prefix[2] = "~";
    prefix[2].append(path);
}

const vector<string> &Terminal::getPrefix() const {
    return prefix;
}

const vector<int> &Terminal::getColorPairs() const {
    return color_pairs;
}

string Terminal::extract_name_of_command(string command){
    //check if i have a space in the command
    int space_found_at = command.find(" ");

    string rez;
    if (space_found_at != -1)
    {
        rez = command.substr(0, space_found_at);
    }
    else {
        //no space, then this is just the name of the command, no parameters
        rez = command;
    }

    return rez;

}


void Terminal::check_if_external_command_exists(string command, string prev) {

    //extract the name of the command
    string name = extract_name_of_command(command);


    if (this_command_exists(name) == 1)
    {
        display.display_debug_file("The name of this command is |" + name + "|");
        display.display_debug_file("Prev string is: " + prev);
        //run exec on it
        string mt;
        execute_external_command(command, prev, mt, true);

    }
    else{
        errors.internal_command_not_found();
    }
    display.display_debug_file("AICI??");
}

char ** Terminal::return_char_pointer_from_vector_of_strings(vector<string> argv_vector)
{
    char ** argv = new char *[argv_vector.size() + 1];

    for(int i=0; i<argv_vector.size(); i++)
    {
        argv[i] = new char[ argv_vector[i].length() + 1 ];
        strcpy(argv[i], argv_vector[i].c_str());
        argv[i][argv_vector[i].length()] = 0;
    }
    argv[argv_vector.size()] = NULL;
    return argv;
}

int Terminal::execute_external_command(string command, string previous_command_result, string &result,bool keep_on_screen) {

    display.display_debug_file("Comanda externa ce va fi rulata este " + command );
    terminal_scanner.scan_command(command);
    int pipe_fd[2];
    int keep[2];
    pipe(pipe_fd);
    pipe(keep);

    write(pipe_fd[1], previous_command_result.c_str(), previous_command_result.length());
    int pid2 = fork();
    if (pid2 == 0) {
        dup2(pipe_fd[0], STDIN_FILENO);
        dup2(keep[1], STDOUT_FILENO);//in output i keep the result
        dup2(keep[1], STDERR_FILENO);//keep the errors
        close(pipe_fd[0]);
        close(pipe_fd[1]);
        close(keep[0]);
        close(keep[1]);

        char ** argv = return_char_pointer_from_vector_of_strings(terminal_scanner.get_everything());

        vector<string> proba = terminal_scanner.get_everything();

        for (string str : proba)
        {
            display.display_debug_file(str);
        }

        execvp(argv[0], argv);
        return 0;
    }

    close(pipe_fd[0]);
    close(pipe_fd[1]);
    close(keep[1]);

//    display.display_debug_file("Before the kids");
    waitpid(pid2, NULL, 0);

    if (pid2) {

        char buffer[1025];
        int num_bytes;
        while( (num_bytes = read(keep[0], buffer, 1024)) > 0 )
        {
            buffer[num_bytes] = 0;
            result += buffer;
        }
        close(keep[0]);
        display.display_debug_file("Finished");
    }
    if (keep_on_screen)
        display.display_a_new_line_for_each_endl(result);
//    display.display_message(result);
    return 0;
}

bool Terminal::this_command_exists(string name)
{
    string mt;
    execute_external_command("which " + name, "", mt, false);
    return !mt.empty();

}

const string &Terminal::getPath() const {
    return path;
}

string Terminal::trim_path_received(string path)
{
    //if i have /../ somewhere in the path -> remove the dir before it
    // maybe i have this /faculta/OS/terminal/../terminal/../unu/, also remove any /./

    vector<string>dirs;
    int start = 0;
    int next_slash = path.find("/");
    string new_dir;
    while(next_slash != string::npos)
    {
        new_dir = path.substr(start, next_slash - start);
        if (!new_dir.empty()) // to avoid the first / getting put as a dir later on
            dirs.push_back(path.substr(start, next_slash - start));
        start = next_slash + 1;
        next_slash = path.find("/", start);
    }
    new_dir = path.substr(start, next_slash - start);
    if (!new_dir.empty())
        dirs.push_back(path.substr(start, next_slash - start));

    vector<string>trimmed;

    for (string dir : dirs)
    {
        if (dir == ".")
        {
            //ignore it
            continue;
        } else if (dir == "..")
        {
            //now i should pop the previous folder, but if i have any
            if (!trimmed.empty())
            {
                trimmed.pop_back();
            }
        } else {
            //well at this stage, it is a normal name
            trimmed.push_back(dir);
        }
    }

    //now to reconstruct the string
    string result;
    for (string trim : trimmed)
    {
        result += "/" + trim;
    }

    //there could be that result is empty, if too many ../ were writen
    if (result.empty())
    {
        result = "/";
    }
    return result;

}

void Terminal::update_path(string path) {
    //this path could be home/area51/faculta/../../

    string trimmed_path = trim_path_received(path);

    display.display_debug("Variable this->path chaging to " + trimmed_path);
    this->path = trimmed_path;
    prefix[2] = "~";
    prefix[2].append(trimmed_path);
    display.display_debug("Prefix[2] is now " + prefix[2]);
    return;
}


#endif //TERMINAL_TERMINAL_H
