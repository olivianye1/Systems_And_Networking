/*
Olivia Nye 
Systems and Networking 
Lab #9 - CRC Error Detection 
Description: Socket server that receives a message from a client that consists of a binary
 message and a client-computed CRC-remainder, and then uses the CRC algorithm to check for 
 errors.
*/
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <strings.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#define SERVER_PORT 5432
#define MAX_PENDING 1
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

int main()
{
	struct sockaddr_in sin;
	char buf[MAX_LINE];
	int len;
  unsigned int l;
	int s, new_s;

	/* build address data structure */
	memset((char *)&sin, 0, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = INADDR_ANY;
	sin.sin_port = htons(SERVER_PORT);

	/* setup passive open */
	if ((s = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
		perror("simplex-talk: socket");
		exit(1);
	}
	if ((bind(s, (struct sockaddr *)&sin, sizeof(sin))) < 0) {
		perror("simplex-talk: bind");
		exit(1);
	}
	listen(s, MAX_PENDING);


	/* wait for connection, then receive and print text */
	while(1) {
		l = sizeof(sin);
		if ((new_s = accept(s, (struct sockaddr *)&sin, &l)) < 0) {
			perror("simplex-talk: accept");
			exit(1);
		}
		//receive and print message from client
		while ((len = recv(new_s, buf, sizeof(buf), 0))){
			// print the received message 
			printf("\nMessage received from server is: ");
			puts(buf);
			printf("Divisor is: 1101");
			/* save number of bits in buf (# of bits in original message + 3 
				(where 3 == # number of bits expected in rem computed by client))*/
			int bufLen = (int) strlen(buf);
			unsigned long rcvdMsg;
			unsigned long divisor;
			unsigned long newrem; 
			unsigned long rem;
			char * ptr;
			
			// convert binary divisor from string type to hexadecimal unsigned long type
			divisor = strtoul("1101", &ptr, 2); // same hard-coded divisor used by client
			// convert binary buf from string type to hexadecimal unsigned long type
			rcvdMsg = strtoul(buf, &ptr, 2);
			
			/* USED FOR ERROR CHECK -- calculate crc-remainder of rcvdMsg (which is 
				original msg followed by client-computed rem) and the same divisor*/
			newrem = crc_remainder(rcvdMsg, divisor, bufLen, 4);
			// print newrem as 
			printf("\nNew remainder is: ");
			printf("%s\n", unsigned_to_binary(newrem, 3));
			
			// copies contents of ul newrem into a string for string comparison
			size_t size = strlen(unsigned_to_binary(newrem, 3)) + 1;
			char *newremStr = malloc(size);
			strcpy(newremStr, unsigned_to_binary(newrem, 3));
			
			/* instantiate string of zeroes, which is what newrem should be if there
				 was no errors detected*/
			const char * zeroes = "000";
			// compare newremStr to zeroes
			if (strcmp(newremStr, zeroes) == 0){
				// they're the same; indicate no errors detected
				puts("ok");
				break;
			}else{
				// they're not the same; indicate that errors have been detected
				puts("ERROR: CRC has detected an error in the message received");
				break;
			}
		}
		//close connection between server and client
		close(new_s);
	}
}
