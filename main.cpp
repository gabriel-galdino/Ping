#include "PingPacket.h"
#include <iostream>
 int main(){
  pid_t pid = getpid();
  PingPacket packet(8, 0, htons(4500), htons(333));
  packet.sendPacket();
  return 0; 
 }
