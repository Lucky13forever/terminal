#include <iostream>
#include <vector>
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
};

class Line{
private:
    vector<Message> messages;
public:
    Line() {};
    void add_message(const string & message, const int color );

    const vector<Message> &getMessages() const;

    void delete_last_message();
};

void Line::add_message(const string & message, const int color ) {
    Message new_message = *new Message(message, color);
    messages.push_back(new_message);
}

void Line::delete_last_message(){
    if (!messages.empty())
        messages.pop_back();
}

const vector<Message> &Line::getMessages() const {
    return messages;
}


class Scroll{
private:
    vector<Line> lines;
public:
    void new_line();
    void add_message_to_current_line(const string & message, const int color);

    const vector<Line> &getLines() const;
    const int get_nr_lines() const;

    void delete_last_message_from_current_line();
}scroll_object;

const vector<Line> &Scroll::getLines() const {
    return lines;
}
void Scroll::new_line(){
    this->lines.push_back(*new Line());
}

void Scroll::add_message_to_current_line(const string & message, const int color) {
    this->lines[ this->lines.size() - 1 ].add_message(message, color);
}

void Scroll::delete_last_message_from_current_line() {
    this->lines[ this->lines.size() - 1].delete_last_message();
}

const int Scroll::get_nr_lines() const {
    return this->lines.size();
}

int main()
{
    scroll_object.new_line();
    scroll_object.add_message_to_current_line("ohio", 42);
    scroll_object.add_message_to_current_line("batman", 50);

    scroll_object.new_line();
    scroll_object.add_message_to_current_line("ana", 49);
    scroll_object.add_message_to_current_line("manu", 51);

    scroll_object.delete_last_message_from_current_line();

    for(Line line : scroll_object.getLines())
    {
        for (Message message : line.getMessages())
        {
            cout << message.getColor() << " " << message.getMessage() << "\n";
        }
    }

    return 0;
}