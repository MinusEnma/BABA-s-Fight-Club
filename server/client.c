#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>

// Display error message and terminate program.
void die_with_error(char *error_msg) {
    printf("%s\n", error_msg);
    exit(-1);
}

// Clear the terminal screen.
void clear_screen(void) {
    printf("\033[2J\033[H");
    fflush(stdout);
}

// Receive one line of text from server.
int receive_line(int client_sock, char *buffer, int size) {
    int i = 0;
    char ch;

    while (i < size - 1) {
        int n = recv(client_sock, &ch, 1, 0);

        // Return -1 only when the connection is closed or an error occurs.
        if (n <= 0) {
            return -1;
        }

        // A newline ends the current line.
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
    char choice[32];
    char request[64];

    // Check if client provided hostname and port number.
    if (argc < 3) {
        printf("Usage: %s hostname port_no\n", argv[0]);
        exit(1);
    }

    printf("Client starting ...\n");

    // Create a TCP socket.
    client_sock = socket(AF_INET, SOCK_STREAM, 0);

    if (client_sock < 0)
        die_with_error("Error: socket() Failed.");

    // Look for the server using its hostname.
    printf("Looking for host '%s'...\n", argv[1]);

    server = gethostbyname(argv[1]);

    if (server == NULL)
        die_with_error("Error: No such host.");

    printf("Host found!\n");

    // Prepare the server's address information.
    port_no = atoi(argv[2]);

    memset(&server_addr, 0, sizeof(server_addr));

    server_addr.sin_family = AF_INET;

    memcpy(&server_addr.sin_addr.s_addr,
           server->h_addr_list[0],
           server->h_length);

    server_addr.sin_port = htons(port_no);

    // Connect to the server.
    printf("Connecting to server at port %d...\n", port_no);

    if (connect(client_sock, (struct sockaddr *) &server_addr,
                sizeof(server_addr)) < 0)
        die_with_error("Error: connect() Failed.");

    printf("Connection successful!\n");

    // Receive the player assignment.
    memset(buffer, 0, sizeof(buffer));

    n = receive_line(client_sock, buffer, sizeof(buffer));

    if (n < 0) {
        printf("Server disconnected.\n");
        goto cleanup;
    }

    printf("\n[server] > %s\n", buffer);

    // Receive and display the fighting style menu.
    while (1) {
        memset(buffer, 0, sizeof(buffer));

        n = receive_line(client_sock, buffer, sizeof(buffer));

        if (n < 0) {
            printf("Server disconnected.\n");
            goto cleanup;
        }

        printf("%s\n", buffer);

        if (strcmp(buffer, "Enter your choice (1-5): ") == 0) {
            break;
        }
    }

    // Keep asking until the server accepts the selection.
    while (1) {

        // Ask the player for a fighting style.
        printf("[you] > ");

        if (fgets(choice, sizeof(choice), stdin) == NULL) {
            goto cleanup;
        }

        // Send the selection to the server.
        n = send(client_sock, choice, strlen(choice), 0);

        if (n < 0) {
            printf("Error sending selection.\n");
            goto cleanup;
        }

        // Receive the server's response.
        memset(buffer, 0, sizeof(buffer));

        n = receive_line(client_sock, buffer, sizeof(buffer));

        if (n < 0) {
            printf("Server disconnected.\n");
            goto cleanup;
        }

        // If the selection is invalid, display the menu again.
        if (strcmp(buffer, "STYLE_INVALID") == 0) {

            clear_screen();

            while (1) {
                memset(buffer, 0, sizeof(buffer));

                n = receive_line(client_sock, buffer, sizeof(buffer));

                if (n < 0) {
                    printf("Server disconnected.\n");
                    goto cleanup;
                }

                printf("%s\n", buffer);

                if (strcmp(buffer, "Enter your choice (1-5): ") == 0) {
                    break;
                }
            }

            continue;
        }

        // If the selection is accepted, proceed.
        if (strcmp(buffer, "STYLE_ACCEPTED") == 0) {
            break;
        }
    }

    // Clear the terminal after choosing a fighting style.
    clear_screen();

    printf("===== FIGHTING STYLE SELECTION =====\n");
    printf("Waiting for the other player to choose...\n");

    // Receive the selection confirmation.
    while (1) {
        memset(buffer, 0, sizeof(buffer));

        n = receive_line(client_sock, buffer, sizeof(buffer));

        if (n < 0) {
            printf("Server disconnected.\n");
            goto cleanup;
        }

        printf("%s\n", buffer);

        if (strcmp(buffer,
                   "Both players have selected their fighting styles!") == 0) {
            break;
        }
    }

    // Receive and display the player's hand.
    printf("\nReceiving your cards...\n");

    while (1) {
        memset(buffer, 0, sizeof(buffer));

        n = receive_line(client_sock, buffer, sizeof(buffer));

        if (n < 0) {
            printf("Server disconnected.\n");
            goto cleanup;
        }

        if (strcmp(buffer, "END OF HAND") == 0) {
            break;
        }

        printf("%s\n", buffer);
    }

    // Begin the card-selection phase.
    printf("\n===== CARD SELECTION =====\n");

    while (1) {

        printf("\nEnter a card number to select it, or READY to finish.\n");
        printf("[you] > ");

        if (fgets(choice, sizeof(choice), stdin) == NULL) {
            goto cleanup;
        }

        // Remove the newline from the user's input.
        choice[strcspn(choice, "\n")] = '\0';

        // Prepare the command to send to the server.
        if (strcmp(choice, "READY") == 0) {
            snprintf(request, sizeof(request), "READY\n");
        } else {
            snprintf(request, sizeof(request), "PICK %s\n", choice);
        }

        // Send the command to the server.
        n = send(client_sock, request, strlen(request), 0);

        if (n < 0) {
            printf("Error sending command.\n");
            goto cleanup;
        }

        // Receive the server's response.
        memset(buffer, 0, sizeof(buffer));

        n = receive_line(client_sock, buffer, sizeof(buffer));

        if (n < 0) {
            printf("Server disconnected.\n");
            goto cleanup;
        }

        // Clear the previous display after receiving the response.
        clear_screen();

        printf("===== CARD SELECTION =====\n");
        printf("\n[server] > %s\n", buffer);

        // Refresh the displayed hand after a successful selection.
        if (strncmp(buffer, "Selected:", 9) == 0) {

            printf("\n===== UPDATED HAND =====\n");

            while (1) {
                memset(buffer, 0, sizeof(buffer));

                n = receive_line(client_sock, buffer, sizeof(buffer));

                if (n < 0) {
                    printf("Server disconnected.\n");
                    goto cleanup;
                }

                if (strcmp(buffer, "END OF HAND") == 0) {
                    break;
                }

                printf("%s\n", buffer);
            }
        }

        // If the player is READY, wait for the opponent.
        if (strcmp(choice, "READY") == 0) {

            printf("\nYou are READY!\n");
            printf("Waiting for the other player...\n");

            while (1) {
                memset(buffer, 0, sizeof(buffer));

                n = receive_line(client_sock, buffer, sizeof(buffer));

                if (n < 0) {
                    printf("Server disconnected.\n");
                    goto cleanup;
                }

                if (strcmp(buffer, "BOTH_READY") == 0) {
                    break;
                }

                printf("[server] > %s\n", buffer);
            }

            // Both players are ready. Wait for combat updates.
            clear_screen();

            printf("====================================\n");
            printf("             COMBAT\n");
            printf("====================================\n");

            while (1) {
                n = receive_line(client_sock, buffer, sizeof(buffer));

                if (n < 0) {
                    printf("Server disconnected.\n");
                    goto cleanup;
                }

                if (strcmp(buffer, "ROUND_COMPLETE") == 0) {
                    break;
                }

                printf("%s\n", buffer);
            }

            // Wait for the next round or encounter result.
            while (1) {
                memset(buffer, 0, sizeof(buffer));

                n = receive_line(client_sock, buffer, sizeof(buffer));

                if (n < 0) {
                    printf("Server disconnected.\n");
                    goto cleanup;
                }

                if (strcmp(buffer, "NEXT_ROUND") == 0) {
                    break;
                }

                printf("%s\n", buffer);
            }


            while (1) {
                memset(buffer, 0, sizeof(buffer));

                n = receive_line(client_sock, buffer, sizeof(buffer));

                if (n < 0) {
                    printf("Server disconnected.\n");
                    goto cleanup;
                }

                if (strcmp(buffer, "END OF HAND") == 0) {
                    break;
                }

                printf("%s\n", buffer);
            }

            printf("\n===== CARD SELECTION =====\n");
        }
    }

    printf("\nCard selection completed.\n");

cleanup:

    // Close the client socket.
    close(client_sock);

    printf("\nConnection closed.\n");

    return 0;
}

