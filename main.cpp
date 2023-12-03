#include <windows.h>
#include <sapi.h>

#include <iostream>
#include <chrono>
#include <iomanip>
#include <ctime>
#include <thread>
#include <csignal>


ISpVoice* pVoice = nullptr;

// Convert to 12hr AM/PM
std::string TimeToString(const std::tm& timeStruct) {
    char buffer[64];
    strftime(buffer, sizeof(buffer), "%I:%M %p", &timeStruct);
    return buffer;
}

// Handler for dectecting when window is closed
BOOL WINAPI ConsoleHandler(DWORD dwCtrlType) {
    if (pVoice) {
        pVoice->Release();
    }

    CoUninitialize();

    return TRUE;
}

int main(int, char**) {
    AllocConsole();                     // Alocate a console (Opens one on executable startup)
    freopen("CONOUT$", "w", stdout);    // Redirect output to CMD window
    freopen("CONIN$", "r", stdin);      // Redirect Input to CMD Window

    CoInitialize(NULL);

    HRESULT hr = CoCreateInstance(CLSID_SpVoice, NULL, CLSCTX_ALL, IID_ISpVoice, (void**)&pVoice);

    if (FAILED(hr)) {
        std::cerr << "Failed to initialize SAPI." << std::endl;
        return 1;
    }

    std::string lastSpokenTime = "";

    SetConsoleCtrlHandler(ConsoleHandler, TRUE);

    while (true) {
        // Get current Time
        auto currentTime = std::chrono::system_clock::now();
        std::time_t timeNow = std::chrono::system_clock::to_time_t(currentTime);

        // Convert currentTime to the local time of the user
        std::tm localTime = *std::localtime(&timeNow);
        std::string currentTimeStr = TimeToString(localTime);

        // Check if time has passed
        if (currentTimeStr != lastSpokenTime) {
            std::cout << "The current time is: " << currentTimeStr << std::endl;
            lastSpokenTime = currentTimeStr;

            // Use SAPI to speak the message
            std::wstring wideTimeStr(currentTimeStr.begin(), currentTimeStr.end());
            std::wstring message = L"The current time is " + wideTimeStr;

            pVoice->Speak(message.c_str(), 0, NULL);
        }

        // Avoid the CPU from being abused
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    return 0;
}
