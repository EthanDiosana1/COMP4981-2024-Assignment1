// temp save server.c

#include "../include/server.h"
#include "../include/stringTools.h"
#include <arpa/inet.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/poll.h>
#include <sys/socket.h>
#include <unistd.h>

#define MAX_BUFFER_SIZE 1024

// NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables)
static volatile sig_atomic_t exit_flag = 0;
// NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables)
static struct pollfd fds[SOMAXCONN];

void server(void) { printf("SERVER\n"); }

int server_setup(char *passedServerInfo[]) {
  struct serverInformation newServer;
  struct clientInformation clients[SOMAXCONN];
  int numClients;
  // sigaction --> close
  // server setup
  newServer.ip = passedServerInfo[0];
  newServer.port = passedServerInfo[1];
  // socket
  newServer.fd = socket_create();
  // bind
  socket_bind(newServer);
  // listen <-- io mult
  start_listen(newServer.fd);

  // Initialize the main server socket in the pollfd array
  fds[0].fd = newServer.fd;
  fds[0].events = POLLIN;

  numClients = 1; // Number of clients (starting with the server itself)

  handle_connection(newServer.fd, clients, &numClients);

  server_close(newServer);
  return 0;
}

// to set up server:

int socket_create(void) {
  int serverSocket;
  serverSocket = socket(AF_INET, SOCK_STREAM, 0);
  if (serverSocket == -1) {
    perror("Socket create failed");
    exit(EXIT_FAILURE);
  }
  return serverSocket;
}

int socket_bind(struct serverInformation activeServer) {
  struct sockaddr_in server_address;
  int port;
  char *endptr;
  const int decimalBase = 10;

  memset(&server_address, 0, sizeof(server_address));
  server_address.sin_family = AF_INET;

  if (inet_pton(AF_INET, activeServer.ip, &server_address.sin_addr) <= 0) {
    perror("Invalid IP address");
    return -1;
  }

  port = (int)strtol(activeServer.port, &endptr, decimalBase);

  if (*endptr != '\0' && *endptr != '\n') {
    perror("Invalid port number");
    return -1;
  }

  server_address.sin_port = htons((uint16_t)port);

  if (bind(activeServer.fd, (struct sockaddr *)&server_address,
           sizeof(server_address)) == -1) {
    perror("Bind failed");
    exit(EXIT_FAILURE);
  }

  printf("Socket bound successfully to %s:%s.\n", activeServer.ip,
         activeServer.port);

  return 0;
}

void start_listen(int server_fd) {
  if (listen(server_fd, SOMAXCONN) == -1) {
    perror("listen failed");
    close(server_fd);
    exit(EXIT_FAILURE);
  }
  printf("Listening for incoming connections...\n");
}

/**
 * Function handles main logic loop for ready client sockets using IO
 * multiplexing
 * @param server_fd server socket
 * @param clients array of client socket fds
 * @param numClients number of clients
 * @return 1 on success
 */
