#include <iostream>
#include <fstream>
#include <cstdlib>
#include <windows.h>
#include <string>
#include <filesystem>
#include <winhttp.h> // Include the WinHTTP library

const std::string fileName = "JBomb.jar";

// Function to launch the JBomb Launcher in a new console
void launchGame(const std::string& command) {
    STARTUPINFOW si;
    PROCESS_INFORMATION pi;

    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    std::wstring commandW(command.begin(), command.end()); // Convert to wide string

    // Create a new console for the process
    if (!CreateProcessW(nullptr, &commandW[0], nullptr, nullptr, FALSE, CREATE_NO_WINDOW, nullptr, nullptr, &si, &pi)) {
        std::cerr << "Failed to launch the game. Error: " << GetLastError() << std::endl;
        return; // Exit the function if process creation fails
    }

    // Close handles to the process and thread, but let the process run independently
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
}

int main() {
    const std::string binDir = "bin";
    const std::string jbombJar = binDir + "\\" + fileName;  // Path for JBomb.jar


    launchGame("java -jar \"" + jbombJar + "\"");

    return 0;
}
