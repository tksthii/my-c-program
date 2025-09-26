#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

// UNO Card structure
typedef struct {
    char color[10];  // Red, Blue, Green, Yellow, Wild
    char value[10];  // 0-9, Skip, Reverse, Draw2, Wild, WildDraw4
} Card;

// Function prototypes
void initializeDeck(Card deck[]);
void shuffleDeck(Card deck[]);
void dealCards(Card deck[], Card playerHand[], Card computerHand[], int *deckIndex);
void printCard(Card card);
void printHand(Card hand[], int handSize);
int canPlayCard(Card card, Card topCard);
void playGame();

int main() {
    printf("=== WELCOME TO SIMPLE UNO GAME ===\n");
    printf("Rules: Match color or number with the top card!\n");
    printf("Special cards: Skip, Reverse, Draw2\n\n");
    
    playGame();
    
    return 0;
}

void initializeDeck(Card deck[]) {
    int index = 0;
    char colors[4][10] = {"Red", "Blue", "Green", "Yellow"};
    char numbers[10][3] = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9"};
    
    // Add number cards (0-9) for each color
    for (int c = 0; c < 4; c++) {
        for (int n = 0; n < 10; n++) {
            strcpy(deck[index].color, colors[c]);
            strcpy(deck[index].value, numbers[n]);
            index++;
        }
    }
    
    // Add special cards
    char specials[3][10] = {"Skip", "Reverse", "Draw2"};
    for (int c = 0; c < 4; c++) {
        for (int s = 0; s < 3; s++) {
            strcpy(deck[index].color, colors[c]);
            strcpy(deck[index].value, specials[s]);
            index++;
        }
    }
}

void shuffleDeck(Card deck[]) {
    for (int i = 0; i < 52; i++) {
        int j = rand() % 52;
        Card temp = deck[i];
        deck[i] = deck[j];
        deck[j] = temp;
    }
}

void dealCards(Card deck[], Card playerHand[], Card computerHand[], int *deckIndex) {
    // Deal 7 cards to each player
    for (int i = 0; i < 7; i++) {
        playerHand[i] = deck[(*deckIndex)++];
        computerHand[i] = deck[(*deckIndex)++];
    }
}

void printCard(Card card) {
    printf("[%s %s]", card.color, card.value);
}

void printHand(Card hand[], int handSize) {
    for (int i = 0; i < handSize; i++) {
        printf("%d. ", i + 1);
        printCard(hand[i]);
        printf(" ");
    }
    printf("\n");
}

int canPlayCard(Card card, Card topCard) {
    return (strcmp(card.color, topCard.color) == 0 || 
            strcmp(card.value, topCard.value) == 0);
}

void playGame() {
    srand(time(NULL));
    
    Card deck[52];
    Card playerHand[20];
    Card computerHand[20];
    Card topCard;
    
    int deckIndex = 0;
    int playerHandSize = 7;
    int computerHandSize = 7;
    int playerTurn = 1;
    
    printf("Initializing game...\n");
    initializeDeck(deck);
    shuffleDeck(deck);
    dealCards(deck, playerHand, computerHand, &deckIndex);
    
    // Set initial top card
    topCard = deck[deckIndex++];
    
    printf("Game started!\n\n");
    
    // Simple game loop (5 turns for demo)
    for (int turn = 0; turn < 10 && playerHandSize > 0 && computerHandSize > 0; turn++) {
        printf("=== TURN %d ===\n", turn + 1);
        printf("Top card: ");
        printCard(topCard);
        printf("\n");
        printf("Cards left - You: %d | Computer: %d\n", playerHandSize, computerHandSize);
        
        if (playerTurn) {
            printf("\nYour hand (%d cards):\n", playerHandSize);
            printHand(playerHand, playerHandSize);
            
            printf("Enter card number to play (1-%d) or 0 to draw: ", playerHandSize);
            int choice;
            scanf("%d", &choice);
            
            if (choice == 0) {
                // Draw card
                if (deckIndex < 52) {
                    playerHand[playerHandSize++] = deck[deckIndex++];
                    printf("You drew a card!\n");
                }
            } else if (choice >= 1 && choice <= playerHandSize) {
                Card selectedCard = playerHand[choice - 1];
                if (canPlayCard(selectedCard, topCard)) {
                    printf("You played: ");
                    printCard(selectedCard);
                    printf("\n");
                    topCard = selectedCard;
                    
                    // Remove card from hand
                    for (int i = choice - 1; i < playerHandSize - 1; i++) {
                        playerHand[i] = playerHand[i + 1];
                    }
                    playerHandSize--;
                } else {
                    printf("Invalid card! Must match color or number.\n");
                    continue;
                }
            }
        } else {
            // Computer turn (simple AI)
            printf("\nComputer's turn (%d cards left)...\n", computerHandSize);
            int played = 0;
            
            for (int i = 0; i < computerHandSize && !played; i++) {
                if (canPlayCard(computerHand[i], topCard)) {
                    printf("Computer played: ");
                    printCard(computerHand[i]);
                    printf("\n");
                    topCard = computerHand[i];
                    
                    // Remove card from computer hand
                    for (int j = i; j < computerHandSize - 1; j++) {
                        computerHand[j] = computerHand[j + 1];
                    }
                    computerHandSize--;
                    played = 1;
                }
            }
            
            if (!played && deckIndex < 52) {
                computerHand[computerHandSize++] = deck[deckIndex++];
                printf("Computer drew a card.\n");
            }
        }
        
        playerTurn = !playerTurn;
        printf("\n");
    }
    
    // Game end
    printf("=== GAME OVER ===\n");
    if (playerHandSize == 0) {
        printf("🎉 YOU WIN! Congratulations!\n");
    } else if (computerHandSize == 0) {
        printf("💻 Computer wins! Better luck next time!\n");
    } else {
        printf("Game ended. Final scores:\n");
        printf("You: %d cards left\n", playerHandSize);
        printf("Computer: %d cards left\n", computerHandSize);
        if (playerHandSize < computerHandSize) {
            printf("🎉 YOU WIN!\n");
        } else {
            printf("💻 Computer wins!\n");
        }
    }
}