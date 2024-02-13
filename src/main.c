// structure: include other relevant .h files
// main should parse args

#include "client.h"
#include "server.h"
#include "sigintHandler.h"
#include "stringTools.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define USAGE "Usage: -t type -i ip -p port\n"

// struct to hold the values for
struct arguments {
  char *type;
  char *ip;
  char *port;
};

// checks that arguments are expected
struct arguments parse_args(int argc, char *argv[]);
// starts server or connects client to server
int handle_args(struct arguments args);

// drives code
int main(int argc, char *argv[]) {
  // Set Ctrl-C override.
  // NOLINTNEXTLINE
  signal(SIGINT, sigintHandler);

  if (handle_args(parse_args(argc, argv))) {
  }

  // else: failstate already

  // handle args --> does stuff with the args

  server();
  client();
  // parse args
  // handle args --> either set up server on ip:port or connect
  return EXIT_SUCCESS;
}

struct arguments parse_args(int argc, char *argv[]) {
  int opt;
  struct arguments newArgs;

  // Set default values
  newArgs.type = NULL;
  newArgs.ip = NULL;
  newArgs.port = NULL;

  while ((opt = getopt(argc, argv, "t:i:p:")) != -1) {
    switch (opt) {
    case 't':
      newArgs.type = optarg;
      break;
    case 'i':
      newArgs.ip = optarg;
      break;
    case 'p':
      newArgs.port = optarg;
      break;
    default:
      fprintf(stderr, "Usage: %s -t type -i ip -p port\n", argv[0]);
      exit(EXIT_FAILURE);
    }
  }

  // Check if required options are provided
  if (newArgs.type == NULL || newArgs.ip == NULL || newArgs.port == NULL ||
      (!checkIfCharInString(newArgs.ip, '.')) ||
      (checkIfCharInString(
          newArgs.port,
          '.'))) { // TODO add || for if ip has no "." or if port has "."
    fprintf(stderr, "Usage: %s -t type -i ip -p port\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  return newArgs;
}

int handle_args(struct arguments passedArgs) {
  char *serverInformation[2];
  serverInformation[0] = passedArgs.ip;
  serverInformation[1] = passedArgs.port;
  printf("%s\n", passedArgs.ip);
  if (passedArgs.type == NULL || passedArgs.ip == NULL ||
      passedArgs.port == NULL) {
    return 1;
  }
  if (strcmp(passedArgs.type, "connect") == 0) {
    // client

    connect_client(serverInformation);
  } else if (strcmp(passedArgs.type, "accept") == 0) {
    // server
    server_setup(serverInformation);
  } else {
    fprintf(stderr,
            "Error: Invalid type: %s\n"
            "Available: 'connect', 'accept'\n%s",
            passedArgs.type, USAGE);
    exit(EXIT_FAILURE);
  }
  return 0;
}
