
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
#include "functionClient.h"
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
	struct hostent *ipserv;
	int fromSize;
	char echoString[ECHOMAX];
	char echoBuffer[ECHOMAX];
	int echoStringLen;
	int respStringLen;
	int conta;


	// CREAZIONE DELLA SOCKET
	if ((sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
		ErrorHandler("socket() failed");
	//leggo da tastiera indirizzo e porta server
	char s_name[BUFSIZE];
	printf ("Inserisci l'indirizzo dell'host a cui connetterti\n");
	scanf ("%s", s_name);
	ipserv=gethostbyname(s_name);
	if(ipserv==NULL){
		printf("Gethostbyname fallito\n");
		system("PAUSE");
		exit(1);
	}
	struct in_addr *ina = (struct in_addr*) ipserv->h_addr_list[0];
	int port;

	printf ("Inserisci la porta a cui connetterti\n");
	scanf("%d", &port);
	// COSTRUZIONE DELL'INDIRIZZO DEL SERVER
	memset(&echoServAddr, 0, sizeof(echoServAddr));
	echoServAddr.sin_family = PF_INET;
	echoServAddr.sin_port = htons(port);
	echoServAddr.sin_addr.s_addr = inet_addr(inet_ntoa(*ina));
	//inserimento della stringa da inviare al server
	printf("Inserisci la prima stringa echo da inviare al server\n");
	scanf("%s", echoString);
	if ((echoStringLen = strlen(echoString)) > ECHOMAX)
		ErrorHandler("echo word too long");
	// INVIO DELLA STRINGA ECHO AL SERVER
	if (sendto(sock, echoString, echoStringLen, 0, (struct sockaddr*)&echoServAddr, sizeof(echoServAddr)) != echoStringLen)
		ErrorHandler("sendto() sent different number of bytes than expected");
	// RITORNO DELLA STRINGA "OK"
	fromSize = sizeof(fromAddr);
	respStringLen = recvfrom(sock, echoBuffer, ECHOMAX, 0, (struct sockaddr*)&fromAddr, &fromSize);

	if (echoServAddr.sin_addr.s_addr != fromAddr.sin_addr.s_addr) {
		fprintf(stderr, "Error: received a packet from unknown source.\n");
		exit(EXIT_FAILURE);
	}
	echoBuffer[respStringLen] = '\0';
	printf("Received: %s\n", echoBuffer); //risposta
	//Rischiesta di inserimento della stringa per contare le vocali
	printf("Inserisci nuova stringa\n");
	scanf("%s", echoString);
	char messaggio[ECHOMAX];
	char parola[ECHOMAX];
	strcpy(messaggio,echoString);
	conta = count(echoString);
	strcpy(parola,echoString);
	sprintf(echoString,"%d",conta);
	//invio del numero di vocali al server
	if (sendto(sock, echoString, echoStringLen, 0, (struct sockaddr*)&echoServAddr, sizeof(echoServAddr)) != echoStringLen)
		ErrorHandler("sendto() sent different number of bytes than expected");
	sprintf(echoString, " %s", " ");
	int flag = 0;
	int i=0;
	while (i <= strlen(parola)){
		if (messaggio[i]=='a'|| messaggio[i]== 'e'|| messaggio[i]== 'i'|| messaggio[i]== 'o'|| messaggio[i]== 'u'){
			echoString[0] = messaggio[i];
			flag = 1;
			//invio di ogni vocale trovata
			if (sendto(sock, echoString, echoStringLen, 0, (struct sockaddr*)&echoServAddr, sizeof(echoServAddr)) != echoStringLen){
				ErrorHandler("sendto() sent different number of bytes than expected");
			}
		}
		strcpy(echoString, " ");
		i++;
	}
	//caso in cui non ci siano vocali da convertire
	if (flag == 0){
		printf("Non ci sono vocali nella stringa inserita \n");
	}

	for(i = 0; i < conta;i++){
		//ritorno delle vocali convertite in maiuscolo
		fromSize = sizeof(fromAddr);
		respStringLen = recvfrom(sock, echoBuffer, ECHOMAX, 0, (struct sockaddr*)&fromAddr, &fromSize);
		if (echoServAddr.sin_addr.s_addr != fromAddr.sin_addr.s_addr) {
			fprintf(stderr, "Error: received a packet from unknown source.\n");
			exit(EXIT_FAILURE);
		}
		printf("Received: vocale convertita : %s \n", echoBuffer);
	}
	closesocket(sock);
	ClearWinSock();
	system("pause");
	return EXIT_SUCCESS;
}
