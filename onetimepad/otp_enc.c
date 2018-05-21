/* otp_enc.c
 * August Lautt */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>
#include <fcntl.h>
#include <sys/stat.h>

// PASSCODE is the message sent when handshaking
#define FILENAME_SIZE 50
#define PASSCODE "EN"


void getFile(char *fileName, char *fileText, int fileSize);
int makeContact(int portNumber);
void handShake(int socketFD, int portNumber);


int main(int argc, char *argv[])
{
	// socket connection file descriptor and port for converting arg to int
	int socketFD, portNumber;
	// stat to get the file sizes
	struct stat textStat, keyStat;
	// ints to hold those files sizes and create strings with
	int textSize, keySize;
	// generic value the holds the number of bytes received or sent
	int numBytes;
	// strings that hold the filenames
	char textName[FILENAME_SIZE], keyName [FILENAME_SIZE];

	// check for 4 args
	if (argc != 4)
	{
		fprintf(stderr, "Usage: %s [plaintext] [key] [port]\n", argv[0]);
		exit(1);
	}

	// grab the file names to read and check
	memset(textName, '\0', sizeof textName);
	memset(keyName, '\0', sizeof keyName);
	strcpy(textName, argv[1]);
	strcpy(keyName, argv[2]);

	// grab the port number and convert it to string
	portNumber = atoi(argv[3]);

	// get the sizes of each file
	stat(textName, &textStat);
	stat(keyName, &keyStat);
	textSize = textStat.st_size;
	keySize = keyStat.st_size;

	// check that key is not too small for encryption/decryption
	if (keySize < textSize)
	{
		fprintf(stderr, "Error: key '%s' is too short\n", keyName);
		exit(1);
	}

	// create strings with the correct sizes
	char plainText[textSize], keyText[keySize], cipherText[textSize];

	memset(plainText, '\0', sizeof plainText);
	memset(keyText, '\0', sizeof keyText);
	memset(cipherText, '\0', sizeof cipherText);

	// load files into strings and also validate the file
	getFile(textName, plainText, textSize);
	getFile(keyName, keyText, keySize);

	// contact the server and create a socket
	socketFD = makeContact(portNumber);

	// perform the secret handshake, make sure enc only connects to enc servers
	handShake(socketFD, portNumber);

	// send the size of text and key to server, so it knows what to expect
	if ((numBytes = send(socketFD, &textSize, sizeof(textSize), 0)) < 0)
	{
		fprintf(stderr, "Error: could not send text size to server\n");
		close(socketFD);
		exit(1);
	}

	if ((numBytes = send(socketFD, &keySize, sizeof(keySize), 0)) < 0)
	{
		fprintf(stderr, "Error: could not send key size to server\n");
		close(socketFD);
		exit(1);
	}

	// send the contents of the plaintext and key files
	if ((numBytes = send(socketFD, plainText, sizeof(plainText), 0)) < 0)
	{
		fprintf(stderr, "Error: could not send plain text to server\n");
		close(socketFD);
		exit(1);
	}

	if ((numBytes = send(socketFD, keyText, sizeof(keyText), 0)) < 0)
	{
		fprintf(stderr, "Error: could not send key text to server\n");
		close(socketFD);
		exit(1);
	}

	// receive the ciphered text contents back using the MSG_WAITALL flag
	// because we exchanged sizes with the server it knows the exact sizes
	// when we receive with MSG_WAITALL we block until all the data has been received
	if ((numBytes = recv(socketFD, cipherText, sizeof(cipherText), MSG_WAITALL)) < 0)
	{
		fprintf(stderr, "Error: could not receive cipher text from server\n");
		close(socketFD);
		exit(1);
	}

	// print to stdout with the newline again
	fprintf(stdout, "%s\n", cipherText);

	close(socketFD);

	return 0;
}


/* Function to read the contents of a provided file into a string and then
 * check that it does not have any invalid characters.
 * Takes: filename to read, a string of the correct size to put contents
 * into and the file size.
 * Returns: nothing. */

void getFile(char *fileName, char *fileText, int fileSize)
{
	int inputFD;
	int numBytes, i;

	// open file with file descriptor and read only flag
	if ((inputFD = open(fileName, O_RDONLY)) == -1)
	{
		fprintf(stderr, "Error: unable to open file '%s'\n", fileName);
		close(inputFD);
		exit(1);
	}

	// read file into file text
	if ((numBytes = read(inputFD, fileText, fileSize)) < 0)
	{
		fprintf(stderr, "Error: unable read file '%s'\n", fileName);
		close(inputFD);
		exit(1);
	}

	// replace the newline that was in the actual text file
	fileText[fileSize - 1] = '\0';

	// search text for non-capitals and non-spaces and exit if found
	for (i = 0; i < strlen(fileText); i++)
	{
		if ((fileText[i] < 'A' || fileText[i] > 'Z') && fileText[i] != ' ')
		{
			fprintf(stderr, "Error: '%s' contains bad characters\n", fileName);
			close(inputFD);
			exit(1);
		}
	}

	close(inputFD);
}


/* Function to create a socket and then attempt form a connection to a server
 * with the provided port and gathered address.
 * Takes: port number that was passed as argument and converted to int.
 * Returns: the socket file descriptor that was created for connection. */

int makeContact(int portNumber)
{
	// socket file descriptor and structs to hold the server information
	int socketFD;
	struct sockaddr_in serverAddress;
	struct hostent *serverHostInfo;

	// create a socket set for TCP IPv4
	if ((socketFD = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		fprintf(stderr, "Error: unable to create socket\n");
		exit(1);
	}

	// get the hostname, we are just using local host
	serverHostInfo = gethostbyname("localhost");

	if (serverHostInfo == NULL)
	{
		fprintf(stderr, "Error: unable to get host name\n");
		exit(1);
	}

	// create IPv4 server address with IP and port information we have gathered
	memset((char*)&serverAddress, '\0', sizeof(serverAddress));

	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(portNumber);

	memcpy((char*)&serverAddress.sin_addr.s_addr, (char*)serverHostInfo->h_addr, serverHostInfo->h_length);

	// connect our client socket to the server address
	if (connect(socketFD, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0)
	{
		fprintf(stderr, "Error: could not contact otp_enc_d on port %d\n", portNumber);
		exit(2);
	}

	return socketFD;
}


/* Function to exchange PASSCODE between client and server to make sure client is connecting
 * to the correct of server type.
 * Takes: the socketFD for the connection and the port number just for printing. */

void handShake(int socketFD, int portNumber)
{
	int numBytes;
	char secret[3];

	// copy the passcode "EN" for encryption and send it to server
	memset(secret, '\0', sizeof secret);
	strcpy(secret, PASSCODE);

	if ((numBytes = send(socketFD, secret, sizeof(secret), 0)) < 0)
	{
		fprintf(stderr, "Error: could not send handshake to server\n");
		close(socketFD);
		exit(1);
	}

	// receive the server's passcode
	if ((numBytes = recv(socketFD, secret, sizeof(secret), 0)) <= 0)
	{
		fprintf(stderr, "Error: could not receive handshake from server\n");
		close(socketFD);
		exit(1);
	}

	// check if server's passcode matches and exit with status 2 if not
	if (strcmp(secret, PASSCODE) != 0)
	{
		fprintf(stderr, "Error: could not contact otp_enc_d on port %d\n", portNumber);
		close(socketFD);
		exit(2);
	}
}
