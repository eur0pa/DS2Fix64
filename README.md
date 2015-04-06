# DS2Fix64

Random fixes for Dark Souls II: Scholar of the First Sin (WIP)

## How to use

1. download the binary from the [release page](https://github.com/eur0pa/DS2Fix64/releases)
2. unpack it in \Dark Souls II Scholar of the First Sin\Game (in your Steam installation directory)
3. play the game

## Current fixes

### Durability bug

Dark Souls II would damage your weapons based on your framerate. SotFS will just apply the damage twice regardless. Let's not do that.

#### Details

```
[ DarkSoulsII.exe+1F4830 ]

[... snip ...]

.text:00000001401F4927     movss   xmm2, dword ptr [rax+154h]
.text:00000001401F492F [1] jmp     short ComputeWeaponDurabilityDamage

[... snip ...]

.text:00000001401F4939 [2] ComputeWeaponDurabilityDamage:
.text:00000001401F4939     mov     rax, [rdi+8]
.text:00000001401F493D     lea     edx, [rsi-2]
.text:00000001401F4940     xor     r9d, r9d
.text:00000001401F4943     mov     rcx, [rax+0B8h]
.text:00000001401F494A     movss   xmm0, dword ptr [rcx+414h]
.text:00000001401F4952     mov     rcx, rdi
.text:00000001401F4955 [3] mulss   xmm0, dword ptr [rbp+18h]
.text:00000001401F495A [4] mulss   xmm2, xmm0
.text:00000001401F495E [5] xorps   xmm2, cs:xmmword_1410A46C0
.text:00000001401F4965 [6] call    ApplyDurabilityDamage
```

here lies the routine responsible for some of the durability damage management. Looking further down @ 1F492F[1] you can see a jmp short to 1F4939[2] — that's where the weapon durability damage is applied. There, the game will compute the damage[3][4][5] and call the routine[6] to apply said damage.

```
[ DarkSoulsII.exe+1F4D80 ]

.text:00000001401F4D80 [7] mov     [rsp+10h], rbx
.text:00000001401F4D85 [8] mov     [rsp+18h], rbp

[... snip ...]

.text:00000001401F4DE2     movzx   r8d, bpl
.text:00000001401F4DE6     mov     edx, esi
.text:00000001401F4DE8     mov     rcx, rdi
.text:00000001401F4DEB     call    GetCurrentDurability [ @ +348A00 ]

[... snip ...]

.text:00000001401F4E18     mov     edx, ebx
.text:00000001401F4E1A     mov     rcx, rdi
.text:00000001401F4E1D     movaps  xmm6, xmm0
.text:00000001401F4E20     call    GetMaxDurability [ @ +342560 ]

[... snip ...]

.text:00000001401F4E30     addss   xmm6, xmm8                  ; apply damage

[... snip ...]

.text:00000001401F4E64 loc_1401F4E64:
.text:00000001401F4E64     movss   xmm2, dword ptr [rax]       ; save new durability
.text:00000001401F4E68     lea     rcx, [rsp+88h+var_60]
.text:00000001401F4E6D     mov     edx, esi
.text:00000001401F4E6F     call    SetNewDurability [ @ +151E90]
```

We'll hook this baby here at its first 5 bytes[7], and jump to our custom naked function (in \_Durability.asm), which will handle the halving of the damage (stored in the **xmm2** registry, set by the previous function at +1F495E). This is necessary since inline assembly is not possible in MSVC_x64 and I seem too stupid to use \_\_fastcall without trashing the shit out of every single register x86_64 has to offer. Thanks, Microsoft.

We'll just get **xmm2**, halve it and jump back to the trampoline function created by MinHook, where the 5 bytes we overwritten beforehand are stored, then we'll jump into Dark Souls II again and let the game go by its business[8].


## Incoming fixes

* +14 weapons save corruption. Is it still there? Drop me a line.
* Namecrash / assert bug. Still alive? Drop me a line.


## Things I'll be on the lookout for

* Framerate unlock (>60fps)


## Thanks

* Everyone in [here](http://redd.it/31i7nb) minus the spambots
* K. J., F. T., Y. B., R. F. and O. V. B., who donated the necessary amount to grant me a copy of SotFS. If you want your names to be shown just hit me : )
* [Atvaark](https://github.com/Atvaark)
* [/dsg/](https://boards.4chan.org/vg/catalog#s=/dsg/), magnificient bastards
