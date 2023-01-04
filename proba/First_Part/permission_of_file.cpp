#include <iostream>
#include <sys/stat.h>
#include <vector>
using namespace std;

string return_permission_string(string file)
{
    struct stat st;
    stat(file.c_str(), &st);

    string result;
    //asta voi face cu functie de type la fisier
    result.append( "d" );
    result.append( (st.st_mode & S_IRUSR) ? "r" : "-" );
    result.append( (st.st_mode & S_IWUSR) ? "w" : "-" );
    result.append( (st.st_mode & S_IXUSR) ? "x" : "-" );
    result.append( (st.st_mode & S_IRGRP) ? "r" : "-" );
    result.append( (st.st_mode & S_IWGRP) ? "w" : "-" );
    result.append( (st.st_mode & S_IXGRP) ? "x" : "-" );
    result.append( (st.st_mode & S_IROTH) ? "r" : "-" );
    result.append( (st.st_mode & S_IWOTH) ? "w" : "-" );
    result.append( (st.st_mode & S_IXOTH) ? "x" : "-" );

    return result;
}
int main()
{
    string file;

//    cout << return_permission_string(file);

    const char * aici = "d";
    file += aici;
    cout << file;
    return 0;
}