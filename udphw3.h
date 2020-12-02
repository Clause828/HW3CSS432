// Project:      CSS432HW3 Socket Class
// Creator:      Jayden Stipek & Duncan Spani 
// Organization: CSS, University of Washington, Bothell
// Date:         December 2, 2020

#ifndef HW3_UDPHW3_H_
#define HW3_UDPHW3_H_

#include "Timer.h"
#include "UdpSocket.h"

using namespace std;

class udphw3 {
    public:
        int clientStopWait( UdpSocket &sock, const int max, int message[] );
        void serverReliable( UdpSocket &sock, const int max, int message[] );
        int clientSlidingWindow( UdpSocket &sock, const int max, int message[], int windowSize );   
        void serverEarlyRetrans( UdpSocket &sock, const int max, int message[], int windowSize );
};
#endif