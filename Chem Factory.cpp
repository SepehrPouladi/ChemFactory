#include <iostream>
#include <string>
#include <cstring>
#include <sstream>
#include <cstdlib>
#include <ctime>

using namespace std;

// Constants
const int MAX_NAME = 30;
const int MAX_ELEMENTS = 10;
const int MAX_COMPOUNDS = 10;
const int ELEMENTS_PER_COMPOUND = 2;
const int XP_PER_LEVEL = 100;
const int MAX_LEVEL = 14;
const int WIN_MONEY = 1000;
const int MAX_DAY = 35;
const int MAX_COMPOUNDS_PER_DAY = 2;

int lastLevel = 1;
int compoundsMadeToday = 0;

struct Element {
    char name[MAX_NAME];
    int levelRequired;
};

struct Compound {
    char name[MAX_NAME];
    char ingredients[ELEMENTS_PER_COMPOUND][MAX_NAME];
    int levelRequired;
    int xpReward;
    int moneyReward;
    bool discovered;
    bool owned;
    int quantity;
};

struct Player {
    char name[MAX_NAME];
    int level;
    int xp;
    int money;
    int day;
    char elementsOwned[MAX_ELEMENTS][MAX_NAME];
    int elementCount;
    bool xpEventTriggered; // Tracks if XP >= 50 event has triggered in current level
    int insuranceCount;    // Accident Insurance (one-time use)
    int analysisCount;     // Technical Analysis (one-time use)
    int vaccineCount;      // Virus Vaccine (one-time use)
    bool hasMemoryChip;    // Memory Chip (permanent)
    bool hasSpaceGun;      // Advanced Space Gun (permanent)
};

struct ShopItem {
    char name[MAX_NAME];
    int price;
    bool isElement;
    bool isPermanent; // Indicates permanent items
    int levelRequired;
};

const int SHOP_ITEMS_COUNT = 14;

ShopItem shopItems[SHOP_ITEMS_COUNT] = {
    {"Hydrogen", 7, true, false, 1},
    {"Sodium", 17, true, false, 2},
    {"Chlorine", 17, true, false, 2},
    {"Carbon", 25, true, false, 3},
    {"Nitrogen", 30, true, false, 3},
    {"Calcium", 40, true, false, 4},
    {"Magnesium", 40, true, false, 4},
    {"Phosphorus", 50, true, false, 5},
    {"Iron", 50, true, false, 5},
    
    
    {"Accident Insurance", 30, false, false, 1},
    {"Technical Analysis", 30, false, false, 1},
    {"Virus Vaccine", 30, false, false, 1},
    {"Memory Chip", 100, false, true, 1},
    {"Advanced Space Gun", 120, false, true, 2}
};

Element elements[MAX_ELEMENTS] = {
    {"Hydrogen", 1},
    {"Oxygen", 1},
    {"Sodium", 2},
    {"Chlorine", 2},
    {"Carbon", 3},
    {"Nitrogen", 3},
    {"Calcium", 4},
    {"Magnesium", 4},
    {"Phosphorus", 5},
    {"Iron", 5}
};

Compound compounds[MAX_COMPOUNDS] = {
    {"Water", {"Hydrogen", "Oxygen"}, 1, 40, 50, false, false, 0},
    {"Salt", {"Sodium", "Chlorine"}, 2, 45, 60, false, false, 0},
    {"Sodium Hydroxide", {"Sodium", "Oxygen"}, 2, 48, 70, false, false, 0},
    {"Carbon Dioxide", {"Carbon", "Oxygen"}, 3, 50, 75, false, false, 0},
    {"Ammonia", {"Nitrogen", "Hydrogen"}, 3, 55, 80, false, false, 0},
    {"Calcium Carbonate", {"Calcium", "Carbon"}, 4, 57, 90, false, false, 0},
    {"Magnesium Chloride", {"Magnesium", "Chlorine"}, 4, 58, 95, false, false, 0},
    {"Calcium Hydroxide", {"Calcium", "Oxygen"}, 4, 59, 100, false, false, 0},
    {"Phosphoric Acid", {"Phosphorus", "Oxygen"}, 5, 60, 110, false, false, 0},
    {"Iron Oxide", {"Iron", "Oxygen"}, 5, 65, 120, false, false, 0}

};

