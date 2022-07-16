#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>
#include "fields.h"
#include "jval.h"
#include "dllist.h"
#include "jrb.h"
#include "sockettome.h"

struct Room {
	int sock;
	char *name;
	Dllist clients;
	Dllist chatQueue;
	pthread_cond_t *condVar;
};

struct ClientFile {
	char *name;
	FILE *file;
};

JRB rooms;
pthread_mutex_t lock;

/**
* @name FreeTree
* @brief Frees all the memory used to store each room in the global JRB variable rooms.
*/
void
FreeTree() {
	JRB rnode; 
	Dllist temp;
	struct Room *room;

	pthread_mutex_lock(&lock);
	jrb_traverse(rnode, rooms) {
		room = (struct Room*)rnode->val.v;		
		free(room->name);
		free(room->condVar);
		dll_traverse(temp, room->clients) {
			struct ClientFile *cf = (struct ClientFile*)temp->val.v;
			free(cf->name);
			free(cf);
		}
		free_dllist(room->clients);
		free_dllist(room->chatQueue);
		free(room);
	}

	pthread_mutex_unlock(&lock);
	jrb_free_tree(rooms);

	exit(0);
}

/**
* @name ServerThread.
* @brief This is the thread for each room that will print out chat's in the chatQueue whenever a client makes one.
* @param[in] roomCond This is the condition variable specific to that thread that is used to unblock the thread when a client writes to the server.
* @return This will never return. It can only end upon server termination
*/
void
*ServerThread(void *roomCond) {
	pthread_cond_t *cond = (pthread_cond_t *)roomCond;
	JRB rnode;
	Dllist temp1, temp2;
	struct Room *currRoom;

	//Loop to print chat's in the chat queue to each client in that room
	while(1) {
		pthread_cond_wait(cond, &lock);

		//Find the room to print to
		jrb_traverse(rnode, rooms) {
			currRoom = (struct Room*)rnode->val.v;
			if (currRoom->condVar == cond) break;
		}
		//Print chat's to each client
		dll_traverse(temp1, currRoom->clients) {
			struct ClientFile *cf = (struct ClientFile*)temp1->val.v;
			FILE *roomFile = cf->file;
			dll_traverse(temp2, currRoom->chatQueue) {
				fputs(temp2->val.s, roomFile);
				fflush(roomFile);
			}
		}
		//Clears out chat queue
		while(!dll_empty(currRoom->chatQueue)) {
			Dllist temp = dll_last(currRoom->chatQueue);
			free(temp->val.s);
			dll_delete_node(temp);
		}
	}

	int retVal = 0;
	pthread_exit(&retVal);
}

