//
// Created by enophyis on 04/02/25.
//

// Client.java
// This class is the client side for a simple echo server based on
// UDP/IP. The client sends a character string to the echo server, then waits
// for the server to send it back to the client.
// Last edited January 9th, 2016

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <string>
#include <unistd.h>
#include "Datagram.h"

#define PORT    23

class Client {
public:
    Client() : sendReceiveSocket() {}

private:
    DatagramSocket sendReceiveSocket;

public:
    void sendAndReceive() {
    std::string filename = "message.txt";
    std::string mode = "oCTeT";
    for (int i = 0; i <= 11; i++)
    {
        std::vector<uint8_t> message;
        message.push_back(0x00);
        //SENDING MESSAGE
        //construct string
        std::cout << "[MESSAGE "<< i <<"] \n[CLIENT] ";
        if (i == 11)
        {
            message.push_back(0x99);
            std::cout << " Preparing INVALID (0x99) op" << std::endl;
        }
        else if(i % 2 == 0){
            message.push_back(0x01);
            std::cout << "Preparing READ (0x01) op" << std::endl;
        }
        else {
            message.push_back(0x02);
            std::cout << " Preparing WRITE (0x02) op" << std::endl;
        }
        for (char c: filename)
        {
            message.push_back(static_cast<uint8_t>(c));
        }
        message.push_back(0x00);
        for (char c: mode)
        {
            message.push_back(static_cast<uint8_t>(c));
        }
        message.push_back(0x00);

        //construct packet to send to port 23
        DatagramPacket request(message, message.size(), InetAddress::getLocalHost(), PORT);
        std::cout << "To host: " << request.getAddressAsString() << std::endl;
        std::cout << "Destination host port: " << request.getPort() << std::endl;
        int len = request.getLength();
        std::cout << "String Data: " << std::endl;
        std::cout << std::string(static_cast<const char *>(request.getData()), len) << std::endl; //
        std::cout << "Raw Hex Data: ";
        for (size_t i = 0; i < request.getLength(); i++) {
            uint8_t byte = static_cast<const uint8_t *>(request.getData())[i];
            // Print each byte as 2-character hexadecimal
            if (byte < 16) { // If the byte is less than 0x10, pad with a '0'
                std::cout << "0";
            }
            std::cout << std::hex << static_cast<int>(byte) << " ";
        }
        std::cout << std::endl;


        //attempt to send packet to port 23
        try {
            sendReceiveSocket.send(request);
        } catch (const std::runtime_error& e) {
            std::cerr << e.what();
            exit(1);
        }
        std::cout << "Client: Packet sent." << std::endl;

        //===========================================================================================================
        // RCV MESSAGE
        // Construct a DatagramPacket for receiving packets up
        // to 100 bytes long (the length of the byte array).
        std::vector<uint8_t> in(100);
        DatagramPacket receivePacket(in, in.size());

        try {
            // Block until a datagram is received via sendReceiveSocket.
            std::cout << "awaiting response from intermediate host" << std::endl;
            sendReceiveSocket.receive(receivePacket);
        } catch(const std::runtime_error& e) {
            std::cerr << e.what();
            exit(1);
        }

        // Process the received datagram.
        std::cout << "Packet received:" << std::endl;
        std::cout << "From host: " << receivePacket.getAddressAsString() << std::endl;
        std::cout << "Host port: " << receivePacket.getPort() << std::endl;
        std::cout << "Response Code: ";
        for (size_t i = 0; i < receivePacket.getLength(); i++) {
            uint8_t byte = static_cast<const uint8_t *>(receivePacket.getData())[i];
            // Print each byte as 2-character hexadecimal
            if (byte < 16) { // If the byte is less than 0x10, pad with a '0'
                std::cout << "0";
            }
            std::cout << std::hex << static_cast<int>(byte) << " ";
        }
        std::cout << std::endl;
        std::cout << "===========================================\n\n";
        //Socket will close with RAII.
    }

	return;
    }
};

int main(int argc, char ** argv)
{
    Client().sendAndReceive();
}