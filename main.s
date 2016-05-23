	.file	"main.cpp"
	.section	.text._ZNSt14_Function_base13_Base_managerIZN4xiix6client4mainEiPPKcEUlPNS1_4sockEE_E10_M_managerERSt9_Any_dataRKSA_St18_Manager_operation,"axG",@progbits,_ZNSt14_Function_base13_Base_managerIZN4xiix6client4mainEiPPKcEUlPNS1_4sockEE_E10_M_managerERSt9_Any_dataRKSA_St18_Manager_operation,comdat
	.p2align 4,,15
	.weak	_ZNSt14_Function_base13_Base_managerIZN4xiix6client4mainEiPPKcEUlPNS1_4sockEE_E10_M_managerERSt9_Any_dataRKSA_St18_Manager_operation
	.type	_ZNSt14_Function_base13_Base_managerIZN4xiix6client4mainEiPPKcEUlPNS1_4sockEE_E10_M_managerERSt9_Any_dataRKSA_St18_Manager_operation, @function
_ZNSt14_Function_base13_Base_managerIZN4xiix6client4mainEiPPKcEUlPNS1_4sockEE_E10_M_managerERSt9_Any_dataRKSA_St18_Manager_operation:
.LFB1800:
	.cfi_startproc
	cmpl	$1, %edx
	je	.L3
	jb	.L4
	cmpl	$2, %edx
	jne	.L2
	movq	(%rsi), %rax
	movq	8(%rsi), %rdx
	movq	%rax, (%rdi)
	movq	%rdx, 8(%rdi)
.L2:
	xorl	%eax, %eax
	ret
	.p2align 4,,10
	.p2align 3
.L4:
	movq	$_ZTIZN4xiix6client4mainEiPPKcEUlPNS_4sockEE_, (%rdi)
	xorl	%eax, %eax
	ret
	.p2align 4,,10
	.p2align 3
.L3:
	movq	%rsi, (%rdi)
	xorl	%eax, %eax
	ret
	.cfi_endproc
.LFE1800:
	.size	_ZNSt14_Function_base13_Base_managerIZN4xiix6client4mainEiPPKcEUlPNS1_4sockEE_E10_M_managerERSt9_Any_dataRKSA_St18_Manager_operation, .-_ZNSt14_Function_base13_Base_managerIZN4xiix6client4mainEiPPKcEUlPNS1_4sockEE_E10_M_managerERSt9_Any_dataRKSA_St18_Manager_operation
	.section	.rodata._ZNSt17_Function_handlerIFbPN4xiix4sockEEZNS0_6client4mainEiPPKcEUlS2_E_E9_M_invokeERKSt9_Any_dataOS2_.str1.1,"aMS",@progbits,1
.LC0:
	.string	"socket"
.LC1:
	.string	"gethostbyname"
.LC2:
	.string	"connect"
.LC3:
	.string	"getfl"
.LC4:
	.string	"nonblock"
.LC5:
	.string	"epolladd"
	.section	.text._ZNSt17_Function_handlerIFbPN4xiix4sockEEZNS0_6client4mainEiPPKcEUlS2_E_E9_M_invokeERKSt9_Any_dataOS2_,"axG",@progbits,_ZNSt17_Function_handlerIFbPN4xiix4sockEEZNS0_6client4mainEiPPKcEUlS2_E_E9_M_invokeERKSt9_Any_dataOS2_,comdat
	.p2align 4,,15
	.weak	_ZNSt17_Function_handlerIFbPN4xiix4sockEEZNS0_6client4mainEiPPKcEUlS2_E_E9_M_invokeERKSt9_Any_dataOS2_
	.type	_ZNSt17_Function_handlerIFbPN4xiix4sockEEZNS0_6client4mainEiPPKcEUlS2_E_E9_M_invokeERKSt9_Any_dataOS2_, @function
_ZNSt17_Function_handlerIFbPN4xiix4sockEEZNS0_6client4mainEiPPKcEUlS2_E_E9_M_invokeERKSt9_Any_dataOS2_:
.LFB1798:
	.cfi_startproc
	pushq	%rbx
	.cfi_def_cfa_offset 16
	.cfi_offset 3, -16
	subq	$16, %rsp
	.cfi_def_cfa_offset 32
	movzbl	(%rdi), %eax
	movq	(%rsi), %rbx
	movq	8(%rdi), %rdx
	movl	$1, %esi
	movl	$2, %edi
	addq	$1, _ZN4xiix6meters5opensE(%rip)
	movq	%rdx, 24(%rbx)
	movb	%al, 52(%rbx)
	xorl	%edx, %edx
	call	socket
	testl	%eax, %eax
	movl	%eax, 4(%rbx)
	js	.L15
	movq	8(%rbx), %rdi
	call	gethostbyname
	testq	%rax, %rax
	je	.L16
	movq	$0, 8(%rsp)
	movslq	20(%rax), %rdx
	leaq	4(%rsp), %rsi
	movq	24(%rax), %rax
	movq	$0, (%rsp)
	movq	(%rax), %rdi
	call	bcopy
	movl	4(%rbx), %edi
	movl	$2, %eax
	movl	$16, %edx
	movq	%rsp, %rsi
	movw	%ax, (%rsp)
	movzwl	16(%rbx), %eax
#APP
# 33 "src/xiix/sock.hpp" 1
	rorw $8, %ax
# 0 "" 2
#NO_APP
	movw	%ax, 2(%rsp)
	call	connect
	testl	%eax, %eax
	js	.L17
	movl	4(%rbx), %edi
	xorl	%edx, %edx
	xorl	%eax, %eax
	movl	$3, %esi
	call	fcntl
	testl	%eax, %eax
	js	.L18
	movl	4(%rbx), %edi
	orb	$8, %ah
	movl	$4, %esi
	movl	%eax, %edx
	xorl	%eax, %eax
	call	fcntl
	cmpl	$-1, %eax
	je	.L19
	movl	4(%rbx), %edx
	movl	(%rbx), %edi
	leaq	32(%rbx), %rcx
	movl	$-2147483644, 32(%rbx)
	movq	%rbx, 36(%rbx)
	movl	$1, %esi
	call	epoll_ctl
	testl	%eax, %eax
	js	.L20
	addq	$16, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 16
	movl	$1, %eax
	popq	%rbx
	.cfi_def_cfa_offset 8
	ret
.L15:
	.cfi_restore_state
	movl	$8, %edi
	call	__cxa_allocate_exception
	xorl	%edx, %edx
	movq	$.LC0, (%rax)
	movl	$_ZTIPKc, %esi
	movq	%rax, %rdi
	call	__cxa_throw
.L20:
	movl	$8, %edi
	call	__cxa_allocate_exception
	xorl	%edx, %edx
	movq	$.LC5, (%rax)
	movl	$_ZTIPKc, %esi
	movq	%rax, %rdi
	call	__cxa_throw
.L19:
	movl	$8, %edi
	call	__cxa_allocate_exception
	xorl	%edx, %edx
	movq	$.LC4, (%rax)
	movl	$_ZTIPKc, %esi
	movq	%rax, %rdi
	call	__cxa_throw
.L18:
	movl	$8, %edi
	call	__cxa_allocate_exception
	xorl	%edx, %edx
	movq	$.LC3, (%rax)
	movl	$_ZTIPKc, %esi
	movq	%rax, %rdi
	call	__cxa_throw
.L17:
	movl	$8, %edi
	call	__cxa_allocate_exception
	xorl	%edx, %edx
	movq	$.LC2, (%rax)
	movl	$_ZTIPKc, %esi
	movq	%rax, %rdi
	call	__cxa_throw
.L16:
	movl	$8, %edi
	call	__cxa_allocate_exception
	xorl	%edx, %edx
	movq	$.LC1, (%rax)
	movl	$_ZTIPKc, %esi
	movq	%rax, %rdi
	call	__cxa_throw
	.cfi_endproc
