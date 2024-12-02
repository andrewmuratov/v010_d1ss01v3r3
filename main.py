import time
import random
import os

game_map = [
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
]

player_pos = [7, 2]
chests = [[2, 2], [2, 8], [2, 14], [2, 20]]
chest_items = ["handle", "trigger and chamber", "shotgun barrel", "bullets"]
items_collected = []
end_game_position = [7, 27]

color_reset = "\033[0m"
color_red = "\033[31m"
color_yellow = "\033[33m"
color_gray = "\033[90m"
color_wall = "\033[38;5;8m"

def clear_screen():
    os.system('cls' if os.name == 'nt' else 'clear')

def render_map():
    clear_screen()
    for r, row in enumerate(game_map):
        row_str = list(row)
        if r == player_pos[0]:
            row_str[player_pos[1]] = f"{color_red}i{color_reset}"
        for idx, chest in enumerate(chests):
            if r == chest[0] and chest not in collected_chests and (player_pos != chest if player_pos else True):
                row_str[chest[1]] = f"{color_yellow}o{color_reset}"
            elif r == chest[0] and chest in collected_chests and (player_pos != chest if player_pos else True):
                row_str[chest[1]] = f"{color_yellow}x{color_reset}"
        if r == end_game_position[0]:
            if player_pos != end_game_position:
                row_str[end_game_position[1]] = f"{color_yellow}o{color_reset}"
            else:
                row_str[end_game_position[1]] = f"{color_red}i{color_reset}"
        print("".join(row_str))
    print(f"Items collected: {', '.join(items_collected) if items_collected else 'None'}\n")

collected_chests = []

while True:
    render_map()
    print("Controls: W = Up, A = Left, S = Down, D = Right, Q = Quit")
    move = input("Enter your move: ").lower()

    if move == 'q':
        print(f"{color_gray}Thanks for playing!{color_reset}")
        break

    new_pos = player_pos[:]
    if move == 'w':
        new_pos[0] -= 1
    elif move == 's':
        new_pos[0] += 1
    elif move == 'a':
        new_pos[1] -= 1
    elif move == 'd':
        new_pos[1] += 1
    else:
        print("Invalid move. Try again!")
        time.sleep(1)
        continue

    if new_pos[0] < 0 or new_pos[0] >= len(game_map) or new_pos[1] < 0 or new_pos[1] >= len(game_map[0]):
        print(f"{color_gray}You can't move there!{color_reset}")
        time.sleep(1)
        continue

    if game_map[new_pos[0]][new_pos[1]] in ['|', '_']:
        print(f"{color_wall}There's a wall there!{color_reset}")
        time.sleep(1)
        continue

    player_pos = new_pos

    if player_pos in chests and player_pos not in collected_chests:
        chest_index = chests.index(player_pos)
        item = chest_items[chest_index]
        items_collected.append(item)
        collected_chests.append(player_pos)
        print(f"{color_yellow}You opened a chest and found a {item}!{color_reset}")
        time.sleep(1)

    if player_pos == end_game_position and len(items_collected) == len(chest_items):
        for _ in range(2):
            for dots in [".", "..", "..."]:
                clear_screen()
                render_map()
                print(dots)
                time.sleep(1)

        clear_screen()
        game_map[end_game_position[0]] = (
            game_map[end_game_position[0]][:end_game_position[1]]
            + f"x"
            + game_map[end_game_position[0]][end_game_position[1] + 1:]
        )
        render_map()
        print(f"{color_red}\nYou shot yourself{color_reset}")
        break
