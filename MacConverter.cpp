// MacConverter.cpp : This file contains the 'main' function. Program execution begins and ends there.

#include <string>
#include <vector>
#include <iostream>

using namespace std;

string stripMac(string inputMac);
string formatColon(string strippedMac);
string formatDot(string strippedMac);
string formatDash(string strippedMac);
bool AskToRunAgain();


int main()
{
    bool bRunAgain = false;
    do
    {
        string mac;
        vector<string> macVectors;
        while (getline(cin, mac))
        {
            if (mac.empty())
            {
                break;
            }
            macVectors.push_back(mac);
        }
        vector<string>::iterator it;
        vector<string> strippedMacs;
        for (it = macVectors.begin(); it != macVectors.end(); it++)
        {
            strippedMacs.push_back(stripMac(*it));
        }

        vector<string>::iterator it2;
        vector<string> formattedMacs;
        for (it2 = strippedMacs.begin(); it2 != strippedMacs.end(); it2++)
        {
            formattedMacs.push_back(formatColon(*it2));
            formattedMacs.push_back(formatDot(*it2));
            formattedMacs.push_back(formatDash(*it2));
            cout << *it << '\n';
        }

        vector<string>::iterator it3;
        for (it3 = formattedMacs.begin(); it3 != formattedMacs.end(); it3++)
        {
            cout << *it3 << '\n';
        }
        bRunAgain = AskToRunAgain();
    } while (bRunAgain);
    return 0;
}

string stripMac(string inputMac)
{
    string stripChars = " ,.:-";
    for (char c : stripChars)
    {
        inputMac.erase(remove(inputMac.begin(), inputMac.end(), c), inputMac.end());
    }
    string lowerMac;
    for (int i = 0; i < inputMac.length(); i++)
    {
        lowerMac += tolower(inputMac[i]);
    }
    return (lowerMac);
}

string formatColon(string strippedMac)
{
    string formattedMac;

    for (int i = 0; i < strippedMac.length(); i++)
    {
        formattedMac += strippedMac[i];

        if (i % 2 == 1 && i < 11)
        {
            formattedMac += ":";
        }
    }

    return formattedMac;
}

string formatDot(string strippedMac)
{
    string formattedMac;

    for (int i = 0; i < strippedMac.length(); i++)
    {
        formattedMac += strippedMac[i];
        if (i % 4 == 3 && i < 11)
        {
            formattedMac += ".";
        }
    }

    return formattedMac;
}

string formatDash(string strippedMac)
{
    string formattedMac;

    for (int i = 0; i < strippedMac.length(); i++)
    {
        formattedMac += strippedMac[i];
        if (i % 6 == 5 && i < 11)
        {
            formattedMac += "-";
        }
    }

    return formattedMac;
}

bool AskToRunAgain()
{
    cout << "Do you want to run again (y/n)? ";
    string Response = "";
    getline(cin, Response);
    return (Response[0] == 'y') || (Response[0] == 'Y');
}