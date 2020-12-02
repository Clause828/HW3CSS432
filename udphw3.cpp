//Created by Jayden Stipek and Duncan Spani 
// 12/2/2020
#include <iostream>
#include "udphw3.h"

#define TIMEOUT 1500

using namespace std;

int clientStopWait( UdpSocket &sock, const int max, int message[] )
{
    /**
     * sends message[] and receives an acknowledgment from the server max
     * If the client cannot receive an acknowledgement immedietly
     * it should start a Timer. If a timeout occurs (i.e., no response after 1500 usec)
     * the client must resend the same message. The function must count the number of messages retransmitted and
     * return it to the main function as its return value.
     **/

    int resubmissions = 0;
    for(int i = 0; i < max; i++){ // loop for the 20000 times
        message[0] = i; //insert the message into message[0]
        Timer timer;
        bool response = false;
        int sent;
        sent = sock.sendTo((char*) message, MSGSIZE); //send the message to the server
        timer.start();
        while(true) //while I have not receieved an response
        {
            int responseData = sock.pollRecvFrom(); //Any data been recieved?
            if(responseData > 0)
                break; //response is true and then cut the while loop

            if(timer.getUsec() >= TIMEOUT) //If we go over the 1500 Timeout 
            {
                timer.lap(); //restart the timer    
                resubmissions++; //add to resubmissions count
                i--; //take away from the for loop total
                break;
            }
        }
        sock.recvFrom((char*) message, MSGSIZE); //receieve the message from ther server
        if(message[0] != i) //If it is a bad message
        {
            i--;    
            resubmissions++;
            continue;
        }
    }
    return resubmissions;
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