.LFE1798:
	.size	_ZNSt17_Function_handlerIFbPN4xiix4sockEEZNS0_6client4mainEiPPKcEUlS2_E_E9_M_invokeERKSt9_Any_dataOS2_, .-_ZNSt17_Function_handlerIFbPN4xiix4sockEEZNS0_6client4mainEiPPKcEUlS2_E_E9_M_invokeERKSt9_Any_dataOS2_
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC6:
	.string	"epollmodread"
	.section	.text.unlikely,"ax",@progbits
	.align 2
	.type	_ZN4xiix4sock15io_request_readEv.part.19, @function
_ZN4xiix4sock15io_request_readEv.part.19:
.LFB1957:
	.cfi_startproc
	movl	$8, %edi
	subq	$8, %rsp
	.cfi_def_cfa_offset 16
	call	__cxa_allocate_exception
	xorl	%edx, %edx
	movq	$.LC6, (%rax)
	movl	$_ZTIPKc, %esi
	movq	%rax, %rdi
	call	__cxa_throw
	.cfi_endproc
.LFE1957:
	.size	_ZN4xiix4sock15io_request_readEv.part.19, .-_ZN4xiix4sock15io_request_readEv.part.19
	.section	.text._ZN4xiix4args9hasoptionEc,"axG",@progbits,_ZN4xiix4args9hasoptionEc,comdat
	.align 2
	.p2align 4,,15
	.weak	_ZN4xiix4args9hasoptionEc
	.type	_ZN4xiix4args9hasoptionEc, @function
_ZN4xiix4args9hasoptionEc:
.LFB19:
	.cfi_startproc
	movl	(%rdi), %edx
	movl	%esi, %r8d
	cmpl	$1, %edx
	je	.L35
	movq	8(%rdi), %rax
	subl	$2, %edx
	leaq	8(%rax,%rdx,8), %rcx
	.p2align 4,,10
	.p2align 3
.L25:
	addq	$8, %rax
	movq	(%rax), %rdx
	cmpb	$45, (%rdx)
	je	.L41
	cmpq	%rcx, %rax
	jne	.L25
.L35:
	xorl	%eax, %eax
	ret
	.p2align 4,,10
	.p2align 3
.L41:
	movsbl	1(%rdx), %eax
	leaq	1(%rdx), %rcx
	cmpb	%sil, %al
	jne	.L38
	jmp	.L36
	.p2align 4,,10
	.p2align 3
.L28:
	addq	$1, %rcx
	movsbl	(%rcx), %eax
	cmpb	%r8b, %al
	je	.L36
.L38:
	testb	%al, %al
	je	.L35
	subl	$48, %eax
	cmpl	$9, %eax
	ja	.L28
	jmp	.L35
	.p2align 4,,10
	.p2align 3
.L36:
	movl	$1, %eax
	ret
	.cfi_endproc
.LFE19:
	.size	_ZN4xiix4args9hasoptionEc, .-_ZN4xiix4args9hasoptionEc
	.section	.rodata._ZN4xiix4sock9parse_bufEv.str1.1,"aMS",@progbits,1
.LC7:
	.string	"bufp!=bufe"
	.section	.rodata._ZN4xiix4sock9parse_bufEv.str1.8,"aMS",@progbits,1
	.align 8
.LC8:
	.string	"GET %s HTTP/1.1\r\nHost: %s:%d\r\n\r\n"
	.section	.rodata._ZN4xiix4sock9parse_bufEv.str1.1
.LC9:
	.string	"GET %s HTTP/1.1\r\nHost: %s\r\n\r\n"
.LC10:
	.string	"brk"
.LC11:
	.string	"send"
.LC12:
	.string	"incompletesend"
.LC13:
	.string	"close"
.LC14:
	.string	"overflow"
.LC15:
	.string	"underflow"
.LC16:
	.string	"chunksizefromhex"
.LC17:
	.string	"illegalstate"
	.section	.text._ZN4xiix4sock9parse_bufEv,"axG",@progbits,_ZN4xiix4sock9parse_bufEv,comdat
	.align 2
	.p2align 4,,15
	.weak	_ZN4xiix4sock9parse_bufEv
	.type	_ZN4xiix4sock9parse_bufEv, @function
_ZN4xiix4sock9parse_bufEv:
.LFB150:
	.cfi_startproc
	pushq	%r13
	.cfi_def_cfa_offset 16
	.cfi_offset 13, -16
	pushq	%r12
	.cfi_def_cfa_offset 24
	.cfi_offset 12, -24
	pushq	%rbp
	.cfi_def_cfa_offset 32
	.cfi_offset 6, -32
	pushq	%rbx
	.cfi_def_cfa_offset 40
	.cfi_offset 3, -40
	leaq	4248(%rdi), %rbp
	movq	%rdi, %rbx
	subq	$1048, %rsp
	.cfi_def_cfa_offset 1088
	movl	44(%rdi), %eax
	.p2align 4,,10
	.p2align 3
.L43:
	testl	%eax, %eax
	je	.L44
	cmpl	$1, %eax
	je	.L130
	cmpl	$2, %eax
	je	.L131
	cmpl	$3, %eax
	je	.L132
	cmpl	$4, %eax
	je	.L133
	cmpl	$5, %eax
	jne	.L43
	movq	4224(%rbx), %rsi
	movq	4232(%rbx), %rbp
	subq	%rsi, %rbp
	cmpb	$0, _ZN4xiix4conf13print_contentE(%rip)
	jne	.L134
.L99:
	addq	%rbp, %rsi
	movq	%rsi, 4224(%rbx)
	.p2align 4,,10
	.p2align 3
.L117:
	movl	4(%rbx), %edx
	movl	(%rbx), %edi
	leaq	32(%rbx), %rcx
	movl	$-2147483647, 32(%rbx)
	movl	$3, %esi
	call	epoll_ctl
	testl	%eax, %eax
	jne	.L135
.L42:
	addq	$1048, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 40
	popq	%rbx
	.cfi_def_cfa_offset 32
	popq	%rbp
	.cfi_def_cfa_offset 24
	popq	%r12
	.cfi_def_cfa_offset 16
	popq	%r13
	.cfi_def_cfa_offset 8
	ret
	.p2align 4,,10
	.p2align 3
.L130:
	.cfi_restore_state
	movq	4232(%rbx), %rdi
	movq	4224(%rbx), %rax
	leaq	1(%rdi), %rcx
	addq	$1, %rax
	jmp	.L62
	.p2align 4,,10
	.p2align 3
.L137:
	leaq	1(%rax), %rdx
	movq	%rax, 4224(%rbx)
	cmpb	$10, -2(%rdx)
	je	.L136
	movq	%rdx, %rax
.L62:
	cmpq	%rax, %rcx
	jne	.L137
	jmp	.L117
	.p2align 4,,10
	.p2align 3
.L132:
	movq	88(%rbx), %rax
	movq	96(%rbx), %rdx
	movl	$4096, %ecx
	movq	%rax, %r12
	subq	%rdx, %r12
	cmpq	$4096, %r12
	cmova	%rcx, %r12
	cmpb	$0, _ZN4xiix4conf13print_contentE(%rip)
	jne	.L138
.L69:
	addq	%rdx, %r12
	movq	4224(%rbx), %rcx
	cmpq	%rax, %r12
	movq	%r12, 96(%rbx)
	je	.L128
	cmpq	%rcx, 4232(%rbx)
	je	.L117
.L115:
	movl	44(%rbx), %eax
	jmp	.L43
	.p2align 4,,10
	.p2align 3
.L133:
	movl	48(%rbx), %eax
	testl	%eax, %eax
	je	.L78
