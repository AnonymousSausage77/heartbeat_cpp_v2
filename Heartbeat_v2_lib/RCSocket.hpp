#ifndef RCSOCKET_H
#define RCSOCKET_H

namespace roboseals::RooCOMMS {
    
class RCSocket {
public:
    RCSocket();
    ~RCSocket();
    
private:
    int *_socket = nullptr;
    
};
    
}





#endif