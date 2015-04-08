#pragma once
#include "stdafx.h"

extern BOOL Begin();
extern BOOL End();

int ApplyDetours();
int RemoveDetours();
SIZE_T CheckDkSVersion();