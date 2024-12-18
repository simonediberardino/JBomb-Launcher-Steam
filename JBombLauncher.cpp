#include <iostream>
#include <fstream>
#include <cstdlib>
#include <windows.h>
#include <string>
#include <filesystem>
#include <winhttp.h> // Include the WinHTTP library

const std::string fileName = "JBomb.jar";
const std::string javaVersion = "jre1.8.0_201";

// Use _dupenv_s to safely retrieve environment variables
std::string getEnvVar(const char* varName) {
    char* value;
    size_t size;
    if (_dupenv_s(&value, &size, varName) == 0 && value != nullptr) {
        std::string result(value);
        free(value); // Don't forget to free the allocated memory
        return result;
    }
    return ""; // Return empty string if failed
}

void launchProcess(const std::string& command) {
    STARTUPINFOW si;
    PROCESS_INFORMATION pi;

    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    std::wstring commandW(command.begin(), command.end()); // Convert to wide string

    DWORD dwCreationFlags = 0;

    if (!CreateProcessW(nullptr, &commandW[0], nullptr, nullptr, FALSE, CREATE_NO_WINDOW, nullptr, nullptr, &si, &pi)) {
        std::cerr << "Failed to launch the game. Error: " << GetLastError() << std::endl;
        return; // Exit the function if process creation fails
    }

    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
}


int main() {
    const std::string localAppDataDir = getEnvVar("LOCALAPPDATA") + "\\JBomb"; 
    const std::string jbombJar = "bin\\" + fileName;  

    const std::string java = "bin\\" + javaVersion + "\\bin\\java.exe";
    const std::string launchCommand = java + " -jar \"" + jbombJar + "\"";

    launchProcess(launchCommand);
}