.L75:
	cmpl	$1, %eax
	je	.L139
	cmpl	$2, %eax
	je	.L140
	cmpl	$3, %eax
	jne	.L115
	movq	4224(%rbx), %rax
	movq	4232(%rbx), %rsi
	addq	$1, %rax
	leaq	1(%rsi), %rdx
	jmp	.L98
	.p2align 4,,10
	.p2align 3
.L141:
	movq	%rax, 4224(%rbx)
	addq	$1, %rax
	cmpb	$10, -2(%rax)
	je	.L128
.L98:
	cmpq	%rdx, %rax
	jne	.L141
	movl	44(%rbx), %eax
	testl	%eax, %eax
	jne	.L117
	cmpb	$0, 53(%rbx)
	jne	.L142
.L46:
	cmpb	$0, 52(%rbx)
	je	.L143
.L47:
	leaq	4248(%rbx), %rax
	movq	$0, 96(%rbx)
	movq	$0, 88(%rbx)
	movq	$0, 112(%rbx)
	movq	$0, 104(%rbx)
	movq	%rax, 4520(%rbx)
	movq	%rax, 4512(%rbx)
	addq	4504(%rbx), %rax
	movb	$0, 4248(%rbx)
	movq	%rax, 4528(%rbx)
	movq	4232(%rbx), %rax
	cmpq	%rax, 4224(%rbx)
	jne	.L144
	movl	16(%rbx), %r9d
	leaq	120(%rbx), %rax
	addq	$1, _ZN4xiix6meters8requestsE(%rip)
	movb	$0, 120(%rbx)
	movl	$0, 48(%rbx)
	leaq	16(%rsp), %r13
	movq	%rax, 4224(%rbx)
	movq	$0, 4240(%rbx)
	cmpl	$80, %r9d
	movq	24(%rbx), %rcx
	movq	8(%rbx), %r8
	je	.L50
	movl	$.LC8, %edx
	movl	$1024, %esi
	movq	%r13, %rdi
	xorl	%eax, %eax
	call	snprintf
	movslq	%eax, %rbp
.L51:
	movl	4(%rbx), %edi
	movl	$16384, %ecx
	movq	%rbp, %rdx
	movq	%r13, %rsi
	call	send
	testq	%rax, %rax
	movq	%rax, %r12
	js	.L145
	addq	%rax, _ZN4xiix6meters6outputE(%rip)
	cmpb	$0, _ZN4xiix4conf13print_trafficE(%rip)
	jne	.L146
.L55:
	cmpq	%r12, %rbp
	jne	.L147
	movl	$1, 44(%rbx)
	jmp	.L117
	.p2align 4,,10
	.p2align 3
.L149:
	leaq	1(%rax), %rdx
	movq	%rdx, 4224(%rbx)
	movzbl	(%rax), %edx
	movq	4512(%rbx), %rax
	movq	%rax, %rcx
	subq	%rbp, %rcx
	cmpq	4504(%rbx), %rcx
	je	.L126
	leaq	1(%rax), %rcx
	cmpb	$10, %dl
	movq	%rcx, 4512(%rbx)
	movb	%dl, (%rax)
	je	.L148
.L78:
	movq	4224(%rbx), %rax
	cmpq	4232(%rbx), %rax
	jne	.L149
	jmp	.L117
	.p2align 4,,10
	.p2align 3
.L136:
	leaq	120(%rbx), %rdx
	movq	%rax, %rsi
	movq	%rax, 4240(%rbx)
	movl	$2, 44(%rbx)
	subq	%rdx, %rsi
	movq	%rdx, 56(%rbx)
	movq	%rsi, %rdx
	subq	$1, %rdx
	cmpq	%rdi, %rax
	movq	%rdx, 64(%rbx)
	je	.L117
.L64:
	cmpq	4224(%rbx), %rdi
	je	.L117
	movq	%rdi, 4224(%rbx)
	jmp	.L117
	.p2align 4,,10
	.p2align 3
.L128:
	cmpb	$0, 52(%rbx)
	movl	$0, 44(%rbx)
	je	.L150
.L44:
	cmpb	$0, 53(%rbx)
	je	.L46
.L142:
	movb	$0, 53(%rbx)
	jmp	.L47
	.p2align 4,,10
	.p2align 3
.L138:
	movq	4224(%rbx), %rsi
	movq	%r12, %rdx
	movl	$1, %edi
	call	write
	movq	88(%rbx), %rax
	movq	96(%rbx), %rdx
	jmp	.L69
	.p2align 4,,10
	.p2align 3
.L139:
	movq	4232(%rbx), %rdi
	movq	104(%rbx), %rdx
	movq	4224(%rbx), %rsi
	movq	112(%rbx), %rcx
	movq	%rdx, %r12
	movq	%rdi, %rax
	subq	%rcx, %r12
	subq	%rsi, %rax
	cmpq	%rax, %r12
	cmova	%rax, %r12
	cmpb	$0, _ZN4xiix4conf13print_contentE(%rip)
	jne	.L151
.L88:
	leaq	(%rsi,%r12), %rax
	addq	%rcx, %r12
	cmpq	%rdx, %r12
	movq	%r12, 112(%rbx)
	movq	%rax, 4224(%rbx)
	jne	.L152
	movl	$2, 48(%rbx)
.L91:
	addq	$1, %rax
	leaq	1(%rdi), %rdx
	jmp	.L94
	.p2align 4,,10
	.p2align 3
.L154:
	movq	%rax, 4224(%rbx)
	movq	%rax, %rcx
	addq	$1, %rax
	cmpb	$10, -2(%rax)
	je	.L153
.L94:
	cmpq	%rax, %rdx
	jne	.L154
	jmp	.L117
	.p2align 4,,10
	.p2align 3
.L148:
	movq	4512(%rbx), %rax
	cmpq	4520(%rbx), %rax
	je	.L155
	leaq	-1(%rax), %rdx
	movq	%rdx, 4512(%rbx)
	movb	$0, -1(%rax)
	movq	4512(%rbx), %r12
	cmpq	4520(%rbx), %r12
	je	.L80
	call	__ctype_b_loc
	jmp	.L81
	.p2align 4,,10
	.p2align 3
.L156:
	leaq	-1(%r12), %rdx
	movq	%rdx, 4512(%rbx)
	movb	$0, -1(%r12)
	movq	4512(%rbx), %r12
	cmpq	4520(%rbx), %r12
	je	.L80
.L81:
	movsbq	-1(%r12), %rcx
	movq	(%rax), %rdx
	testb	$32, 1(%rdx,%rcx,2)
	jne	.L156
.L80:
	movq	%r12, %rax
	subq	%rbp, %rax
	cmpq	4504(%rbx), %rax
	je	.L126
	leaq	1(%r12), %rax
	leaq	16(%rsp), %rdi
	movq	%rax, 4512(%rbx)
	movb	$0, (%r12)
	movq	4520(%rbx), %rsi
	movq	4512(%rbx), %rdx
	subq	%rsi, %rdx
	call	strncpy
	leaq	8(%rsp), %rsi
	leaq	16(%rsp), %rdi
	movl	$16, %edx
	call	strtoul
	movq	8(%rsp), %rdx
	cmpb	$0, (%rdx)
	jne	.L157
	cmpq	$1, %rax
	movq	%rax, 104(%rbx)
	movq	4232(%rbx), %rsi
	sbbl	%eax, %eax
	movq	$0, 112(%rbx)
	andl	$2, %eax
	addl	$1, %eax
	cmpq	%rsi, 4224(%rbx)
	movl	%eax, 48(%rbx)
	jne	.L75
	jmp	.L117
	.p2align 4,,10
	.p2align 3
