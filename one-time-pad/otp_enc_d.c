/* otp_enc_d.c
 * August Lautt */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/wait.h>

// PASSCODE is code for handshaking
#define PASSCODE "EN"
#define QUEUE 5


int startUp(int portNumber);
void handShake(int clientFD);
void exchangeText(int clientFD);
void encryptText(char *keyText, char *plainText, char *cipherText);


int main(int argc, char *argv[])
{
	// socketFD for listening, clientFD for accepted connections, port number holds port arg
	int socketFD, clientFD, portNumber;
	pid_t pid;

	// check for correct number of arguments
	if (argc != 2)
	{
		fprintf(stderr, "Usage: %s [listening_port]\n", argv[0]);
		exit(1);
	}

	// grab converted port number from args
	portNumber = atoi(argv[1]);

	// start the server with the provided port number and begin listening
	socketFD = startUp(portNumber);

	// continuously accept connections on listening socket
	// only stops if admin terminates/kills
	while(1)
	{
		// accept a client's connect request and store in new socket file descriptor
		clientFD = accept(socketFD, NULL, NULL);

		// for the process
		pid = fork();

		// if pid returns an error, exit
		if (pid < 0)
		{
			fprintf(stderr, "Error: unable to fork\n");
			exit(1);
		}
		// in child process: perform handshake, receive files, transform, then send
		else if (pid == 0)
		{
			close(socketFD);

			handShake(clientFD);
			exchangeText(clientFD);

			close(clientFD);
			exit(0);
		}
		// in parent: clean up and close the client socket
		else
		{
			// exiting in child will produce zombies, we exit for errors and on success
			// so wait in parent for our pid and to clean it up
			waitpid(pid, NULL, 0);
			close(clientFD);
		}
	}

	return 0;
}


/* Function to create socket with port number provided, get address information for
 * our server, bind the address to our socket and then begin listening for connections.
 * Takes: Port number provided in arguments.
 * Returns: Listening socket file descriptor to accept connections with. */

int startUp(int portNumber)
{
	// socketFD is socket to create and bind and listen with
	int socketFD;
	int yes = 1;
	struct sockaddr_in serverAddress;

	// create a TCP IPv4 socket
	if ((socketFD = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		fprintf(stderr, "Error: server unable to create socket\n");
		exit(1);
	}

	// set the socket options to be able to reuse addresses
	if (setsockopt(socketFD, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) < 0)
	{
		fprintf(stderr, "Error: server unable to set socket option\n");
		exit(1);
	}

	// set port to short network byte order and the address to IPv4 and INADDR_ANY for localhost
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(portNumber);
	serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);

	// bind the server address to the socket
	if (bind(socketFD, (struct sockaddr *) &serverAddress, sizeof(serverAddress)) < 0)
	{
		fprintf(stderr, "Error: server unable to bind socket");
		exit(1);
	}

	// listen for connect calls on our socket, we listen for 5 max in this program
	if (listen(socketFD, QUEUE) < 0)
	{
		fprintf(stderr, "Error: otp_enc_d could not listen on port %d\n", portNumber);
		exit(1);
	}

	return socketFD;
}


/* Function to exchange PASSCODE with accepted client to ensure client matches
 * the server.
 * Takes: the accepted client socket file descriptor to send and receive. */

void handShake(int clientFD)
{
	int numBytes;
	char secret[3], response[3];

	// copy our passcode, in this instance "EN"
	memset(secret, '\0', sizeof secret);
	strcpy(secret, PASSCODE);

	// receive passcode from client
	if ((numBytes = recv(clientFD, response, sizeof(response), 0)) <= 0)
	{
		fprintf(stderr, "Error: server could not receive handshake\n");
		close(clientFD);
		exit(1);
	}

	// send server passcode to client
	if ((numBytes = send(clientFD, secret, sizeof(secret), 0)) < 0)
	{
		fprintf(stderr, "Error: server could not send handshake\n");
		close(clientFD);
		exit(1);
	}

	// if client passcode does not match server passcode
	// exit because we are in a child process
	if (strcmp(response, PASSCODE) != 0)
	{
		close(clientFD);
		exit(2);
	}
}


/* Function to receive data from client, perform text transformation, then
 * send transformed text back to client.
 * Takes: the accepted client socket file descriptor to send/receive with. */

void exchangeText(int clientFD)
{
	int textSize, keySize;
	int numBytes;

	// receive and store incoming text size from client
	if ((numBytes = recv(clientFD, &textSize, sizeof(textSize), 0)) < 0)
	{
		fprintf(stderr, "Error: server unable to receive text size\n");
		close(clientFD);
		exit(1);
	}

	// create strings of received size for holding the text
	char plainText[textSize], cipherText[textSize];
	memset(plainText, '\0', sizeof plainText);
	memset(cipherText, '\0', sizeof cipherText);

	// receive and store incoming key size from client
	if ((numBytes = recv(clientFD, &keySize, sizeof(keySize), 0)) < 0)
	{
		fprintf(stderr, "Error: server unable to receive key size\n");
		close(clientFD);
		exit(1);
	}

	// create string of received size for holding key
	char keyText[keySize];
	memset(keyText, '\0', sizeof keyText);

	// since we received the file sizes, we know exactly how much much data to expect
	// we receive key text and plain text using flag MSG_WAITALL to block until we get it all
	if ((numBytes = recv(clientFD, plainText, sizeof(plainText), MSG_WAITALL)) < 0)
	{
		fprintf(stderr, "Error: server unable to receive plain text\n");
		close(clientFD);
		exit(1);
	}

	if ((numBytes = recv(clientFD, keyText, sizeof(keyText), MSG_WAITALL)) < 0)
	{
		fprintf(stderr, "Error: server unable to receive key text\n");
		close(clientFD);
		exit(1);
	}

	// this server encrypts text, so perform that with the three files
	encryptText(keyText, plainText, cipherText);

	// send the transformed text back to the client for it to output
	if ((numBytes = send(clientFD, cipherText, sizeof(cipherText), 0)) < 0)
	{
		fprintf(stderr, "Error: server unable to send cipher text\n");
		close(clientFD);
		exit(1);
	}
}


/* Function to transform plain text contents using a provided key. Uses
 * OTP logic described in specifications.
 * Takes: received key text and plain text strings and an empty
 * cipher text string.
 * Returns: nothing. */

void encryptText(char *keyText, char *plainText, char *cipherText)
{
	// chars to hold values of each text type
	char keyChar, plainChar, cipherChar;
	int i;

	// loop through the contents of the plain text character by character
	for (i = 0; i < strlen(plainText); i++)
	{
		// temporarily convert the key and plain text values to 0-26, so
		// that we can % them easily, spaces being 26.
		// the chars have been validated to only be CAPS, so we can just
		// subtract from the uppercase range to get the correct values
		if (keyText[i] == ' ')
		{
			keyChar = 26;
		}
		else
		{
			keyChar = keyText[i] - 'A';
		}

		if (plainText[i] == ' ')
		{
			plainChar = 26;
		}
		else
		{
			plainChar = plainText[i] - 'A';
		}

		// add the values and then mod by 27 as described in specs
		cipherChar = (plainChar + keyChar) % 27;

		// convert the 0-26 char back to ASCII
		if (cipherChar == 26)
		{
			cipherChar = ' ';
		}
		else
		{
			cipherChar += 'A';
		}

		// place the transformed character in the empty cipherText string
		cipherText[i] = cipherChar;
	}
}
