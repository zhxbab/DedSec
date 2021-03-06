; import global function
extern real_divide_error
extern real_single_step_exception
extern real_nmi
extern real_breakpoint_exception
extern real_overflow
extern real_bounds_check
extern real_inval_opcode
extern real_copr_not_available
extern real_double_fault
extern real_copr_seg_overrun
extern real_inval_tss
extern real_segment_not_present
extern real_stack_exception
extern real_general_protection
extern real_page_fault
extern real_copr_error
extern real_int_8259_0
extern real_int_8259_1
extern real_int_8259_2
extern real_int_8259_3
extern real_int_8259_4
extern real_int_8259_5
extern real_int_8259_6
extern real_int_8259_7
extern real_int_8259_8
extern real_int_8259_9
extern real_int_8259_10
extern real_int_8259_11
extern real_int_8259_12
extern real_int_8259_13
extern real_int_8259_14
extern real_int_8259_15
extern real_int_80h
extern current_thread_id
extern Task_frame
extern restart_next_task
extern sys_get_tick


STACK_FRAME_COUNT 	equ 	18
THREAD_FRAME_SIZE 	equ 	90
SYSCALL_GET_TICK	equ		0x1

[section .text]	; 代码在此
global	divide_error
global	single_step_exception
global	nmi
global	breakpoint_exception
global	overflow
global	bounds_check
global	inval_opcode
global	copr_not_available
global	double_fault
global	copr_seg_overrun
global	inval_tss
global	segment_not_present
global	stack_exception
global	general_protection
global	page_fault
global	copr_error
global  int_8259_0
global  int_8259_1
global  int_8259_2
global  int_8259_3
global  int_8259_4
global  int_8259_5
global  int_8259_6
global  int_8259_7
global  int_8259_8
global  int_8259_9
global  int_8259_10
global  int_8259_11
global  int_8259_12
global  int_8259_13
global  int_8259_14
global  int_8259_15
global  int_80h

; 中断和异常 -- 异常
divide_error:
	cli
	call	real_divide_error
	sti
	iretd
single_step_exception:
	cli
	call	real_single_step_exception
	sti
	iretd
nmi:
	cli
	call	real_nmi
	sti
	iretd
breakpoint_exception:
	cli
	call	real_breakpoint_exception
	sti
	iretd
overflow:
	cli
	call	real_overflow
	sti
	iretd
bounds_check:
	cli
	call	real_bounds_check
	sti
	iretd
inval_opcode:
	cli
	call	real_inval_opcode
	sti
	iretd
copr_not_available:
	cli
	call	real_copr_not_available
	sti
	iretd
double_fault:
	cli
	call	real_double_fault
	sti
	iretd
copr_seg_overrun:
	cli
	call	real_copr_seg_overrun
	sti
	iretd
inval_tss:
	cli
	call	real_inval_tss
	sti
	iretd
segment_not_present:
	cli
	call	real_segment_not_present
	sti
	iretd
stack_exception:
	cli
	call	real_stack_exception
	sti
	iretd
general_protection:
	cli
	call	real_general_protection
	sti
	iretd
page_fault:
	cli
	call	real_page_fault
	sti
	iretd
copr_error:
	cli
	call	real_copr_error
	sti
	iretd
int_8259_0:
	cli
	call timer0_save
;	call real_int_8259_0
;real_int_8259_0_exit:
;	sti
	iretd
int_8259_1:
	cli
	call    real_int_8259_1
	sti
	iretd
int_8259_2:
	cli
	call    real_int_8259_2
	sti
	iretd
int_8259_3:
	cli
	call    real_int_8259_3
	sti
	iretd
int_8259_4:
	cli
	call    real_int_8259_4
	sti
	iretd
int_8259_5:
	cli
	call    real_int_8259_5
	sti
	iretd
int_8259_6:
	cli
	call    real_int_8259_6
	sti
	iretd
int_8259_7:
	cli
	call    real_int_8259_7
	sti
	iretd
int_8259_8:
	cli
	call    real_int_8259_8
	sti
	iretd
int_8259_9:
	cli
	call    real_int_8259_9
	sti
	iretd
int_8259_10:
	cli
	call    real_int_8259_10
	sti
	iretd
int_8259_11:
	cli
	call    real_int_8259_11
	sti
	iretd
int_8259_12:
	cli
	call    real_int_8259_12
	sti
	iretd
int_8259_13:
	cli
	call    real_int_8259_13
	sti
	iretd
int_8259_14:
	cli
	call    real_int_8259_14
	sti
	iretd
int_8259_15:
	cli
	call    real_int_8259_15
	sti
	iretd
int_80h:
	cli
	cmp eax, SYSCALL_GET_TICK
	je syscall_get_tick
	jmp int_80h_exit
syscall_get_tick:
	call sys_get_tick
	jmp int_80h_exit
int_80h_exit:
	sti
	iretd

timer0_save:
        pushad          ; `.
        push    ds      ;  |
        push    es      ;  | 保存原寄存器值
        push    fs      ;  |
        push    gs      ; /
        mov     dx, ss
        mov     ds, dx
        mov     es, dx

        mov     esi, esp                    ;esi = 进程表起始地址
		mov     edx, [current_thread_id]
		mov		ebx, Task_frame
		mov		eax, THREAD_FRAME_SIZE
.1:
		add		ebx, eax
		dec		edx
		jnz		.1
		mov		edi, ebx
		mov		ecx,STACK_FRAME_COUNT
		rep		movsd
		pushfd
		mov		eax,[esp]
		bts		eax, 14
		mov		[esp], eax
		popfd
		jmp		restart_next_task
;        inc     dword [k_reenter]           ;k_reenter++;
;        cmp     dword [k_reenter], 0        ;if(k_reenter ==0)
;        jne     .1                          ;{
;        mov     esp, StackTop               ;  mov esp, StackTop <--切换到内核栈
;        push    restart                     ;  push restart
;        jmp     [esi + RETADR - P_STACKBASE];  return;
;.1:                                         ;} else { 已经在内核栈，不需要再切换
;        push    restart_reenter             ;  push restart_reenter
;        jmp     [esi + RETADR - P_STACKBASE];  return;
                                            ;}
[section .data]	; 数据在此

