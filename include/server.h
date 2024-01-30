#ifndef MAIN_SERVER_H
#define MAIN_SERVER_H

struct serverInformation
{
    int   fd;
    char *ip;
    char *port;
    //
};

struct clientInformation
{
    int fd;
};

void server(void);
int  server_setup(char *passedServerInfo[]);
int  socket_create(void);
int  socket_bind(struct serverInformation server);
int  socket_close(struct serverInformation server);
void start_listen(int server_fd);
int  handle_connection(int server_fd, struct clientInformation clients[], int *numClients);
void get_req_response(int client_socket);
int  run_server(void);
#endif    // MAIN_SERVER_H
