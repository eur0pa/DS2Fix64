;
; give me my fucking x64 inline assembly back, microsoft.
; this has to be the most retarded shit i've been forced
; to do to date. jesus christ.
;


;
; return address for the trampoline function
; this exists in the main obj and it's being set by minhook
;
EXTERN bApplyDurabilityDamage:QWORD


;
; variables
;
.data
    factor  REAL4   2.0

;
; executable code
;
.code

    tApplyDurabilityDamage proc

        divss xmm2, [factor]            ; obtain a single tick of damage by halving the original value (sotfs only)
        jmp [bApplyDurabilityDamage]    ; go back to the trampoline, then back to darksouls ii

    tApplyDurabilityDamage endp


end