/**
* @name ClientThread
* @brief This is the thread for each client. It will print out the rooms and its occupants, read in the clients name and desired room, and then read in each clients message and signal the room to print it.
* @param[in] file This is the file descriptor used for reading and writing from the socket.
* @return This won't return, but it will terminate and free memory upon detection of EOF (client leaves the server).
*/
void
*ClientThread(void *file) {
	int *fdp = (int *)file;
	FILE *clientFile, *roomFile;
	JRB rnode1;
	Dllist temp;
	
	clientFile = fdopen(*fdp, "r");
	roomFile = fdopen(*fdp, "w");

	//Printing out chat rooms and clients
	fputs("Chat Rooms:\n\n", roomFile);
	pthread_mutex_lock(&lock);
	jrb_traverse(rnode1, rooms) {
		char *roomName = rnode1->key.s;
		fputs(roomName, roomFile);
		fputs(":", roomFile);

		struct Room *room = (struct Room*)rnode1->val.v;
		dll_traverse(temp, room->clients) {
			if(strcmp(roomName, room->name) == 0) {
				struct ClientFile *cf = (struct ClientFile*)temp->val.v;
				fputs(" ", roomFile);
				fputs(cf->name, roomFile);
			}
		}
		fputs("\n", roomFile);
	}
	pthread_mutex_unlock(&lock);

	//Reading in name
	fputs("\nEnter your chat name (no spaces):\n", roomFile);
	fflush(roomFile);
	char name[100];
	fgets(name, 100, clientFile);
	if(feof(clientFile)) {
		printf("Client Has Left\n");
		fclose(clientFile);
		fclose(roomFile);
		free(file);
		int retVal = 1;
		pthread_exit(&retVal);
	}
	name[strlen(name)-1] = '\0';

	//Reading in room name
	fputs("Enter chat room:\n", roomFile);
	fflush(roomFile);
	char roomName[100];
	fgets(roomName, 100, clientFile);
	if(feof(clientFile)) {
		printf("Client Has Left\n");
		fclose(clientFile);
		fclose(roomFile);
		free(file);
		int retVal = 1;
		pthread_exit(&retVal);
	}
	roomName[strlen(roomName)-1] = '\0';

	//Putting client and file descriptor into the specified room's list
	pthread_mutex_lock(&lock);
	rnode1 = jrb_find_str(rooms, roomName);
	struct Room *room = (struct Room*)rnode1->val.v;
	struct ClientFile *cf = malloc(sizeof(*cf));
	cf->name = strdup(name);
	cf->file = roomFile;
	dll_append(room->clients, new_jval_v((void*)cf));
	pthread_mutex_unlock(&lock);

	//Adding join message to the specified room's chat queue
	char input[1000];
	strcat(input, name);
	strcat(input, " has joined\n\0");
	pthread_mutex_lock(&lock);
	dll_append(room->chatQueue, new_jval_s(strdup(input)));
	pthread_cond_signal(room->condVar);
	pthread_mutex_unlock(&lock);

	//Loop to read in clients text
	while(1) {
		char clientText[1000];
		clientText[0] = '\0';
		fgets(clientText, 1000, clientFile);
		
		//Frees memory and kills thread on EOF
		if(feof(clientFile)) {
			fclose(clientFile);
			fclose(roomFile);
			pthread_mutex_lock(&lock);
			dll_traverse(temp, room->clients) {
				cf = (struct ClientFile*)temp->val.v;
				if (strcmp(cf->name, name) == 0) break;
			}
			free(cf->name);
			free(cf);
			dll_delete_node(temp);
			char leaving[100];
			strcat(leaving, name);
			strcat(leaving, " has left\n");
			dll_append(room->chatQueue, new_jval_s(strdup(leaving)));
			pthread_cond_signal(room->condVar);
			pthread_mutex_unlock(&lock);
			free(file);
			printf("Client Has Left\n");
			int retVal = 1;
			pthread_exit(&retVal);
		}

		clientText[strlen(clientText)-1] = '\0';
		input[0] = '\0';
		strcat(input, name);
		strcat(input, ": ");
		strcat(input, clientText);
		strcat(input, "\n");

		pthread_mutex_lock(&lock);
		dll_append(room->chatQueue, new_jval_s(strdup(input)));
		pthread_cond_signal(room->condVar);
		pthread_mutex_unlock(&lock);
	}

	fclose(clientFile);
	return 0;
}

int main(int argc, char** argv) {
	if(argc < 3) {
		perror("usage: chat_server port roomname1...\n");
		exit(1);
	}

	int sock, port = atoi(argv[1]);
	if (port < 50000) {
		perror("usage: chat_server port roomname1...\n");
		perror("       port must be > 5000\n");
		exit(1);
	}

	sock = serve_socket(port);
	printf("Server Online...\n");

	pthread_mutex_init(&lock, NULL);
	pthread_t serverThreads[argc-2];
	pthread_attr_t attr[argc-1];
	rooms = make_jrb();

	//Read creating room threads
	for (int i = 2; i  < argc; i++) {
		struct Room *room = malloc(sizeof(*room));
		pthread_cond_t *cond = malloc(sizeof(*cond));
		pthread_cond_init(cond, NULL);
		char *temp = strdup(argv[i]);
		room->sock = sock;
		room->name = temp;
		room->clients = new_dllist();
		room->chatQueue = new_dllist();
		room->condVar = cond;
		jrb_insert_str(rooms, temp, new_jval_v((void*)room));

		pthread_attr_init(attr+(i-1));
		pthread_attr_setscope(attr+(i-1), PTHREAD_SCOPE_SYSTEM);
		if(pthread_create(serverThreads+(i-2), attr+(i-1), ServerThread, (void*)cond) != 0) {
			perror("pthread_create");
			exit(1);
		}
	}
	//Frees room memory on ctrl-c
	signal(SIGINT, FreeTree);
	printf("Rooms Initialized...\n");
	printf("Now Accepting Clients...\n\n");

	//Creating room threads
	pthread_attr_init(attr);
	pthread_attr_setscope(attr, PTHREAD_SCOPE_SYSTEM);
	while(1) {
		int *fdp, fd = accept_connection(sock);
		fdp = (int *)malloc(sizeof(int));
		*fdp = fd;
		pthread_t *client = (pthread_t *)malloc(sizeof(*client));
		printf("Client Connection Accepted\n");
		if(pthread_create(client, attr, ClientThread, fdp) != 0) {
			perror("pthread_create");
			exit(1);
		}
		free(client);
	}

	return 0;
}