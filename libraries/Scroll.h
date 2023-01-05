//
// Created by area51 on 26.12.22.
//

#ifndef TERMINAL_SCROLL_H
#define TERMINAL_SCROLL_H
using namespace std;

class Message{
private:
    string message;
    int color;
public:
    Message(const string &message, int color) : message(message), color(color) {}

    const string &getMessage() const {
        return message;
    }

    int getColor() const {
        return color;
    }

    void delete_last_char() {
        if (!message.empty())
            message.erase( message.length() - 1, 1 );
    }
};

class Line{
private:
    vector<Message> messages;
public:
    Line() {};
    void add_message(const string & message, const int color );

    const vector<Message> &getMessages() const;

    void delete_last_message();

    void delete_last_char();
};

void Line::add_message(const string & message, const int color ) {
    Message new_message = *new Message(message, color);
    messages.push_back(new_message);
}

void Line::delete_last_message(){
    if (!messages.empty())
        messages.pop_back();
}
void Line::delete_last_char(){
    messages.back().delete_last_char();
}

const vector<Message> &Line::getMessages() const {
    return messages;
}


class Scroll{
private:
    vector<Line> lines;
public:
    void clear() {lines.clear();};
    void clear_last_line() {lines.pop_back(); lines.push_back(*new Line);};
    void new_line();
    void add_message_to_current_line(const string & message, const int color);

    const vector<Line> &getLines() const;
    const int get_nr_of_lines() const;

    void delete_last_message_from_current_line();

    Line get_line(int i) {return this->lines[i];};

    void delete_last_char_from_current_line();
}scroll_object;

const vector<Line> &Scroll::getLines() const {
    return lines;
}
void Scroll::new_line(){
    this->lines.push_back(*new Line());
}

void Scroll::add_message_to_current_line(const string & message, const int color) {
    if (this->lines.empty())
        this->new_line();
    this->lines[ this->lines.size() - 1 ].add_message(message, color);
}

void Scroll::delete_last_message_from_current_line() {
    this->lines[ this->lines.size() - 1].delete_last_message();
}
void Scroll::delete_last_char_from_current_line() {
    this->lines[ this->lines.size() - 1].delete_last_char();
}

const int Scroll::get_nr_of_lines() const {
    return this->lines.size();
}

#endif //TERMINAL_SCROLL_H
