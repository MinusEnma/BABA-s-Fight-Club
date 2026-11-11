#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include "../player/player.h"
#include "../dice/dice.h"

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

// Send a combat message to both players.
void broadcast_message(
    int client_sock1,
    int client_sock2,
    const char *message
) {
    printf("%s", message);

    send_message(client_sock1, message);
    send_message(client_sock2, message);
}

// Receive one line of text from client.
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

// Clear the terminal screen.
void clear_screen(void) {
    printf("\033[2J\033[H");
    fflush(stdout);
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

    char energy_message[64];

    snprintf(energy_message, sizeof(energy_message),
             "Available Energy: %d\n", player->energy);

    send_message(client_sock, energy_message);

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

// Execute one selected action.
void execute_action(
    Player *attacker,
    Player *defender,
    int action_number,
    int client_sock1,
    int client_sock2
) {
    uint8_t card_id = attacker->combo[action_number];
    const Card *card = card_get(card_id);

    char message[256];

    if (card == NULL) {
        snprintf(message, sizeof(message),
                 "%s has an invalid card in their action queue.\n",
                 attacker->name);

        broadcast_message(client_sock1, client_sock2, message);
        return;
    }

    snprintf(message, sizeof(message),
             "%s executes: %s\n",
             attacker->name, card->name);

    broadcast_message(client_sock1, client_sock2, message);

    // Activate defensive cards.
    if (card->type == CARD_BLOCK || card->type == CARD_EVADE) {
        player_activate_defense(attacker, card);
    }

    // Process attack cards.
    else if (card->type == CARD_ATTACK) {

        int attack_roll = 0;

        // Only roll a d12 if the defender has active Evade.
        if (defender->evade > 0) {
            attack_roll = player_roll_card(attacker, card);
        }

        HitResult result = player_receive_attack(defender, card, attack_roll);

            if (result.attack_roll > 0) {
                snprintf(message, sizeof(message),
                    "Attack: %d(%d) = %d damage\n"
                    "%s\n"
                    "Damage dealt: %d\n"
                    "%s HP remaining: %d\n",
                    result.attack_base,
                    result.attack_roll,
                    result.attack_total,
                    result.evaded ? "Evade: Successful!" :
                    result.evade_broken ? "Evade: Broken!" : "",
                    result.hp_lost,
                    defender->name,
                    defender->hp);
            } else {
                snprintf(message, sizeof(message),
                    "Attack: %d = %d damage\n"
                    "Damage dealt: %d\n"
                    "%s HP remaining: %d\n",
                    result.attack_base,
                    result.attack_total,
                    result.hp_lost,
                    defender->name,
                    defender->hp);
            }

            broadcast_message(client_sock1, client_sock2, message);
    }
}

// Resolve both players' selected action sequences.
void resolve_combat(
    Player *player1,
    Player *player2,
    int client_sock1,
    int client_sock2
) {
    int p1_index = 0;
    int p2_index = 0;

    broadcast_message(client_sock1, client_sock2,
        "\n====================================\n"
        "           COMBAT BEGINS\n"
        "====================================\n");

    while (p1_index < player1->combo_count ||
           p2_index < player2->combo_count) {

        broadcast_message(client_sock1, client_sock2,
            "\n---------- NEW EXCHANGE ----------\n");

        // Check whether both players still have an action.
        int p1_has_action = p1_index < player1->combo_count;
        int p2_has_action = p2_index < player2->combo_count;

        // If only Player 1 has an action remaining.
        if (p1_has_action && !p2_has_action) {
            execute_action(
                player1, player2, p1_index,
                client_sock1, client_sock2);
            p1_index++;
            continue;
        }

        // If only Player 2 has an action remaining.
        if (!p1_has_action && p2_has_action) {
            execute_action(
                player2, player1, p2_index,
                client_sock1, client_sock2);
            p2_index++;
            continue;
        }

        // Retrieve both players' selected cards.
        const Card *p1_card = card_get(player1->combo[p1_index]);
        const Card *p2_card = card_get(player2->combo[p2_index]);

        // Check that both cards are valid.
        if (p1_card == NULL || p2_card == NULL) {
            broadcast_message(client_sock1, client_sock2,
                "Error: Invalid card detected.\n");

            p1_index++;
            p2_index++;
            continue;
        }

        // CASE 1: Attack vs. Attack
        if (p1_card->type == CARD_ATTACK &&
            p2_card->type == CARD_ATTACK) {

            int p1_roll;
            int p2_roll;

            do {
                p1_roll = player_roll_initiative(player1);
                p2_roll = player_roll_initiative(player2);

                char message[256];

                snprintf(message, sizeof(message),
                         "\n%s rolls: %d\n%s rolls: %d\n",
                         player1->name, p1_roll,
                         player2->name, p2_roll);

                broadcast_message(client_sock1, client_sock2, message);

                if (p1_roll == p2_roll) {
                    broadcast_message(client_sock1, client_sock2,
                        "It's a tie! Both players reroll.\n");
                }

            } while (p1_roll == p2_roll);

            // Higher initiative executes their attack.
            if (p1_roll > p2_roll) {
                char message[128];

                snprintf(message, sizeof(message),
                         "\n%s wins the speed contest!\n",
                         player1->name);

                broadcast_message(client_sock1, client_sock2, message);

                execute_action(
                    player1, player2, p1_index,
                    client_sock1, client_sock2);

            } else {
                char message[128];

                snprintf(message, sizeof(message),
                         "\n%s wins the speed contest!\n",
                         player2->name);

                broadcast_message(client_sock1, client_sock2, message);

                execute_action(
                    player2, player1, p2_index,
                    client_sock1, client_sock2);
            }
        }

        // CASE 2: Attack vs. Defense
        else if (p1_card->type == CARD_ATTACK ||
                 p2_card->type == CARD_ATTACK) {

            // Player 1 attacks, Player 2 defends.
            if (p1_card->type == CARD_ATTACK) {

                // Activate Player 2's defense first.
                execute_action(
                    player2, player1, p2_index,
                    client_sock1, client_sock2);

                // Resolve Player 1's attack afterward.
                execute_action(
                    player1, player2, p1_index,
                    client_sock1, client_sock2);

            } else {

                // Player 1 defends, Player 2 attacks.
                execute_action(
                    player1, player2, p1_index,
                    client_sock1, client_sock2);

                execute_action(
                    player2, player1, p2_index,
                    client_sock1, client_sock2);
            }
        }

        // CASE 3: Defense vs. Defense
        else {
            broadcast_message(client_sock1, client_sock2,
                "Both players selected defensive cards.\n"
                "Neither card takes effect. Both cards are discarded.\n");
        }

        // Both players' paired actions are consumed.
        p1_index++;
        p2_index++;
    }

    broadcast_message(client_sock1, client_sock2,
        "\n====================================\n"
        "           ROUND COMPLETE\n"
        "====================================\n");

    // Clear temporary defensive effects.
    player_end_turn(player1);
    player_end_turn(player2);

    // Move all queued cards into the discard piles.
    player_clear_queue(player1);
    player_clear_queue(player2);

    // Notify clients that combat updates have finished.
    broadcast_message(client_sock1, client_sock2,
                      "ROUND_COMPLETE\n");
}
// Process a player's card-selection command.
void handle_selection_command(
    int client_sock,
    Player *player,
    char *command,
    int *ready
) {
    char response[256];

    // Check if the player is ready.
    if (strcmp(command, "READY") == 0) {
        *ready = 1;

        send_message(client_sock,
                     "You are READY. Waiting for the other player...\n");
        return;
    }

    // Check if the command is a card selection.
    int card_number;
    char extra;

    if (sscanf(command, "PICK %d %c", &card_number, &extra) != 1) {
        send_message(client_sock,
                     "Invalid command. Use PICK <number> or READY.\n");
        return;
    }

    // Convert the player's displayed number into a hand index.
    int hand_index = card_number - 1;

    if (hand_index < 0 || hand_index >= player->hand_count) {
        send_message(client_sock, "Invalid card number.\n");
        return;
    }

    // Retrieve the selected card.
    const Card *card = card_get(player->hand[hand_index]);

    if (card == NULL) {
        send_message(client_sock, "Invalid card.\n");
        return;
    }

    // Check whether the player has enough Energy.
    if (card->energy_cost > player->energy) {
        send_message(client_sock, "Not enough Energy.\n");
        return;
    }

    // Queue the card and remove it from the player's hand.
    if (!player_queue_combo(player, hand_index)) {
        send_message(client_sock,
                     "Unable to select card. Your action queue may be full.\n");
        return;
    }

    snprintf(response, sizeof(response),
             "Selected: %s | Energy remaining: %d\n",
             card->name, player->energy);

    send_message(client_sock, response);

    // Send the updated hand after removing the selected card.
    send_player_hand(client_sock, player);
}

// Handle the card-selection phase for both players.
int selection_phase(
    int client_sock1,
    int client_sock2,
    Player *player1,
    Player *player2
) {
    int player1_ready = 0;
    int player2_ready = 0;
    int n;

    char command[256];

    printf("\n===== CARD SELECTION PHASE =====\n");

    while (!player1_ready || !player2_ready) {

        // Receive Player 1's command if they are not READY.
        if (!player1_ready) {

            n = receive_line(client_sock1, command, sizeof(command));

            if (n < 0) {
                printf("Player 1 disconnected.\n");
                return 0;
            }

            handle_selection_command(
                client_sock1, player1, command, &player1_ready
            );
        }

        // Receive Player 2's command if they are not READY.
        if (!player2_ready) {

            n = receive_line(client_sock2, command, sizeof(command));

            if (n < 0) {
                printf("Player 2 disconnected.\n");
                return 0;
            }

            handle_selection_command(
                client_sock2, player2, command, &player2_ready
            );
        }
    }

    printf("\nBoth players are READY!\n");

    send_message(client_sock1, "BOTH_READY\n");
    send_message(client_sock2, "BOTH_READY\n");

    return 1;
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

        if (n < 0) {
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

        if (n < 0) {
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

    // Begin the encounter's round system.
    int round_number = 1;
    int encounter_finished = 0;

    while (!encounter_finished) {

        printf("\n====================================\n");
        printf("             ROUND %d\n", round_number);
        printf("====================================\n");

        // Both players select their cards.
        if (!selection_phase(
                client_sock1, client_sock2,
                &player1, &player2)) {
            goto cleanup;
        }

        // Resolve the selected actions.
        resolve_combat(
            &player1,
            &player2,
            client_sock1,
            client_sock2
        );

        // Check whether a player has been defeated.
        if (player1.hp <= 0 || player2.hp <= 0) {
            encounter_finished = 1;
            break;
        }

        // Prepare both players for the next round.
        player_reset_round(&player1);
        player_reset_round(&player2);

        // Send the new hands to their respective clients.
        send_message(client_sock1, "NEXT_ROUND\n");
        send_message(client_sock2, "NEXT_ROUND\n");

        send_player_hand(client_sock1, &player1);
        send_player_hand(client_sock2, &player2);

        round_number++;
    }

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

