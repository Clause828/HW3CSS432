//Created by Jayden Stipek and Duncan Spani 
// 12/2/2020
#include <iostream>
#include "udphw3.h"

#define TIMEOUT 1500

using namespace std;

int clientStopWait( UdpSocket &sock, const int max, int message[] )
{
    "sends message[] and receives an acknowledgment from the server max";
    "If the client cannot receive an acknowledgement immedietly,"; 
    "it should start a Timer. If a timeout occurs (i.e., no response after 1500 usec), ";
    "the client must resend the same message. The function must count the number of messages retransmitted and";
    "return it to the main function as its return value.";
    for(int i = 0; i < max; i++){ // loop for the 20000 times
        message[0] = i; //insert the message into message[0]
        Timer timer;
        bool response = false;
        int sent;
        int resubmissions = 0;
        timer.start();
        while(!response) //while I have not receieved an response
        {
            sent = sock.sendTo((char*) message, MSGSIZE); //send the message to the server
            if(sent)
                response = true; //response is true and then cut the while loop

            if(timer.getUsec() >= TIMEOUT) //If we go over the 1500 Timeout 
            {
                timer.lap(); //restart the timer    
                sock.sendTo((char*) message, MSGSIZE); //resend the messages
                resubmissions++; //add to resubmissions count
                i--; //take away from the for loop total
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