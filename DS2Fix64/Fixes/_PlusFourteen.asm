;
; i fucking hate you for this, microsoft
; why, jesus christ on a pogo stick, why
;

;
; return address for the trampoline functions
;
EXTERN bPlusFourteen_1:QWORD
EXTERN bPlusFourteen_2:QWORD

;
; executable code
; thanks Marisa: bouncing off ideas is always helpful : }
;
.code

    tPlusFourteen_1 proc
        
        movzx ecx, byte ptr[rbx+25h]    ; obtain current upgrade level
        cmp ecx, 0Ah                    ; +10 max
        jbe nevermind
        xor ecx, ecx                    ; upgrade level is >10, zero it

    nevermind:
        mov rax, [rbp+10h]
        jmp [bPlusFourteen_1]

    tPlusFourteen_1 endp

    ; --------------------------------

    tPlusFourteen_2 proc
        
        movzx eax, byte ptr[rax+25h]    ; obtain current upgrade level
        cmp eax, 0Ah                    ; +10 max
        jbe nevermind
        xor eax, eax                    ; upgrade level is >10, zero it

    nevermind:
        and al, 0Fh
        mov byte ptr[rbx], al
        mov rax, rbx
        jmp [bPlusFourteen_2]

    tPlusFourteen_2 endp

end