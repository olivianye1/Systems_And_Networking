/*
Olivia Nye 
Systems and Networking 
Lab #9 - CRC Error Detection 
Description: Code came pre-written. I just annotated the file by leaving comments for every line.
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* converts a message from unsigned long type to binary numbers in string type
	(pointer to array of chars)*/
const char *unsigned_to_binary(unsigned long msg, int size)
{
	/* instantiates string, b as a pointer to a string (a character array)
		 allocates size for the string, b, based on the argument to the size param */
	char* b = (char *)malloc(size + 1);
	/* starts the string with a null character - will
		 eventually end up as the last value in the string b*/
	b[0] = '\0';
	
	// instantiates z to be used as an iterator in the next for-loop
	int z;
	//loops through msg, from last character in the message to the first
	for (z = (1<<(size-1)); z > 0; z >>= 1)
	{	
		/* adds "0" or "1" as chars to the string depending 
			 on the value of the bit in msg at index z */
		strcat(b, ((msg & z) == z) ? "1" : "0");
	}
	// returns b 
	return b;
}

// calculates the remainder of msg and check using the crc algorithm
unsigned long crc_remainder(unsigned long msg, unsigned long check, int mlen, int clen) {
	/* instantiates ul  newmsg variable with original msg left shifted so 
		that there is room ((clen - 1) number of bits) at the end available
		for the remainder to be added later*/
	unsigned long newmsg = msg << (clen-1);
	
	// instantiates n as an it
	unsigned long n;
	// instantiates i as an iterator for the subsequent for loop
	int i;
	// iterate through msg from the last index to the first
	for (i = mlen; i > 0; i--) {
		// if the upper-most bit in the message == 1
		if ((newmsg & (1 << (i+clen-2))) != 0) {
			// shift the next bit of the message into the remainder(n)
			n = check << (i - 1);
			// swap contents of newmsg with n 
			newmsg = newmsg ^ n;
		}
	}
	// only return the part of newmsg that holds the remainder
	return newmsg & ((1 << clen) - 1);
}

int main() { 
	// instantiates unsigned long msg variable with a hexadecimal msg 
	unsigned long msg = 0xC74A;
	// instantiates unsigned long divisor variable with a hexadecimal divider 
	unsigned long divisor = 0xB;
	/*calculates remainder of message and divider and concatenates message
	 and remainder into newmsg ul var (which is being instantiated in this line)*/
	unsigned long newmsg = (msg << 3) + crc_remainder(msg, divisor, 16, 4);
	/* instantiates variables to hold the value of the remainder of msg crc-divided by divisor
	 (rem) and the remainder of newmsg crc-divided by same divisor */
	unsigned long rem, newrem;
	// prints msg as a string of binary numbers
	printf("Message is: ");
	printf("%s\n", unsigned_to_binary(msg, 16));
	// prints divisor as a string of binary numbers
	printf("Divisor is: ");
	printf("%s\n", unsigned_to_binary(divisor, 4));
	// calculates remainder of msg being crc-divided by divisor
	rem = crc_remainder(msg, divisor, 16, 4);
	// prints rem as a string of binary numbers
	printf("Remainder of message and divisor: ");
	printf("%s\n", unsigned_to_binary(rem, 3));
	// prints newmsg (consisting of original message followed by rem) as a string of binary numbers
	printf("Message with added bits is: ");
	printf("%s\n", unsigned_to_binary(newmsg, 19));
	// this is the error check: calculates the remainder of newmsg being crc-divided by the divisor
	newrem = crc_remainder(newmsg, divisor, 19, 4);
	// prints the newrem as a string of binary numbers
	printf("Remainder is: ");
	printf("%s\n", unsigned_to_binary(newrem, 3)); // if this line prints "000", this would indicate no error, else error 


	printf("\nEXAMPLE FROM BOOK:\n\n");
	// instantiates pointer that is needed for conversion of variables from string type to ul type
	char * ptr;
	// convert msg stored as hard-coded string to unsigned long time
	msg = strtoul("10011010", &ptr, 2);
	// convert divisor stored as hard-coded string to unsigned long time
	divisor = strtoul("1101", &ptr, 2);

	// prints msg as a string of binary numbers
	printf("Message is: ");
	printf("%s\n", unsigned_to_binary(msg, 8));
	// prints divisor as a string of binary numbers
	printf("Divisor is: ");
	printf("%s\n", unsigned_to_binary(divisor, 4));
	rem = crc_remainder(msg, divisor, 8, 4);
	// calculates remainder of msg being crc-divided by divisor
	printf("Remainder of message and divisor: ");
	// prints rem as a string of binary numbers
	printf("%s\n", unsigned_to_binary(rem, 3));
	// fills newmsg var with the result of the concatenation of the msg and the rem
	newmsg = (msg << 3) + rem;
	// prints newmsg (consisting of original message followed by rem) as a string of binary numbers
	printf("Message with added bits is: ");
	printf("%s\n", unsigned_to_binary(newmsg, 11));
	// this is the error check: calculates the remainder of newmsg being crc-divided by the divisor
	newrem = crc_remainder(newmsg, divisor, 11, 4);
	// prints the newrem as a string of binary numbers
	printf("Remainder is: ");
	printf("%s\n", unsigned_to_binary(newrem, 3)); // if this line prints "000", this would indicate no error, else error 
	// function concludes successfully
	return 0;
}