int compoundCount = 10;
int elementCount = 10;

Player player;

// Check if all compounds are discovered
bool allCompoundsDiscovered() {
    for (int i = 0; i < compoundCount; ++i) {
        if (!compounds[i].discovered) {
            return false;
        }
    }
    return true;
}

// Display the compound guide
void showCompoundGuide() {
    cout << "========== Compound Guide ==========\n";
    cout << "List of all compounds to discover (" << compoundCount << " total):\n";
    for (int i = 0; i < compoundCount; ++i) {
        Compound& c = compounds[i];
        cout << (i + 1) << ". ";
        if (c.discovered) {
            cout << c.name << " [X]";
        } else {
            cout << "(Hint: " << c.ingredients[0] << ", " << c.ingredients[1] << ")";
        }
        cout << " | Level Required: " << c.levelRequired;
        cout << " | Status: " << (c.discovered ? "Discovered" : "Not Discovered");
        cout << "\n";
    }
    cout << "====================================\n";
}

// Check if game has ended
bool checkGameEnd() {
    cout << "========== Checking Game Status ==========\n";
    bool allCompoundsDone = allCompoundsDiscovered();
    
    if (player.money >= WIN_MONEY && player.day <= MAX_DAY && allCompoundsDone) {
        cout << "\n Final Status:\n";
        cout << "Day: " << player.day << " | Level: " << player.level << " | Money: $" << player.money << "\n";
        cout << "Congratulations! You won the game by reaching $" << WIN_MONEY << " and discovering all compounds!\n";
        cout << "====================================\n";
        return true;
    }
    if (player.money <= 0) {
        cout << "\nGame Over! Final Status:\n";
        cout << "Day: " << player.day << " | Level: " << player.level << " | Money: $" << player.money << "\n";
        cout << "You lost! Your money reached zero or below.\n";
        cout << "====================================\n";
        return true;
    }
    if (player.day > MAX_DAY) {
        cout << "\nGame Over! Final Status:\n";
        cout << "Day: " << player.day << " | Level: " << player.level << " | Money: $" << player.money << "\n";
        cout << "Time's up! You didn't reach $" << WIN_MONEY << " or discover all compounds by Day 50.\n";
        cout << "====================================\n";
        return true;
    }
    if (player.money >= WIN_MONEY && !allCompoundsDone) {
        cout << "You have reached $" << WIN_MONEY << ", but you haven't discovered all compounds yet.\n";
        cout << "Keep playing to discover all " << compoundCount << " compounds!\n";
        cout << "====================================\n";
        return false;
    }
    cout << "====================================\n";
    return false;
}

// Check if player has a specific element
bool hasElement(const char* el) {
    for (int i = 0; i < player.elementCount; ++i) {
        if (strcmp(player.elementsOwned[i], el) == 0)
            return true;
    }
    return false;
}

// Add an element to player's inventory
void addElementToPlayer(const char* el) {
    if (!hasElement(el)) {
        strcpy(player.elementsOwned[player.elementCount++], el);
    }
}

// Remove one compound from player's inventory
void removeOneCompound() {
    for (int i = 0; i < compoundCount; ++i) {
        if (compounds[i].owned && compounds[i].quantity > 0) {
            compounds[i].quantity--;
            cout << "========== Warning ==========\n";
            cout << "Compound " << compounds[i].name << " was broken and removed from your inventory.\n";
            cout << "=============================\n";
            if (compounds[i].quantity == 0) {
                compounds[i].owned = false;
            }
            return;
        }
    }
    cout << "========== Warning ==========\n";
    cout << "You have no compounds to lose.\n";
    cout << "=============================\n";
}

