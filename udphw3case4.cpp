//Created by Jayden Stipek and Duncan Spani 
// 12/2/2020
// CSS432 HW3
#include <iostream>
#include "udphw3.h"

#define TIMEOUT 1500
#define DROPRATE 1
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

    cerr << "beginning clientStopWait" << endl;
    int resubmissions = 0;
    for(int i = 0; i < max; i++){ // loop for the 20000 times
        message[0] = i; //insert the message into message[0]
        Timer timer;
        bool response = false;
        int sent;
        sent = sock.sendTo((char*) message, MSGSIZE); //send the message to the server
        cerr << "data sent to server" << endl;
        timer.start();
        while(true) //while I have not receieved an response
        {
            int responseData = sock.pollRecvFrom(); //Any data been recieved?
            if(responseData > 0)
                break; //response is true and then cut the while loop

            if(timer.lap() > TIMEOUT && !response) //If we go over the 1500 Timeout
            {
                cerr << "response found " << endl;
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
    cerr << "finishing" << endl;
    return resubmissions;
}

/**
 * repeats receiving message[] and sending an acknowledgment at a server side max (=20,000) times using the sock object.
 *
**/
void serverReliable( UdpSocket &sock, const int max, int message[] )
{
    cerr << "inside serverReliable" << endl;
    for(int i = 0; i < max; i++)    //loop through the 20000 messages
    {
        while(true)
        {
            int recievedData = 0;
            recievedData = sock.pollRecvFrom(); //Any data been recieved?
            if(recievedData > 0)
            {
                cerr << "message recieved" << endl;
                sock.recvFrom((char*) message, MSGSIZE); //recieve the information
                if(message[0] == i) //only if its the correct one
                {
                    cerr << "ack sent back to client" << endl;
                    sock.ackTo((char *) &i, sizeof(i)); //if data has been receievd then I need to send it acknoledge it
                    break;
                }
            }
        }
    }
}

/**
 *sends message[] and receiving an acknowledgment from a server max (=20,000) times using the sock object.
 * As described above, the client can continuously send a new message[] and increasing the sequence number as long as the number of in-transit messages (i.e., # of unacknowledged messages) is less than "windowSize."
 * That number should be decremented every time the client receives an acknowledgment. If the number of unacknowledged messages reaches "windowSize," the client should start a Timer. If a timeout occurs (i.e., no response after 1500 usec),
 * it must resend the message with the minimum sequence number among those which have not yet been acknowledged.
 * The function must count the number of messages (not bytes) re-transmitted and return it to the main function as its return value.
 **/
int clientSlidingWindow( UdpSocket &sock, const int max, int message[], int windowSize )
{
    cerr << "beginning clientSlidingWindow" << endl;
    int resubmissions = 0;
    int unacknowledged = 0;
    int acknowledgements = 0;
    for(int i = 0; i < max; i++)
    {
        cerr << "unacknowledged: " << unacknowledged << "windowsize: " << windowSize << endl;
        if(unacknowledged < windowSize)//if the unacknowledged messages is less than windowsize
        {
            message[0] = i; //insert the message into message[0]
            sock.sendTo((char*) message, MSGSIZE); //send the message to the server
            unacknowledged++;
        }

        if(unacknowledged == windowSize) //If the sliding window is full 
        {
            Timer timer;
            timer.start();
            while(true)
            {
                if(sock.pollRecvFrom() > 0)
                {
                    cerr << "recieved data " << endl;
                    sock.recvFrom((char * )message, MSGSIZE);
                    if(message[0] == acknowledgements)
                    {
                        cerr << "correct message cerring as acknolegement" << endl;
                        acknowledgements++; //increase the amount acknowledged
                        unacknowledged--; //take away from the for loop total
                        break; //response is true and then cut the while loop
                    }
                }
                if(timer.lap() > TIMEOUT && unacknowledged == windowSize) //If we go over the 1500 Timeout
                {
                   // cerr << "Timed Out resending message" << endl;
                    resubmissions = resubmissions + (i + windowSize - acknowledgements); //add to resubmissions count
                    i = acknowledgements + 1; //resetting back to the last correctly submitted ack
                    //cerr << "after acknolegements i:  " << i << endl;
                    unacknowledged = 0; //go back to last valid ack
                    break;
                }
            }
        }
    }
    cerr << "finishing" << endl;
    return resubmissions;
}
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
    cerr << "inside Server EarlyRetrans" << endl;
    int cumAck[max];
    int count = 0;
    while(count != max)
    {
        if(sock.pollRecvFrom() > 0)
        {
            //cerr << "message recieved" << endl;
            sock.recvFrom((char*) message, MSGSIZE)  ; //recieve the information
            int percentage = rand() % 10;
            if(percentage < DROPRATE)   //MODIFICATion for case 4 Drops 10% of the time
                continue;
            //cerr << "message: " << message[0] << " count: "  << count << endl;
            if(message[0] == count){
                //cerr << "acknolegment sent" << endl; //never gets in here
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

