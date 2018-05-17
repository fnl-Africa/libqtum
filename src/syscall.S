// User mode
.global __qtum_syscall
// long syscall(long number, long p1, long p2, long p3, long p4, long p5, long p6)
__qtum_syscall:
  push %ebp
  mov %esp,%ebp
  push %edi
  push %esi
  push %ebx
  mov 8+0*4(%ebp),%eax
  mov 8+1*4(%ebp),%ebx
  mov 8+2*4(%ebp),%ecx
  mov 8+3*4(%ebp),%edx
  mov 8+4*4(%ebp),%esi
  mov 8+5*4(%ebp),%edi
  mov 8+6*4(%ebp),%ebp
  int $0x40
  pop %ebx
  pop %esi
  pop %edi
  pop %ebp
  ret