// Remove one element from player's inventory (except Oxygen)
void removeOneElement() {
    for (int i = 0; i < player.elementCount; ++i) {
        if (strcmp(player.elementsOwned[i], "Oxygen") != 0) {
            cout << "========== Warning ==========\n";
            cout << "Element " << player.elementsOwned[i] << " was lost.\n";
            cout << "=============================\n";
            for (int j = i; j < player.elementCount - 1; ++j) {
                strcpy(player.elementsOwned[j], player.elementsOwned[j + 1]);
            }
            player.elementCount--;
            return;
        }
    }
    cout << "========== Warning ==========\n";
    cout << "No removable element found.\n";
    cout << "=============================\n";
}

void levelUpIfNeeded();

// Trigger a random event
void randomEvent() {
    int event = rand() % 12 + 1; // Randomly select from 12 events
    cout << "========== Random Event ==========\n";
    switch (event) {
        case 1: {
            int foundMoney = 7 + rand() % 7;
            player.money += foundMoney;
            cout << "You found $" << foundMoney << "!\n";
            break;
        }
        case 2: {
            int lostMoney = 5 + rand() % 5;
            if (player.money < lostMoney) lostMoney = player.money;
            player.money -= lostMoney;
            cout << "You lost $" << lostMoney << " due to a mishap.\n";
            break;
        }
        case 3: {
            int bonusXP = 10 + rand() % 10;
            player.xp += bonusXP;
            cout << "You gained " << bonusXP << " bonus XP!\n";
            levelUpIfNeeded();
            break;
        }
        case 4: { // Memory loss
            if (player.hasMemoryChip) {
                cout << "Your Memory Chip protected you from memory loss!\n";
            } else {
                int lostXP = 3 + rand() % 8;
                player.xp -= lostXP;
                if (player.xp < 0) player.xp = 0;
                cout << "You lost " << lostXP << " XP due to memory loss.\n";
            }
            break;
        }
        case 5: {
            int foundXP = 15 + rand() % 15;
            player.xp += foundXP;
            cout << "You made a lucky discovery! XP +" << foundXP << "\n";
            levelUpIfNeeded();
            break;
        }
        case 6: { // Equipment failure
            if (player.insuranceCount > 0) {
                player.insuranceCount--;
                cout << "Your Accident Insurance protected you from equipment failure!\n";
            } else {
                int lostMoney = 7 + rand() % 15;
                if (player.money < lostMoney) lostMoney = player.money;
                player.money -= lostMoney;
                cout << "Your equipment broke! Lost $" << lostMoney << "\n";
            }
            break;
        }
        case 7: { // Lab explosion
            if (player.insuranceCount > 0) {
                player.insuranceCount--;
                cout << "Your Accident Insurance protected you from lab explosion!\n";
            } else {
                player.xp -= 10;
                if (player.xp < 0) player.xp = 0;
                player.money -= 10;
                if (player.money < 0) player.money = 0;
                cout << "Your lab exploded! XP -10, Money -10\n";
            }
            break;
        }
        case 8: { // Cat breaks compound
            if (player.insuranceCount > 0) {
                player.insuranceCount--;
                cout << "Your Accident Insurance protected your compound from the cat!\n";
            } else {
                cout << "A cat broke a compound!\n";
                removeOneCompound();
            }
            break;
        }
        case 9: { // Taxes to oppressive sheriff
            if (player.hasSpaceGun) {
                cout << "Your Advanced Space Gun scared off the oppressive sheriff!\n";
            } else {
                player.money -= 10;
                if (player.money < 0) player.money = 0;
                cout << "You paid $10 in taxes to the oppressive sheriff.\n";
            }
            break;
        }
        case 10: { // Viral disease
            if (player.vaccineCount > 0) {
                player.vaccineCount--;
                cout << "Your Virus Vaccine protected you from the viral disease!\n";
            } else {
                player.money -= 10;
                if (player.money < 0) player.money = 0;
                player.day++;
                cout << "You caught a viral disease! Lost $10 for treatment and 1 day for quarantine.\n";
            }
            break;
        }
        case 11: { // Currency fluctuations
            if (player.analysisCount > 0) {
                player.analysisCount--;
                cout << "Your Technical Analysis skill prevented loss from currency fluctuations!\n";
            } else {
                player.money -= 7;
                if (player.money < 0) player.money = 0;
                cout << "Currency fluctuations! You lost $7.\n";
            }
            break;
        }
        case 12: {
            player.money += 30;
            cout << "You won a small lottery! Money +$30\n";
            break;
        }
    }
    cout << "====================================\n";
}

