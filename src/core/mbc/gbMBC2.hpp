#pragma once
#include "gbMBC.hpp"

namespace GeimBoi
{
	// Supports ROM sizes up to 2mb (16 banks), and has a internal ram of 512 x 4bits
	class gbMBC2 : public gbMBC
	{
	public:
		~gbMBC2() override;

		uint16_t GetCurRomBank() const override;

		bool SaveBattery(const std::string& path) override;
		bool LoadBattery(const std::string& path) override;

		uint8_t ReadByte(uint16_t addr) const        override;
		void    WriteByte(uint16_t addr, uint8_t data) override;
		void    Reset() override;
	private:
		bool mRamEnable = false; // aka 'RAMG'
		uint8_t mCurBank = 0x01; // aka 'ROMB'
		uint8_t mRam[0x200];     // Interal ram, only 4 lower bits used.

	protected:
		gbMBC2(gbCart* cart);
		friend gbMBC;
	};

} // Namespace

