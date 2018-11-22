CC = clang
CFLAGS = -Wall -std=gnu99 -g 

all: TCP UDP Data Vote
TCP: TCPEchoServer4 TCPEchoClient4 GetAddrInfo TCPEchoServer TCPEchoClient
TCPEchoServer4: TCPEchoServer4.c DieWithMessage.c TCPServerUtility.c AddressUtility.c
	$(CC) $(CFLAGS) -o TCPEchoServer4 TCPEchoServer4.c DieWithMessage.c TCPServerUtility.c AddressUtility.c
TCPEchoClient4: TCPEchoClient4.c DieWithMessage.c
	$(CC) $(CFLAGS) -o TCPEchoClient4 TCPEchoClient4.c DieWithMessage.c
GetAddrInfo: GetAddrInfo.c DieWithMessage.c TCPServerUtility.c AddressUtility.c
	$(CC) $(CFLAGS) -o GetAddrInfo GetAddrInfo.c DieWithMessage.c TCPServerUtility.c AddressUtility.c
TCPEchoServer: TCPEchoServer.c DieWithMessage.c TCPServerUtility.c AddressUtility.c
	$(CC) $(CFLAGS) -o TCPEchoServer TCPEchoServer.c DieWithMessage.c TCPServerUtility.c AddressUtility.c
TCPEchoClient: TCPEchoClient.c DieWithMessage.c TCPClientUtility.c
	$(CC) $(CFLAGS) -o TCPEchoClient TCPEchoClient.c DieWithMessage.c TCPClientUtility.c

UDP: UDPEchoClient UDPEchoServer
UDPEchoClient: UDPEchoClient.c DieWithMessage.c AddressUtility.c
	$(CC) $(CFLAGS) -o UDPEchoClient UDPEchoClient.c DieWithMessage.c AddressUtility.c
UDPEchoServer: UDPEchoServer.c DieWithMessage.c AddressUtility.c
	$(CC) $(CFLAGS) -o UDPEchoServer UDPEchoServer.c DieWithMessage.c AddressUtility.c

Data: TestSizes BruteForceCoding
TestSizes:
	$(CC) $(CFLAGS) -o TestSizes TestSizes.c
BruteForceCoding:
	$(CC) $(CFLAGS) -o BruteForceCoding BruteForceCoding.c

Vote: vsb vst vcb vct
vsb: VoteEncoding.h VoteProtocol.h VoteServerTCP.c VoteEncodingBin.c DieWithMessage.c TCPServerUtility.c AddressUtility.c DelimFramer.c
	$(CC) $(CFLAGS) -o vsb VoteServerTCP.c VoteEncodingBin.c DieWithMessage.c TCPServerUtility.c AddressUtility.c DelimFramer.c
vst: VoteEncoding.h VoteProtocol.h VoteServerTCP.c VoteEncodingText.c DieWithMessage.c TCPServerUtility.c AddressUtility.c DelimFramer.c
	$(CC) $(CFLAGS) -o vst VoteServerTCP.c VoteEncodingText.c DieWithMessage.c TCPServerUtility.c AddressUtility.c DelimFramer.c
vcb: VoteEncoding.h VoteProtocol.h VoteClientTCP.c VoteEncodingBin.c DieWithMessage.c TCPClientUtility.c AddressUtility.c DelimFramer.c
	$(CC) $(CFLAGS) -o vcb VoteClientTCP.c VoteEncodingBin.c DieWithMessage.c TCPClientUtility.c DelimFramer.c
vct: VoteEncoding.h VoteProtocol.h VoteClientTCP.c VoteEncodingText.c DieWithMessage.c TCPClientUtility.c AddressUtility.c DelimFramer.c
	$(CC) $(CFLAGS) -o vct VoteClientTCP.c VoteEncodingText.c DieWithMessage.c TCPClientUtility.c DelimFramer.c

clean:
	-rm *.o
	-rm TCPEchoServer4 TCPEchoClient4 GetAddrInfo TCPEchoServer TCPEchoClient
	-rm UDPEchoClient UDPEchoServer
	-rm TestSizes BruteForceCoding
	-rm vsb vst vcb vct

.PHONY: clean