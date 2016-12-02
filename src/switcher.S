.text
.global thread_frame_switch
thread_frame_switch:
	movq 4(%rsp), %rax
	movq 8(%rsp), %rdx

	pushq %rbx
	pushq %rcx
	pushq %rbp
	pushq %rsi
	pushq %rdi

        movq %rsp, (%rax)
	movq %rdx, %rsp

	popq %rdi
	popq %rsi
	popq %rbp
	popq %rcx
	popq %rbx

	iret

