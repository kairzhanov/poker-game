#include <iostream>
#include <cstdio>
#include <vector>
#include <algorithm>
using namespace std;
// global variable, which will be used in the Tie case only
bool winner_found = false;
// structure for each card
struct Card {
    int value;
    char suit;
};

class Hand
{
private:
    // array, which represents how much times card was occurred
    // Ex: TD TC 4D 2H AC represented as 2 2 1 1 1
    int matching[5];
public:
    // constants to define combinations with their ranks
    static const int HIGH_CARD = 1;
    static const int PAIR = 2;
    static const int TWO_PAIRS = 3;
    static const int THREE_OF_A_KIND = 4;
    static const int STRAIGHT = 5;
    static const int FLUSH = 6;
    static const int FULL_HOUSE = 7;
    static const int FOUR_OF_A_KIND = 8;
    static const int STRAIGHT_FLUSH = 9;
    Card cards[5]; // defining 5 Cards for each Hand
    int highest_rank; // highest rank (combination)
    int valuable_card; // the index of the card, which is important if the combinations are same
    // the function, which stores card in the array cards[] in the beginning
    void readCards(char value, char suit, int i) { // (value of card, suit of card, number of card)
        cards[i].suit = suit;
        switch(value) {
            case 'T': cards[i].value = 10; break;
            case 'J': cards[i].value = 11; break; // for the chars we give bigger values
            case 'Q': cards[i].value = 12; break;
            case 'K': cards[i].value = 13; break;
            case 'A': cards[i].value = 14; break;
            default: cards[i].value = value - 48;
        }
    }
    // sorting cards by the value: Highest first
    void sortCards() {
        for (int i = 0; i < 5; ++i) {
            for (int j = 0; j < 5; ++j) {
                if (cards[i].value > cards[j].value) {
                    Card store = cards[j];
                    cards[j] = cards[i];
                    cards[i] = store;
                }
            }
        }
    }

