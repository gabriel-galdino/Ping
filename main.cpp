#include "PingPacket.h"

 int main(){
  pid_t pid = getpid();
  PingPacket packet(8, 0, htons(4500), htons(1));
  packet.sendPacket();
  return 0; 
 }
