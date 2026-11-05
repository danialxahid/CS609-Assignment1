// CS609_Assignment1_TextFileAnalyzer
// Student Name: Muhammad Daniyal
// Student ID: BC220403190
// Course: CS609 - System Programming

#include <windows.h>
#include <iostream>
#include <string>

int main()
{
    std::string filePath;

    // Ask the user for the path of the text file.
    std::cout << "Enter the path of the text file: ";
    std::getline(std::cin, filePath);

    // Also allow a file path pasted with surrounding double quotation marks.
    if (filePath.length() >= 2 && filePath[0] == '"' &&
        filePath[filePath.length() - 1] == '"')
    {
        filePath = filePath.substr(1, filePath.length() - 2);
    }

    if (filePath.empty())
    {
        std::cerr << "Error: No file path was entered.\n";
        return 1;
    }

    // Open an existing file in read-only mode using the Windows API.
    HANDLE fileHandle = CreateFileA(
        filePath.c_str(),
        GENERIC_READ,
        FILE_SHARE_READ,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );

    if (fileHandle == INVALID_HANDLE_VALUE)
    {
        std::cerr << "Error: Unable to open the specified file. "
                  << "Windows error code: " << GetLastError() << "\n";
        return 1;
    }

    const DWORD BUFFER_SIZE = 1024;
    char buffer[BUFFER_SIZE];
    DWORD bytesRead = 0;
    unsigned long long totalBytesRead = 0;

    std::cout << "\nFile Contents:\n";

    // Read and display each chunk until ReadFile reaches the end of the file.
    while (true)
    {
        BOOL readSucceeded = ReadFile(
            fileHandle,
            buffer,
            BUFFER_SIZE,
            &bytesRead,
            NULL
        );

        if (!readSucceeded)
        {
            DWORD errorCode = GetLastError();
            CloseHandle(fileHandle);
            std::cerr << "\nError: Failed to read the file. "
                      << "Windows error code: " << errorCode << "\n";
            return 1;
        }

        if (bytesRead == 0)
        {
            break; // End of file.
        }

        // Display exactly the number of bytes returned by ReadFile.
        std::cout.write(buffer, bytesRead);
        if (!std::cout)
        {
            CloseHandle(fileHandle);
            std::cerr << "\nError: Failed to display the file contents.\n";
            return 1;
        }

        totalBytesRead += bytesRead;
    }

    // Release the Windows file handle after reading is complete.
    if (!CloseHandle(fileHandle))
    {
        std::cerr << "\nError: Failed to close the file handle. "
                  << "Windows error code: " << GetLastError() << "\n";
        return 1;
    }

    std::cout << "\n\nTotal bytes read: " << totalBytesRead << "\n";
    return 0;
}
