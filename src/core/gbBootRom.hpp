#pragma once
#include <cstdint>
#include <string>

namespace GeimBoi
{
	class gbGameBoy;
	class gbBootRom
	{
	public:
		gbBootRom() = default;
		~gbBootRom() = default;

		// Retruns true if the load was successful
		bool LoadBios(const std::string& path);
		uint8_t ReadByte(uint16_t addr) const;

		inline void Reset() { mLoaded = true; }
		inline bool IsBiosLoaded() const { return mLoaded && mHasBios; }

	private:
		uint8_t mBootRom[0x100] = {};
		mutable bool mLoaded = false; // Mapped into the gameboy's bus?
		bool mHasBios = false;        // If the boot rom was loaded successfully? 
	};

	

} // Namespace