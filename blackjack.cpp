#include <random>       // for std::mt19937
#include <ctime>        // std::time(nullptr)
#include <algorithm>    // std::shuffle
#include <array>
#include <iostream>



enum CardSuit
{
    SUIT_CLUB,
    SUIT_DIAMOND,
    SUIT_HEART,
    SUIT_SPADE,

    MAX_SUITS
};

enum CardRank
{
    RANK_2,
    RANK_3,
    RANK_4,
    RANK_5,
    RANK_6,
    RANK_7,
    RANK_8,
    RANK_9,
    RANK_10,
    RANK_JACK,
    RANK_QUEEN,
    RANK_KING,
    RANK_ACE,

    MAX_RANKS
};

struct Card
{
    CardRank rank{};
    CardSuit suit{};
};

using deck_type = std::array<Card, 52>;
using index_type = deck_type::size_type;

void printCard(const Card& card)
{
  switch (card.rank)
  {
      case CardRank::RANK_2:    std::cout << '2';   break;
      case CardRank::RANK_3:    std::cout << '3';   break;
      case CardRank::RANK_4:    std::cout << '4';   break;
      case CardRank::RANK_5:    std::cout << '5';   break;
      case CardRank::RANK_6:    std::cout << '6';   break;
      case CardRank::RANK_7:    std::cout << '7';   break;
      case CardRank::RANK_8:    std::cout << '8';   break;
      case CardRank::RANK_9:    std::cout << '9';   break;
      case CardRank::RANK_10:   std::cout << 'T';   break;
      case CardRank::RANK_JACK: std::cout << 'J';   break;
      case CardRank::RANK_QUEEN:std::cout << 'Q';   break;
      case CardRank::RANK_KING: std::cout << 'K';   break;
      case CardRank::RANK_ACE:  std::cout << 'A';   break;
      default:                  std::cout << '?';   break;
  }
 
  switch (card.suit)
  {
      case CardSuit::SUIT_CLUB:     std::cout << 'C';   break;
      case CardSuit::SUIT_DIAMOND:  std::cout << 'D';   break;
      case CardSuit::SUIT_HEART:    std::cout << 'H';   break;
      case CardSuit::SUIT_SPADE:    std::cout << 'S';   break;
      default:                      std::cout << '?';   break;
  }
}

deck_type createDeck()
{
    deck_type deck{};

    index_type card{ 0 };

    auto suits{ static_cast<index_type>(CardSuit::MAX_SUITS) };
    auto ranks{ static_cast<index_type>(CardRank::MAX_RANKS) };

    for (index_type suit{ 0 }; suit < suits; ++suit)
    {
        for (index_type rank{ 0 }; rank < ranks; ++rank)
        {
            deck[card].suit = static_cast<CardSuit>(suit);
            deck[card].rank = static_cast<CardRank>(rank);
            ++card;
        }
    }

    return deck;
}

void printDeck(const deck_type &deck)
{
    for (const auto &card : deck)
    {
        printCard(card);
        std::cout << ' ';
    }

    std::cout << '\n';
}

void shuffleDeck(deck_type &deck)
{
    std::mt19937 mt{ static_cast<std::mt19937::result_type>
                        (std::time(nullptr)) };
    std::shuffle(deck.begin(), deck.end(), mt);
}

int getCardValue(const Card &card, const int playerOrDealer,
                                             const bool playersTurn)
{
    if (card.rank <= CardRank::RANK_10)
    {
        return (static_cast<int>(card.rank) + 2);
    }

    switch (card.rank)
    {
        case CardRank::RANK_JACK: 
        case CardRank::RANK_QUEEN:
        case CardRank::RANK_KING: return 10;
        case CardRank::RANK_ACE: 
            {
                if (playerOrDealer + 11 <= 21 )
                    return 11;
                else
                    return 1;                            
            }
     
        default:    return 0;
    }
}

void printScore(const int dealer, const int player, const bool playersTurn,
                                const bool printingIntialScore)
{
    if (printingIntialScore)
    {
        std::cout << "\n\nInitial Scores--->\t" << "Dealer: " << dealer 
                    << "\tPlayer: " << player << "\n\n";
    }
    else if (playersTurn) 
    {
        std::cout << "\nPlayer played--->\t" << "Dealer: " << dealer 
                    << "\tPlayer: " << player << "\n\n";
    }
    else 
    {
        std::cout << "\nDealer played--->\t" << "Dealer: " << dealer 
                    << "\tPlayer: " << player << "\n\n";
    }
    
}

void playersMove(int &player, int &currentCardIndex,
                            const deck_type &deck, const bool playersTurn)
{
    player += getCardValue(deck[currentCardIndex++], player, playersTurn);
}

void dealersMove(int &dealer, int &currentCardIndex,
                            const deck_type &deck, const bool playersTurn)
{
    dealer += getCardValue(deck[currentCardIndex++], dealer, playersTurn);
}

char playBlackjack(const deck_type &deck)
{
    int currentCardIndex{0};

    static bool playersTurn{ true };

    char gameResult{};
    bool printingIntialScore{ true };

    static int dealer{ 0 };
    static int player{ 0 };

    // initial moves
    playersMove(player, currentCardIndex, deck, playersTurn);
    dealersMove(dealer, currentCardIndex, deck, playersTurn);
    playersMove(player, currentCardIndex, deck, playersTurn);

    // print initial score
    printScore(dealer, player, playersTurn, printingIntialScore);
    printingIntialScore = false;
    // check for blackjack condition
    if (player == 21)
        std::cout << "\n\n---HOORAY! BLACKJACK---\n";

    while (player < 21 && playersTurn)
    {
        char hitStand{};
        std::cout << "Do you want to hit(h) or stand(s): ";
        std::cin >> hitStand;
        std::cin.ignore(32767, '\n');
        if (std::cin.fail())
        {
            std::cin.clear();
        }

        if (hitStand != 'h' && hitStand != 's')
            continue;

        if (hitStand == 'h')
        {
            playersMove(player, currentCardIndex, deck, playersTurn);
            printScore(dealer, player, playersTurn, printingIntialScore);
        }
        else if (hitStand == 's')
        {
            playersTurn = false;
        }

    }

    while (dealer < 17  && !playersTurn)
    {
        dealersMove(dealer, currentCardIndex, deck, playersTurn);
        printScore(dealer, player, playersTurn, printingIntialScore);
    }

    if (player > 21 && dealer > 21)
        gameResult = 'l';
    else if (player > 21 && dealer <= 21)
        gameResult = 'l';
    else if (dealer > 21 && player <= 21)
        gameResult = 'w';
    else if (player > dealer)
        gameResult = 'w';
    else if (dealer > player)
        gameResult = 'l';
    else if (player == dealer)
        gameResult = 't';

    return gameResult;
}

int main()
{
    auto deck{ createDeck() };

    std::cout << "\n\n\t\t***Card in the deck***\n\n";
    printDeck(deck);

    shuffleDeck(deck);
    std::cout << "\n\n\t\t***Shuffled deck***\n\n";
    printDeck(deck);

    char gameResult{ playBlackjack(deck) };

    if (gameResult == 'w')
    {
        std::cout <<"\n\n\t\t***CONGRATULATIONS! YOU WON***\n";
    } 
    else if (gameResult == 'l')
    {
        std::cout <<"\n\n\t\t***OOPS! YOU LOST***\n";
    }
    else if (gameResult == 't')
    {
        std::cout <<"\n\n\t\t***PUSH! IT'S TIE***\n";
    }


    return 0;
    
}
