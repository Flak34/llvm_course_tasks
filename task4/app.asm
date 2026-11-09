FUNC compute_julia(IR51, IR52, IR53, FR51, FR52):
; IR51 - указатель на буфер, IR52 - width, IR53 - height, FR51 - cx, FR52 - cy

julia_entry_label:
    CMP_GT IR54, IR53, 0
    JNZ julia_label_1, IR54
    JMP julia_label_4

julia_label_1:
    CMP_GT IR55, IR52, 0
    SITOFP FR53, IR52
    MUL FR54, FR53, 0.5
    SITOFP FR55, IR53
    MUL FR56, FR55, 0.5
    MOV IR56, 0
    JMP julia_label_2

julia_label_2:
    JNZ julia_label_3, IR55
    JMP julia_label_5

julia_label_3:
    SITOFP FR57, IR56
    SUB FR58, FR57, FR56
    MUL FR59, FR58, 3.0
    DIV FR60, FR59, FR55
    MUL FR61, FR60, FR60
    MOV IR58, 0
    JMP julia_label_6

julia_label_4:
    RET

julia_label_5:
    INC_EQ IR67, IR56, IR53
    JNZ julia_label_4, IR67
    JMP julia_label_2

julia_label_6:
    SITOFP FR62, IR58
    SUB FR63, FR62, FR54
    MUL FR64, FR63, 3.0
    DIV FR65, FR64, FR53
    MULADD FR69, FR65, FR65, FR61
    CMP_LT IR61, FR69, 4.0
    MOV FR66, FR65
    MOV FR67, FR60
    MOV FR68, FR61
    MOV IR59, 0
    JNZ julia_label_7, IR61
    JMP julia_label_8

julia_label_7:
    MULSUB FR70, FR66, FR66, FR68
    MUL2ADD FR67, FR66, FR67, FR52
    MUL FR68, FR67, FR67
    ADD FR66, FR70, FR51
    MULADD FR71, FR66, FR66, FR68
    CMP_LT IR62, FR71, 4.0
    CMP_LT IR63, IR59, 254
    AND IR64, IR62, IR63
    ADD IR59, IR59, 1
    JNZ julia_label_7, IR64

julia_label_8:
    MULADD IR65, IR56, IR52, IR58
    STORE_SCALE IR51, IR65, 8, IR59
    INC_EQ IR66, IR58, IR52
    JNZ julia_label_5, IR66
    JMP julia_label_6

FUNC app():
app_entry_label:
    ALLOCI IR1, 73728
    MOV FR1, 0x3F589374C0000000
    MOV FR2, 0x3F60624DE0000000
    MOV FR3, 0x3FD14A2340000000
    MOV FR4, 0xBFE6666660000000

app_label_1:
    CALL compute_julia, IR1, 384, 192, FR4, FR3
    MOV IR2, 0

app_label_2:
    MOV IR3, 0
    JMP app_label_5

app_label_3:
    FLUSH
    ADD FR12, FR3, FR1
    ADD FR13, FR4, FR2
    NEG FR14, FR1
    NEG FR15, FR2
    CMP_LT IR15, FR12, 0x3FC99999A0000000
    CMP_LT IR16, FR13, 0xBFE99999A0000000
    CMP_GT IR17, FR12, 0x3FD6666660000000
    CMP_GT IR18, FR13, 0xBFE3333340000000
    OR IR19, IR17, IR15
    OR IR20, IR18, IR16
    SELECT FR1, IR19, FR14, FR1
    SELECT FR2, IR20, FR15, FR2
    MOV FR3, FR12
    MOV FR4, FR13
    JMP app_label_1

app_label_4:
    INC_EQ IR11, IR2, 192
    JNZ app_label_3, IR11
    JMP app_label_2

app_label_5:
    MULADD IR4, IR2, 384, IR3
    LOAD_SCALE IR5, IR1, IR4, 8
    CMP_EQ IR6, IR5, 255
    MOV IR7, 0
    JNZ app_label_7, IR6

app_label_6:
    MUL IR8, IR5, 9
    SREM IR9, IR8, 256
    MOV IR7, IR9

app_label_7:
    PUT_PIXEL IR3, IR2, IR7
    INC_EQ IR10, IR3, 384
    JNZ app_label_4, IR10
    JMP app_label_5
    RET
