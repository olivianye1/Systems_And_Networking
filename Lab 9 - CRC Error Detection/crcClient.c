/*
Olivia Nye 
Systems and Networking 
Lab #9 - CRC Error Detection 
Description: Client that reads a binary message from a textfile, 
computes its CRC remainder, makes a purposeful error in the message 30% of the time,
and sends the message and the rem to the server
*/

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>
#include <unistd.h>
#include <time.h>


#define SERVER_PORT 5432
#define MAX_LINE 256


const char *unsigned_to_binary(unsigned long msg, int size)
{
	char* b = (char *)malloc(size + 1);
	b[0] = '\0';

	int z;
	for (z = (1<<(size-1)); z > 0; z >>= 1)
	{
		strcat(b, ((msg & z) == z) ? "1" : "0");
	}

	return b;
}

unsigned long crc_remainder(unsigned long msg, unsigned long check, int mlen, int clen) {
	unsigned long newmsg = msg << (clen-1);
	unsigned long n;
	int i;
	for (i = mlen; i > 0; i--) {
		if ((newmsg & (1 << (i+clen-2))) != 0) {
			n = check << (i - 1);
			newmsg = newmsg ^ n;
		}
	}
	return newmsg & ((1 << clen) - 1);
}


int main(int argc, char * argv[])
{
	FILE *fp;
	struct hostent *hp;
	struct sockaddr_in sin;
	char *host;
	char buf[MAX_LINE];
	int s;
	int len;

	if (argc==2) {
		host = argv[1];
	}
	else {
		fprintf(stderr, "usage: simplex-talk host\n");
		exit(1);
	}

	/* translate host name into peer's IP address */
	hp = gethostbyname(host);
	if (!hp) {
		fprintf(stderr, "simplex-talk: unknown host: %s\n", host);
		exit(1);
	}

	/* build address data structure */
	memset((char *)&sin, 0, sizeof(sin));
	sin.sin_family = AF_INET;
	memcpy((char *)&sin.sin_addr, hp->h_addr, hp->h_length);
	sin.sin_port = htons(SERVER_PORT);

	/* active open */
	if ((s = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
		perror("simplex-talk: socket");
		exit(1);
	}
	if (connect(s, (struct sockaddr *)&sin, sizeof(sin)) < 0)
	{
		perror("simplex-talk: connect");
		close(s);
		exit(1);
	}
	

	// read message in from textfile 
	FILE* msgFile = fopen("sampleMsg.txt", "r");  
	/* instantiate very long string for message so there is no limit to the length of 
		the message that can be read in*/
	char msg[100];
	// read in message from file
	while (fgets(msg, 100, msgFile)) {
		// save length of message read in 
		int msgLen = (int) strlen(msg);
		
		unsigned long msgUl;
		unsigned long newMsg;
		unsigned long divisor;
		unsigned long rem, newrem;
		char * ptr;
		
		// convert binary msg from string type to hexadecimal unsigned long type
		msgUl = strtoul(msg, &ptr, 2);
		// convert hardcoded binary divisor number from string type to hexadecimal unsigned long type
		divisor = strtoul("1101", &ptr, 2);
		
		// prints msg as a string of binary numbers
		printf("Message is: ");
		printf("%s\n", unsigned_to_binary(msgUl, msgLen));
		// prints divisor as a string of binary numbers
		printf("Divisor is: ");
		printf("%s\n", unsigned_to_binary(divisor, 4));
		
		// calculates remainder of msgUl being crc-divided by divisor
		rem = crc_remainder(msgUl, divisor, msgLen, 4);
		// prints remainder as a string of binary numbers
		printf("Remainder of message and divisor is: ");
		printf("%s\n", unsigned_to_binary(rem, 3));
		
		// fills newmsg var with the result of the concatenation of the msgUl and the rem
		newMsg = (msgUl << 3) + rem;
		// prints newmsg (consisting of original message followed by rem) as a string of binary numbers
		printf("Message with added bits is: ");
		int newMsgLen = msgLen + 3;
		printf("%s", unsigned_to_binary(newMsg, newMsgLen));
		
		// copies contents of ul newMsg into a string 
		size_t size = strlen(unsigned_to_binary(newMsg, newMsgLen)) + 1;
		char *newMsgStr = malloc(size);
		strcpy(newMsgStr, unsigned_to_binary(newMsg, newMsgLen));

		int eventVar;
		srand ( time(NULL) );
		// randomly select a number between 1 and 100
		eventVar = rand() % 100 + 1;
		// should only enter this loop 30 percent of the time, depending on val of eventVar
		if (eventVar <= 30){
			// notify client user that they should expect a server-side failure
			printf("\nALERT: THIS SHOULD ERROR ON PURPOSE");
			// randomly select an index of newMsgStr that holds one of the bits from the original msg
			int randIndex = rand()%(msgLen - 1);
			// MAKE MISTAKE ON PURPOSE - flip the bit at randIndex of newMsg
			if (newMsgStr[randIndex] == '1'){
				newMsgStr[randIndex] = '0';
			}else{
				newMsgStr[randIndex] = '1';
			}
		}		
		// send new message to server
		if( send(s, newMsgStr, newMsgLen , 0) < 0){
				puts("\nSend failed");
				return 1;
		}else{
			puts("\nNew message has been successfully sent to the server");
			
		}
		break;
 	}
 	// close file
	fclose(msgFile);
	return 0;	
}
		