    void checkCombinations() {
        // counting the number of overlapping cards by their value
        for (int i = 0; i < 5; ++i) {
            matching[i] = count(&cards[0].value, &cards[5].value, cards[i].value);
        }
        highest_rank = Hand::HIGH_CARD; // if no combination exists, check by the highest cards
        int pairs_count = 0;
        bool three_cards = false;
        bool four_cards = false;
        bool straight = true;
        bool flush = true;
        for (int i = 1; i < 5; ++i) {
            if (matching[i] == 2 && matching[i-1] == 2) {
                pairs_count++; // if there are two same cards, count the pairs
                valuable_card = i; // and the valuable card is index of pair
            }
            if (matching[i] == 3 && matching[i-1] == 3) {
                three_cards = true; // three of a kind
            }
            if (matching[i] == 4) {
                four_cards = true; // four of a kind
                valuable_card = i;
            }
            // checking for straight and flush
            if (cards[i-1].value - cards[i].value != 1) straight = false;
            if (cards[i].suit != cards[i-1].suit) flush = false;
        }
        // checking the combinations, starting from Highest
        if (straight && flush) {
            highest_rank = Hand::STRAIGHT_FLUSH;
            return;
        }
        if (four_cards) {
            highest_rank = Hand::FOUR_OF_A_KIND;
            return;
        }
        if (pairs_count == 1 && three_cards) {
            highest_rank = Hand::FULL_HOUSE; // in any case valuable card will be in the middle
            return; // and one card from three_of_kind
        }
        if (flush) {
            highest_rank = Hand::FLUSH; // we will compare by the first card
            return;
        }
        if (straight) {
            highest_rank = Hand::STRAIGHT; // we will compare by the first card
            return;
        }
        if (three_cards) {
            highest_rank = Hand::THREE_OF_A_KIND; // in any case valuable card will be in the middle
            return; // and one card from three_of_kind
        }
        if (pairs_count == 2 || pairs_count == 3) {
            highest_rank = Hand::TWO_PAIRS;
            for (int i = 0; i < 5; ++i) { // the valuable card is the card which doesn't have a pair
                if (matching[i] == 1) valuable_card = i;
            }
            return;
        }
        if (pairs_count == 1) {
            highest_rank = Hand::PAIR; // valuable card is the index of card which has a pair
            return;
        }
    }
    // function which starts the game: sort and check the combinations
    void startGame() {
        this->sortCards();
        this->checkCombinations();
    }
};
// output in case if player Black wins
void blackWins() {
    cout << "Black wins." << '\n';
    winner_found = true;
}
// output in case if player White wins
void whiteWins() {
    cout << "White wins." << '\n';
    winner_found = true;
}
// comparing ranks
void compareRanks(Hand black, Hand white) {
    if (black.highest_rank > white.highest_rank) blackWins();
    else if (black.highest_rank < white.highest_rank) whiteWins();
    else { // if their ranks are same
        winner_found = false;
        // if no combination or combination is Flush, then compare by the values of cards
        if(black.highest_rank == Hand::HIGH_CARD || black.highest_rank == Hand::FLUSH) {
            for (int i = 0; i < 5; ++i) {
                if (black.cards[i].value > white.cards[i].value) {
                    blackWins();
                    break;
                }
                else if (black.cards[i].value < white.cards[i].value) {
                    whiteWins();
                    break;
                }
            }
        }
        if (black.highest_rank == Hand::PAIR) {
            // comparing their cards in pair
            if (black.cards[black.valuable_card].value > white.cards[white.valuable_card].value) {
                blackWins();
            }
            else if (black.cards[black.valuable_card].value < white.cards[white.valuable_card].value) {
                whiteWins();
            }
            else { // if that cards are equal, check by cards not forming a pair
                int black_pair_card = black.cards[black.valuable_card].value; // card forming a pair
                int white_pair_card = white.cards[white.valuable_card].value;
                vector <int> black_cards, white_cards; // array of cards not forming a pair
                for (int i = 0; i < 5; ++i) {
                    if (black.cards[i].value != black_pair_card) black_cards.push_back(black.cards[i].value);
                    if (white.cards[i].value != white_pair_card) white_cards.push_back(white.cards[i].value);
                }
                for (int i = 0; i < 3; ++i) { // comparing the cards not forming a pair
                    if (black_cards[i] > white_cards[i]) {
                        blackWins();
                    } else if (black_cards[i] < white_cards[i]) {
                        whiteWins();
                    } else {
                        continue;
                    }
                }
                black_cards.clear();
                white_cards.clear();
            }
        }
        if (black.highest_rank == Hand::TWO_PAIRS) {
            // in this case we are comparing the cards that are forming a pair for sure
            if (black.cards[1].value > white.cards[1].value) { // second cards of each Hand
                blackWins();
            }
            else if (black.cards[1].value < white.cards[1].value) {
                whiteWins();
            }
            // and forth cards of each Hand
            if (black.cards[3].value > white.cards[3].value && !winner_found) {
                blackWins();
            }
            else if (black.cards[3].value < white.cards[3].value && !winner_found) {
                whiteWins();
            }
            // at the end compare by the card not forming a pair (valuable card)
            if (black.cards[black.valuable_card].value > white.cards[white.valuable_card].value && !winner_found) {
                blackWins();
            }
            if (black.cards[black.valuable_card].value < white.cards[white.valuable_card].value && !winner_found) {
                whiteWins();
            }
        }
        // in this case we are comparing cards in the middle, which are forming a set of three for sure
        if (black.highest_rank == Hand::THREE_OF_A_KIND || black.highest_rank == Hand::FULL_HOUSE) {
            if (black.cards[2].value > white.cards[2].value) {
                blackWins();
            }
            if (black.cards[2].value < white.cards[2].value) {
                whiteWins();
            }
        }
        if (black.highest_rank == Hand::FOUR_OF_A_KIND) {
            if (black.cards[3].value > white.cards[3].value) {
                blackWins();
            }
            else if (black.cards[3].value < white.cards[3].value) {
                whiteWins();
            }
        }
        // compare by the highest cards
        if (black.highest_rank == Hand::STRAIGHT || black.highest_rank == Hand::STRAIGHT_FLUSH) {
            if (black.cards[0].value > white.cards[0].value) {
                blackWins();
            }
            else if (black.cards[0].value < white.cards[0].value) {
                whiteWins();
            }
        }
        // if winner wasn't found output "Tie"
        if (!winner_found) cout << "Tie." << '\n';
    }
}

int main()
{
    char line[50];
    char value[10], suit[10]; // temporary array of chars to read the input and transfer to objects
    while (fgets(line, sizeof line, stdin) != NULL) { // read till the end of the input
        Hand black, white; // creating objects for 2 players
        sscanf(line, "%c%c %c%c %c%c %c%c %c%c %c%c %c%c %c%c %c%c %c%c",
               &value[0], &suit[0], &value[1], &suit[1], &value[2], &suit[2], &value[3], &suit[3],
               &value[4], &suit[4], &value[5], &suit[5],&value[6], &suit[6], &value[7], &suit[7],
               &value[8], &suit[8], &value[9], &suit[9]);
        for (int i = 0; i < 5; ++i) {
            black.readCards(value[i], suit[i], i); // assigning cards for the object Black
        }
        int j = 0;
        for (int i = 5; i < 10; ++i) {
            white.readCards(value[i], suit[i], j); // assigning cards for the object White
            j++;
        }
        black.startGame(); // sorting and checking for highest rank
        white.startGame();
        compareRanks(black, white); // comparing their ranks to define the winner and output
    }
    return 0;
}