// Handle level-up and XP-based random event
void levelUpIfNeeded() {
    if (player.xp >= 50 && !player.xpEventTriggered) {
        player.xpEventTriggered = true;
        randomEvent();
    }
    while (player.xp >= XP_PER_LEVEL && player.level < MAX_LEVEL) {
        player.xp -= XP_PER_LEVEL;
        player.level++;
        player.xpEventTriggered = false; // Reset for new level
        cout << "========== Level Up ==========\n";
        cout << "Level up! You reached Level " << player.level << "!\n";
        for (int i = 0; i < elementCount; ++i) {
            if (elements[i].levelRequired == player.level) {
                addElementToPlayer(elements[i].name);
                cout << "Element " << elements[i].name << " unlocked!\n";
            }
        }
        cout << "=============================\n";
        randomEvent();
    }
    lastLevel = player.level;
}

// Display player status
void showStatus() {
    cout << "========== Player Status ==========\n";
    cout << "Day " << player.day << " | Player: " << player.name 
         << " | Level: " << player.level << " | XP: " << player.xp 
         << " | Money: $" << player.money;
    if (player.money < 20) {
        cout << " (In Bankruptcy)";
    }
    cout << "\nCompounds Discovered: ";
    int discoveredCount = 0;
    for (int i = 0; i < compoundCount; ++i) {
        if (compounds[i].discovered) {
            discoveredCount++;
        }
    }
    cout << discoveredCount << "/" << compoundCount;
    cout << "\nItems: ";
    cout << "Insurance: " << player.insuranceCount << ", ";
    cout << "Technical Analysis: " << player.analysisCount << ", ";
    cout << "Vaccine: " << player.vaccineCount << ", ";
    cout << "Memory Chip: " << (player.hasMemoryChip ? "Yes" : "No") << ", ";
    cout << "Space Gun: " << (player.hasSpaceGun ? "Yes" : "No");
    cout << "\n====================================\n";
}

// Display player's elements
void showElements() {
    cout << "========== Your Elements ==========\n";
    for (int i = 0; i < player.elementCount; ++i) {
        cout << "- " << player.elementsOwned[i] << "\n";
    }
    cout << "====================================\n";
}

// Display player's compounds
void showOwnedCompounds() {
    cout << "========== Your Compounds ==========\n";
    bool hasCompounds = false;
    for (int i = 0; i < compoundCount; ++i) {
        if (compounds[i].owned && compounds[i].quantity > 0) {
            cout << "- " << compounds[i].name << " (Quantity: " << compounds[i].quantity << ")\n";
            hasCompounds = true;
        }
    }
    if (!hasCompounds) {
        cout << "You have no compounds yet.\n";
    }
    cout << "====================================\n";
}

// Display available compounds for creation
void showAvailableCompounds() {
    cout << "========== Available Compounds ==========\n";
    int shown = 0;
    for (int i = 0; i < compoundCount; ++i) {
        Compound& c = compounds[i];
        if (player.level >= c.levelRequired && 
            hasElement(c.ingredients[0]) &&
            hasElement(c.ingredients[1])) {
            shown++;
            cout << shown << ". ";
            if (c.discovered)
                cout << c.name << " [X]";
            else
                cout << "(Hint: " << c.ingredients[0] << ", " << c.ingredients[1] << ")";
            cout << "\n";
        }
    }
    if (shown == 0) {
        cout << "No compounds available to create at your level.\n";
    }
    cout << "====================================\n";
}

