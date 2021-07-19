#include "gbCart.hpp"
#include <cstdio>   // fopen
#include <string.h>
#include <filesystem>

using namespace Giffi;

std::string gbCart::GetGameName() const
{
    char name[12] = "";
    memcpy( &name, mCart + 0x134, sizeof(char) * 11 );
    return std::string(name);
}

uint8_t gbCart::GetGameVersion() const
{
    return mCart[0x14C];
}

// In Kb (KiloBytes), 1 rom bank is 16kb
uint16_t gbCart::GetRomSize() const
{
    return (32 << mCart[0x148]);
}

uint16_t gbCart::GetCurRomBank() const
{
    return mMBC->GetCurRomBank();
}

// 1 rom bank is 16kb mCart[0x148] * 4
uint16_t gbCart::GetRomBankCount() const
{
    return GetRomSize() / 16;
}

// In Kb (KiloBytes), 1 ram bank is 8kb
uint16_t gbCart::GetRamSize() const
{
    switch (mCart[0x149])
    {
        case 0x1: return 2;
        case 0x2: return 8;
        case 0x3: return 64;
        case 0x4: return 256;
        case 0x5: return 512;
        default: return 0;
    }
}

uint16_t gbCart::GetRamBankCount() const
{
    uint16_t ram = GetRamSize();
    if (ram == 2) { return 1; }
    return ram / 8;
}

uint16_t gbCart::GetCurRamBank() const
{
    return mMBC->GetCurRamBank();
}

gbCartType gbCart::GetCartType() const
{
    switch (mCart[0x147])
    {
        case 0x01: // MBC1
        case 0x02: // MBC1+RAM
        case 0x03: // MBC1+RAM+BATTERY
            return gbCartType::MBC1;

        case 0x04: // MBC2
        case 0x05: // MBC2+BATTERY
        case 0x06: // MBC2+RAM+BATTERY
            return gbCartType::MBC2;
        
        case 0x11: // MBC3
        case 0x12: // MBC3+RAM
        case 0x13: // MBC3+RAM+BATTERY
            return gbCartType::MBC3;

        default: // None or unsupported
            return gbCartType::None;
    } 
}

std::string gbCart::GetCartTypeText() const
{
    switch ( GetCartType() )
    {
        case gbCartType::None:
            return "NONE";
        case gbCartType::MBC1:
            return "MBC1";
        case gbCartType::MBC2:
            return "MBC2";
        case gbCartType::MBC3:
            return "MBC3";
        default:
            return "Unknown";
    }
}

void gbCart::Reset()
{
    if (mMBC != nullptr) { mMBC->Reset(); }
}

void gbCart::LoadRom(const std::string& _path)
{
    if (!std::filesystem::exists(_path))
    {
        printf("Can't find a file at %s\n", _path.c_str());
        mGameLoaded = false;
        return;
    }

    // Init Cart
    memset(mCart,0,sizeof(mCart));
    FILE *in;
    in = fopen( _path.c_str(), "rb" );
    fread(mCart, 1, 0x200000, in);
    fclose(in); 
    
    mMBC = std::unique_ptr<gbMBC>(gbMBC::CreateMBC(this));
    mGameLoaded = true;

    // Reset cart variables to defaults
    Reset();
}

uint8_t gbCart::ReadByte(uint16_t _addr) const
{
    return mMBC->ReadByte(_addr);
}

void gbCart::WriteByte(uint16_t _addr, uint8_t _data)
{
    mMBC->WriteByte(_addr, _data);
}

bool gbCart::IsGameLoaded() const
{
    return mGameLoaded;
}