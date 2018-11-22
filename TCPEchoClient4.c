#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "Practical.h"

int main(int argc, char *argv[]) {

  if (argc < 3 || argc > 4) // Test for correct number of arguments
    DieWithUserMessage("Parameter(s)",
        "<Server Address> <Echo Word> [<Server Port>]");

  char *servIP = argv[1];     // First arg: server IP address (dotted quad)
  char *echoString = argv[2]; // Second arg: string to echo

  // Third arg (optional): server port (numeric).  7 is well-known echo port
  in_port_t servPort = (argc == 4) ? atoi(argv[3]) : 7;

  // Create a reliable, stream socket using TCP
  int sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (sock < 0)
    DieWithSystemMessage("socket() failed");

  // Construct the server address structure
  struct sockaddr_in servAddr;            // Server address
  memset(&servAddr, 0, sizeof(servAddr)); // Zero out structure
  servAddr.sin_family = AF_INET;          // IPv4 address family
  // Convert address
  int rtnVal = inet_pton(AF_INET, servIP, &servAddr.sin_addr.s_addr);
  if (rtnVal == 0)
    DieWithUserMessage("inet_pton() failed", "invalid address string");
  else if (rtnVal < 0)
    DieWithSystemMessage("inet_pton() failed");
  servAddr.sin_port = htons(servPort);    // Server port

  // allow reuse binded port immediately 
  int on=1;  
  if((setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on)))<0)  
    DieWithSystemMessage("setsockopt() failed");

  // Bind to a fixed port number rather than auto allocated by connect()
  struct sockaddr_in clntAddr;
  memset(&clntAddr, 0, sizeof(clntAddr)); // Zero out structure
  clntAddr.sin_family = AF_INET;          // IPv4 address family
  // Convert address
  int rtnVal_1 = inet_pton(AF_INET, "127.0.0.1", &clntAddr.sin_addr.s_addr);
  if (rtnVal_1 == 0)
    DieWithUserMessage("inet_pton() failed", "invalid address string");
  else if (rtnVal_1 < 0)
    DieWithSystemMessage("inet_pton() failed");
  clntAddr.sin_port = htons(1112);    // Client port
  if (bind(sock, (struct sockaddr*) &clntAddr, sizeof(clntAddr)) < 0)
    DieWithSystemMessage("bind() failed");

  // Establish the connection to the echo server
  if (connect(sock, (struct sockaddr *) &servAddr, sizeof(servAddr)) < 0)
    DieWithSystemMessage("connect() failed");

  socklen_t addrLen = sizeof(struct sockaddr_in);
  struct sockaddr_in localAddr;
  if(getsockname(sock, (struct sockaddr *)&localAddr, &addrLen)<0)
    DieWithSystemMessage("could not getsockname");
  else{
    char clntName[INET_ADDRSTRLEN]; // String to contain client address
    if (inet_ntop(AF_INET, &localAddr.sin_addr.s_addr, clntName,
        sizeof(clntName)) != NULL)
      printf("info client %s/%d\n", clntName, ntohs(localAddr.sin_port));
    else
      puts("Unable to get client address");
  }

  struct sockaddr_in remoteAddr;
  if(getpeername(sock, (struct sockaddr *)&remoteAddr, &addrLen)<0)
    DieWithSystemMessage("could not getpeername");
  else{
    char srvName[INET_ADDRSTRLEN]; // String to contain server address
    if (inet_ntop(AF_INET, &remoteAddr.sin_addr.s_addr, srvName,
        sizeof(srvName)) != NULL)
      printf("info server %s/%d\n", srvName, ntohs(remoteAddr.sin_port));
    else
      puts("Unable to get server address");
  }

  size_t echoStringLen = strlen(echoString); // Determine input length

  // Send the string to the server
  ssize_t numBytes = send(sock, echoString, echoStringLen, 0);
  if (numBytes < 0)
    DieWithSystemMessage("send() failed");
  else if (numBytes != echoStringLen)
    DieWithUserMessage("send()", "sent unexpected number of bytes");

  // Receive the same string back from the server
  unsigned int totalBytesRcvd = 0; // Count of total bytes received
  fputs("Received: ", stdout);     // Setup to print the echoed string
  while (totalBytesRcvd < echoStringLen) {
    char buffer[BUFSIZE]; // I/O buffer
    /* Receive up to the buffer size (minus 1 to leave space for
     a null terminator) bytes from the sender */
    numBytes = recv(sock, buffer, BUFSIZE - 1, 0);
    if (numBytes < 0)
      DieWithSystemMessage("recv() failed");
    else if (numBytes == 0)
      DieWithUserMessage("recv()", "connection closed prematurely");
    totalBytesRcvd += numBytes; // Keep tally of total bytes
    buffer[numBytes] = '\0';    // Terminate the string!
    fputs(buffer, stdout);      // Print the echo buffer
    fflush(stdout);
  }

  fputc('\n', stdout); // Print a final linefeed

  close(sock);
  exit(0);
}
