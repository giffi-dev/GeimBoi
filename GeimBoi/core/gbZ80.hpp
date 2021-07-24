#pragma once
#include "reg.hpp"

namespace Giffi
{

#define CPU_CLOCKSPEED 1048576
#define FLAG_Z 7 // Zero Flag, set if previous operation resulted in a zero
#define FLAG_N 6 // Substract Flag, set if previous operation substracted otherwise 0
#define FLAG_H 5 // Half Carry Flag, 
#define FLAG_C 4 // Carry Flag

#define INTERUPT_VBLANK 0U
#define INTERUPT_LCD    1U
#define INTERUPT_TIMER  2U
#define INTERUPT_SERIAL 3U
#define INTERUPT_JOYPAD 4U

class gbGameBoy;
class gbPPU;

enum class gbFlag
{
    Zero      = 7,
    Substract = 6,
    HalfCarry = 5,
    Carry     = 4,
};

enum class gbInterrupt
{
    VBlank = 0,
    LCD    = 1,
    Timer  = 2,
    Serial = 3,
    Joypad = 4,
};

class gbZ80
{
public:
    gbZ80(gbGameBoy* _gameboy)
        : mGameBoy(_gameboy) { Reset(); }
    ~gbZ80() = default;

    void FrameAdvance();
    void Clock();
    void Reset();

    // Only public because of opcodes
    uint8_t  ReadByte(uint16_t _addr) const;
    void     WriteByte(uint16_t _addr, uint8_t _data);
    uint16_t ReadWord() const;
    void     PushWordOntoStack(uint16_t _word);
    uint16_t PopWordOffStack  ();


    bool GetFlag(gbFlag flag)   const { return (mRegAF.low >> (int)flag) & 1; }
    bool GetIE(gbInterrupt i)   const { return (ReadByte(0xFFFF) >> (int)i) & 1; } // Get 'Interrupt Enable'
    bool GetIF(gbInterrupt i)   const { return (ReadByte(0xFF0F) >> (int)i) & 1; } // Get 'Interrupt Flag'
    bool GetIME()               const { return mEnableInterrupts; }                // 'Interrupt Master Enable'
    bool IsHalted()             const { return mIsHalted; }
private:
    void ExecuteNextOpcode();
    void ExecuteExtendedOpcode();


    void UpdateTimers( uint16_t cycles );
    void RequestInterupt(uint8_t _id);
    void DoInterupts();
    void ServiceInterupt(uint8_t _interupt);

public:
    Reg16 mRegAF, mRegBC, mRegDE, mRegHL, mRegSP, mRegPC;
    bool mIsHalted = false;
    bool mEnableInterrupts = true;
    unsigned int mCounterFreq    = 0; // How many cycles does it take to count the counter once.
    unsigned int mDividerCounter = 0; // Counts the diviertimer cycles, till count up.
    unsigned int mTimerCounter   = 0; // Counts the timer cycles, till count up.
    unsigned int mCyclesDone = 0;

    // Debugging
    uint16_t mLastExecutedOpcode = 0x00;
private:
    gbGameBoy* mGameBoy = nullptr;

    friend gbGameBoy;
    friend gbPPU;
};

} // Namespace