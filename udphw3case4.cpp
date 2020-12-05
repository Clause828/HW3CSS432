//Created by Jayden Stipek and Duncan Spani 
// 12/2/2020
// CSS432 HW3
#include <iostream>
#include "udphw3.h"

#define TIMEOUT 1500
#define DROPRATE 1
using namespace std;


/**
 *Modify hw3case4.cpp to include test case 4. Test case 4 runs similar to test case 3, 
 but the "sliding window" algorithm/implementation only runs for a sliding window of size 1 and size 30. 
 Add code to the serverEarlyRetrans function in udphw3case4.cpp so that packets are randomly dropped packets N% of the time, 
 where N is every integer from 0 to 10. You will also need to modify the output of hw3case4.cpp so that it outputs the drop percentage 
 (instead of the window size) into the file.
  You can simulate a drop by just not returning an ACK when you receive a packet.
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
            int percentage = rand() % 10;
            if(percentage < DROPRATE)   //MODIFICATion for case 4 Drops 10% of the time
                continue;
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