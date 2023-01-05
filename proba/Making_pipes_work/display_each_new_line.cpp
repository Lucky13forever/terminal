#include <iostream>
using namespace std;
int main()
{
    string result = "here\n"
                    "i\n"
                    "go\n"
                    "again\n";

    int start = 0;
    int end = result.find('\n', start);
    while(end != std::string::npos)
    {
        string substring = result.substr(start, end - start);

        cout << substring << "\n\n\n\n";

        start = end + 1;

        end = result.find('\n', start);
    }
    return 0;
}