.L153:
	movq	%rbp, %rax
	addq	4504(%rbx), %rax
	cmpq	%rdi, %rcx
	movq	%rbp, 4520(%rbx)
	movq	%rbp, 4512(%rbx)
	movb	$0, 4248(%rbx)
	movl	$0, 48(%rbx)
	movq	%rax, 4528(%rbx)
	jne	.L115
	jmp	.L117
	.p2align 4,,10
	.p2align 3
.L131:
	movq	4232(%rbx), %rdi
	jmp	.L64
.L151:
	movq	%r12, %rdx
	movl	$1, %edi
	call	write
	movq	104(%rbx), %rdx
	movq	4232(%rbx), %rdi
	movq	4224(%rbx), %rsi
	movq	112(%rbx), %rcx
	jmp	.L88
.L145:
	call	__errno_location
	movl	(%rax), %eax
	movl	$8, %edi
	cmpl	$32, %eax
	je	.L103
	cmpl	$104, %eax
	je	.L103
	call	__cxa_allocate_exception
	xorl	%edx, %edx
	movq	$.LC11, (%rax)
	movl	$_ZTIPKc, %esi
	movq	%rax, %rdi
	call	__cxa_throw
.L50:
	movl	$.LC9, %edx
	movl	$1024, %esi
	movq	%r13, %rdi
	xorl	%eax, %eax
	call	snprintf
	movslq	%eax, %rbp
	jmp	.L51
.L146:
	movq	%rax, %rdx
	movq	%r13, %rsi
	movl	$1, %edi
	call	write
	jmp	.L55
.L103:
	call	__cxa_allocate_exception
	xorl	%edx, %edx
	movq	$.LC10, (%rax)
	movl	$_ZTIPKc, %esi
	movq	%rax, %rdi
	call	__cxa_throw
.L134:
	movq	%rbp, %rdx
	movl	$1, %edi
	call	write
	movq	4224(%rbx), %rsi
	jmp	.L99
.L143:
	movl	4(%rbx), %edi
	call	close
	jmp	.L42
.L152:
	cmpq	%rdi, %rax
	je	.L117
	movl	$8, %edi
	call	__cxa_allocate_exception
	xorl	%edx, %edx
	movq	$.LC17, (%rax)
	movl	$_ZTIPKc, %esi
	movq	%rax, %rdi
	call	__cxa_throw
	.p2align 4,,10
	.p2align 3
.L140:
	movq	4224(%rbx), %rax
	movq	4232(%rbx), %rdi
	jmp	.L91
.L126:
	movl	$8, %edi
	call	__cxa_allocate_exception
	xorl	%edx, %edx
	movq	$.LC14, (%rax)
	movl	$_ZTIPKc, %esi
	movq	%rax, %rdi
	call	__cxa_throw
.L157:
	movl	$8, %edi
	call	__cxa_allocate_exception
	xorl	%edx, %edx
	movq	$.LC16, (%rax)
	movl	$_ZTIPKc, %esi
	movq	%rax, %rdi
	call	__cxa_throw
.L150:
	movl	$8, %edi
	call	__cxa_allocate_exception
	xorl	%edx, %edx
	movq	$.LC13, (%rax)
	movl	$_ZTIPKc, %esi
	movq	%rax, %rdi
	call	__cxa_throw
.L147:
	movl	$8, %edi
	call	__cxa_allocate_exception
	xorl	%edx, %edx
	movq	$.LC12, (%rax)
	movl	$_ZTIPKc, %esi
	movq	%rax, %rdi
	call	__cxa_throw
.L144:
	movl	$8, %edi
	call	__cxa_allocate_exception
	xorl	%edx, %edx
	movq	$.LC7, (%rax)
	movl	$_ZTIPKc, %esi
	movq	%rax, %rdi
	call	__cxa_throw
.L155:
	movl	$8, %edi
	call	__cxa_allocate_exception
	xorl	%edx, %edx
	movq	$.LC15, (%rax)
	movl	$_ZTIPKc, %esi
	movq	%rax, %rdi
	call	__cxa_throw
.L135:
	call	_ZN4xiix4sock15io_request_readEv.part.19
	.cfi_endproc
.LFE150:
	.size	_ZN4xiix4sock9parse_bufEv, .-_ZN4xiix4sock9parse_bufEv
	.section	.rodata._ZN4xiix6client4mainEiPPKc.str1.1,"aMS",@progbits,1
.LC18:
	.string	"80"
.LC19:
	.string	"localhost"
.LC20:
	.string	"1"
.LC21:
	.string	"/"
.LC22:
	.string	"yes"
.LC23:
	.string	"no"
	.section	.rodata._ZN4xiix6client4mainEiPPKc.str1.8,"aMS",@progbits,1
	.align 8
.LC24:
	.string	"usage: %s [host=localhost] [port=8088] [uri=/] [connections=1] -psqv\n"
	.section	.rodata._ZN4xiix6client4mainEiPPKc.str1.1
.LC25:
	.string	"\n  x i i x   m a r k"
	.section	.rodata._ZN4xiix6client4mainEiPPKc.str1.8
	.align 8
.LC26:
	.string	"           url: http://%s:%d%s\n"
	.section	.rodata._ZN4xiix6client4mainEiPPKc.str1.1
.LC27:
	.string	"           url: http://%s%s\n"
.LC28:
	.string	"       clients: %d\n"
.LC29:
	.string	"        repeat: %s\n"
.LC30:
	.string	" print traffic: %s\n"
.LC31:
	.string	"   print stats: %s\n"
.LC32:
	.string	"   print reply: %s\n"
.LC33:
	.string	""
.LC34:
	.string	"epoll_create"
.LC35:
	.string	"incompleteparse"
.LC36:
	.string	"closed2"
.LC37:
	.string	"recverr"
	.section	.rodata._ZN4xiix6client4mainEiPPKc.str1.8
	.align 8
.LC38:
	.string	"   requests: %zu  events: %zu  writes: %zu    reads: %zu \r"
	.section	.rodata._ZN4xiix6client4mainEiPPKc.str1.1
.LC39:
	.string	"responsecode"
.LC40:
	.string	"*** exception caught"
	.section	.text.startup._ZN4xiix6client4mainEiPPKc,"axG",@progbits,_ZN4xiix6client4mainEiPPKc,comdat
	.align 2
	.p2align 4,,15
	.weak	_ZN4xiix6client4mainEiPPKc
	.type	_ZN4xiix6client4mainEiPPKc, @function
_ZN4xiix6client4mainEiPPKc:
.LFB1484:
	.cfi_startproc
	.cfi_personality 0x3,__gxx_personality_v0
	.cfi_lsda 0x3,.LLSDA1484
	pushq	%r15
	.cfi_def_cfa_offset 16
	.cfi_offset 15, -16
	pushq	%r14
	.cfi_def_cfa_offset 24
	.cfi_offset 14, -24
	pushq	%r13
	.cfi_def_cfa_offset 32
	.cfi_offset 13, -32
	pushq	%r12
	.cfi_def_cfa_offset 40
	.cfi_offset 12, -40
	pushq	%rbp
	.cfi_def_cfa_offset 48
	.cfi_offset 6, -48
	pushq	%rbx
	.cfi_def_cfa_offset 56
	.cfi_offset 3, -56
	movl	%esi, %ebx
	movq	%rdx, %rbp
	subq	$104, %rsp
	.cfi_def_cfa_offset 160
	leaq	48(%rsp), %rdi
	movl	%esi, 48(%rsp)
	movl	$104, %esi
	movq	%rdx, 56(%rsp)
	call	_ZN4xiix4args9hasoptionEc
	testb	%al, %al
	jne	.L295
	cmpl	$1, %ebx
	je	.L222
	movq	8(%rbp), %rdi
	leaq	8(%rbp), %rax
	movq	%rax, %rcx
	movzbl	(%rdi), %r8d
	cmpb	$45, %r8b
	jne	.L296
	leal	-2(%rbx), %edx
	movq	%rdx, %rbx
	leaq	8(%rbp,%rdx,8), %r9
	jmp	.L164
	.p2align 4,,10
	.p2align 3
