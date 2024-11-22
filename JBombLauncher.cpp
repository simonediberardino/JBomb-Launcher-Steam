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

bool downloadFile(const std::string& url, const std::string& outputPath) {
    std::string host;
    std::string path;

    // Extract host and path from the URL
    size_t protocolEnd = url.find("://") + 3; // Find end of protocol
    size_t pathStart = url.find('/', protocolEnd); // Find start of path
    host = url.substr(protocolEnd, pathStart - protocolEnd); // Extract host
    path = url.substr(pathStart); // Extract path

    // Initialize WinHTTP session
    HINTERNET hSession = WinHttpOpen(L"A WinHTTP Example Program/1.0",
        WINHTTP_ACCESS_TYPE_DEFAULT_PROXY,
        WINHTTP_NO_PROXY_NAME,
        WINHTTP_NO_PROXY_BYPASS, 0);
    if (!hSession) {
        std::cerr << "WinHttpOpen failed. Error: " << GetLastError() << std::endl;
        return false;
    }

    // Connect using HTTPS port
    HINTERNET hConnect = WinHttpConnect(hSession, std::wstring(host.begin(), host.end()).c_str(), INTERNET_DEFAULT_HTTPS_PORT, 0);
    if (!hConnect) {
        std::cerr << "WinHttpConnect failed. Error: " << GetLastError() << std::endl;
        WinHttpCloseHandle(hSession);
        return false;
    }

    // Open a request with INTERNET_FLAG_SECURE for HTTPS
    HINTERNET hRequest = WinHttpOpenRequest(hConnect, L"GET", std::wstring(path.begin(), path.end()).c_str(),
        nullptr, WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES, WINHTTP_FLAG_SECURE);
    if (!hRequest) {
        std::cerr << "WinHttpOpenRequest failed. Error: " << GetLastError() << std::endl;
        WinHttpCloseHandle(hConnect);
        WinHttpCloseHandle(hSession);
        return false;
    }

    // Send the request
    if (WinHttpSendRequest(hRequest, WINHTTP_NO_ADDITIONAL_HEADERS, 0, WINHTTP_NO_REQUEST_DATA, 0, 0, 0) == FALSE) {
        std::cerr << "WinHttpSendRequest failed. Error: " << GetLastError() << std::endl;
        WinHttpCloseHandle(hRequest);
        WinHttpCloseHandle(hConnect);
        WinHttpCloseHandle(hSession);
        return false;
    }

    // Receive the response
    if (WinHttpReceiveResponse(hRequest, NULL) == FALSE) {
        std::cerr << "WinHttpReceiveResponse failed. Error: " << GetLastError() << std::endl;
        WinHttpCloseHandle(hRequest);
        WinHttpCloseHandle(hConnect);
        WinHttpCloseHandle(hSession);
        return false;
    }

    // Open file for writing
    std::ofstream outputFile(outputPath, std::ios::binary);
    if (!outputFile) {
        std::cerr << "Failed to open output file: " << outputPath << std::endl;
        WinHttpCloseHandle(hRequest);
        WinHttpCloseHandle(hConnect);
        WinHttpCloseHandle(hSession);
        return false;
    }

    // Read data from the response and write to file
    DWORD bytesRead;
    char buffer[4096];
    while (WinHttpReadData(hRequest, buffer, sizeof(buffer), &bytesRead) && bytesRead > 0) {
        outputFile.write(buffer, bytesRead);
    }

    // Cleanup
    outputFile.close();
    WinHttpCloseHandle(hRequest);
    WinHttpCloseHandle(hConnect);
    WinHttpCloseHandle(hSession);

    return true;
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
