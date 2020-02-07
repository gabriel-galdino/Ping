#include "PingPacket.h"

std::vector<uint8_t> PingPacket::serialize(){
    uint8_t varaux;
    std::vector<uint8_t> serializedData;
    
    serializedData.push_back(_type);
    serializedData.push_back(_code);
    _checksum = calculateChecksum(serializedData);
    
    varaux = static_cast<uint8_t> (_checksum >> 8);
    serializedData.push_back(static_cast<uint8_t> (_checksum));
    serializedData.push_back(varaux);
    
    varaux = static_cast<uint8_t> (_identifier >> 8);
    serializedData.push_back(static_cast<uint8_t> (_identifier));
    serializedData.push_back(varaux);

    varaux = static_cast<uint8_t> (_sequenceNumber >> 8);
    serializedData.push_back(static_cast<uint8_t> (_sequenceNumber));
    serializedData.push_back(varaux);

    return serializedData;
}

int PingPacket::sendPacket(){
    int nbytes;
    int sock_fd;
    struct sockaddr_in address;
    bzero(&address, sizeof(address));
    address.sin_family = AF_INET;
    if(inet_pton(AF_INET, "8.8.8.8", &(address.sin_addr)) <= 0)
        std::cout << "Erro na   conversão da string para endereço IP" << std::endl;
    std::vector<uint8_t> serialzedPacket = serialize();
    if((sock_fd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP)) > 0){
        struct sockaddr* convertedAddr = reinterpret_cast<sockaddr*>(&address);
        printPacket(serialzedPacket);
        return sendto(sock_fd, serialzedPacket.data(), serialzedPacket.size(), 0, convertedAddr, sizeof(sockaddr));
    }
    else std::cout << "erro " << sock_fd << std::endl;
    return -1;
}

uint16_t PingPacket::calculateChecksum(std::vector<uint8_t> serializedData){
    uint32_t sum = 0;
    uint16_t checksum;
    for(int i = 0; i < serializedData.size(); i+=2){
        sum += serializedData[i] + serializedData[i + 1];
    }
    if(serializedData.size() & 1)
        sum += (serializedData[serializedData.size() - 1] << 8);        

    checksum = ~sum;
    return checksum;
}

// uint16_t PingPacket::calculateChecksum(void* serializedData, size_t len){
//     char* data  = reinterpret_cast<char*>(serializedData); 
//     uint32_t acc = 0xffff;
//     for(size_t i = 0; i +1 < len;  i+= 2){
//         uint16_t word;
//         memcpy(&word, data + i, 2);
//         acc += ntohs(word);
//         if(acc > 0xffff)
//             acc -= 0xffff;
//     }
//     if(len & 1){
//         uint16_t word = 0;
//         memcpy(&word, data + len -1, 1);
//         acc += ntohs(word);
//         if(acc > 0xffff)
//             acc -= 0xffff;
//     }
//     return htons(~acc);
// }


void PingPacket::printPacket(std::vector<uint8_t> data){
    for(int i = 0; i < data.size(); i++){
        std::cout << std::hex 
                  << std::setfill('0') 
                  << std::setw(2) << static_cast<unsigned int>(data[i]) 
                  << " "
                  << std::endl;
    }
}   