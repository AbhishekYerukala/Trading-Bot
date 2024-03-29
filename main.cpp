#include <iostream>
#include <vector>

// #include "csvhandler.hpp"
#ifndef MARKET_HPP
#define MARKET_HPP
#include "market.hpp"
#endif

#ifndef ORDER_HPP
#define ORDER_HPP
#include "order.hpp"
#endif

#ifndef BOT_HPP
#define BOT_HPP
#include "bot.hpp"
#endif

using namespace std;

// Observer // behavioural pattern
// Strategy // behavioural pattern
// Adapter // structural pattern 
// factory // creational pattern
// singleton // creational pattern

int main(int argc, char const *argv[]) // driver code
{
    // Opening input data file -> All the stock market prices are stored in the file data.tsv as tab-seperated values
    CSV* data = new CSV("data.tsv"); // only one object is instantiated

    // Creating an account (bank+trading) for the user Abhishek
    string name;
    cout << "Please enter the account holder name: ";
    cin >> name;
    float deposit;
    cout << "Please enter inital balance: ";
    cin >> deposit;
    Account* account = new Account(name, deposit);

    // Market is established with the above data
    Market* NSE = new Market(data); 

    //Strategy of trading bot is defined as a CONCRETE STRATEGY linked to Account: Abhishek
    Buydip* bd = new Buydip(NSE, account);

    // Bot is an observer of the stock market NSE and has the strategy of "Buydip"
    Bot* b = new Bot(account, NSE, bd);

    // Stock market is run, indicating 1 second = 1 day
    NSE->run();

    // Finally we are printing the account information
    account->information();
    cout << "Total profit earned: " << account->take_profit() << endl;

    return 0;
}