#include<iostream>
using namespace std;
int main ()
{
    char c;
    cout << "Enter a character : ";
    cin >> c;
    for (; c < 'f'; c++)
        std::cout << c << std::endl;
    cout << "ASCII value of " << c <<" is :  " << (int)c;
    return 0;
}