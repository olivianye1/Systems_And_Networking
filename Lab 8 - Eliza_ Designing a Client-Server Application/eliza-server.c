/*
Olivia Nye 
CMPS 2301
Lab #8 Eliza – Designing a Client-Server Application
Server (Powers the Eliza Chatbot - receives messages from the client and sends automatic random responses back)

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
	

	char elizaisms[17][256] = 
		{"What are you thinking?",
		"What are you feeling?",
		"Tell me more...",
		"I totally understand that.",
		 "How does that make you feel?",
		  "Really?",
		   "Are you sure?", 
		   "Can you please expand on that a little more?", 
		   "Uh huh...",
		   "Why do you say that?",
		   "Are you being honest with yourself?",
		   "What's happened since we've last chatted?",
		   "Have you spoke to anybody else about this?",
		   "I am here for you.",
		   "Very interesting...",
		   "Go on...",
		   "I see..."
		   };
		   
	char questionReplies[5][256] = 
		{"Why do you ask that?",
		"It is only you who can truly answer that.", 
		"What do you think?",
		"I'm not sure.",
		"This is about you, not me."
		};
	int randomIndex;
	int isQuestion = 0;

	/* wait for connection, then receive and print text */
	while(1) {
		l = sizeof(sin);
		if ((new_s = accept(s, (struct sockaddr *)&sin, &l)) < 0) {
			perror("simplex-talk: accept");
			exit(1);
		}
		//receive and print message from client
		while ((len = recv(new_s, buf, sizeof(buf), 0))){
			printf("Client: %s", buf);
			
			// check whether or not the message is a question 
			isQuestion = 0;
			for (int i = 0; i < strlen(buf); i++){
				if (buf[i] == '?'){
					isQuestion = 1;
				}
			}
			// send random reply to the question asked
			if (isQuestion == 1){
				randomIndex = rand()%4;
				printf("Your Response: %s\n", questionReplies[randomIndex]);
				if( send(new_s, questionReplies[randomIndex], sizeof(questionReplies[randomIndex]) , 0) < 0){
					puts("Send failed");
					return 1;
				}
			// send random reply to the statement made
			}else{
				randomIndex =  rand()/(RAND_MAX /16);
				printf("Your Response: %s\n", elizaisms[randomIndex]);
				if( send(new_s, elizaisms[randomIndex], sizeof(elizaisms[randomIndex]) , 0) < 0){
					puts("Send failed");
					return 1;
				}
			}
		}
		close(new_s);
		
	}
}