// Remove an element from player's inventory
void removeElementFromPlayer(const char* el) {
    if (strcmp(el, "Oxygen") == 0) return;
    int pos = -1;
    for (int i = 0; i < player.elementCount; ++i) {
        if (strcmp(player.elementsOwned[i], el) == 0) {
            pos = i;
            break;
        }
    }
    if (pos == -1) return;
    for (int i = pos; i < player.elementCount - 1; ++i) {
        strcpy(player.elementsOwned[i], player.elementsOwned[i + 1]);
    }
    player.elementCount--;
}

// Get index of selected compound
int getAvailableCompoundIndex(int userChoice) {
    int counter = 0;
    for (int i = 0; i < compoundCount; ++i) {
        Compound& c = compounds[i];
        if (player.level >= c.levelRequired && 
            hasElement(c.ingredients[0]) &&
            hasElement(c.ingredients[1])) {
            counter++;
            if (counter == userChoice)
                return i;
        }
    }
    return -1;
}

// Create a compound
void makeCompound() {
    if (compoundsMadeToday >= MAX_COMPOUNDS_PER_DAY) {
        cout << "========== Warning ==========\n";
        cout << "You have reached the daily limit for creating compounds.\n";
        cout << "=============================\n";
        return;
    }
    showAvailableCompounds();
    cout << "\nEnter the number of the compound to create (0 to cancel): ";
    int choice;
    cin >> choice;
    if (choice == 0) return;
    int index = getAvailableCompoundIndex(choice);
    if (index == -1) {
        cout << "========== Warning ==========\n";
        cout << "Invalid choice.\n";
        cout << "=============================\n";
        return;
    }
    Compound& c = compounds[index];
    if (player.level < c.levelRequired) {
        cout << "========== Warning ==========\n";
        cout << "You do not have enough level to create " << c.name << ".\n";
        cout << "=============================\n";
        return;
    }
    cout << "========== Success ==========\n";
    cout << "You created " << c.name << "! (+XP: " << c.xpReward 
         << ", +Money: " << c.moneyReward << ")\n";
    cout << "=============================\n";
    c.discovered = true;
    c.owned = true;
    c.quantity++;
    player.xp += c.xpReward;
    player.money += c.moneyReward;
    for (int i = 0; i < ELEMENTS_PER_COMPOUND; ++i) {
        removeElementFromPlayer(c.ingredients[i]);
    }
    compoundsMadeToday++;
    player.day++;
    levelUpIfNeeded();
}

// Laboratory menu
void laboratoryMenu() {
    while (true) {
        cout << "========== Laboratory Menu ==========\n";
        cout << "1. Show Elements & Compounds\n";
        cout << "2. Show Available Compounds\n";
        cout << "3. Create Compound\n";
        cout << "4. Compound Guide\n";
        cout << "0. Back to Main Menu\n";
        cout << "Enter your choice: ";
        int choice;
        cin >> choice;
        cout << "====================================\n";
        switch (choice) {
            case 1:
                showElements();
                showOwnedCompounds();
                break;
            case 2:
                showAvailableCompounds();
                break;
            case 3:
                makeCompound();
                break;
            case 4:
                showCompoundGuide();
                break;
            case 0:
                return;
            default:
                cout << "========== Warning ==========\n";
                cout << "Invalid choice. Try again.\n";
                cout << "=============================\n";
        }
    }
}

