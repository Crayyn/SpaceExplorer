#include "space_explorer.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

typedef struct {
    unsigned int *visited;
    int visited_count;
    unsigned int previous_planet;
    double previous_distance;
    bool already_visited_this_planet;
    bool just_used_random;
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
        state->visited = malloc(sizeof(int) * 300);
        memset(state->visited, 0, sizeof(int) * 300);
        state->previous_planet = 0;
        state->previous_distance = 10000.0;
        state->visited_count = 0;
    } else {
        state = ship_state;
        state->already_visited_this_planet = false;
    }



    for (int i = 0; i < state->visited_count; i++) {
        if (state->visited[i] == crt_planet) {
            state->already_visited_this_planet = true;
            break;
        }
    }

    if (!state->already_visited_this_planet) {
        state->visited[state->visited_count] = crt_planet;
        state->visited_count++;
    }

    int* connections_ive_been_to = malloc(sizeof(int) * num_connections);
    memset(connections_ive_been_to, 0, sizeof(int) * num_connections);
    for (int i = 0; i < num_connections; i++) {
        for (int j = 0; j < state->visited_count; j++) {
            if (state->visited[j] == connections[i]) {
                connections_ive_been_to[i] = 1;
                break;
            }
        }
    }


    ShipAction action;
    printf("Current planet: %u Visited: %d\n", crt_planet, state->already_visited_this_planet);
    printf("Connections: %d\n", num_connections);
    for (int i = 0; i < num_connections; i++) {
        printf("Connection %d: %u Visited: %d\n", i, connections[i], connections_ive_been_to[i]);
    }
    printf("Distance from mixer: %f\n", distance_from_mixer);
    printf("previous_planet: %u\n", state->previous_planet);

    int num_of_connections_visited = 0;
    if(distance_from_mixer <= state->previous_distance) {
        if (distance_from_mixer != state->previous_distance) {
            state->previous_planet = crt_planet;
            state->previous_distance = distance_from_mixer;
        }

        action.ship_state = state;
        for (int i = 0; i < num_connections; i++) {
            if (connections_ive_been_to[i] == 0) {
                action.next_planet = connections[i];
                state->just_used_random = false;
                break;
            } else {
                num_of_connections_visited++;
            }
        }
        if (num_of_connections_visited == num_connections) {
            action.next_planet = RAND_PLANET;
            printf("random planet used\n");
            state->just_used_random = true;

        }

    } else if (distance_from_mixer > state->previous_distance && state->just_used_random) {
        action.ship_state = state;
        action.next_planet = RAND_PLANET;
        printf("random planet used\n");

    } else {
        action.ship_state = state;
        action.next_planet = state->previous_planet;
    }

    return action;
}