.L285:
	addq	$8, %rcx
	movq	(%rcx), %r13
	cmpb	$45, 0(%r13)
	jne	.L163
.L164:
	cmpq	%rcx, %r9
	jne	.L285
	movl	$.LC19, %r13d
.L163:
	leaq	8(%rbp,%rbx,8), %rcx
	movl	$2, %edx
	jmp	.L220
	.p2align 4,,10
	.p2align 3
.L297:
	movq	8(%rax), %rdi
	addq	$8, %rax
	movzbl	(%rdi), %r8d
.L220:
	cmpb	$45, %r8b
	je	.L166
	cmpl	$1, %edx
	je	.L161
	movl	$1, %edx
.L166:
	cmpq	%rcx, %rax
	jne	.L297
	movl	$.LC18, %edi
.L161:
	movl	$10, %edx
	xorl	%esi, %esi
	call	strtol
	movl	48(%rsp), %edx
	movq	%rax, %rbx
	movl	%eax, %r15d
	movq	56(%rsp), %rax
	cmpl	$1, %edx
	je	.L298
	subl	$2, %edx
	movl	$3, %ecx
	leaq	8(%rax,%rdx,8), %rsi
	movq	%rax, %rdx
	.p2align 4,,10
	.p2align 3
.L170:
	addq	$8, %rdx
	movq	(%rdx), %rdi
	cmpb	$45, (%rdi)
	movq	%rdi, (%rsp)
	je	.L171
	subl	$1, %ecx
	je	.L169
.L171:
	cmpq	%rsi, %rdx
	jne	.L170
	movq	$.LC21, (%rsp)
.L169:
	movl	$4, %edx
	.p2align 4,,10
	.p2align 3
.L221:
	addq	$8, %rax
	movq	(%rax), %rdi
	cmpb	$45, (%rdi)
	je	.L172
	subl	$1, %edx
	je	.L167
.L172:
	cmpq	%rsi, %rax
	jne	.L221
	movl	$.LC20, %edi
.L167:
	movl	$10, %edx
	xorl	%esi, %esi
	call	strtol
	leaq	48(%rsp), %rdi
	movl	$112, %esi
	movq	%rax, 16(%rsp)
	movl	%eax, 24(%rsp)
	call	_ZN4xiix4args9hasoptionEc
	leaq	48(%rsp), %rdi
	movl	$115, %esi
	movb	%al, _ZN4xiix4conf13print_trafficE(%rip)
	call	_ZN4xiix4args9hasoptionEc
	leaq	48(%rsp), %rdi
	movl	$113, %esi
	movb	%al, _ZN4xiix4conf11print_statsE(%rip)
	call	_ZN4xiix4args9hasoptionEc
	testb	%al, %al
	jne	.L299
.L173:
	leaq	48(%rsp), %rdi
	movl	$114, %esi
	call	_ZN4xiix4args9hasoptionEc
	leaq	48(%rsp), %rdi
	movl	$118, %esi
	movb	%al, 31(%rsp)
	call	_ZN4xiix4args9hasoptionEc
	testb	%al, %al
	jne	.L300
.L174:
	movl	16(%rsp), %edi
	call	epoll_create
	testl	%eax, %eax
	movl	%eax, %r14d
	js	.L301
	movl	16(%rsp), %eax
	testl	%eax, %eax
	jle	.L182
	xorl	%r12d, %r12d
	movq	$0, 8(%rsp)
	xorl	%ebp, %ebp
	jmp	.L184
	.p2align 4,,10
	.p2align 3
.L303:
	addl	$1, %ebp
	cmpl	%ebp, 24(%rsp)
	movq	%rax, 8(%r12)
	movq	%rax, %r12
	je	.L302
.L184:
	movl	$4536, %edi
.LEHB0:
	call	_Znwm
	movq	%rax, %rbx
	movl	%r14d, (%rax)
	movl	$0, 4(%rax)
	movq	%r13, 8(%rax)
	movl	%r15d, 16(%rax)
	leaq	4248(%rax), %rax
	movq	$.LC21, -4224(%rax)
	movl	$0, -4204(%rax)
	movl	$16, %edi
	movl	$0, -4200(%rax)
	movb	$0, -4196(%rax)
	movb	$1, -4195(%rax)
	movq	$0, -4192(%rax)
	movq	$0, -4184(%rax)
	movq	$0, -4176(%rax)
	movq	$0, -4168(%rax)
	movq	$0, -4160(%rax)
	movq	$0, -4152(%rax)
	movq	$0, -4144(%rax)
	movq	$0, -4136(%rax)
	movq	$0, -24(%rax)
	movq	$0, -16(%rax)
	movq	$0, -8(%rax)
	movq	$256, 256(%rax)
	movq	%rax, 4512(%rbx)
	movq	%rax, 4520(%rbx)
	leaq	4504(%rbx), %rax
	addq	$1, _ZN4xiix6meters5socksE(%rip)
	movq	%rax, 4528(%rbx)
	call	_Znwm
.LEHE0:
	testq	%r12, %r12
	movq	%rbx, (%rax)
	movq	$0, 8(%rax)
	jne	.L303
	addl	$1, %ebp
	cmpl	%ebp, 24(%rsp)
	movq	%rax, 8(%rsp)
	movq	%rax, %r12
	jne	.L184
	.p2align 4,,10
	.p2align 3
.L302:
	movzbl	31(%rsp), %eax
	movq	$_ZNSt17_Function_handlerIFbPN4xiix4sockEEZNS0_6client4mainEiPPKcEUlS2_E_E9_M_invokeERKSt9_Any_dataOS2_, 88(%rsp)
	movq	$_ZNSt14_Function_base13_Base_managerIZN4xiix6client4mainEiPPKcEUlPNS1_4sockEE_E10_M_managerERSt9_Any_dataRKSA_St18_Manager_operation, 80(%rsp)
	movb	%al, 64(%rsp)
	movq	(%rsp), %rax
	movq	%rax, 72(%rsp)
	movq	8(%rsp), %rax
	testq	%rax, %rax
	je	.L234
	movq	%rax, %rbx
	movl	$_ZNSt14_Function_base13_Base_managerIZN4xiix6client4mainEiPPKcEUlPNS1_4sockEE_E10_M_managerERSt9_Any_dataRKSA_St18_Manager_operation, %eax
	.p2align 4,,10
	.p2align 3
.L188:
	movq	(%rbx), %rdx
	testq	%rax, %rax
	movq	%rdx, 40(%rsp)
	je	.L304
	leaq	64(%rsp), %rbp
	leaq	40(%rsp), %rsi
	movq	%rbp, %rdi
.LEHB1:
	call	*88(%rsp)
.LEHE1:
	testb	%al, %al
	movq	80(%rsp), %rax
	je	.L187
	movq	8(%rbx), %rbx
	testq	%rbx, %rbx
	jne	.L188
.L187:
	testq	%rax, %rax
	je	.L189
.L185:
	movl	$3, %edx
	movq	%rbp, %rsi
	movq	%rbp, %rdi
	call	*%rax
.L189:
	movslq	16(%rsp), %rdi
	movl	$12, %esi
	call	calloc
	movq	%rax, (%rsp)
.L191:
	movq	(%rsp), %rbx
	movl	24(%rsp), %edx
	movl	$-1, %ecx
	movl	%r14d, %edi
	movq	%rbx, %rsi
.LEHB2:
	call	epoll_wait
