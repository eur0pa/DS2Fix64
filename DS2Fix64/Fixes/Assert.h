#pragma once
#include "stdafx.h"

typedef void(__stdcall *NamecrashAssert)();
extern NamecrashAssert oNamecrashAssert;
extern BYTE tNamecrashAssert[];
extern BOOL PatchNamecrashAssert();