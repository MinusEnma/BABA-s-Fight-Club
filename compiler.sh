mkdir -p build
gcc dice/dice.c player/player.c card/cardDatabase.c server/server.c fightingStyle/fightingStyle.c -o build/server
gcc dice/dice.c player/player.c card/cardDatabase.c server/client.c fightingStyle/fightingStyle.c -o build/client
