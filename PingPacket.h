#include <cstdint>
#include <cstring>
#include <iomanip>
#include <vector>
#include <string>
#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h> 
#include <arpa/inet.h>


class PingPacket{
private:
    uint8_t _type = 8;
    uint8_t _code;
    uint16_t _checksum = 0;
    uint16_t _identifier;
    uint16_t _sequenceNumber;
public:
    PingPacket(uint8_t type, uint8_t code, uint16_t identifier, uint16_t sequenceNumber){
        _type = type;
        _code = code;
        _identifier = identifier;
        _sequenceNumber = sequenceNumber;
    }
    std::vector<uint8_t> serialize();
    uint16_t calculateChecksum(std::vector<uint8_t>);
    int sendPacket();
    void extractIcmpHeader(std::vector<uint8_t>);
    void printPacket(std::vector<uint8_t>);
};
