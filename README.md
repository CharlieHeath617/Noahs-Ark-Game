# Noah's Ark Inventory (CIS 142)

Build: `g++ main.cpp Animal.cpp -o ark` then run `./ark`

## Core features
- 6 species (cat, dog, goldfish, shark, eagle, parakeet), 1 male + 1 female each = 12 animals, all 0 months old
- Menu: pass a month / check inventory / quit
- Births: mammals every 12 months, fish every 6, birds every 9 (females only)
- Ark capacity is 50; no births when full
- Animals can die (0.5% chance per animal per month)
- Game over if a species goes extinct or has no male or no female left
- Win by reaching month 36 with all species alive and able to breed

## Added features (for extra credit)
- Save / load game (menu option 4, also offered on quit; file `ark_save.txt`)
- Offer to load a saved game at startup
- Event messages for each birth and death
- Species totals in the inventory
- Input validation for the menu
