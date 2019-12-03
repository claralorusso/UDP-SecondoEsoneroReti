#if defined WIN32
#include <winsock.h>
#else
#define closesocket close
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#endif
#include <stdio.h>
#include <string.h> /* for memset() */
#include "function.h"
#define ECHOMAX 255
#define BUFSIZE 30
void ErrorHandler(char *errorMessage) {
	printf(errorMessage);
}
void ClearWinSock() {
#if defined WIN32
	WSACleanup();
#endif
}

int main() {
#if defined WIN32
	WSADATA wsaData;
	int iResult = WSAStartup(MAKEWORD(2 ,2), &wsaData);
	if (iResult != 0) {
		printf ("error at WSASturtup\n");
		return EXIT_FAILURE;
	}
#endif
	int sock;
	struct sockaddr_in echoServAddr;
	struct sockaddr_in fromAddr;
	struct hostent *hostcl;
	unsigned int fromSize;
	char echoString[ECHOMAX];
	char echoBuffer[ECHOMAX];
	int echoStringLen;
	int respStringLen;
	int conta;


	// CREAZIONE DELLA SOCKET
	if ((sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
		ErrorHandler("socket() failed");
	// COSTRUZIONE DELL'INDIRIZZO DEL SERVER
	//leggo da tastiera indirizzo e porta server
	char s_name[BUFSIZE];
	printf ("Inserisci l'indirizzo dell'host a cui connetterti\n");
	scanf ("%s", s_name);
	hostcl = gethostbyname(s_name);
	int port;
	printf ("Inserisci la porta a cui connetterti\n");
	scanf("%d", &port);

	memset(&echoServAddr, 0, sizeof(echoServAddr));
	echoServAddr.sin_family = PF_INET;
	echoServAddr.sin_port = htons(port);
	echoServAddr.sin_addr.s_addr = inet_addr(s_name);
	//inserimento della stringa da inviare al server
	printf("Inserisci la prima stringa echo da inviare al server\n");
	scanf("%s", echoString);
	if ((echoStringLen = strlen(echoString)) > ECHOMAX)
		ErrorHandler("echo word too long");

	// INVIO DELLA STRINGA ECHO AL SERVER
	if (sendto(sock, echoString, echoStringLen, 0, (struct sockaddr*)&echoServAddr, sizeof(echoServAddr)) != echoStringLen)
		ErrorHandler("sendto() sent different number of bytes than expected");
	// RITORNO DELLA STRINGA ECHO
	fromSize = sizeof(fromAddr);
	respStringLen = recvfrom(sock, echoBuffer, ECHOMAX, 0, (struct sockaddr*)&fromAddr, &fromSize);

	if (echoServAddr.sin_addr.s_addr != fromAddr.sin_addr.s_addr) {
		fprintf(stderr, "Error: received a packet from unknown source.\n");
		exit(EXIT_FAILURE);
	}
	echoBuffer[respStringLen] = '\0';
	printf("Received: %s\n", echoBuffer);
	printf("Inserisci la stringa echo da inviare al server\n");
	scanf("%s", echoString);
	char messaggio[ECHOMAX];
	strcpy(messaggio,echoString);
	conta = count(echoString);
	sprintf(echoString,"%d",conta);
	if (sendto(sock, echoString, echoStringLen, 0, (struct sockaddr*)&echoServAddr, sizeof(echoServAddr)) != echoStringLen)
		ErrorHandler("sendto() sent different number of bytes than expected");
	int i=0;
	while (i < conta){
		if (messaggio[i]=='a'|| messaggio[i]== 'e'|| messaggio[i]== 'i'|| messaggio[i]== 'o'|| messaggio[i]== 'u')
			echoString[i] = messaggio[i];
			printf("%c", messaggio[i]);
			system("pause");
			if (sendto(sock, echoString, echoStringLen, 0, (struct sockaddr*)&echoServAddr, sizeof(echoServAddr)) != echoStringLen)
				ErrorHandler("sendto() sent different number of bytes than expected");
			printf("%c", echoString[i]);
			strcpy(echoString, " ");
			i++;

	}
	closesocket(sock);
	ClearWinSock();
	system("pause");
	return EXIT_SUCCESS;
}
