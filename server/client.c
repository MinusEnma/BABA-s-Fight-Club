
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>

// Display an error message and terminate the program.
void die_with_error(char *error_msg) {
    printf("%s\n", error_msg);
    exit(-1);
}

// Receive one line of text from the server.
int receive_line(int client_sock, char *buffer, int size) {
    int i = 0;
    char ch;

    while (i < size - 1) {
        int n = recv(client_sock, &ch, 1, 0);

        if (n <= 0) {
            return n;
        }

        if (ch == '\n') {
            break;
        }

        buffer[i++] = ch;
    }

    buffer[i] = '\0';
    return i;
}

int main(int argc, char *argv[]) {

    int client_sock, port_no, n;

    struct sockaddr_in server_addr;
    struct hostent *server;

    char buffer[256];

    if (argc < 3) {
        printf("Usage: %s hostname port_no\n", argv[0]);
        exit(1);
    }

    printf("Client starting ...\n");

    // STEP 1: Create a TCP socket.
    client_sock = socket(AF_INET, SOCK_STREAM, 0);

    if (client_sock < 0)
        die_with_error("Error: socket() Failed.");

    // STEP 2: Look for the server using its hostname.
    printf("Looking for host '%s'...\n", argv[1]);

    server = gethostbyname(argv[1]);

    if (server == NULL)
        die_with_error("Error: No such host.");

    printf("Host found!\n");

    // STEP 3: Prepare the server's address information.
    port_no = atoi(argv[2]);

    memset(&server_addr, 0, sizeof(server_addr));

    server_addr.sin_family = AF_INET;

    // Use memcpy instead of the older bcopy function.
    memcpy(&server_addr.sin_addr.s_addr,
           server->h_addr_list[0],
           server->h_length);

    server_addr.sin_port = htons(port_no);

    // STEP 4: Connect to the server.
    printf("Connecting to server at port %d...\n", port_no);

    if (connect(client_sock, (struct sockaddr *) &server_addr,
                sizeof(server_addr)) < 0)
        die_with_error("Error: connect() Failed.");

    printf("Connection successful!\n");

    // STEP 5: Receive the player assignment.
    memset(buffer, 0, sizeof(buffer));

    n = receive_line(client_sock, buffer, sizeof(buffer));

    if (n <= 0) {
        printf("Server disconnected.\n");
        close(client_sock);
        return 0;
    }

    printf("\n[server] > %s\n", buffer);

    // STEP 6: Receive and display the fighting style menu.
    while (1) {
        memset(buffer, 0, sizeof(buffer));

        n = receive_line(client_sock, buffer, sizeof(buffer));

        if (n <= 0) {
            printf("Server disconnected.\n");
            goto cleanup;
        }

        printf("%s\n", buffer);

        // Stop reading when the selection prompt arrives.
        if (strcmp(buffer, "Enter your choice (1-5): ") == 0) {
            break;
        }
    }

    // STEP 7: Ask the player for a fighting style.
    char choice[32];

    printf("[you] > ");

    if (fgets(choice, sizeof(choice), stdin) == NULL) {
        goto cleanup;
    }

    // Send the selection to the server.
    n = send(client_sock, choice, strlen(choice), 0);

    if (n <= 0) {
        printf("Error sending selection.\n");
        goto cleanup;
    }

    printf("\nWaiting for the other player to choose...\n");

    // STEP 8: Receive the selection confirmation.
    while (1) {
        memset(buffer, 0, sizeof(buffer));

        n = receive_line(client_sock, buffer, sizeof(buffer));

        if (n <= 0) {
            printf("\nServer disconnected.\n");
            break;
        }

        printf("%s\n", buffer);

        // Both players have finished selecting.
        if (strcmp(buffer,
                   "Both players have selected their fighting styles!") == 0) {
            break;
        }
    }

cleanup:

    // STEP 9: Close the client socket.
    close(client_sock);

    printf("\nConnection closed.\n");

    return 0;
}

