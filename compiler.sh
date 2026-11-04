mkdir -p build
gcc dice.c player/player.c card/cardDatabase.c server/server.c -o build/server
gcc dice.c player/player.c card/cardDatabase.c server/client.c -o build/client
