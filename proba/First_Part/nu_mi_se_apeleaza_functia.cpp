#include <bits/stdc++.h>
using namespace std;

class Message{
private:
    string message;
    int color;
public:
    string getMessage()  {
        return message;
    }

    int getColor()  {
        return color;
    }
    Message(string message, int color) : message(message), color(color) {}
};

class Line{
private:
    vector<Message> messages;
public:
    vector<Message> getMessages() {return messages;};
    Line();
    void add_message(Message);

    void delete_last_message();
};

void Line::add_message(Message message) {
    this->messages.push_back(message);
}

void Line::delete_last_message() {
    if (!this->messages.empty())
        this->messages.pop_back();
}

Line::Line() {
    Message * new_message = new Message("", 0);
    this->messages.push_back(*new_message);
}


class Scroll{
private:


    vector<Line> lines;


public:
    Line get_current_line();
    vector<Line> getLines() {
        return lines;
    }
    void clear(){
        lines.clear();
        this->new_line();//create the first line empty
    }
    //append the latest message to the latest line
    void append_message_to_current_line(string message, int color);
    //when backspacing, last message will be deleted
    void delete_last_message_from_current_line();
    //just create a new line, push_back a new empty line
    void new_line();


    int get_nr_of_lines();

    Line get_line(int index);
}scroll_object;

void Scroll::append_message_to_current_line(string message, int color) {
    Line current_line = this->get_current_line(); // last line -> current
    Message * new_message = new Message(message, color);
    current_line.add_message(*new_message);
}

void Scroll::new_line() {
    Line * new_line = new Line();
    lines.push_back(*new_line);
}

void Scroll::delete_last_message_from_current_line() {
    Line current_line = this->get_current_line();
    current_line.delete_last_message();
}

Line Scroll::get_line(int index)
{
    return this->lines[index];
}

Line Scroll::get_current_line() {
    if(this->lines.empty())
    {
        new_line();
    }
    return this->lines.back();
}

int Scroll::get_nr_of_lines() {
    return this->lines.size();
}


int main()
{

    scroll_object.new_line();
    scroll_object.append_message_to_current_line("mesaj", 1);

    scroll_object.getLines().back().getMessages().back().getMessage();

    return 0;
}