#include <cstdint>
#include <iostream>
#include <cassert>
#include <algorithm>

//#include "blablabla.hpp"

enum state_e : std::uint8_t {
    START=0,
    LOCKED,
    UNLOCKED,
    OUT_OF_ORDER,
    STOP,
};

enum input_e : std::uint8_t {
 PUSH = 0,
 COIN,
 ON,
 OFF,
 END,
 NONE,
};

std::string to_lower(const std::string &s) {
    std::string str{s};
    std::transform(str.begin(), str.end(), str.begin(), ::tolower);
    return str;
}

class Turnstile {
    state_e m_state{START}; //estado atual
    input_e m_input{NONE}; // oq recebeu
    size_t m_coins{0}; // contagem de moedas

public:
    
    Turnstile()=default;

    bool out_of_order() const {return m_state == OUT_OF_ORDER; }
    bool ended() const {return m_state == STOP; }
    void process_events();
    void update();
    void render();

};

void Turnstile::process_events(){
if (m_state == START)
{
return;
}
std::string command;
std::getline(std::cin, command);

command = to_lower(command);
if (command == "coin" or command == "c")
{
    m_input = COIN;
}
//... OS PROXIMOS É ANÁLOGO COMO: push, end off

}

void Turnstile::update(){
if (m_state==START)
{
m_state = LOCKED;
return;
}
if (m_state == LOCKED)
{
    if (m_input == COIN)
    {
        m_state = UNLOCKED;
        m_coins++;
    }
    if (m_input == OFF)
    {
        m_state = OUT_OF_ORDER;
    }
    return;
}
if (m_state == UNLOCKED)
{
    if (m_input == PUSH)
    {
        m_state = LOCKED;
    }
    if (m_input == OFF)
    {
        m_state = OUT_OF_ORDER;
    }
    if (m_input == COIN)
    {
        m_coins++;
    }
    return;
}
if (m_state == OUT_OF_ORDER)
{
    if (m_input == ON)
    {
        m_state = LOCKED;
    }
    if (m_input == END)
    {
        m_state = STOP;
    }
    
}


}

void Turnstile::render(){
if (m_state == OUT_OF_ORDER)
{
    std::cout << "";
    std::cout << "" << std::flush;
    return;
}
if (m_state == LOCKED)
{
    std::cout << "";
    std::cout << "" << std::flush;
    return;
}
if (m_state == UNLOCKED)
{
    std::cout << "";
    std::cout << "" << std::flush;
    return;
}
 if (m_state == START)
 {
    assert("blablabla" && false);
 }
 
}


int main () {
    Turnstile ts;

    while (not ts.ended())
    {
        ts.process_events();
        ts.update();
        ts.render();
    }
    

    return 0;
}