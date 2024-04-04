#include "space_explorer.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct {
    int *visited;
    int connection_search;
    unsigned int really_close;
}ShipState;


ShipAction space_hop(unsigned int crt_planet,
                   unsigned int *connections,
                   int num_connections,
                   double distance_from_mixer,
                   void *ship_state)
{
    ShipState *state;

    if (ship_state == NULL) {
        state = malloc(sizeof(ShipState));
        state->visited = malloc(sizeof(int) * num_connections);
        memset(state->visited, 0, sizeof(int) * num_connections);
        state->connection_search = -1;
        state->really_close = 0;
    } else {
        state = ship_state;
    }

    if (distance_from_mixer <= 0.0 && state->really_close == 0) {
        state->really_close = crt_planet;
    }

    ShipAction action;
    printf("Current planet: %u\n", crt_planet);
    printf("Connections: %d\n", num_connections);
    for (int i = 0; i < num_connections; i++) {
        printf("Connection %d: %u\n", i, connections[i]);
    }
    printf("Distance from mixer: %f\n", distance_from_mixer);
    printf("really_close: %u\n", state->really_close);

    if (distance_from_mixer > 0.0 && state->really_close == 0) {
        action.ship_state = state;
        action.next_planet = RAND_PLANET;
    } else if (distance_from_mixer > 0.0 && state->really_close != 0) {
        action.ship_state = state;
        action.next_planet = state->really_close;

    } else if (distance_from_mixer <= 0.0 && state->really_close != 0 && state->really_close != crt_planet) {
        action.ship_state = state;
        action.next_planet = state->really_close;
    } else {
        state->connection_search++;
        printf("connection_search: %d\n", state->connection_search);
        action.ship_state = state;
        action.next_planet = connections[state->connection_search];
    }





    return action;
}
