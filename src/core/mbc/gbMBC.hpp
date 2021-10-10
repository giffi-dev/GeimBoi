#pragma once
#include <cstdint>
#include <string>
#include <cstring>

namespace Giffi
{
	class gbCart;

	enum class gbCartType
	{
		None = 0,
		MBC1 = 1,
		MBC2 = 2,
		MBC3 = 3,
	};

	/*
		MBC (Memory Bank Controller) is essentually a mapper on the cartridge,
		but there can also be a Battery, RAM or other hardware.
		Basicly inherited classes accounts for any extra hardware in the cartridge.
	*/
	class gbMBC
	{
	public:
		virtual ~gbMBC() = default;

		virtual uint16_t GetCurRomBank() const { return 0; }
		virtual uint16_t GetCurRamBank() const { return 0; }

		virtual bool SaveBattery(const std::string& _path) { printf("No MBC\n"); return false; }
		virtual bool LoadBattery(const std::string& _path) { printf("No MBC\n"); return false; }

		virtual uint8_t ReadByte(uint16_t _addr) const;
		virtual void WriteByte(uint16_t _addr, uint8_t _data);
		virtual void Reset() {}

		/*
			Gets the correct "mapper" for the cartridge.
		*/
		static std::unique_ptr<gbMBC> CreateMBC(gbCart* _cart);
	protected:
		gbCart* mCart = nullptr;
		
		bool SaveBatteryImpl(const std::string& _path, uint8_t* src, size_t size);
		bool LoadBatteryImpl(const std::string& _path, uint8_t* dst, size_t size);

		gbMBC(gbCart* _cart)
			:mCart(_cart) {};
	};
}