// Sell a compound
void sellCompound() {
    cout << "========== Sell Compound ==========\n";
    cout << "Your compounds available for selling:\n";
    int count = 0;
    for (int i = 0; i < compoundCount; ++i) {
        if (compounds[i].owned && compounds[i].quantity > 0) {
            count++;
            cout << count << ". " << compounds[i].name << " (Qty: " << compounds[i].quantity << ")\n";
        }
    }
    if (count == 0) {
        cout << "You have no compounds to sell.\n";
        cout << "====================================\n";
        return;
    }
    cout << "Enter the number of compound to sell (0 to cancel): ";
    int choice;
    cin >> choice;
    if (choice == 0) {
        cout << "====================================\n";
        return;
    }
    int counter = 0;
    int index = -1;
    for (int i = 0; i < compoundCount; ++i) {
        if (compounds[i].owned && compounds[i].quantity > 0) {
            counter++;
            if (counter == choice) {
                index = i;
                break;
            }
        }
    }
    if (index == -1) {
        cout << "========== Warning ==========\n";
        cout << "Invalid choice.\n";
        cout << "=============================\n";
        return;
    }
    bool isFraud = (rand() % 100) < 30;
    Compound& c = compounds[index];
    cout << "Selling one unit of " << c.name << "... ";
    if (isFraud) {
        int lostMoney = c.moneyReward / 2;
        player.money -= lostMoney;
        c.quantity--;
        if (c.quantity == 0) c.owned = false;
        cout << "\n========== Warning ==========\n";
        cout << "Oh no! The buyer tricked you. You lost $" << lostMoney << "!\n";
        cout << "=============================\n";
    } else {
        player.money += c.moneyReward;
        c.quantity--;
        if (c.quantity == 0) c.owned = false;
        cout << "\n========== Success ==========\n";
        cout << "Sold successfully for $" << c.moneyReward << "!\n";
        cout << "=============================\n";
    }
    player.day++;
    levelUpIfNeeded();
}


// Coin Flip game
void coinFlipGame() {
    cout << "========== Coin Flip Game ==========\n";
    cout << "Pick a side:\n";
    cout << "0 - Heads\n";
    cout << "1 - Tails\n";
    cout << "(Enter any other number to quit)\n";
    cout << "====================================\n";
    int choice;
    while (true) {
        cout << "\nYour choice (0 = Heads, 1 = Tails): ";
        cin >> choice;
        if (choice != 0 && choice != 1) {
            cout << "========== Message ==========\n";
            cout << "Exiting Coin Flip...\n";
            cout << "=============================\n";
            break;
        }
        int flip = rand() % 2;
        string resultStr = (flip == 0) ? "Heads" : "Tails";
        cout << "========== Result ==========\n";
        cout << "The coin landed on: " << resultStr << "!\n";
        if (choice == flip) {
            cout << "Correct guess! You earned 15 XP.\n";
            cout << "=============================\n";
            player.xp += 15;
            levelUpIfNeeded();
        } else {
            cout << "Wrong guess. You lost $5.\n";
            cout << "=============================\n";
            player.money -= 5;
            if (player.money < 0) player.money = 0;
            if (checkGameEnd()) return;
        }
        cout << "\nPlay again? (1 = yes, 0 = no): ";
        int again;
        cin >> again;
        if (again != 1) break;
    }
}

// Guess the Number game
void guessNumberGame() {
    cout << "========== Guess the Number ==========\n";
    int target = rand() % 10 + 1;
    cout << "Guess a number between 1 and 10: ";
    int guess;
    cin >> guess;
    if (guess == target) {
        cout << "========== Success ==========\n";
        cout << "Correct! You earned 30 XP.\n";
        cout << "=============================\n";
        player.xp += 30;
        levelUpIfNeeded();
    } else {
        cout << "========== Failure ==========\n";
        cout << "Wrong! The number was " << target << ". You lost $10.\n";
        cout << "=============================\n";
        player.money -= 10;
        if (player.money < 0) player.money = 0;
        if (checkGameEnd()) return;
    }
}

