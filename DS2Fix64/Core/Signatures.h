#pragma once
#include "stdafx.h"

Signature fsApplyDurabilityDamage = {
    // 1.0.1 @ 00007FF629164D80 ;; <darksoulsii.SetNewDurability>
    "\x48\x89\x5C\x24\x10"      // mov qword ptr ss:[rsp+10], rbx <<< function start
    // --- shortest sig ends here --- mask: xxxxx
    "\x48\x89\x6C\x24\x18"      // mov qword ptr ss:[rsp+18], rbp
    "\x56"                      // push rsi
    "\x57"                      // push rdi
    "\x41\x56"                  // push r14
    "\x48\x83\xEC\x70"          // sub rsp, 70
    "\x48\x8B\xD9"              // mov rbx, rcx
    "\x48\x8B\x49\x08"          // mov rcx, qword ptr ds:[rcx+8]
    "\x44\x0F\x29\x44\x24\x40", // movaps dqword ptr ss:[rsp+40], xmm8
    "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx",
    strlen(fsApplyDurabilityDamage.mask),
    0
};

Signature fsPlusFourteenCrash_1 = {
    // 1.0.1 @ 00007FF629121650 ;; +14 crash #1
    "\x0F\xB6\x4B\x25"          // movzx ecx, byte ptr ds:[rbx+25] <<< highlighted item upgrade level
    "\x48\x8B\x45\x10"          // mov rax, qword ptr ss:[rbp+10]  
    "\x88\x00\x00\x00\x00\x00"  // mov byte ptr ds:[rax+A6], cl    <<< return here, not above
    "\x48\x8B\x45\x10"          // mov rax, qword ptr ss:[rbp+10] 
    "\x0F\xB6\x4B\x26"          // movzx ecx, byte ptr ds:[rbx+26]
    "\x88\x00\x00\x00\x00\x00"  // mov byte ptr ds:[rax+A7], cl   
    "\x48\x8B\x47\x20"          // mov rax, qword ptr ds:[rdi+20] 
    "\x48\x8B\x48\x38"          // mov rcx, qword ptr ds:[rax+38] 
    "\x8B\x51\x14",             // mov edx, dword ptr ds:[rcx+14] 
    "xxxxxxxxx?????xxxxxxxxx?????xxxxxxxxxxx",
    strlen(fsPlusFourteenCrash_1.mask),
    8
};

Signature fsPlusFourteenCrash_2 = {
    // 1.0.1 @ 00007FF628FA7DCD ;; +14 crash #2
    "\x0F\xB6\x40\x25"      // movzx eax, byte ptr ds:[rax+25] <<< highlighted item upgrade level
    "\x24\x0F"              // and al, F
    // --- shortest sig ends here --- mask: xxxxxx
    "\x88\x03"              // mov byte ptr ds:[rbx], al
    "\x48\x8B\xC3"          // mov rax, rbx
    "\x48\x8B\x5C\x24\x40"  // mov rbx, qword ptr ss:[rsp+40]  <<< return here, not above
    "\x48\x8B\x74\x24\x48"  // mov rsi, qword ptr ss:[rsp+48]
    "\x48\x83\xC4\x30"      // add rsp, 30 
    "\x5F"                  // pop rdi
    "\xC3",                 // ret
    "xxxxxxxxxxxxxxxxxxxxxxxxxxx",
    strlen(fsPlusFourteenCrash_2.mask),
    11
};

Signature fsNamecrashAssert = {
    // 1.0.1 @ 00007FF64AC17366
    "\xC7\x04\x25\x00\x00\x00\x00\xBA\xAD\xDE\x00",  // mov dword ptr ds:[0], DEADBA | ; namecrash / assert
    "xxxxxxxxxxx",
    strlen(fsNamecrashAssert.mask),
    0
};