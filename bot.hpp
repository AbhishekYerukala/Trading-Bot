#include <iostream>

#ifndef ORDER_HPP
#define ORDER_HPP
#include "order.hpp"
#endif

#ifndef MARKET_HPP
#define MARKET_HPP
#include "market.hpp"
#endif

using namespace std;

class Stock;
class Bot;


class Strategy
{
public:
    Strategy(){}
    virtual ~Strategy(){}
    virtual void watchout_all(Market*) = 0;
    virtual Order* track(Stock*) = 0; 
    virtual void set_bot(Bot*) = 0;
};

class Bot
{
    Account * account;
    Market * market;
    Strategy * strategy;

public:

    Bot(Account* acc, Market* m, Strategy* s)
    {
        account = acc;
        market = m;
        strategy = s;
        strategy->watchout_all(market);
        strategy->set_bot(this);
    }

    void endbot()
    {
        strategy = nullptr;
    }

    ~Bot(){}
};

class Buydip : public Strategy, public Observer
{
    map<Stock*, int> stock_dips;
    int quantity;
    Account* account;
    Market * market;
    Bot * bot;
    bool active;

public:
    Buydip(Market * m, Account* acc, int q = 20)
    {
        quantity = q;
        account = acc;
        market = m;
        bot = nullptr;

    }

    void end_Buydip()
    {
        active = false;
    }

    virtual void set_bot(Bot* b) override
    {
        bot = b;
    }

    void watchout(Stock*s)
    {
        active = true;
        stock_dips[s]=0;
        this->subscribe(s, 0);
    }

    virtual void watchout_all(Market *m) override
    {
        auto s = m->get_all_stocks();
        for(auto e : s)
            watchout(e);
        cout << "Added all stocks to watch list.\n";
    }

    virtual void action(Stock* s) override
    {
        if(active)
            track(s);
    }

    virtual void action(Subject* s) override
    {
        if(active)
            track((Stock*)s);
    }

    virtual Order* track(Stock*s) override
    {
        if(s->get_prev_value() > s->get_value())
        {
            stock_dips[s]++;
            if(stock_dips[s] == 3)
            {
                stock_dips[s] = 0;
                // cout << s->name << endl;
                Order* o = new Order(market, 1, s->name, 10);
                if(o->execute(account))
                    return o;
                else
                    end_Buydip();

            }
        }
        return nullptr;
    }

    ~Buydip(){}
    
}; 