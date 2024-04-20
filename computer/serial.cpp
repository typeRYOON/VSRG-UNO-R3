#include "serial.h"
#include <iostream>
#include <fstream>
#include <time.h>


SimpleSerial::SimpleSerial(const std::string &comPort, DWORD COM_BAUD_RATE)
{
    this->init(comPort, COM_BAUD_RATE);
}


SimpleSerial::~SimpleSerial()
{
    if (connected)
    {
        connected = false;
        CloseHandle(IOHandler);
    }
}


void SimpleSerial::init(const std::string &comPort, DWORD COM_BAUD_RATE)
{
    if (connected) {
        std::cout << "Error :: COM port already in use.\n";
        return;
    }

    IOHandler = CreateFileA(
        comPort.c_str(),
        GENERIC_READ | GENERIC_WRITE,
        0,
        nullptr,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        nullptr
        );

    if (IOHandler == INVALID_HANDLE_VALUE) {
        if (GetLastError() == ERROR_FILE_NOT_FOUND) {
            std::cout << "Error :: " << comPort << " not available.\n";
            return;
        }
    }

    DCB dcbSerialParams = {0};
    if (!GetCommState(IOHandler, &dcbSerialParams)) {
        std::cout << "Error :: Failed to get current serial params\n";
        return;
    }

    dcbSerialParams.BaudRate    = COM_BAUD_RATE;
    dcbSerialParams.ByteSize    = 8;
    dcbSerialParams.StopBits    = ONESTOPBIT;
    dcbSerialParams.Parity      = NOPARITY;
    dcbSerialParams.fDtrControl = DTR_CONTROL_ENABLE;

    if (!SetCommState(IOHandler, &dcbSerialParams)) {
        std::cout << "Warning: could not set serial port params\n";
        return;
    }

    connected = true;
    PurgeComm(IOHandler, PURGE_RXCLEAR | PURGE_TXCLEAR);
}


void SimpleSerial::CustomSyntax(const std::string& syntaxType)
{
    std::ifstream syntaxExists("syntax.cfg");
    if (!syntaxExists) {
        std::ofstream syntaxfile;
        syntaxfile.open("syntax.cfg");

        if (syntaxfile) {
            syntaxfile << "json { }\n";
            syntaxfile << "greater_less_than < >\n";
            syntaxfile.close();
        }
    }
    syntaxExists.close();

    std::ifstream syntaxIN;
    syntaxIN.open("syntax.cfg");

    std::string line;
    bool found = false;

    if (!syntaxIN.is_open()) {
        std::cout << "Warning: No file open\n";
        return;
    }

    while (syntaxIN)
    {
        syntaxIN >> syntaxName >> frontDelimiter >> endDelimiter;
        getline(syntaxIN, line);

        if (syntaxName == syntaxType) {
            found = true;
            break;
        }
    }
    syntaxIN.close();

    if (!found) {
        syntaxName = "";
        frontDelimiter = ' ';
        endDelimiter = ' ';
        std::cout << "Warning: Could not find delimiters, may cause problems!\n";
    }
}


std::string SimpleSerial::ReadSerialPort(int reply_wait_time, const std::string& syntaxType)
{
    DWORD readBytes;
    char inc_msg[1];
    std::string complete_inc_msg;
    bool began = false;

    CustomSyntax(syntaxType);
    time_t start = time(nullptr);
    ClearCommError(IOHandler, &errors, &status);

    while ((time(nullptr) - start) < reply_wait_time)
    {
        if (status.cbInQue)
        {
            if (ReadFile(IOHandler, inc_msg, 1, &readBytes, nullptr))
            {
                if (inc_msg[0] == frontDelimiter || began)
                {
                    began = true;

                    if (inc_msg[0] == endDelimiter)
                    {
                        return complete_inc_msg;
                    }

                    if (inc_msg[0] != frontDelimiter)
                    {
                        complete_inc_msg.append(inc_msg, 1);
                    }
                }
            }
            else
            {
                return "Warning: Failed to receive data.\n";
            }
        }
    }
    return complete_inc_msg;
}


bool SimpleSerial::WriteSerialPort(const std::string &data)
{
    DWORD sentBytes;
    unsigned int dataLength = static_cast<unsigned int>(data.length());

    if (!WriteFile(IOHandler, data.c_str(), dataLength, &sentBytes, nullptr))
    {
        ClearCommError(IOHandler, &errors, &status);
        return false;
    }
    return true;
}


bool SimpleSerial::CloseSerialPort()
{
    if (connected) {
        connected = false;
        CloseHandle(IOHandler);
        return true;
    }
    return false;
}


bool SimpleSerial::IsConnected() const { return this->connected; }
