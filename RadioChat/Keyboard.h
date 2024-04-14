#pragma once
#include <stdint.h>
#include <functional>

/* Class for 74HC165 */
class Keyboard
{
public:
    using KeyCallback = std::function<void(uint8_t keyNum1, uint8_t keyNum2, uint8_t keyNum3)>;

    Keyboard(uint8_t SH_LD, // pin 1  - SH/LD 
             uint8_t INH,   // pin 15 - INH
             uint8_t QH,    // pin 9 - QH
             uint8_t CLK,   // pin 2 - CLK
             uint8_t countRegisters,
             KeyCallback onKeyUp);
    ~Keyboard();
    void init();
    void check();

private:
    void get(uint8_t& key1, uint8_t& key2, uint8_t& key3);

    uint8_t shPin_;
    uint8_t inhPin_;
    uint8_t qhPin_;
    uint8_t clkPin_;
    uint8_t countRegisters_;
    KeyCallback onKeyUp_;
    uint8_t key1_;
    uint8_t key2_;
    uint8_t key3_;
    uint8_t key1_prev;
    uint8_t key2_prev;
    uint8_t key3_prev;
};