// temp save server.c

#include "../include/server.h"
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

  // Convert the port from string to integer
  port = (int)strtol(activeServer.port, &endptr, decimalBase);

  // Check for errors during conversion
  if (*endptr != '\0' && *endptr != '\n') {
    perror("Invalid port number");
    // Handle the error accordingly
    return -1;
  }

  // Now 'port' contains the converted value
  server_address.sin_port = htons((uint16_t)port);

  // Bind the socket to the specified address
  if (bind(activeServer.fd, (struct sockaddr *)&server_address,
           sizeof(server_address)) == -1) {
    perror("Bind failed");
    exit(EXIT_FAILURE);
  }

  printf("Socket bound successfully to %s:%s.\n", activeServer.ip,
         activeServer.port);

  // Return success
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

// set up io multiplexing using poll

int handle_connection(int server_fd, struct clientInformation clients[],
                      int *numClients) {
  while (!exit_flag) {
    // Add all active clients to the pollfd array
    int activity;
    for (int i = 1; i < *numClients; i++) {
      fds[i].fd = clients[i].fd;
      fds[i].events = POLLIN;
    }

    // Call poll to wait for events (incoming data or new connections)
    activity = poll(fds, (nfds_t)*numClients, -1); // -1 means wait indefinitely

    if (activity == -1) {
      perror("poll failed");
      exit(EXIT_FAILURE);
    }

    // Check for new connection on the main server socket
    if (fds[0].revents & POLLIN) {
      // Accept the new connection and add the client to the array
      int client_fd = accept(server_fd, NULL, NULL);
      if (client_fd == -1) {
        perror("accept failed");
        // Handle error if needed
      } else {
        // Add the new client to the array
        printf("New client: adding to array\n");
        clients[*numClients].fd = client_fd;
        // Initialize any other client-specific information if needed
        (*numClients)++;

        // Add the new client to the pollfd array
        fds[*numClients - 1].fd = client_fd;
        // wait for any clients to give input
        fds[*numClients - 1].events = POLLIN;
        if (get_req_response(client_fd) == -1) {
          client_close(client_fd);
        }
      }
    }

    // Check for data on existing clients
    for (int i = 1; i < *numClients; ++i) {
      if (fds[i].revents & POLLIN) {
        char buffer[MAX_BUFFER_SIZE];
        ssize_t bytesRead;
        printf("Client ready; working..\n");
        // read in req
        bytesRead = recv(clients[i].fd, buffer, sizeof(buffer), 0);
        if (bytesRead == 0) {
          // client dcs
          printf("Client disconnected: %d\n", clients[i].fd);
          close(clients[i].fd);
          for (int j = i; j < *numClients - 1; ++j) {
            clients[j] = clients[j + 1];
            fds[j] = fds[j + 1];
          }

          (*numClients)--;
        } else if (bytesRead < 0) {
          // handle other recv errors
          perror("recv failed");
          // add more error handling if needed
        } else {
          // process request
          printf("%s\n", buffer);
          // send back req as body (replace with struct)
          get_req_response(clients[i].fd);
        }
      }
    }
  }
  return 1;
}

int server_close(struct serverInformation activeServer) {
  if (close(activeServer.fd) == -1) {
    perror("close failed");
    exit(EXIT_FAILURE);
  }
  printf("Closing the server.\n");
  return 0;
}

int client_close(int activeClient) {
  if (close(activeClient) == -1) {
    perror("close failed");
    exit(EXIT_FAILURE);
  }
  printf("Closing the client.\n");
  return 0;
}

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

int get_req_response(int client_socket) {
  // Open and read the HTML file
  FILE *html_file = fopen("./index.html", "re");
  char buffer[MAX_BUFFER_SIZE];
  size_t bytesRead;
  size_t totalBytesRead = 0;
  char *html_content;

  if (html_file == NULL) {
    perror("Error opening HTML file");
    return -1;
  }

  // read html file
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
  while ((bytesRead = fread(html_content + totalBytesRead, 1, MAX_BUFFER_SIZE,
                            html_file)) > 0) {
    totalBytesRead += bytesRead;
  }
  html_content[totalBytesRead] = '\0';
  fclose(html_file);

  // send the HTTP response
  if (send_response(client_socket, html_content) == -1) {
    free(html_content);
    return -1;
  }

  free(html_content);
  return 0;
}

#include "server.h"