.LEHE2:
	testl	%eax, %eax
	jle	.L191
	subl	$1, %eax
	movq	%rbx, %r12
	leaq	3(%rax,%rax,2), %rax
	leaq	(%rbx,%rax,4), %r15
	jmp	.L201
	.p2align 4,,10
	.p2align 3
.L308:
	js	.L305
	addq	%rax, _ZN4xiix6meters5inputE(%rip)
	cmpb	$0, _ZN4xiix4conf13print_trafficE(%rip)
	jne	.L306
.L198:
	addq	%r13, %rbp
	movq	%rbp, 4232(%rbx)
	movb	$0, 0(%rbp)
.L192:
	movq	%rbx, %rdi
.LEHB3:
	call	_ZN4xiix4sock9parse_bufEv
.L214:
	cmpb	$0, _ZN4xiix4conf11print_statsE(%rip)
	jne	.L199
.L202:
	addq	$12, %r12
	cmpq	%r12, %r15
	je	.L191
.L201:
	addq	$1, _ZN4xiix6meters6eventsE(%rip)
	testb	$1, (%r12)
	movq	4(%r12), %rbx
	je	.L192
	addq	$1, _ZN4xiix6meters5readsE(%rip)
	movq	4232(%rbx), %rax
	cmpq	%rax, 4224(%rbx)
	jne	.L307
	leaq	120(%rbx), %rbp
	movl	4(%rbx), %edi
	movb	$0, 120(%rbx)
	xorl	%ecx, %ecx
	movl	$4096, %edx
	movq	%rbp, 4224(%rbx)
	movq	%rbp, %rsi
	call	recv
	testq	%rax, %rax
	movq	%rax, %r13
	jne	.L308
	movl	$8, %edi
	call	__cxa_allocate_exception
	xorl	%edx, %edx
	movq	$.LC10, (%rax)
	movl	$_ZTIPKc, %esi
	movq	%rax, %rdi
	call	__cxa_throw
.LEHE3:
	.p2align 4,,10
	.p2align 3
.L199:
	movq	_ZN4xiix6meters5readsE(%rip), %r8
	movq	_ZN4xiix6meters6writesE(%rip), %rcx
	movl	$.LC38, %edi
	movq	_ZN4xiix6meters6eventsE(%rip), %rdx
	movq	_ZN4xiix6meters8requestsE(%rip), %rsi
	xorl	%eax, %eax
.LEHB4:
	call	printf
.LEHE4:
	jmp	.L202
	.p2align 4,,10
	.p2align 3
.L306:
	movq	%rax, %rdx
	movq	%rbp, %rsi
	movl	$1, %edi
.LEHB5:
	call	write
	jmp	.L198
	.p2align 4,,10
	.p2align 3
.L305:
	call	__errno_location
	movl	(%rax), %eax
	cmpl	$11, %eax
	je	.L309
	cmpl	$104, %eax
	je	.L310
.L197:
	movl	$8, %edi
	call	__cxa_allocate_exception
	xorl	%edx, %edx
	movq	$.LC37, (%rax)
	movl	$_ZTIPKc, %esi
	movq	%rax, %rdi
	call	__cxa_throw
.LEHE5:
.L299:
	movb	$0, _ZN4xiix4conf13print_contentE(%rip)
	jmp	.L173
.L300:
	movl	$.LC25, %edi
.LEHB6:
	call	puts
	cmpl	$80, %ebx
	je	.L175
	movq	(%rsp), %rcx
	movl	%ebx, %edx
	movq	%r13, %rsi
	movl	$.LC26, %edi
	xorl	%eax, %eax
	call	printf
.L176:
	movl	16(%rsp), %esi
	movl	$.LC28, %edi
	xorl	%eax, %eax
	movl	$.LC22, %ebp
	movl	$.LC23, %ebx
	call	printf
	cmpb	$0, 31(%rsp)
	movq	%rbp, %rsi
	movl	$.LC29, %edi
	cmove	%rbx, %rsi
	xorl	%eax, %eax
	call	printf
	cmpb	$0, _ZN4xiix4conf13print_trafficE(%rip)
	movq	%rbp, %rsi
	movl	$.LC30, %edi
	cmove	%rbx, %rsi
	xorl	%eax, %eax
	call	printf
	cmpb	$0, _ZN4xiix4conf11print_statsE(%rip)
	movq	%rbp, %rsi
	movl	$.LC31, %edi
	cmove	%rbx, %rsi
	xorl	%eax, %eax
	call	printf
	cmpb	$0, _ZN4xiix4conf13print_contentE(%rip)
	movq	%rbp, %rsi
	movl	$.LC32, %edi
	cmove	%rbx, %rsi
	xorl	%eax, %eax
	call	printf
	movl	$.LC33, %edi
	call	puts
	jmp	.L174
.L295:
	movq	0(%rbp), %rdx
	movq	stderr(%rip), %rdi
	movl	$.LC24, %esi
	xorl	%eax, %eax
	call	fprintf
	xorl	%eax, %eax
.L158:
	addq	$104, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 56
	popq	%rbx
	.cfi_def_cfa_offset 48
	popq	%rbp
	.cfi_def_cfa_offset 40
	popq	%r12
	.cfi_def_cfa_offset 32
	popq	%r13
	.cfi_def_cfa_offset 24
	popq	%r14
	.cfi_def_cfa_offset 16
	popq	%r15
	.cfi_def_cfa_offset 8
	ret
.L175:
	.cfi_restore_state
	movq	(%rsp), %rdx
	movq	%r13, %rsi
	movl	$.LC27, %edi
	xorl	%eax, %eax
	call	printf
.LEHE6:
	jmp	.L176
.L310:
	movl	$8, %edi
	call	__cxa_allocate_exception
	xorl	%edx, %edx
	movq	$.LC36, (%rax)
	movl	$_ZTIPKc, %esi
	movq	%rax, %rdi
.LEHB7:
	call	__cxa_throw
.LEHE7:
.L234:
	movl	$_ZNSt14_Function_base13_Base_managerIZN4xiix6client4mainEiPPKcEUlPNS1_4sockEE_E10_M_managerERSt9_Any_dataRKSA_St18_Manager_operation, %eax
	leaq	64(%rsp), %rbp
	jmp	.L185
.L237:
	subq	$1, %rdx
	movq	%rax, %rdi
	jne	.L288
	call	__cxa_begin_catch
	movl	$.LC13, %edi
	movq	%rax, %rsi
	movq	%rax, %rbp
	call	strcmp
	testl	%eax, %eax
	je	.L210
	movq	%rbp, %rsi
	movl	$.LC39, %edi
	call	strcmp
	testl	%eax, %eax
	je	.L210
	movq	%rbp, %rsi
	movl	$.LC10, %edi
	call	strcmp
	testl	%eax, %eax
	jne	.L211
.L210:
	testq	%rbx, %rbx
	je	.L212
	movl	$4536, %esi
	movq	%rbx, %rdi
	subq	$1, _ZN4xiix6meters5socksE(%rip)
	call	_ZdlPvm
.L212:
	cmpq	$0, _ZN4xiix6meters5socksE(%rip)
	je	.L213
.L216:
	call	__cxa_end_catch
	jmp	.L214
.L222:
	movl	$.LC19, %r13d
	movl	$.LC18, %edi
	jmp	.L161
.L298:
	movq	$.LC21, (%rsp)
	movl	$.LC20, %edi
	jmp	.L167
.L296:
	movq	%rdi, %r13
	subl	$2, %ebx
	jmp	.L163
