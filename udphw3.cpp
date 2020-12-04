//Created by Jayden Stipek and Duncan Spani 
// 12/2/2020
// CSS432 HW3
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
    cout << "beginning clientStopWait" << endl;
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
            cout << "inside While loop" << endl;
            int responseData = sock.pollRecvFrom(); //Any data been recieved?
            if(responseData > 0)
                break; //response is true and then cut the while loop

            if(timer.lap() > TIMEOUT && !response) //If we go over the 1500 Timeout 
            {
                response = true;
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
    cout << "inside serverReliable" << endl;    
    for(int i = 0; i < max; i++)    //loop through the 20000 messages
    {
        while(true)
        {
            cout << "inside serverReliable loop" << endl;
            int recievedData = sock.pollRecvFrom(); //Any data been recieved?
            if(recievedData > 0)
            {
                sock.recvFrom((char*) message, MSGSIZE); //recieve the information
                if(message[0] == i) //only if its the correct one
                {
                    cout << "message recieved" << endl;
                    sock.ackTo((char *) &i, sizeof(i)); //if data has been receievd then I need to send it acknoledge it 
                    break;                
                }
            }            
         }
    }
}
int clientSlidingWindow( UdpSocket &sock, const int max, int message[], int windowSize )
{
    int resubmissions = 0;
    return resubmissions;
}
void serverEarlyRetrans( UdpSocket &sock, const int max, int message[], int windowSize )
{
    return;
}