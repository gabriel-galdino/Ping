#include "PingPacket.h"
#include <iostream>
 int main(){
   PingPacket packet(8, 0, 0, 0); 
   std::cout << packet.sendPacket() << std::endl;
   return 0;
 }
