#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include <ctype.h>  // Include this header for tolower function

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

#define MAP_HEIGHT 11
#define MAP_WIDTH 31
#define MAX_CHESTS 4
#define MAX_ITEMS 50

char game_map[MAP_HEIGHT][MAP_WIDTH + 1] = {
    "_____ _____ _____ _____       ",
    "|   | |   | |   | |   |       ",
    "|   | |   | |   | |   |       ",
    "|   | |   | |   | |   |  _____",
    "|   | |   | |   | |   |  |   |",
    "|   |_|   |_|   |_|   |__|   |",
    "|                            |",
    "|                            |",
    "|________________________    |",
    "                         |   |",
    "                         |___|"
};

int player_pos[2] = {7, 2};
int chests[MAX_CHESTS][2] = {{2, 2}, {2, 8}, {2, 14}, {2, 20}};
const char* chest_items[MAX_CHESTS] = {"handle", "trigger and chamber", "shotgun barrel", "bullets"};
char items_collected[MAX_ITEMS][50];
int collected_items_count = 0;
int end_game_position[2] = {7, 27};
bool collected_chests[MAX_CHESTS] = {false};

const char* color_reset = "\033[0m";
const char* color_red = "\033[31m";
const char* color_yellow = "\033[33m";
const char* color_gray = "\033[90m";
const char* color_wall = "\033[38;5;8m";

void clear_screen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

bool positions_equal(int pos1[2], int pos2[2]) {
    return pos1[0] == pos2[0] && pos1[1] == pos2[1];
}

void render_map() {
    clear_screen();
    for (int r = 0; r < MAP_HEIGHT; r++) {
        for (int c = 0; c < MAP_WIDTH; c++) {
            if (r == player_pos[0] && c == player_pos[1]) {
                printf("%si%s", color_red, color_reset);
            } else if (r == end_game_position[0] && c == end_game_position[1]) {
                printf("%so%s", color_yellow, color_reset);
            } else {
                bool is_chest = false;
                for (int i = 0; i < MAX_CHESTS; i++) {
                    if (r == chests[i][0] && c == chests[i][1]) {
                        if (collected_chests[i]) {
                            printf("%sx%s", color_yellow, color_reset);
                        } else {
                            printf("%so%s", color_yellow, color_reset);
                        }
                        is_chest = true;
                        break;
                    }
                }
                if (!is_chest) {
                    printf("%c", game_map[r][c]);
                }
            }
        }
        printf("\n");
    }
    
    printf("Items collected: ");
    if (collected_items_count == 0) {
        printf("None\n\n");
    } else {
        for (int i = 0; i < collected_items_count; i++) {
            printf("%s%s", items_collected[i], (i < collected_items_count - 1) ? ", " : "\n\n");
        }
    }
}

void sleep_ms(int milliseconds) {
    #ifdef _WIN32
        Sleep(milliseconds);
    #else
        usleep(milliseconds * 1000);
    #endif
}

int main() {
    char move;
    int new_pos[2];
    
    while (1) {
        render_map();
        printf("Controls: W = Up, A = Left, S = Down, D = Right, Q = Quit\n");
        printf("Enter your move: ");
        scanf(" %c", &move);
        move = tolower(move);

        if (move == 'q') {
            printf("%sThanks for playing!%s\n", color_gray, color_reset);
            break;
        }

        new_pos[0] = player_pos[0];
        new_pos[1] = player_pos[1];

        switch (move) {
            case 'w': new_pos[0]--; break;
            case 's': new_pos[0]++; break;
            case 'a': new_pos[1]--; break;
            case 'd': new_pos[1]++; break;
            default:
                printf("Invalid move. Try again!\n");
                sleep_ms(1000);
                continue;
        }

        if (new_pos[0] < 0 || new_pos[0] >= MAP_HEIGHT || new_pos[1] < 0 || new_pos[1] >= MAP_WIDTH) {
            printf("%sYou can't move there!%s\n", color_gray, color_reset);
            sleep_ms(1000);
            continue;
        }

        if (game_map[new_pos[0]][new_pos[1]] == '|' || game_map[new_pos[0]][new_pos[1]] == '_') {
            printf("%sThere's a wall there!%s\n", color_wall, color_reset);
            sleep_ms(1000);
            continue;
        }

        player_pos[0] = new_pos[0];
        player_pos[1] = new_pos[1];

        for (int i = 0; i < MAX_CHESTS; i++) {
            if (positions_equal(player_pos, chests[i]) && !collected_chests[i]) {
                strcpy(items_collected[collected_items_count++], chest_items[i]);
                collected_chests[i] = true;
                printf("%sYou opened a chest and found a %s!%s\n", color_yellow, chest_items[i], color_reset);
                sleep_ms(1000);
                break;
            }
        }

        if (positions_equal(player_pos, end_game_position) && collected_items_count == MAX_CHESTS) {
            const char* dots[] = {".", "..", "..."};
            for (int i = 0; i < 2; i++) {
                for (int j = 0; j < 3; j++) {
                    clear_screen();
                    render_map();
                    printf("%s\n", dots[j]);
                    sleep_ms(1000);
                }
            }

            clear_screen();
            game_map[end_game_position[0]][end_game_position[1]] = 'x';
            render_map();
            printf("%s\nYou shot yourself%s\n", color_red, color_reset);
            break;
        }
    }

    return 0;
}