int handle_connection(int server_fd, struct clientInformation clients[],
                      int *numClients) {
  TokenAndStr requestFirstLine;

  while (!exit_flag) {
    int activity;
    for (int i = 1; i < *numClients; i++) {
      fds[i].fd = clients[i].fd;
      fds[i].events = POLLIN;
    }

    // poll used for IO multiplexing
    activity = poll(fds, (nfds_t)*numClients, -1); // -1 means wait indefinitely

    if (activity == -1) {
      perror("poll failed");
      exit(EXIT_FAILURE);
    }

    // check for new connection on the main server socket
    if (fds[0].revents & POLLIN) {
      int client_fd = accept(server_fd, NULL, NULL);
      if (client_fd == -1) {
        perror("accept failed");
        // Handle error if needed
      } else {
        printf("New client: adding to array\n");
        clients[*numClients].fd = client_fd;
        // TODO add to client struct here when ready using clients[*numClients]
        (*numClients)++;

        fds[*numClients - 1].fd = client_fd;

        // waits for next input from any client
        fds[*numClients - 1].events = POLLIN;
      }
    }

    // check if any client is ready to send a response to
    for (int i = 1; i < *numClients; ++i) {
      if (fds[i].revents & POLLIN) {
        char buffer[MAX_BUFFER_SIZE];
        ssize_t bytesRead;
        printf("Client ready; working..\n");
        // read in req
        bytesRead = recv(clients[i].fd, buffer, sizeof(buffer), 0);
        if (bytesRead == 0) {
          printf("Client disconnected: %d\n", clients[i].fd);
          close(clients[i].fd);
          for (int j = i; j < *numClients - 1; ++j) {
            clients[j] = clients[j + 1];
            fds[j] = fds[j + 1];
          }
          (*numClients)--;
        } else if (bytesRead < 0) {
          perror("recv failed");
        } else {
          printf("Request::\n%s\n", buffer);

          // TODO: Parse the request into a struct.

          // Get only the first line.
          requestFirstLine = getFirstToken(buffer, "\n");

          printf("Request first line: %s\n", requestFirstLine.token);

          // send back req
          get_req_response(clients[i].fd);
        }
      }
    }
  }
  return 1;
}

/**
 * Function to close the given server socket
 * @param activeServer server socket fd
 * @return 0 if success
 */
int server_close(struct serverInformation activeServer) {
  if (close(activeServer.fd) == -1) {
    perror("close failed");
    exit(EXIT_FAILURE);
  }
  printf("Closing the server.\n");
  return 0;
}

/**
 * Function to close a client socket
 * @param activeClient client socket fd
 * @return 0 if success
 */
int client_close(int activeClient) {
  if (close(activeClient) == -1) {
    perror("close failed");
    exit(EXIT_FAILURE);
  }
  printf("Closing the client.\n");
  return 0;
}

/**
 * Function to construct the response and send to client socket (todo breakdown)
 * @param client_socket client to send the response to
 * @param content content of the resource requested
 * @return 0 if success
 */
int send_response(int client_socket, const char *content) {
  char response[MAX_BUFFER_SIZE];
  snprintf(response, MAX_BUFFER_SIZE,
           "HTTP/1.1 200 OK\r\nContent-Length: %ld\r\n\r\n%s",
           (long)strlen(content), content);
  if (send(client_socket, response, strlen(response), 0) == -1) {
    perror("Error sending response");
    return -1;
  }
  return 0;
}

/**
 * Function to read the resource, save the resource into a malloc, send the
 * resource back to the client (todo breakdown into funcs)
 * @param client_socket client socket that sends the req
 * @return 0 if success
 */
int get_req_response(int client_socket) {
  // todo remove hardcoded "./index.html" fopen when relevant modules are
  // available to parse req
  FILE *html_file = fopen("html/index.html", "re");
  char buffer[MAX_BUFFER_SIZE];
  size_t bytesRead;
  size_t totalBytesRead = 0;
  char *html_content;

  if (html_file == NULL) {
    perror("Error opening HTML file");
    return -1;
  }

  // read for size of file
  while ((bytesRead = fread(buffer, 1, sizeof(buffer), html_file)) > 0) {
    totalBytesRead += bytesRead;
  }
  fclose(html_file);

  // allocate memory for the HTML
  html_content = (char *)malloc(totalBytesRead + 1);
  if (html_content == NULL) {
    perror("Error allocating memory");
    return -1;
  }

  // read HTML content again to store it in html_content
  html_file = fopen("./index.html", "re");
  if (html_file == NULL) {
    perror("Error opening HTML file");
    free(html_content);
    return -1;
  }

  totalBytesRead = 0;

  // read the file into html_content
  while ((bytesRead = fread(html_content + totalBytesRead, 1, MAX_BUFFER_SIZE,
                            html_file)) > 0) {
    totalBytesRead += bytesRead;
  }
  html_content[totalBytesRead] = '\0';
  fclose(html_file);

  // construct the response and send
  if (send_response(client_socket, html_content) == -1) {
    free(html_content);
    return -1;
  }

  free(html_content);
  return 0;
}

#include "server.h"
