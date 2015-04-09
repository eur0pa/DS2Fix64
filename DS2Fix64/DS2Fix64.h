#pragma once
#include "stdafx.h"

extern BOOL Begin();
extern BOOL End();

extern int ApplyDetours();
extern int RemoveDetours();
extern SIZE_T CheckDkSVersion();