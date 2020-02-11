#include "PingPacket.h"

std::vector<uint8_t> PingPacket::serialize(){
    uint8_t varaux;
    std::vector<uint8_t> serializedData;

    serializedData.push_back(_type);
    serializedData.push_back(_code);
    
    serializedData.push_back(0);
    serializedData.push_back(0);
    
    varaux = static_cast<uint8_t> (_identifier >> 8);
    serializedData.push_back(static_cast<uint8_t> (_identifier));
    serializedData.push_back(varaux); 

    varaux = static_cast<uint8_t> (_sequenceNumber >> 8);
    serializedData.push_back(static_cast<uint8_t> (_sequenceNumber));
    serializedData.push_back(varaux);


    _checksum = calculateChecksum(serializedData);

    varaux = static_cast<uint8_t> (_checksum >> 8);
    serializedData[2] = static_cast<uint8_t> (_checksum);
    serializedData[3] = varaux;

    return serializedData;
}

int PingPacket::sendPacket(){
    bool pingLoop = true;
    int sockfd, nsent, nread, seqNum;
    std::vector<uint8_t> packet;
    std::vector<uint8_t> recvdPacket(1024);
    struct sockaddr_in address;

    if((sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP)) < 0){
      std::cout << "Falha em adquirir descritor de arquivo" << std::endl;
      return -1;    
    }

    if((sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP)) < 0){
      std::cout << "Falha em adquirir descritor de arquivo" << std::endl;
      return -1;    
    }
    
    bzero(&address, sizeof(address));
    address.sin_family = AF_INET;
    
    if(inet_pton(AF_INET, "8.8.8.8", &(address.sin_addr)) <= 0)
        std::cout << "Erro na   conversão da string para endereço IP" << std::endl;
    
    while(pingLoop){
        packet = serialize();
        printPacket(packet);
        struct sockaddr* convertedAddr = reinterpret_cast<sockaddr*>(&address);
        
        if(nsent = sendto(sockfd, packet.data(), packet.size(), 0, convertedAddr, sizeof(sockaddr))){
          std::cout << nsent << std::endl;
        }
        else {
          std::cout << "Erro ao enviar pacote" << std::endl;
        }

        nread = recv(sockfd, recvdPacket.data(), 1024, 0);
        if( nread == -1){
          std::cout << "falha ao obter pacote" << std::endl;
        }
        else if(nread == 0){
          std::cout << "desconectado do socket destino" << std::endl;
        }
        else
        {
          std::cout << std::dec << nread << std::endl;
          // for(int i = 0; i < 28; i++){
          //   std::cout << std::hex << ntohs(recvdPacket[i]) << std::endl;
          // }
          extractIcmpHeader(recvdPacket);
        }
        pingLoop = false;
    }
    return 0;

}

void PingPacket::extractIcmpHeader(std::vector<uint8_t> recvdPacket){
  std::vector<uint8_t> icmpHeader;
  for(int i = 20; i < 28; i+=2){
      std::cout << std::hex << ntohs(recvdPacket[i] + recvdPacket[i + 1] * 256) << std::endl;
  }
}

uint16_t PingPacket::calculateChecksum(std::vector<uint8_t> serializedData){
    uint32_t sum = 0;
    for(int i = 0; i < serializedData.size(); i+=2){
        sum += (serializedData[i] << 8) + serializedData[i + 1];
    }
    if(serializedData.size() & 1)
        sum += serializedData[serializedData.size() - 1];        
    return htons(~sum);
}

void PingPacket::printPacket(std::vector<uint8_t> data){
    for(int i = 0; i < data.size(); i++){
        std::cout << std::hex 
                  << std::setfill('0') 
                  << std::setw(2) << static_cast<unsigned int>(data[i]) 
                  << " "
                  << std::endl;
    }
}

