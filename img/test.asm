00000000  skipping 0xDC bytes
000000DC  80FA8F            cmp dl,0x8f
000000DF  7F04              jg 0xe5
000000E1  88162D06          mov [0x62d],dl
000000E5  BE8707            mov si,0x787
000000E8  E88D00            call word 0x178
000000EB  BEBE07            mov si,0x7be
000000EE  31C0              xor ax,ax
000000F0  B90400            mov cx,0x4
000000F3  F60480            test byte [si],0x80
000000F6  7403              jz 0xfb
000000F8  40                inc ax
000000F9  89F5              mov bp,si
000000FB  81C61000          add si,0x10
000000FF  E2F2              loop 0xf3
00000101  48                dec ax
00000102  7402              jz 0x106
00000104  CD18              int 0x18
00000106  BF0500            mov di,0x5
00000109  BE1D06            mov si,0x61d
0000010C  C744020100        mov word [si+0x2],0x1
00000111  668B4608          mov eax,[bp+0x8]
00000115  66894408          mov [si+0x8],eax
00000119  B80042            mov ax,0x4200
0000011C  8A162D06          mov dl,[0x62d]
00000120  CD13              int 0x13
00000122  730D              jnc 0x131
00000124  4F                dec di
00000125  7449              jz 0x170
00000127  30E4              xor ah,ah
00000129  8A162D06          mov dl,[0x62d]
0000012D  CD13              int 0x13
0000012F  EBD8              jmp short 0x109
00000131  A1FE7D            mov ax,[0x7dfe]
00000134  3D55AA            cmp ax,0xaa55
00000137  7537              jnz 0x170
00000139  FA                cli
0000013A  66A14C00          mov eax,[0x4c]
0000013E  66A33F06          mov [0x63f],eax
00000142  BE1304            mov si,0x413
00000145  8B04              mov ax,[si]
00000147  48                dec ax
00000148  8904              mov [si],ax
0000014A  C1E006            shl ax,byte 0x6
0000014D  8EC0              mov es,ax
0000014F  31FF              xor di,di
00000151  BE1D06            mov si,0x61d
00000154  B96000            mov cx,0x60
00000157  FC                cld
00000158  F3A5              rep movsw
0000015A  C7064C001700      mov word [0x4c],0x17
00000160  A34E00            mov [0x4e],ax
00000163  FB                sti
00000164  8A162D06          mov dl,[0x62d]
00000168  89EE              mov si,bp
0000016A  FA                cli
0000016B  EA007C0000        jmp word 0x0:0x7c00
00000170  BEAA07            mov si,0x7aa
00000173  E80200            call word 0x178
00000176  EBFE              jmp short 0x176
00000178  AC                lodsb
00000179  20C0              and al,al
0000017B  7409              jz 0x186
0000017D  B40E              mov ah,0xe
0000017F  BB0700            mov bx,0x7
00000182  CD10              int 0x10
00000184  EBF2              jmp short 0x178
00000186  C3                ret
00000187  53                push bx
00000188  7461              jz 0x1eb
0000018A  7274              jc 0x200
0000018C  20626F            and [bp+si+0x6f],ah
0000018F  6F                outsw
00000190  7469              jz 0x1fb
00000192  6E                outsb
00000193  67206672          and [esi+0x72],ah
00000197  6F                outsw
00000198  6D                insw
00000199  205553            and [di+0x53],dl
0000019C  42                inc dx
0000019D  206465            and [si+0x65],ah
000001A0  7669              jna 0x20b
000001A2  63652E            arpl [di+0x2e],sp
000001A5  2E2E0D0A00        cs or ax,0xa
000001AA  42                inc dx
000001AB  6F                outsw
000001AC  6F                outsw
000001AD  7420              jz 0x1cf
000001AF  6661              popad
000001B1  696C656400        imul bp,[si+0x65],word 0x64
000001B6  0000              add [bx+si],al
000001B8  EAEBD4CA00        jmp word 0xca:0xd4eb
000001BD  0000              add [bx+si],al
000001BF  0000              add [bx+si],al
000001C1  0000              add [bx+si],al
000001C3  0000              add [bx+si],al
000001C5  0000              add [bx+si],al
000001C7  0000              add [bx+si],al
000001C9  0000              add [bx+si],al
000001CB  0000              add [bx+si],al
000001CD  0000              add [bx+si],al
000001CF  0000              add [bx+si],al
000001D1  0000              add [bx+si],al
000001D3  0000              add [bx+si],al
000001D5  0000              add [bx+si],al
000001D7  0000              add [bx+si],al
000001D9  0000              add [bx+si],al
000001DB  0000              add [bx+si],al
000001DD  0000              add [bx+si],al
000001DF  0000              add [bx+si],al
000001E1  0000              add [bx+si],al
000001E3  0000              add [bx+si],al
000001E5  0000              add [bx+si],al
000001E7  0000              add [bx+si],al
000001E9  0000              add [bx+si],al
000001EB  0000              add [bx+si],al
000001ED  00800101          add [bx+si+0x101],al
000001F1  000B              add [bp+di],cl
000001F3  FE                db 0xfe
000001F4  7FE9              jg 0x1df
000001F6  3F                aas
000001F7  0000              add [bx+si],al
000001F9  00823F78          add [bp+si+0x783f],al
000001FD  0055AA            add [di-0x56],dl
