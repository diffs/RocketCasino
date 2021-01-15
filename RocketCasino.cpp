/* File: RocketCasino.cpp
 * Date: Jan 14, 2021
 * Description:
 * This program simulates the online casino game Crash using a command-line interface
 */
#include <time.h>
#include <iostream>
#include <string>
#include <algorithm>
#include <future>
#include <thread>

#ifdef _WIN32
#include <Windows.h>
#include <io.h>
#include <fcntl.h>

#else
#include <unistd.h>
#endif

using namespace std;
#define INPUT_LENGTH 80

#pragma warning(disable: 4996)


double getDouble(void);
char getChar(void);
static void getAnswer();
double playRound(double result, double autoCashout);
double generateRandomCrashpoint(void);

int main() {
    // Seed the Random Number Generator and set locale to allow for certain characters to be printed to the console
    srand(unsigned int(time(NULL)));
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    char* locale = setlocale(LC_ALL, "English");
    std::locale lollocale(locale);
    setlocale(LC_ALL, locale);
    std::wcout.imbue(lollocale);

    cout << "• • • Welcome to the Rocket Casino! • • •" << endl << endl;
    cout << "Please enter your desired starting balance: " << endl;

    double balance = getDouble();

    cout << "Initial balance confirmed! Starting game..." << endl << endl;

    int gameId = 1;
    while (true) {
        cout << "• GAME #" << gameId << " •" << endl << endl;
        cout << "Please enter bet amount in dollars:" << endl;
        double bet = getDouble();
        if (bet > balance) {
            cout << "Bet cannot be greater than balance. Please try again..." << endl;
            continue;
        } else if (bet <= 0) {
            cout << "Bet cannot be $0 or less. Please try again..." << endl;
            continue;
        }

        cout << "Do you want to automatically cash out? If so, enter a cashout point above 1.0 (1.5 = 1.5x). If not, enter 0." << endl;
        double autoCashout = getDouble();
        if (autoCashout <= 1.0) {
            cout << "Automatic cashout disabled!" << endl;
            autoCashout = 0.0;
        } else {
            cout << "Automatic cashout set at: " << autoCashout << endl;
        }
        cout << "Game starting. To cashout at any time, press ENTER." << endl;

        for (int i = 5; i > 0; i--) {
            cout << "Game starting in "<<i<<" seconds..." << endl;
            Sleep(100);
        }
        cout << endl;

        double result = generateRandomCrashpoint();
        double cashout = playRound(result, autoCashout);
        if (cashout > 0)
            printf("You cashed out at %.2fx and won $%.2f!\n", cashout, bet * cashout);
        else
            printf("You did not cashout, so you lost your bet ($%.2f)\n", bet);

        balance -= bet;
        balance += bet * cashout;

        printf("Your current balance is: $%.2f\n\n", balance);

        if (balance <= 0.0)
            break;
        else {
            cout << "Do you want to play another round? Type Y or N" << endl;
            char reply = getChar();

            if (reply != 'Y' && reply != 'y')
                break;
        }

        gameId++;
    }
    cout << "-----------------------------------" << endl;
    cout << "• GAME OVER •" << endl << endl;
    printf("You walk away with: $%.2f\n", balance);
    cout << "Thanks for playing the Rocket Casino!" << endl;

    return 0;
}


/* Function: getAnswer()
 * Description: This function is meant to be used asynchronously with a C++ Future. It detects any time a user presses enter on any input.
 * Parameter: void
 * Returns: void
 */
static void getAnswer() {
    char record[INPUT_LENGTH] = {
      0
    };
    char character = 0;
    fgets(record, INPUT_LENGTH, stdin);
}


/* Function: playRound()
 * Description: This function displays the game UI to the user and handles inputs for a manual cashout as well as automatic cashouts
 * Parameter: double crashPoint: The round's ending multiplier (where the game crashes)
 *            double autoCashout: The user's specified auto-cashout value
 * Returns: double: The user's winnings (or returns 0 for total loss)
 */
double playRound(double crashPoint, double autoCashout) {

    double progress = 1.0;
    double increment = 0.03;
    double incrementMultiplier = 0.1;

    std::future<void> future = async(getAnswer);
    std::chrono::seconds timeout(0);
    double cashout = 0; //default to 0


    while (progress <= crashPoint) {
        int barWidth = 100;

        std::cout << "[";
        int pos = barWidth * progress;
        pos = pos / 50;
        for (int i = 0; i < barWidth; ++i) {
            if (i < pos)
                std::cout << "=";
            else if (i == pos)
                std::cout << ">";
            else
                std::cout << " ";
        }

        printf("] %.2fx\r", progress);
        std::cout.flush();

        Sleep(250);

        if (cashout == 0 && future.wait_for(timeout) == std::future_status::ready) {
            cashout = progress;
            printf("Cashed out at %.2fx\n", progress);
            std::cout.flush();
        }

        progress += increment;
        increment += increment * incrementMultiplier;
        incrementMultiplier = incrementMultiplier * 0.95;

        if (autoCashout > 0 && cashout == 0 && progress >= autoCashout && autoCashout < crashPoint) {
            cashout = autoCashout;
            printf("Automatically cashed out at %.2fx\n", autoCashout);
            std::cout.flush();
        }


    }
    printf("The game crashed at %.2fx.\n", crashPoint);
    if (future.wait_for(timeout) == std::future_status::timeout)
        cout << "Press enter to continue..." << endl;
    else
        cout << endl;

    return cashout;

}


/* Function: getDouble()
 * Description: This function gets a floating-point number from the user. If the user enters a valid floating-point number, the value is returned
 * Parameter: void
 * Returns: double: The user's input
 */
double getDouble() {
    char record[INPUT_LENGTH] = { 0 };
    double number = 0;

    /* use fgets() to get a string from the keyboard */
    fgets(record, INPUT_LENGTH, stdin);
    if (sscanf(record, "%lf", &number) == 1) {
        return number;
    }

    return 0;
}


/* Function: getChar()
 * Description: Gets the user's input in char format. Retries if invalid input provided.
 * Paramaters: void
 * Returns: char: The user's char input
 */
char getChar(void) {
    while (true) {
        /* function limitation: only accepts 120 characters of input*/
        char record[INPUT_LENGTH] = {
          0
        };
        char character = 0;
        fgets(record, INPUT_LENGTH, stdin);
        if (sscanf(record, "%c", &character) != 1) {
            cout << "Invalid Input! Please try again." << endl;
        } else {
            return character;
        }

    }
}


/* Function: generateRandomCrashpoint()
 * Description: Generates a random crashpoint number for use in a game.
 *              There is a 3% chance the game crashes instantly.
 * Paramaters: void
 * Returns: double: A random crashpoint
 */
double generateRandomCrashpoint(void) {
    int randNum = rand() % 100;
    if (randNum < 3) {
        return 1.0;
    } else {
        randNum = rand() % 1000;
        //cout << " rand " << randNum << endl;
        double dRandNum = randNum / (double) 1000;
        //cout << " dRand " << dRandNum << endl;
        double x = (double) 99 / (1 - dRandNum);
      //  cout << " x " << x << endl;
        x = floor(x);
        //cout << " floor'ed x " << x << endl;

        return max(1, double(x) / double(100));
    }
}