// Minigame menu
void minigame() {
    cout << "========== Mini Games Menu ==========\n";
    cout << "1. Coin Flip\n";
    cout << "2. Guess the Number\n";
    cout << "3. Exit\n";
    cout << "Choose a game: ";
    int choice;
    cin >> choice;
    cout << "====================================\n";
    switch (choice) {
        case 1: coinFlipGame(); break;
        case 2: guessNumberGame(); break;
        case 3: break;
        default:
            cout << "========== Warning ==========\n";
            cout << "Invalid option.\n";
            cout << "=============================\n";
    }
}

// Display shop with two shelves
void showShop() {
    cout << "========== Welcome to the Shop ==========\n";
    cout << "Shelf 1: Elements\n";
    int elementCount = 0;
    for (int i = 0; i < SHOP_ITEMS_COUNT; ++i) {
        if (shopItems[i].isElement && player.level >= shopItems[i].levelRequired) {
            elementCount++;
            cout << elementCount << ". " << shopItems[i].name 
                 << " - $" << shopItems[i].price 
                 << " (Req Level: " << shopItems[i].levelRequired << ")\n";
        }
    }
    if (elementCount == 0) {
        cout << "No elements available for your level.\n";
    }
    cout << "\nShelf 2: Tools & Services\n";
    int toolCount = 0;
    for (int i = 0; i < SHOP_ITEMS_COUNT; ++i) {
        if (!shopItems[i].isElement && player.level >= shopItems[i].levelRequired) {
            if (shopItems[i].isPermanent) {
                if (strcmp(shopItems[i].name, "Memory Chip") == 0 && player.hasMemoryChip) continue;
                if (strcmp(shopItems[i].name, "Advanced Space Gun") == 0 && player.hasSpaceGun) continue;
            }
            toolCount++;
            cout << (elementCount + toolCount) << ". " << shopItems[i].name 
                 << " - $" << shopItems[i].price 
                 << " (Req Level: " << shopItems[i].levelRequired 
                 << (shopItems[i].isPermanent ? ", Permanent" : ", One-Time Use)") << "\n";
        }
    }
    if (toolCount == 0) {
        cout << "No tools or services available for your level.\n";
    }
    cout << "\n0. Exit shop\n";
    cout << "Enter item number to buy: ";
    int choice;
    cin >> choice;
    if (choice == 0) {
        cout << "====================================\n";
        return;
    }
    int totalItems = elementCount + toolCount;
    if (choice < 1 || choice > totalItems) {
        cout << "========== Warning ==========\n";
        cout << "Invalid choice.\n";
        cout << "=============================\n";
        return;
    }
    int actualIndex = -1;
    int count = 0;
    for (int i = 0; i < SHOP_ITEMS_COUNT; ++i) {
        if (player.level >= shopItems[i].levelRequired) {
            if (shopItems[i].isElement) {
                count++;
                if (count == choice) {
                    actualIndex = i;
                    break;
                }
            } else {
                if (shopItems[i].isPermanent) {
                    if (strcmp(shopItems[i].name, "Memory Chip") == 0 && player.hasMemoryChip) continue;
                    if (strcmp(shopItems[i].name, "Advanced Space Gun") == 0 && player.hasSpaceGun) continue;
                }
                count++;
                if (count == choice) {
                    actualIndex = i;
                    break;
                }
            }
        }
    }
    if (actualIndex == -1) {
        cout << "========== Warning ==========\n";
        cout << "Invalid choice.\n";
        cout << "=============================\n";
        return;
    }
    ShopItem& item = shopItems[actualIndex];
    if (player.money < item.price) {
        cout << "========== Warning ==========\n";
        cout << "Not enough money to buy " << item.name << ".\n";
        cout << "=============================\n";
        return;
    }
    player.money -= item.price;
    cout << "========== Success ==========\n";
    cout << "You bought " << item.name << "!\n";
    cout << "=============================\n";
    if (item.isElement) {
        addElementToPlayer(item.name);
    } else {
        if (strcmp(item.name, "Accident Insurance") == 0) {
            player.insuranceCount++;
        } else if (strcmp(item.name, "Technical Analysis") == 0) {
            player.analysisCount++;
        } else if (strcmp(item.name, "Virus Vaccine") == 0) {
            player.vaccineCount++;
        } else if (strcmp(item.name, "Memory Chip") == 0) {
            player.hasMemoryChip = true;
        } else if (strcmp(item.name, "Advanced Space Gun") == 0) {
            player.hasSpaceGun = true;
        }
    }
}

