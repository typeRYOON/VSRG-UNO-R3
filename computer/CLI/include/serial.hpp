#pragma once
#include <Windows.h>
#include <string>


class SimpleSerial {
private:
    HANDLE IOHandler;
    COMSTAT status;
    DWORD errors;

    std::string syntaxName;
    bool connected = false;
    char frontDelimiter;
    char endDelimiter;

    void CustomSyntax(const std::string& syntax_type);	

public:
    SimpleSerial(const std::string& comPort, DWORD COM_BAUD_RATE);
   ~SimpleSerial();
    void init(const std::string& comPort, DWORD COM_BAUD_RATE);
    std::string ReadSerialPort(int reply_wait_time, const std::string& syntax_type);
    bool WriteSerialPort(const std::string& data_sent);
    bool CloseSerialPort();
    bool IsConnected() const;
};
