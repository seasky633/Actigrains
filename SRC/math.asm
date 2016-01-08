/**********************************************************************************************
* void delay(Word n)
* note: delay 20*(n+2)*Tsys
**********************************************************************************************/
?PR?_delay?delay SEGMENT CODE         
PUBLIC _delay
RSEG  ?PR?_delay?delay
_delay:                                                                
                                        ;2T        MOV        R7,xx        
                                        ;2T        MOV        R6,xx
                                        ;7T        LCALL
        nop                             ;1T
        ;-----------------------------------------------------        
??delay_1:
        mov        A,R7                    ;1T
        jz        ??delay_2                ;3T        JZ        no taken(3T)
        dec        R7                      ;2T
        nop                                ;1T
        nop                                ;1T
        nop                                ;1T
        nop                                ;1T
        nop                                ;1T
        nop                                ;1T
        nop                                ;1T
        nop                                ;1T
        nop                                ;1T
        nop                                ;1T
        sjmp        ??delay_1              ;4T
??delay_2:
                                           ;2T        JZ        taken(add more 2T)
        mov        A,R6                    ;1T
        jz        ??delay_3                ;3T        JZ        no taken(3T)
        dec        R6                      ;2T
        mov        R7,#0xff                ;2T
        nop                                ;1T
        nop                                ;1T
        sjmp        ??delay_1              ;4T
??delay_3:
                                           ;2T        JZ        taken(add more 2T)
        nop                                ;1T
        nop                                ;1T
        nop                                ;1T
        nop                                ;1T
        nop                                ;1T
        nop                                ;1T
        nop                                ;1T
        nop                                ;1T
        ;-----------------------------------------------------
        RET                                ;8T


end