.L182:
	movzbl	31(%rsp), %eax
	movq	$_ZNSt17_Function_handlerIFbPN4xiix4sockEEZNS0_6client4mainEiPPKcEUlS2_E_E9_M_invokeERKSt9_Any_dataOS2_, 88(%rsp)
	leaq	64(%rsp), %rbp
	movq	$_ZNSt14_Function_base13_Base_managerIZN4xiix6client4mainEiPPKcEUlPNS1_4sockEE_E10_M_managerERSt9_Any_dataRKSA_St18_Manager_operation, 80(%rsp)
	movq	$0, 8(%rsp)
	movb	%al, 64(%rsp)
	movq	(%rsp), %rax
	movq	%rax, 72(%rsp)
	movl	$_ZNSt14_Function_base13_Base_managerIZN4xiix6client4mainEiPPKcEUlPNS1_4sockEE_E10_M_managerERSt9_Any_dataRKSA_St18_Manager_operation, %eax
	jmp	.L185
.L304:
.LEHB8:
	call	_ZSt25__throw_bad_function_callv
.LEHE8:
.L236:
	movq	%rax, %rbx
	movq	80(%rsp), %rax
	testq	%rax, %rax
	je	.L287
	leaq	64(%rsp), %rsi
	movl	$3, %edx
	movq	%rsi, %rdi
	call	*%rax
.L287:
	movq	8(%rsp), %rax
	testq	%rax, %rax
	je	.L219
	movq	8(%rax), %rbp
	movl	$16, %esi
	movq	%rax, %rdi
	call	_ZdlPvm
	movq	%rbp, 8(%rsp)
	jmp	.L287
.L307:
	movl	$8, %edi
	call	__cxa_allocate_exception
	xorl	%edx, %edx
	movq	$.LC35, (%rax)
	movl	$_ZTIPKc, %esi
	movq	%rax, %rdi
.LEHB9:
	call	__cxa_throw
.LEHE9:
.L301:
	movl	$8, %edi
	call	__cxa_allocate_exception
	xorl	%edx, %edx
	movq	$.LC34, (%rax)
	movl	$_ZTIPKc, %esi
	movq	%rax, %rdi
.LEHB10:
	call	__cxa_throw
.LEHE10:
.L288:
	call	__cxa_begin_catch
	movl	$.LC40, %edi
.LEHB11:
	call	puts
.LEHE11:
	movq	(%rsp), %rdi
	call	free
.LEHB12:
	call	__cxa_end_catch
.LEHE12:
	orl	$-1, %eax
.L203:
	movq	8(%rsp), %rcx
	testq	%rcx, %rcx
	je	.L158
	movq	8(%rcx), %rbx
	movl	$16, %esi
	movq	%rcx, %rdi
	movl	%eax, 24(%rsp)
	call	_ZdlPvm
	movq	%rbx, 8(%rsp)
	movl	24(%rsp), %eax
	jmp	.L203
.L211:
	movl	$.LC40, %edi
.LEHB13:
	call	puts
	movq	%rbp, %rdi
	call	puts
.LEHE13:
	jmp	.L216
.L239:
	movq	%rax, %rbx
	call	__cxa_end_catch
	jmp	.L287
.L238:
	movq	%rax, %rbx
	call	__cxa_end_catch
	jmp	.L287
.L309:
	movl	4(%rbx), %edx
	movl	(%rbx), %edi
	leaq	32(%rbx), %rcx
	movl	$-2147483647, 32(%rbx)
	movl	$3, %esi
	call	epoll_ctl
	testl	%eax, %eax
	je	.L197
.LEHB14:
	call	_ZN4xiix4sock15io_request_readEv.part.19
.LEHE14:
.L240:
	movq	%rax, %rbx
	jmp	.L287
.L213:
	movq	(%rsp), %rdi
	call	free
	call	__cxa_end_catch
	xorl	%eax, %eax
	jmp	.L203
.L219:
	movq	%rbx, %rdi
.LEHB15:
	call	_Unwind_Resume
.LEHE15:
	.cfi_endproc
.LFE1484:
	.globl	__gxx_personality_v0
	.section	.gcc_except_table._ZN4xiix6client4mainEiPPKc,"aG",@progbits,_ZN4xiix6client4mainEiPPKc,comdat
	.align 4
.LLSDA1484:
	.byte	0xff
	.byte	0x3
	.uleb128 .LLSDATT1484-.LLSDATTD1484
.LLSDATTD1484:
	.byte	0x1
	.uleb128 .LLSDACSE1484-.LLSDACSB1484
.LLSDACSB1484:
	.uleb128 .LEHB0-.LFB1484
	.uleb128 .LEHE0-.LEHB0
	.uleb128 .L240-.LFB1484
	.uleb128 0
	.uleb128 .LEHB1-.LFB1484
	.uleb128 .LEHE1-.LEHB1
	.uleb128 .L236-.LFB1484
	.uleb128 0
	.uleb128 .LEHB2-.LFB1484
	.uleb128 .LEHE2-.LEHB2
	.uleb128 .L240-.LFB1484
	.uleb128 0
	.uleb128 .LEHB3-.LFB1484
	.uleb128 .LEHE3-.LEHB3
	.uleb128 .L237-.LFB1484
	.uleb128 0x3
	.uleb128 .LEHB4-.LFB1484
	.uleb128 .LEHE4-.LEHB4
	.uleb128 .L240-.LFB1484
	.uleb128 0
	.uleb128 .LEHB5-.LFB1484
	.uleb128 .LEHE5-.LEHB5
	.uleb128 .L237-.LFB1484
	.uleb128 0x3
	.uleb128 .LEHB6-.LFB1484
	.uleb128 .LEHE6-.LEHB6
	.uleb128 0
	.uleb128 0
	.uleb128 .LEHB7-.LFB1484
	.uleb128 .LEHE7-.LEHB7
	.uleb128 .L237-.LFB1484
	.uleb128 0x3
	.uleb128 .LEHB8-.LFB1484
	.uleb128 .LEHE8-.LEHB8
	.uleb128 .L236-.LFB1484
	.uleb128 0
	.uleb128 .LEHB9-.LFB1484
	.uleb128 .LEHE9-.LEHB9
	.uleb128 .L237-.LFB1484
	.uleb128 0x3
	.uleb128 .LEHB10-.LFB1484
	.uleb128 .LEHE10-.LEHB10
	.uleb128 0
	.uleb128 0
	.uleb128 .LEHB11-.LFB1484
	.uleb128 .LEHE11-.LEHB11
	.uleb128 .L239-.LFB1484
	.uleb128 0
	.uleb128 .LEHB12-.LFB1484
	.uleb128 .LEHE12-.LEHB12
	.uleb128 .L240-.LFB1484
	.uleb128 0
	.uleb128 .LEHB13-.LFB1484
	.uleb128 .LEHE13-.LEHB13
	.uleb128 .L238-.LFB1484
	.uleb128 0
	.uleb128 .LEHB14-.LFB1484
	.uleb128 .LEHE14-.LEHB14
	.uleb128 .L237-.LFB1484
	.uleb128 0x3
	.uleb128 .LEHB15-.LFB1484
	.uleb128 .LEHE15-.LEHB15
	.uleb128 0
	.uleb128 0
.LLSDACSE1484:
	.byte	0x2
	.byte	0
	.byte	0x1
	.byte	0x7d
	.align 4
	.long	0

	.long	_ZTIPKc
.LLSDATT1484:
	.section	.text.startup._ZN4xiix6client4mainEiPPKc,"axG",@progbits,_ZN4xiix6client4mainEiPPKc,comdat
	.size	_ZN4xiix6client4mainEiPPKc, .-_ZN4xiix6client4mainEiPPKc
	.section	.rodata.str1.1
.LC41:
	.string	"exception caught"
	.section	.text.startup,"ax",@progbits
	.p2align 4,,15
	.globl	main
	.type	main, @function
