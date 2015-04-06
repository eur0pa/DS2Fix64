#pragma once
#include "stdafx.h"

typedef void(__stdcall *ApplyDurabilityDamage)();
extern ApplyDurabilityDamage oApplyDurabilityDamage;
extern "C" ApplyDurabilityDamage bApplyDurabilityDamage;
extern "C" void __stdcall tApplyDurabilityDamage();