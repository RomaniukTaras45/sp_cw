.386
.model flat, stdcall
option casemap :none
include masm32\include\windows.inc
include masm32\include\kernel32.inc
include masm32\include\masm32.inc
include masm32\include\user32.inc
include masm32\include\msvcrt.inc
includelib masm32\lib\kernel32.lib
includelib masm32\lib\masm32.lib
includelib masm32\lib\user32.lib
includelib masm32\lib\msvcrt.lib

.DATA
;===User Data==============
	_Art_	dd	00h
	_Bi_	dd	00h
	_Cp_	dd	00h
	_I_	dd	00h

;===Addition Data===========================================
	hConsoleInput	dd	?
	hConsoleOutput	dd	?
	ErrorMessage	db	"Error: division by zero", 0
	InputBuf	db	15 dup (?)
	InMessage	db	"Input: ", 0
	OutMessage	db	"Output: "," %d",0
	ResMessage	db	20 dup (?)
	NumberOfCharsRead	dd	?
	NumberOfCharsWrite	dd	?
	msg1310	dw	13,10
	buf	dd	0
	lb1	dd	?
	lb2	dd	?

.CODE
start:
invoke AllocConsole
invoke GetStdHandle, STD_INPUT_HANDLE
mov hConsoleInput, EAX
invoke GetStdHandle, STD_OUTPUT_HANDLE
mov hConsoleOutput, EAX
finit
	mov dword ptr buf,1
	fild buf
	call Not_
	fistp buf
	call Output
	mov dword ptr buf,0
	fild buf
	call Not_
	fistp buf
	call Output
	mov dword ptr buf,3
	fild buf
	call Not_
	fistp buf
	call Output
	mov dword ptr buf,3
	fild buf
	mov dword ptr buf,4
	fild buf
	call And_
	fistp buf
	call Output
	mov dword ptr buf,1
	fild buf
	mov dword ptr buf,0
	fild buf
	call Or_
	fistp buf
	call Output
	mov dword ptr buf,0
	fild buf
	mov dword ptr buf,0
	fild buf
	call Or_
	fistp buf
	call Output
	mov dword ptr buf,1
	fild buf
	mov dword ptr buf,1
	fild buf
	call Or_
	fistp buf
	call Output
	mov dword ptr buf,6
	fild buf
	mov dword ptr buf,3
	fild buf
	call Eq_
	fistp buf
	call Output
	mov dword ptr buf,6
	fild buf
	mov dword ptr buf,3
	fild buf
	call Eq_
	call Not_
	fistp buf
	call Output
	mov dword ptr buf,6
	fild buf
	mov dword ptr buf,6
	fild buf
	call Eq_
	call Not_
	fistp buf
	call Output
	mov dword ptr buf,6
	fild buf
	mov dword ptr buf,6
	fild buf
	call Eq_
	fistp buf
	call Output
	mov dword ptr buf,1
	fild buf
	mov dword ptr buf,1
	fild buf
	call Or_
	fistp buf
	call Output
	call Input
	fild buf
	fistp _Bi_
	mov dword ptr buf,5
	fild buf
	call Not_
	fistp _Art_
	fild _Art_
	fistp buf
	call Output
	fild _Bi_
	fistp buf
	call Output
exit_label:
invoke WriteConsoleA,hConsoleOutput,ADDR msg1310,SIZEOF msg1310,ADDR NumberOfCharsWrite,0
invoke ReadConsoleA,hConsoleInput,ADDR InputBuf,1,ADDR NumberOfCharsRead,0
invoke ExitProcess, 0

;===Procedure Input==========================================================================
Input PROC
invoke WriteConsoleA,hConsoleOutput,ADDR msg1310,SIZEOF msg1310,ADDR NumberOfCharsWrite,0
invoke WriteConsoleA,hConsoleOutput,ADDR InMessage,SIZEOF InMessage,ADDR NumberOfCharsWrite,0
invoke ReadConsoleA,hConsoleInput,ADDR InputBuf,11,ADDR NumberOfCharsRead,0
invoke  crt_atoi, addr InputBuf
mov dword ptr buf, eax
ret
Input ENDP
;============================================================================================


;===Procedure Output=========================================================================
Output PROC
invoke WriteConsoleA,hConsoleOutput,ADDR msg1310,SIZEOF msg1310,ADDR NumberOfCharsWrite,0
invoke wsprintf, addr ResMessage, addr OutMessage, dword ptr buf
invoke WriteConsoleA,hConsoleOutput,ADDR ResMessage,eax,ADDR NumberOfCharsWrite,0
ret
Output ENDP
;============================================================================================


;===Procedure And=========================
And_ PROC
	push eax
	push edx
	pushf
	fistp lb1
	fistp lb2
	mov eax,lb1
	cmp eax,0
	jnz and_t1
	jz and_false
and_t1:
	mov eax,lb2
	cmp eax,0
	jnz and_true
and_false:
	fldz
	jmp and_fin
and_true:
	fld1
and_fin:

	popf
	pop edx
	pop eax

	ret
And_ ENDP
;=========================================


;===Procedure Or==========================
Or_ PROC
	push eax
	push edx
	pushf
	fistp lb1
	fistp lb2
	mov eax,lb1
	cmp eax,0
	jnz or_true
	mov eax,lb2
	cmp eax,0
	jnz or_true
	fldz
	jmp or_fin
or_true:
	fld1
or_fin:

	popf
	pop edx
	pop eax

	ret
Or_ ENDP
;=========================================


;===Procedure Not=========================
Not_ PROC
	push eax
	pushf
	fistp lb1
	mov eax,lb1
	cmp eax,0
	jne not_false
	fld1
	jmp not_fin
not_false:
	fldz
not_fin:

	popf
	pop eax

	ret
Not_ ENDP
;=========================================


;===Procedure Eq==========================
Eq_ PROC
	push eax
	push edx
	pushf
	fistp lb1
	fistp lb2
	mov eax,lb1
	mov edx,lb2
	cmp eax,edx
	jne not_eq
	fld1
	jmp eq_fin
not_eq:
	fldz
eq_fin:
	popf
	pop edx
	pop eax

	ret
Eq_ ENDP
;=========================================

end start
