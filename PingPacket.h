#include <cstdint>
#include <cstring>
#include <vector>
#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h> 
#include <arpa/inet.h>

class PingPacket{
    uint8_t type;
    uint8_t code;
    uint16_t checksum = 0;
    uint16_t identifier;
    uint16_t sequenceNumber;
public:
    PingPacket(uint8_t type, uint8_t code, uint16_t identifier, uint16_t sequenceNumber){
        type = type;
        code = code;
        identifier = identifier;
        sequenceNumber = sequenceNumber;
    }
    std::vector<uint8_t> serialize();
    int sendPacket();
};
