//Created by Jayden Stipek and Duncan Spani 
// 12/2/2020
// CSS432 HW3
#include <iostream>
#include "udphw3.h"

#define TIMEOUT 1500

using namespace std;

   /**
     * sends message[] and receives an acknowledgment from the server max
     * If the client cannot receive an acknowledgement immedietly
     * it should start a Timer. If a timeout occurs (i.e., no response after 1500 usec)
     * the client must resend the same message. The function must count the number of messages retransmitted and
     * return it to the main function as its return value.
     **/
int clientStopWait( UdpSocket &sock, const int max, int message[] )
{
 
    cout << "beginning clientStopWait" << endl;
    int resubmissions = 0;
    for(int i = 0; i < max; i++){ // loop for the 20000 times
        message[0] = i; //insert the message into message[0]
        Timer timer;
        bool response = false;
        int sent;
        sent = sock.sendTo((char*) message, MSGSIZE); //send the message to the server
        cout << "data sent to server" << endl;
        timer.start();
        while(true) //while I have not receieved an response
        {
            int responseData = sock.pollRecvFrom(); //Any data been recieved?
            if(responseData > 0)
                break; //response is true and then cut the while loop

            if(timer.lap() > TIMEOUT && !response) //If we go over the 1500 Timeout 
            {
                cout << "response found " << endl;
                response = true;
                resubmissions++; //add to resubmissions count
                i--; //take away from the for loop total     
                break;
            }
        }
        if(response) //if the response is found dont recieve the message
            continue; //skip the iteration

        sock.recvFrom((char*) message, MSGSIZE); //receieve the message from ther server
        if(message[0] != i) //If it is a bad message
        {
            i--;    
            resubmissions++;
            continue; //Restart
        }
    }
    cout << "finishing" << endl;
    return resubmissions;
}

/**
 * repeats receiving message[] and sending an acknowledgment at a server side max (=20,000) times using the sock object.
 * 
**/

void serverReliable( UdpSocket &sock, const int max, int message[] )
{
    cout << "inside serverReliable" << endl;    
    for(int i = 0; i < max; i++)    //loop through the 20000 messages
    {
        while(true)
        {
            int recievedData = 0;
            recievedData = sock.pollRecvFrom(); //Any data been recieved?
            if(recievedData > 0)
            {
                cout << "message recieved" << endl;
                sock.recvFrom((char*) message, MSGSIZE); //recieve the information
                if(message[0] == i) //only if its the correct one
                {
                    cout << "ack sent back to client" << endl;
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