//Write a program to determine if anything in the rockyou.txt would pass that list, and if so print them to screen.

//password requirements : At least 2 uppercase letters, Plus 2 lowercase letters, Plus 2 numbers, Plus 2 punctuation marks
//https://dumbpasswordrules.com/sites/alibaba/

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>

using namespace std;

int main()
{
    int password_count = 0;
    ifstream file("rockyou.txt");
    string line;
    vector<string> passwords;
    while (getline(file, line))
    {
        passwords.push_back(line);
    }
    file.close();

    cout << "Total passwords: " << passwords.size() << endl;

    for (int i = 0; i < passwords.size(); i++)
    {
        string password = passwords[i];
        int upper = 0;
        int lower = 0;
        int number = 0;
        int punctuation = 0;
        for (int j = 0; j < password.size(); j++)
        {
            if (password[j] >= 'A' && password[j] <= 'Z')
            {
                if(i == 6656402){
                    cout << password[j] << ": uppercase" << endl;
                };
                upper++;
            }
            else if (password[j] >= 'a' && password[j] <= 'z')
            {
                if(i == 6656402){
                    cout << password[j] << ": lowercase" << endl;
                };
                lower++;
            }
            else if (password[j] >= '0' && password[j] <= '9')
            {
                if(i == 6656402){
                    cout << password[j] << ": number" << endl;
                };
                number++;
            }
            else
            {
                if(i == 6656402){
                    cout << password[j] << ": special character" << endl;
                };
                punctuation++;
            }
        }
        if (upper >= 2 && lower >= 2 && number >= 2 && punctuation >= 2)
        {
            cout << password << endl;
            password_count++;
        }
    }

    cout << "Total passwords that pass the requirements: " << password_count << endl;
    cout << fixed << setprecision(2);
    cout << "% of passwords that pass the requirements: " << (static_cast<double>(password_count) * 100) / passwords.size() << "%" << endl;
    return 0;
}