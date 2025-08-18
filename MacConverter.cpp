// MacConverter.cpp : This file contains the 'main' function. Program execution begins and ends there.

#include <string>
#include <vector>
#include <chrono>
#include <iomanip>
#include <fstream>
#include <numeric>
#include <ShlObj.h>
#include <iostream>
#include <algorithm>
#include <windows.h>
#pragma comment(lib, "shell32.lib")

using namespace std;

wstring getDocumentsFolderPath();
void formatMacs(vector<string>& macs);
string stripMac(string inputMac);
string formatColon(string strippedMac);
string formatDot(string strippedMac);
string formatDash(string strippedMac);
void logMacs(vector<string>& macs);
bool AskToRunAgain();

wstring documentsPath = getDocumentsFolderPath();
wstring logPath = documentsPath + L"\\" + L"mac.log";


int main()
{
    bool bRunAgain = false;
    do
    {
		// Prevent system from sleeping and display from turning off
		SetThreadExecutionState(ES_CONTINUOUS | ES_SYSTEM_REQUIRED | ES_DISPLAY_REQUIRED);

        cout << "Enter as many macs as you would like converted then, press enter twice:" << endl;
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

        formatMacs(strippedMacs);
        vector<string> macs = strippedMacs;

        logMacs(macs);

        vector<string>::iterator it3;
        for (it3 = macs.begin(); it3 != macs.end(); it3++)
        {
            cout << *it3 << '\n';
        }

        // auto function to insert newlines between elements in vector
        auto newLineFold = [](string a, string b)
        {
            return move(a) + '\n' + b;
        };
        
        // makes the input vector into a string for conversion to c string
        string macStrings = accumulate(next(macs.begin()), macs.end(), macs[0], newLineFold);
        const char* macChars = macStrings.c_str();

        // saves c string to clipboard
        if (OpenClipboard(NULL))
        {
            EmptyClipboard();
            HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, strlen(macChars) + 1);
            if (hMem)
            {            
                char* pMem = (char*)GlobalLock(hMem);
                strcpy_s(pMem, strlen(macChars) + 1, macChars);
                GlobalUnlock(hMem);
                SetClipboardData(CF_TEXT, hMem);
            }
            CloseClipboard();
        }

        cout << "All the conversions have been output to the console, and in your copy buffer." << endl;
        wstring logPathMessage = L"All the conversions have been saved to: " + logPath;
        wcout << logPathMessage << endl;

        bRunAgain = AskToRunAgain();
    } while (bRunAgain);

	// To allow the system to sleep again (when your program no longer needs to prevent it)
	SetThreadExecutionState(ES_CONTINUOUS);
    return 0;
}

wstring getDocumentsFolderPath()
{
    PWSTR path = nullptr;
    HRESULT hr = SHGetKnownFolderPath(FOLDERID_Documents, 0, nullptr, &path);
    if (SUCCEEDED(hr))
    {
        wstring documentsPath(path);
        CoTaskMemFree(path);
        return documentsPath;
    }
    return L"";
}

string stripMac(string inputMac)
{
    string stripChars = " ,.:-";
    for (char b : stripChars)
    {
        inputMac.erase(remove(inputMac.begin(), inputMac.end(), b), inputMac.end());
    }
    string lowerMac;
    for (size_t i = 0; i < inputMac.length(); i++)
    {
        lowerMac += tolower(inputMac[i]);
    }
    return (lowerMac);
}

void formatMacs(vector<string>& macs)
{
    vector<string>::iterator it;
    vector<string> formattedMacs;
    vector<string> colonMacs;
    vector<string> dotMacs;
    vector<string> dashMacs;

    for (it = macs.begin(); it != macs.end(); it++)
    {
        formattedMacs.push_back(*it);
        colonMacs.push_back(formatColon(*it));
        dotMacs.push_back(formatDot(*it));
        dashMacs.push_back(formatDash(*it));
        // cout << *it << '\n';
    }
    formattedMacs.push_back("");
    formattedMacs.insert(end(formattedMacs), begin(colonMacs), end(colonMacs));
    formattedMacs.push_back("");
    formattedMacs.insert(end(formattedMacs), begin(dotMacs), end(dotMacs));
    formattedMacs.push_back("");
    formattedMacs.insert(end(formattedMacs), begin(dashMacs), end(dashMacs));
    
    macs = formattedMacs;
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

void logMacs(vector<string>& macs)
{
    ofstream logFile(logPath, ios_base::app);
    if (logFile.is_open())
    {
        auto now = chrono::system_clock::now();
        time_t nowTime = chrono::system_clock::to_time_t(now);
        struct tm timeInfo;
        if (localtime_s(&timeInfo, &nowTime) == 0)
        {
            logFile << put_time(&timeInfo, "%Y-%m-%d %H:%M:%S") << endl;

            vector<string>::iterator it;
            for (it = macs.begin(); it != macs.end(); it++)
            {
                logFile << "\t" << *it << endl;
            }
            logFile << "\n";
        }
        else
        {
            cerr << "Failed to get local time." << endl;
        }
        logFile.close();
    }
    else
    {
        cerr << "Error opening log file!" << endl;
    }

}

bool AskToRunAgain()
{
    cout << "Do you want to run again (y/n)? ";
    string Response = "";
    getline(cin, Response);
    if (Response.empty())
    {
        return true;
    }
    return (Response[0] == 'y') || (Response[0] == 'Y');
}