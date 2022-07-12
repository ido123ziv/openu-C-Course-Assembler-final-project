
; comment - test it out

MAIN: mov S1.1 ,LENGTH
 add r2,STR
LOOP: jmp END
macro m1
 inc K
 mov S1.2 ,r3
endmacro
 prn #-5
 sub r1, r4
m1
 bne LOOP
END: hlt
STR: .string "abcdef"
LENGTH: .data 6,-9,15
K: .data 22
S1: .struct 8, "ab"