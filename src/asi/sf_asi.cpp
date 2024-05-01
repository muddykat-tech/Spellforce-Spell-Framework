#include "sf_asi.h"

namespace ASI
{
	/// allows rewriting code in a given memory region by overwriting permissions for that memory region
	bool BeginRewrite(MemoryRegion& mem_region)
	{
		return VirtualProtect((LPVOID)mem_region.memory_offset, (SIZE_T)mem_region.memory_length, 0x40, &mem_region.old_reg_perm);   // 0x40 - enable everything?
	}

	/// ends rewriting by restoring old permissions in a given memory region
	bool EndRewrite(MemoryRegion& mem_region)
	{
		DWORD tmp_old_region_permission;
		bool b = VirtualProtect((LPVOID)mem_region.memory_offset, (SIZE_T)mem_region.memory_length, mem_region.old_reg_perm, &tmp_old_region_permission) != 0;
		if (b)
			FlushInstructionCache((HANDLE)0xFFFFFFFF, 0, 0);
		return b;
	}

 	unsigned int GAME_BASE; 
 	int WINDOW_OFFSET;
 	unsigned int APPMAIN_OFFSET;
	void SetGameBase()
	{
		GAME_BASE = (unsigned int)GetModuleHandleA("spellforce.exe");
	}

	/// required for everything to work... why?
	bool Init(HMODULE lib_module)
	{
		if (!DisableThreadLibraryCalls(lib_module))
			return false;
		ASI::SetGameBase();
		if (CheckSFVersion(SF_154))
		{
			WINDOW_OFFSET = 0x97CB5C;
			APPMAIN_OFFSET = 0x9644D0;
		}
		if (CheckSFVersion(SF_BETA))
		{
			WINDOW_OFFSET = 0x93A25C;
			APPMAIN_OFFSET = 0x9229A8; //FIXME
		}
		return true;
	}

	/// check version of the game that was hooked into
	bool __stdcall CheckSFVersion(SF_Version sf_version)
	{
		switch (sf_version)
		{
		case SF_154:
			if (*(int*)ASI::AddrOf(0xEFF8) == 75000) // 1.54.75000
				return true;
			break;
		case SF_161:
			if (*(int*)ASI::AddrOf(0x1E6EE6) == 10180) //1.61.10180
				return true;
			break;
		case SF_BETA:
			if (*(int*)ASI::AddrOf(0x177e63) == 11213) //1.61.11213
				return true;
		default:
			return false;
		}
		return false;
	}
}