#--------------------- s.s file --------------------------
	.global main, mymain, myprintf
main:    
        pushl  %ebp
        movl   %esp, %ebp
	pushl $FMT
	call myprintf
	pushl b
	pushl a
	pushl $fmt
	call myprintf
	pushl 16(%ebp)
	pushl 12(%ebp)
	pushl 8(%ebp)
	call mymain
	movl  %ebp, %esp
	popl  %ebp
	ret
#---------- DATA section of assembly code ---------------
	.data
FMT:	.asciz "main() in assembly call mymain() in C\n"
a:	.long 1234
b:	.long 5678
fmt:	.asciz "a=%d b=%d\n"
#---------  end of s.s file ----------------------------
