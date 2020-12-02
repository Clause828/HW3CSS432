//Crated by Jayden Stipek and Duncan Spani 
// 12/2/2020
#include <iostream>
#include "UdpSocket.h"
#include "Timer.h"
#define TIMEOUT 1500

using namespace std;

int clientStopWait( UdpSocket &sock, const int max, int message[] )
{
    "sends message[] and receives an acknowledgment from the server max";
    "If the client cannot receive an acknowledgement immedietly,"; 
    "it should start a Timer. If a timeout occurs (i.e., no response after 1500 usec), ";
    "the client must resend the same message. The function must count the number of messages retransmitted and";
    "return it to the main function as its return value.";
    if(max >= 20000)
    {
        Timer timer;
        bool response = false;
        int sent;
        int resubmissions = 0;
        timer.start();
        while(!response)
        {
            sock.sendTo((char*) message, MSGSIZE);
            if(sent)
                response = true;

            if(timer.getUsec() >= TIMEOUT)
            {
                sock.sendTo((char*) message, MSGSIZE);
                resubmissions++;
            }
        }
    }
    return -1;
}
void serverReliable( UdpSocket &sock, const int max, int message[] )
{
    return;
}
int clientSlidingWindow( UdpSocket &sock, const int max, int message[], int windowSize )
{
    return -1;
}
void serverEarlyRetrans( UdpSocket &sock, const int max, int message[], int windowSize )
{
    return;
}