main:
.LFB1489:
	.cfi_startproc
	.cfi_personality 0x3,__gxx_personality_v0
	.cfi_lsda 0x3,.LLSDA1489
	pushq	%rbx
	.cfi_def_cfa_offset 16
	.cfi_offset 3, -16
	movq	%rsi, %rdx
	movl	%edi, %esi
	subq	$16, %rsp
	.cfi_def_cfa_offset 32
	leaq	15(%rsp), %rdi
.LEHB16:
	call	_ZN4xiix6client4mainEiPPKc
.LEHE16:
.L312:
	addq	$16, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 16
	xorl	%eax, %eax
	popq	%rbx
	.cfi_def_cfa_offset 8
	ret
.L321:
	.cfi_restore_state
	subq	$1, %rdx
	movq	%rax, %rdi
	jne	.L322
	call	__cxa_begin_catch
	movq	%rax, %rdi
.LEHB17:
	call	puts
.LEHE17:
.L316:
	call	__cxa_end_catch
	jmp	.L312
.L322:
	call	__cxa_begin_catch
	movl	$.LC41, %edi
.LEHB18:
	call	puts
.LEHE18:
	jmp	.L316
.L320:
	movq	%rax, %rbx
	call	__cxa_end_catch
	movq	%rbx, %rdi
.LEHB19:
	call	_Unwind_Resume
.LEHE19:
.L319:
	movq	%rax, %rbx
	call	__cxa_end_catch
	movq	%rbx, %rdi
.LEHB20:
	call	_Unwind_Resume
.LEHE20:
	.cfi_endproc
.LFE1489:
	.section	.gcc_except_table,"a",@progbits
	.align 4
.LLSDA1489:
	.byte	0xff
	.byte	0x3
	.uleb128 .LLSDATT1489-.LLSDATTD1489
.LLSDATTD1489:
	.byte	0x1
	.uleb128 .LLSDACSE1489-.LLSDACSB1489
.LLSDACSB1489:
	.uleb128 .LEHB16-.LFB1489
	.uleb128 .LEHE16-.LEHB16
	.uleb128 .L321-.LFB1489
	.uleb128 0x3
	.uleb128 .LEHB17-.LFB1489
	.uleb128 .LEHE17-.LEHB17
	.uleb128 .L320-.LFB1489
	.uleb128 0
	.uleb128 .LEHB18-.LFB1489
	.uleb128 .LEHE18-.LEHB18
	.uleb128 .L319-.LFB1489
	.uleb128 0
	.uleb128 .LEHB19-.LFB1489
	.uleb128 .LEHE19-.LEHB19
	.uleb128 0
	.uleb128 0
	.uleb128 .LEHB20-.LFB1489
	.uleb128 .LEHE20-.LEHB20
	.uleb128 0
	.uleb128 0
.LLSDACSE1489:
	.byte	0x2
	.byte	0
	.byte	0x1
	.byte	0x7d
	.align 4
	.long	0

	.long	_ZTIPKc
.LLSDATT1489:
	.section	.text.startup
	.size	main, .-main
	.weak	_ZTSZN4xiix6client4mainEiPPKcEUlPNS_4sockEE_
	.section	.rodata._ZTSZN4xiix6client4mainEiPPKcEUlPNS_4sockEE_,"aG",@progbits,_ZTSZN4xiix6client4mainEiPPKcEUlPNS_4sockEE_,comdat
	.align 32
	.type	_ZTSZN4xiix6client4mainEiPPKcEUlPNS_4sockEE_, @object
	.size	_ZTSZN4xiix6client4mainEiPPKcEUlPNS_4sockEE_, 41
_ZTSZN4xiix6client4mainEiPPKcEUlPNS_4sockEE_:
	.string	"ZN4xiix6client4mainEiPPKcEUlPNS_4sockEE_"
	.weak	_ZTIZN4xiix6client4mainEiPPKcEUlPNS_4sockEE_
	.section	.rodata._ZTIZN4xiix6client4mainEiPPKcEUlPNS_4sockEE_,"aG",@progbits,_ZTIZN4xiix6client4mainEiPPKcEUlPNS_4sockEE_,comdat
	.align 8
	.type	_ZTIZN4xiix6client4mainEiPPKcEUlPNS_4sockEE_, @object
	.size	_ZTIZN4xiix6client4mainEiPPKcEUlPNS_4sockEE_, 16
_ZTIZN4xiix6client4mainEiPPKcEUlPNS_4sockEE_:
	.quad	_ZTVN10__cxxabiv117__class_type_infoE+16
	.quad	_ZTSZN4xiix6client4mainEiPPKcEUlPNS_4sockEE_
	.globl	_ZN4xiix4conf13print_contentE
	.data
	.type	_ZN4xiix4conf13print_contentE, @object
	.size	_ZN4xiix4conf13print_contentE, 1
_ZN4xiix4conf13print_contentE:
	.byte	1
	.globl	_ZN4xiix4conf11print_statsE
	.bss
	.type	_ZN4xiix4conf11print_statsE, @object
	.size	_ZN4xiix4conf11print_statsE, 1
_ZN4xiix4conf11print_statsE:
	.zero	1
	.globl	_ZN4xiix4conf13print_trafficE
	.type	_ZN4xiix4conf13print_trafficE, @object
	.size	_ZN4xiix4conf13print_trafficE, 1
_ZN4xiix4conf13print_trafficE:
	.zero	1
	.globl	_ZN4xiix6meters6errorsE
	.align 8
	.type	_ZN4xiix6meters6errorsE, @object
	.size	_ZN4xiix6meters6errorsE, 8
_ZN4xiix6meters6errorsE:
	.zero	8
	.globl	_ZN4xiix6meters4brksE
	.align 8
	.type	_ZN4xiix6meters4brksE, @object
	.size	_ZN4xiix6meters4brksE, 8
_ZN4xiix6meters4brksE:
	.zero	8
	.globl	_ZN4xiix6meters6writesE
	.align 8
	.type	_ZN4xiix6meters6writesE, @object
	.size	_ZN4xiix6meters6writesE, 8
_ZN4xiix6meters6writesE:
	.zero	8
	.globl	_ZN4xiix6meters5readsE
	.align 8
	.type	_ZN4xiix6meters5readsE, @object
	.size	_ZN4xiix6meters5readsE, 8
_ZN4xiix6meters5readsE:
	.zero	8
	.globl	_ZN4xiix6meters5opensE
	.align 8
	.type	_ZN4xiix6meters5opensE, @object
	.size	_ZN4xiix6meters5opensE, 8
_ZN4xiix6meters5opensE:
	.zero	8
	.globl	_ZN4xiix6meters8requestsE
	.align 8
	.type	_ZN4xiix6meters8requestsE, @object
	.size	_ZN4xiix6meters8requestsE, 8
_ZN4xiix6meters8requestsE:
	.zero	8
	.globl	_ZN4xiix6meters6outputE
	.align 8
	.type	_ZN4xiix6meters6outputE, @object
	.size	_ZN4xiix6meters6outputE, 8
_ZN4xiix6meters6outputE:
	.zero	8
	.globl	_ZN4xiix6meters5inputE
	.align 8
	.type	_ZN4xiix6meters5inputE, @object
	.size	_ZN4xiix6meters5inputE, 8
_ZN4xiix6meters5inputE:
	.zero	8
	.globl	_ZN4xiix6meters6eventsE
	.align 8
	.type	_ZN4xiix6meters6eventsE, @object
	.size	_ZN4xiix6meters6eventsE, 8
_ZN4xiix6meters6eventsE:
	.zero	8
	.globl	_ZN4xiix6meters5socksE
	.align 8
	.type	_ZN4xiix6meters5socksE, @object
	.size	_ZN4xiix6meters5socksE, 8
_ZN4xiix6meters5socksE:
	.zero	8
	.ident	"GCC: (GNU) 6.1.1 20160501"
	.section	.note.GNU-stack,"",@progbits
