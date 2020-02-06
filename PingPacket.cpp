#include "PingPacket.h"

std::vector<uint8_t> PingPacket::serialize(){
    std::vector<uint8_t> aux;
    aux.push_back(type);
    aux.push_back(code);
    aux.push_back(0);
    aux.push_back(0);
    uint8_t varaux = static_cast<uint8_t> (identifier >> 8);
    aux.push_back(varaux);
    aux.push_back(static_cast<uint8_t> (identifier));
    varaux = static_cast<uint8_t> (sequenceNumber >> 8);
    aux.push_back(varaux);
    aux.push_back(static_cast<uint8_t> (sequenceNumber));
    return aux;
}

int PingPacket::sendPacket(){
    int sock_fd;
    struct sockaddr_in address;
    bzero(&address, sizeof(address));
    address.sin_family = AF_INET;
    if(inet_pton(AF_INET, "8.8.8.8", &(address.sin_addr)) <= 0)
        std::cout << "Erro na conversão da string para endereço IP" << std::endl;
    std::vector<uint8_t> serialzedPacket = serialize();
    if((sock_fd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP)) > 0){
        struct sockaddr* addr = reinterpret_cast<sockaddr*>(&address);
        return sendto(sock_fd, serialzedPacket.data(), serialzedPacket.size(), 0, addr, sizeof(sockaddr));
    }
    else std::cout << "erro " << sock_fd << std::endl;
    return -1;
}
