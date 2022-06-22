#pragma once
#include <cstdint>

namespace GeimBoi
{

    class gbGameBoy;

    class apuBaseChannel
    {
    public:
        virtual double GetAmplitude(double) = 0;
    };

    class apuChannel1 : public apuBaseChannel
    {
    public:
        apuChannel1(gbGameBoy* gb);
        ~apuChannel1();

        void UpdateTimers(uint16_t cycles);
        void WriteByte(uint16_t addr, uint8_t data);
        
        virtual double GetAmplitude(double time);
    private:
        bool mEnabled = true;
        double mVolume = 0.0;
        float mCycleDuty = 0.5f; 
        uint16_t mFreq = 0b0;
        int16_t mSweepTimer = 0;

        double mSoundLength = 0.0; // seconds
        double mSoundLengthTimer = 0.0; // seconds

        double mEnvelopeTimer = 0.0;

    private:
        void DoEnvelope(uint16_t cycles);
        void DoSweep(uint16_t cycles);

        gbGameBoy* mGameBoy;
    };

}
