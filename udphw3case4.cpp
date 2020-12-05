//Created by Jayden Stipek and Duncan Spani 
// 12/2/2020
// CSS432 HW3
#include <iostream>
#include "udphw3.h"

#define TIMEOUT 1500

using namespace std;


/**
 * receives message[] and sends an acknowledgment to the client max (=20,000) times using the sock object. 
 * Every time the server receives a new message[], it must save the message's sequence number in an array and return a cumulative acknowledgment, 
 * i.e.,* the last received message in order.
 * 
 **/
void serverEarlyRetrans( UdpSocket &sock, const int max, int message[], int windowSize )
{
    cout << "inside Server EarlyRetrans" << endl;
    int cumAck[max];
    int count = 0;
    while(count != max)
    {
        if(sock.pollRecvFrom() > 0)
        {
            //cout << "message recieved" << endl;
            sock.recvFrom((char*) message, MSGSIZE)  ; //recieve the information
            //cout << "message: " << message[0] << " count: "  << count << endl;
            if(message[0] == count){
                //cout << "acknolegment sent" << endl; //never gets in here
                sock.ackTo((char *) &count, sizeof(count)); //if data has been receievd then I need to send it acknoledge it 
                cumAck[count] = message[0];
                count++;
            }
            else
            {
                sock.ackTo((char *) &count, sizeof(count)); //if message[0] != count 2 != 3
            }
        }   
    }
}