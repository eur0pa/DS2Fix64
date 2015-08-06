# DS2Fix64

Random fixes for Dark Souls II: Scholar of the First Sin (WIP)

## How to use

1. download the binary from the [release page](https://github.com/eur0pa/DS2Fix64/releases)
2. unpack it in \Dark Souls II Scholar of the First Sin\Game (in your Steam installation directory)
3. play the game

## Can I get (soft)banned for using this?

Read [this](http://kotaku.com/no-ones-sure-why-players-are-suddenly-getting-banned-in-1702590460) and [this](https://github.com/eur0pa/DS2Fix64/blob/master/Softbans.md).

Yes, you will get softbanned. No, you won't get softbanned. Get it? Softbans are handled manually by a team of people. They will or will not flag you depending on a bunch of reasons and/or mods. This was stated in a softban confirmation email from BAMCO:

>On the DS2fix: The ds2fix was not an officially released FROMSOFTWARE file/mod and as such they are well within their rights to restrict any players using it. We're not really sure of the exact reasons as to why FROMSOFTWARE decides certain mods are violations of the EULA while others appear to go unnoticed (but these will most likely still lead to restrictions once FROM is aware of them) and as such we strongly recommend to our end users that they simply not use any mods, hacks, or external files with the game (since they will all be a "try at your own risk of restrictions").
 
>These are some other apps that can cause issues - using __x360ce, SweetFX, ReShade, DXTory or the Durazno Dead-zone fix.__

So there you have it. You can use DS2Fix64 and be completely fine, or get banned two days later. Or you can avoid using it and get banned regardless because of SweetFX, or X360CE without which your aftermarket gamepad doesn't even work. Shit sucks, I know. But now you do, too (know, not suck). Take care.

## Current fixes

### Durability bug -- disabled: fixed in SotFS 1.0.2

Dark Souls II would damage your weapons based on your framerate. SotFS will just apply the damage twice regardless. Let's not do that.

#### Details

```Assembly
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

here lies the routine responsible for some of the durability damage management. Looking further down @ 1F492F[1] you can see a jmp short to 1F4939[2] — that's where the weapon durability damage is applied. Here, the game will compute the damage[3][4][5] and call the routine[6] to apply said damage.

```Assembly
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

We'll hook this baby here at its first 5 bytes[7], and jump to our custom naked function (in \_Durability.asm), which will handle the halving of the damage (stored in the **xmm2** registry, set by the previous function at +1F495E). This is necessary since inline assembly is not possible in MSVC_x64 and I seem to be too dumb to invoke a \_\_fastcall without trashing the shit out of every single register x86_64 has to offer. Thanks, Microsoft.

We'll just get **xmm2**, halve it and jump back to the trampoline function created by MinHook, where the 5 bytes we overwritten beforehand are stored, then we'll jump into Dark Souls II again and let the game go by its business[8].


### +14 upgraded items crash / save corruption

Dark Souls II handles upgrades to +10 in game — anything above that (via cheeeeety means) could, can and will cause unexpected behaviour. Notably, the +14 upgrade will crash the game upon opening the inventory / equipment menu and will prevent you from dropping the item, effectively condemning that character to deletion. Let's avoid that, shall we.

#### Details

SotFS seems to manage this exception better than its predecessor — the game *will* remove the offending item and de-upgrade it to +0. Thing is, it will still crash.

Yeah. Don't ask.

```Assembly
[ DarkSoulsII.exe+1B1650 ]

00007FF629121650    movzx ecx, byte ptr ds:[rbx+25]         | ;+14 crash #1 - inject here

[ injected ]

        movzx ecx, byte ptr[rbx+25h]    ; obtain current upgrade level
        cmp ecx, 0Ah                    ; +10 max
        jbe nevermind
        xor ecx, ecx                    ; upgrade level is >10, zero it

    nevermind:
        mov rax, [rbp+10h]
        jmp [return]

[ /injected ]

00007FF629121654    mov rax, qword ptr ss:[rbp+10]          | ;first opcode lost due to hook jmp
00007FF629121658    mov byte ptr ds:[rax+A6], cl            | ;return here
00007FF62912165E    mov rax, qword ptr ss:[rbp+10]          |
00007FF629121662    movzx ecx, byte ptr ds:[rbx+26]         |
00007FF629121666    mov byte ptr ds:[rax+A7], cl            |
00007FF62912166C    mov rax, qword ptr ds:[rdi+20]          |
00007FF629121670    mov rcx, qword ptr ds:[rax+38]          |
00007FF629121674    mov edx, dword ptr ds:[rcx+14]          |
00007FF629121677    cmp edx, 319750                         |
00007FF62912167D    jnz darksoulsii.7FF629121694            |
```

```Assembly
[ DarkSoulsII.exe+37DCD ]

00007FF628FA7DCD    movzx eax, byte ptr ds:[rax+25]         | ;+14 crash #2 - inject here

[ injected ]

        movzx eax, byte ptr[rax+25h]    ; obtain current upgrade level
        cmp eax, 0Ah                    ; +10 max
        jbe nevermind
        xor eax, eax                    ; upgrade level is >10, zero it

    nevermind:
        and al, 0Fh
        mov byte ptr[rbx], al
        mov rax, rbx
        jmp [return]

[ /injected ]

00007FF628FA7DD1    and al, F                               | ;lost due to hook jmp
00007FF628FA7DD3    mov byte ptr ds:[rbx], al               | ;lost due to hook jmp
00007FF628FA7DD5    mov rax, rbx                            | ;lost due to hook jmp
00007FF628FA7DD8    mov rbx, qword ptr ss:[rsp+40]          | ;return here
00007FF628FA7DDD    mov rsi, qword ptr ss:[rsp+48]          |
00007FF628FA7DE2    add rsp, 30                             |
00007FF628FA7DE6    pop rdi                                 |
00007FF628FA7DE7    ret                                     |
```

Any idea what the fuck is going on? Hit me.


### Namecrash / assert bug

The old namecrash returns. The game isn't able to properly account for mis-tagged text strings that may or may not be voluntarily introduced into your world (read between the lines...) and will crash to desktop.

### Details

```Assembly
[ DarkSoulsII.exe+1DE7366 ]

00007FF64AC17359    jmp darksoulsii.7FF649952B57            |
00007FF64AC1735E    mov rbx, qword ptr ss:[rsp+80]          |
00007FF64AC17366    mov dword ptr ds:[0], DEADBA            | ;namecrash / assert
00007FF64AC17371    add rsp, 70                             |
00007FF64AC17375    mov rdi, qword ptr ss:[rsp]             |
```

Every time the game should encounter an improperly tagged text string, it will take a jump to the commented opcode and try and move 0xDEADBA to NULL. Endlessly, thus crashing shortly after. Bear in mind that being this an assert / part of a SEH, it cannot be simply patched out as I'm doing right now (nopping the shit out of it and requesting a short jmp to the first useful opcode), but needs more work to be deemed stable.

## Incoming fixes

* ?


## Things I'll be on the lookout for

* Framerate unlock (>60fps)


## Thanks

* Everyone in [here](http://redd.it/31i7nb) minus the spambots
* K. J., Fafniroth, Y. B., Riccardo Finello and O. V. B., who donated the necessary amount to grant me a copy of SotFS. If you want your names to be shown just hit me : )
* [Atvaark](https://github.com/Atvaark)
* [Marisa](https://github.com/OrdinaryMagician)
* [/dsg/](https://boards.4chan.org/vg/catalog#s=/dsg/), magnificient bastards
