#include "space_explorer.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

typedef struct {
    unsigned int *visited;
    unsigned int *previous_connections;
    unsigned int *useable_rands;
    int previous_num_connections;
    int visited_count;
    unsigned int closest_planet;
    double closest_distance;
    bool already_visited_this_planet;
    bool just_used_random;
    int previous_connections_count;
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
        state->previous_num_connections = num_connections;
        state->previous_connections = malloc(sizeof(int) * 300);
        memset(state->previous_connections, 0, sizeof(int) * 300);
        state->useable_rands = malloc(sizeof(int) * 300);
        memset(state->useable_rands, 0, sizeof(int) * 300);
        state->previous_connections_count = 0;
        state->closest_planet = 0;
        state->closest_distance = 10000.0;
        state->visited_count = 0;
    } else {
        state = ship_state;
        state->already_visited_this_planet = false;
    }

    unsigned int random_planet = RAND_PLANET;
    bool already_visited_random = false;

    // Check if I've already visited this planet
    for (int i = 0; i < state->visited_count; i++) {
        if (state->visited[i] == random_planet) {
            already_visited_random = true;
        }
        else if (state->visited[i] == crt_planet) {
            state->already_visited_this_planet = true;
        }
    }


    for (int i = 0; i < 300; i++) {
        if (state->useable_rands[i] == 0 && !already_visited_random) {
            state->useable_rands[i] = random_planet;

        } else if (state->useable_rands[i] == crt_planet && state->already_visited_this_planet) {
            state->useable_rands[i] = 0;
        }
    }


    // Add this planet to the visited array
    if (!state->already_visited_this_planet) {
        state->visited[state->visited_count] = crt_planet;
        state->visited_count++;
    }
    // Check if I've already visited any of the local connections
    int* local_connections_ive_been_to = malloc(sizeof(int) * num_connections);
    memset(local_connections_ive_been_to, 0, sizeof(int) * num_connections);
    for (int i = 0; i < num_connections; i++) {
        for (int j = 0; j < state->visited_count; j++) {
            if (state->visited[j] == connections[i]) {
                local_connections_ive_been_to[i] = 1;
                break;
            }
        }
    }

    // Check if I've already visited any of the previous connections
    int* previous_connections_ive_been_to = malloc(sizeof(int) * state -> previous_num_connections);
    memset(previous_connections_ive_been_to, 0, sizeof(int) * state -> previous_num_connections);
    int num_of_previous_connections_visited = 0;
    for (int i = 0; i < state -> previous_num_connections; i++) {
        for (int j = 0; j < state->visited_count; j++) {
            if (state->visited[j] == state->previous_connections[i]) {
                previous_connections_ive_been_to[i] = 1;
                num_of_previous_connections_visited++;
                break;
            }
        }
    }


    ShipAction action;
    printf("Current planet: %u Visited: %d\n", crt_planet, state->already_visited_this_planet);
    printf("Connections: %d\n", num_connections);
    for (int i = 0; i < num_connections; i++) {
        printf("Connection %d: %u Visited: %d\n", i, connections[i], local_connections_ive_been_to[i]);
    }
    printf("Distance from mixer: %f\n", distance_from_mixer);
    printf("closest_planet: %u\n", state->closest_planet);
    printf("closest_distance: %f\n", state->closest_distance);
    printf("previous_num_connections: %d\n", state->previous_num_connections);
    printf("num_of_previous_connections_visited: %d\n", num_of_previous_connections_visited);



    if(distance_from_mixer <= state->closest_distance) {

        state->closest_planet = crt_planet;
        state->closest_distance = distance_from_mixer;
        state->previous_num_connections = num_connections;
        memset(state->previous_connections, 0, sizeof(int) * 300);
        for (int i = 0; i < num_connections; i++) {
            state->previous_connections[i] = connections[i];
        }


        action.ship_state = state;
        for (int i = 0; i < num_connections; i++) {
            if (local_connections_ive_been_to[i] == 0) {
                action.next_planet = connections[i];
                state->just_used_random = false;
                break;
            }
        }


    } else if (num_of_previous_connections_visited == state->previous_num_connections) {
        action.ship_state = state;
        for (int i = 0; i < 300; i++) {
            if (state->useable_rands[i] != 0) {
                action.next_planet = state->useable_rands[i];
                break;
            } else{
                action.next_planet = RAND_PLANET;
            }
        }
        printf("random planet used\n");
        state->just_used_random = true;

    } else if (distance_from_mixer > state->closest_distance && state->just_used_random) {
        action.ship_state = state;
        for (int i = 0; i < 300; i++) {
            if (state->useable_rands[i] != 0) {
                action.next_planet = state->useable_rands[i];
                break;
            } else{
                action.next_planet = RAND_PLANET;
            }
        }
        printf("random planet used\n");

    } else {
        action.ship_state = state;
        for (int i = 0; i < state->previous_num_connections; i++) {
            if (previous_connections_ive_been_to[i] == 0) {
                action.next_planet = state->previous_connections[i];
                break;
            }
        }
    }

    return action;
}