// Display game introduction
void showIntro() {
    cout << "========== Welcome to ChemFactory: Year 2345 ==========\n";
    cout << "In a world ruined by nuclear war, you are a chemist in a decaying lab.\n";
    cout << "The fascist regime plans to destroy your lab in "<<MAX_DAY<<" days.\n";
    cout << "Craft and sell 10 compounds (like water and salt) to earn $" << WIN_MONEY << ".\n";
    cout << "Discover all " << compoundCount << " compounds to prove your worth.\n";
    cout << "Buy elements from the black market and avoid disasters!\n";
    cout << "====================================\n";
    cout << "Press Enter to start...";
    cin.get();
}

// Get player's name
void askPlayerName() {
    cout << "Enter your name: ";
    cin.getline(player.name, MAX_NAME);
    if (strlen(player.name) == 0) {
        strcpy(player.name, "Player");
    }
}

// Initialize player data
void initializePlayer() {
    for (int i = 0; i < compoundCount; ++i) {
        compounds[i].quantity = 0;
        compounds[i].discovered = false;
        compounds[i].owned = false;
    }
    player.level = 1;
    player.xp = 0;
    player.money = 20;
    player.day = 1;
    player.elementCount = 0;
    player.xpEventTriggered = false;
    player.insuranceCount = 0;
    player.analysisCount = 0;
    player.vaccineCount = 0;
    player.hasMemoryChip = false;
    player.hasSpaceGun = false;
    addElementToPlayer("Hydrogen");
    addElementToPlayer("Oxygen");
}

// Ask if player wants to play again
bool askPlayAgain() {
    cout << "\nPlay again? (1 = yes, 0 = no): ";
    int choice;
    cin >> choice;
    cout << "====================================\n";
    if (choice == 1) {
        initializePlayer();
        compoundsMadeToday = 0;
        lastLevel = 1;
        return true;
    } else {
        cout << "========== Message ==========\n";
        cout << "Thanks for playing! Goodbye.\n";
        cout << "=============================\n";
        return false;
    }
}

// Display main menu
void showMainMenu() {
    cout << "========== Main Menu ==========\n";
    cout << "1. Laboratory (View Elements & Create Compounds)\n";
    cout << "2. Sell Compound\n";
    cout << "3. MiniGame (Earn XP to level up!)\n";
    cout << "4. Shop\n";
    cout << "5. Exit\n";
    cout << "Enter choice: ";
}

// Main game loop
void gameLoop() {
    while (true) {
        compoundsMadeToday = 0;
        showStatus();
        if (checkGameEnd()) {
            if (askPlayAgain()) {
                continue;
            } else {
                break;
            }
        }
        showMainMenu();
        int choice;
        cin >> choice;
        cout << "====================================\n";
        switch (choice) {
            case 1:
                laboratoryMenu();
                break;
            case 2:
                sellCompound();
                break;
            case 3:
                minigame();
                break;
            case 4:
                showShop();
                break;
            case 5:
                cout << "========== Message ==========\n";
                cout << "Thanks for playing! Goodbye.\n";
                cout << "=============================\n";
                exit(0);
            default:
                cout << "========== Warning ==========\n";
                cout << "Invalid option, try again.\n";
                cout << "=============================\n";
        }
    }
}

// Main function
int main() {
    srand(static_cast<unsigned int>(time(nullptr)));
    showIntro();
    askPlayerName();
    initializePlayer();
    gameLoop();
    return 0;
}