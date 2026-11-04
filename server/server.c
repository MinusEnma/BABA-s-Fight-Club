
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include "../player/player.h"
#include "../dice.h"

// Display error message and terminate program.
void die_with_error(char *error_msg) {
    printf("%s\n", error_msg);
    exit(-1);
}

// Send message to client.
void send_message(int client_sock, const char *message) {
    if (send(client_sock, message, strlen(message), 0) < 0) {
        die_with_error("Error: send() Failed.");
    }
}

// Receive one line of text from client.
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

// Convert selection number into fighting style.
const char *get_style_name(int selection) {
    switch (selection) {
        case 1: return "Boxing";
        case 2: return "Muay Thai";
        case 3: return "Taekwondo";
        case 4: return "Brazilian Jiu-Jitsu";
        case 5: return "Karate";
        default: return "Unknown";
    }
}

// Send a player's hand to their respective client.
void send_player_hand(int client_sock, Player *player) {
    char message[256];

    send_message(client_sock, "===== YOUR HAND =====\n");

    for (int i = 0; i < player->hand_count; i++) {

        // Retrieve card information using its ID.
        const Card *card = card_get(player->hand[i]);

        printf("Hand index: %d | Card ID: %d | Card: %s\n",
            i, player->hand[i],
            card == NULL ? "NULL" : card->name);

        if (card == NULL) {
            continue;
        }

        snprintf(message, sizeof(message),
                 "%d. %s | %s | Energy: %d\n",
                 i + 1,
                 card->name,
                 card->description,
                 card->energy_cost);

        send_message(client_sock, message);
    }

    send_message(client_sock, "END OF HAND\n");
}

int main(int argc, char *argv[]) {

    dice_seed(); // Initialize random number generator for deck shuffling

    int server_sock, client_sock1, client_sock2, port_no, n;
    socklen_t client_size;

    char buffer[256];

    struct sockaddr_in server_addr, client_addr;

    if (argc < 2) {
        printf("Usage: %s port_no\n", argv[0]);
        exit(1);
    }

    printf("Server starting ...\n");

    // Create TCP socket.
    server_sock = socket(AF_INET, SOCK_STREAM, 0);

    if (server_sock < 0)
        die_with_error("Error: socket() Failed.");

    // Allow the server to reuse its port after restarting.
    int reuse = 1;

    setsockopt(server_sock, SOL_SOCKET, SO_REUSEADDR,
               &reuse, sizeof(reuse));

    // Prepare the server's address information.
    memset(&server_addr, 0, sizeof(server_addr));

    port_no = atoi(argv[1]);

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port_no);

    // Bind socket to specified port.
    if (bind(server_sock, (struct sockaddr *) &server_addr,
             sizeof(server_addr)) < 0)
        die_with_error("Error: bind() Failed.");

    // Make socket listen for incoming connections.
    if (listen(server_sock, 5) < 0)
        die_with_error("Error: listen() Failed.");

    printf("Server listening to port %d ...\n", port_no);

    // Wait for Player 1 to connect.
    printf("Waiting for Player 1 ...\n");

    client_size = sizeof(client_addr);

    client_sock1 = accept(server_sock,
                          (struct sockaddr *) &client_addr,
                          &client_size);

    if (client_sock1 < 0)
        die_with_error("Error: accept() Failed.");

    printf("Player 1 successfully connected!\n");

    // Wait for Player 2 to connect.
    printf("Waiting for Player 2 ...\n");

    client_size = sizeof(client_addr);

    client_sock2 = accept(server_sock,
                          (struct sockaddr *) &client_addr,
                          &client_size);

    if (client_sock2 < 0)
        die_with_error("Error: accept() Failed.");

    printf("Player 2 successfully connected!\n");

    // Assign player numbers.
    send_message(client_sock1, "You are Player 1.\n");
    send_message(client_sock2, "You are Player 2.\n");

    printf("\nBoth players are connected!\n");

    // Send fighting style menu to both players.
    const char *style_menu =
        "===== SELECT YOUR FIGHTING STYLE =====\n"
        "1. Boxing\n"
        "2. Muay Thai\n"
        "3. Taekwondo\n"
        "4. Brazilian Jiu-Jitsu\n"
        "5. Karate\n"
        "Enter your choice (1-5): \n";

    send_message(client_sock1, style_menu);
    send_message(client_sock2, style_menu);

    int player1_choice = 0;
    int player2_choice = 0;

    // Receive Player 1's selection.
    printf("\nWaiting for Player 1's fighting style...\n");

    while (1) {
        memset(buffer, 0, sizeof(buffer));

        n = receive_line(client_sock1, buffer, sizeof(buffer));

        if (n <= 0) {
            printf("Player 1 disconnected.\n");
            goto cleanup;
        }

        player1_choice = atoi(buffer);

        if (player1_choice >= 1 && player1_choice <= 5) {
            break;
        }

        send_message(client_sock1,
                     "Invalid selection. Please enter a number from 1 to 5: \n");
    }

    printf("Player 1 selected: %s\n",
           get_style_name(player1_choice));

    // Receive Player 2's selection.
    printf("Waiting for Player 2's fighting style...\n");

    while (1) {
        memset(buffer, 0, sizeof(buffer));

        n = receive_line(client_sock2, buffer, sizeof(buffer));

        if (n <= 0) {
            printf("Player 2 disconnected.\n");
            goto cleanup;
        }

        player2_choice = atoi(buffer);

        if (player2_choice >= 1 && player2_choice <= 5) {
            break;
        }

        send_message(client_sock2,
                     "Invalid selection. Please enter a number from 1 to 5: \n");
    }

    printf("Player 2 selected: %s\n",
           get_style_name(player2_choice));

    // Convert player choices into FightingStyle enum values.
    FightingStyle player1_style = (FightingStyle)(player1_choice - 1);
    FightingStyle player2_style = (FightingStyle)(player2_choice - 1);

    // Create both players using their selected fighting styles.
    Player player1;
    Player player2;

    player_create(&player1, "Player 1", player1_style);
    player_create(&player2, "Player 2", player2_style);

    // Build and shuffle each player's starter deck.
    player_build_deck(&player1);
    player_build_deck(&player2);

    // Draw the initial hand for both players.
    player_draw_hand(&player1);
    player_draw_hand(&player2);

    // Notify both players that selections are complete.
    char result[256];

    snprintf(result, sizeof(result),
             "===== FIGHTING STYLES CONFIRMED =====\n"
             "Player 1: %s\n"
             "Player 2: %s\n"
             "Both players have selected their fighting styles!\n",
             get_style_name(player1_choice),
             get_style_name(player2_choice));

    send_message(client_sock1, result);
    send_message(client_sock2, result);

    // Send each player's hand to their respective client.
    send_player_hand(client_sock1, &player1);
    send_player_hand(client_sock2, &player2);

    printf("\nBoth players have selected their fighting styles!\n");

    printf("\n===== PLAYER DECK INITIALIZATION =====\n");
    printf("Player 1 deck size: %d\n", player1.deck_count);
    printf("Player 2 deck size: %d\n", player2.deck_count);

    printf("\n===== INITIAL HAND =====\n");
    printf("Player 1 hand: %d cards\n", player1.hand_count);
    printf("Player 2 hand: %d cards\n", player2.hand_count);

cleanup:

    // Close both client connections.
    printf("\nBoth players have selected their fighting styles!\n");

    close(client_sock1);
    close(client_sock2);

    // Close the server's listening socket.
    close(server_sock);

    printf("\nServer closed.\n");

    return 0;
}

