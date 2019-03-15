  401190:	push   r15
  401192:	push   r14
  401194:	mov    esi,0x401b10
  401199:	push   r13
  40119b:	push   r12
  40119d:	push   rbp
  40119e:	push   rbx
  40119f:	mov    rbx,rdi
  4011a2:	mov    edi,0x401ae0
  4011a7:	sub    rsp,0x8
  4011ab:	call   401640 <hash_new>
  4011b0:	test   rbx,rbx
  4011b3:	mov    r13,rax
  4011b6:	je     4012b2 <group_into_basic_blocks+0x122>
  4011bc:	mov    rax,QWORD PTR [rbx]
  4011bf:	test   rax,rax
  4011c2:	je     4011f8 <group_into_basic_blocks+0x68>
  4011c4:	mov    rbp,rbx
  4011c7:	jmp    4011d9 <group_into_basic_blocks+0x49>
  4011c9:	nop    DWORD PTR [rax+0x0]
  4011d0:	mov    rax,QWORD PTR [rbp+0x0]
  4011d4:	test   rax,rax
  4011d7:	je     4011f8 <group_into_basic_blocks+0x68>
  4011d9:	mov    rsi,QWORD PTR [rax+0x10]
  4011dd:	test   rsi,rsi
  4011e0:	je     4011ef <group_into_basic_blocks+0x5f>
  4011e2:	mov    edx,0x1
  4011e7:	mov    rdi,r13
  4011ea:	call   401700 <hash_put>
  4011ef:	mov    rbp,QWORD PTR [rbp+0x8]
  4011f3:	test   rbp,rbp
  4011f6:	jne    4011d0 <group_into_basic_blocks+0x40>
  4011f8:	xor    eax,eax
  4011fa:	xor    r12d,r12d
  4011fd:	mov    r15d,0x1
  401203:	call   401b30 <list_empty>
  401208:	mov    rbp,QWORD PTR [rbx]
  40120b:	mov    r14,rax
  40120e:	test   rbp,rbp
  401211:	jne    401246 <group_into_basic_blocks+0xb6>
  401213:	jmp    4012c0 <group_into_basic_blocks+0x130>
  401218:	nop    DWORD PTR [rax+rax*1+0x0]
  401220:	mov    rsi,r12
  401223:	mov    rdi,rbp
  401226:	call   401b40 <list_cons>
  40122b:	mov    rbx,QWORD PTR [rbx+0x8]
  40122f:	mov    r12,rax
  401232:	test   rbx,rbx
  401235:	je     4012c0 <group_into_basic_blocks+0x130>
  40123b:	mov    rbp,QWORD PTR [rbx]
  40123e:	xor    r15d,r15d
  401241:	test   rbp,rbp
  401244:	je     4012c0 <group_into_basic_blocks+0x130>
  401246:	mov    rsi,QWORD PTR [rbp+0x8]
  40124a:	test   rsi,rsi
  40124d:	je     40125b <group_into_basic_blocks+0xcb>
  40124f:	mov    rdi,r13
  401252:	call   4018d0 <hash_contains>
  401257:	test   al,al
  401259:	jne    401260 <group_into_basic_blocks+0xd0>
  40125b:	test   r15b,r15b
  40125e:	je     401220 <group_into_basic_blocks+0x90>
  401260:	test   r12,r12
  401263:	je     4012a3 <group_into_basic_blocks+0x113>
  401265:	mov    rdi,r12
  401268:	call   401c50 <list_reverse>
  40126d:	mov    rdi,r12
  401270:	mov    r15,rax
  401273:	call   401b70 <list_free>
  401278:	mov    rdi,r15
  40127b:	call   401ba0 <list_isEmpty>
  401280:	test   al,al
  401282:	jne    40133c <group_into_basic_blocks+0x1ac>
  401288:	mov    edi,0x8
  40128d:	call   400910 <malloc@plt>
  401292:	mov    rsi,r14
  401295:	mov    QWORD PTR [rax],r15
  401298:	mov    rdi,rax
  40129b:	call   401b40 <list_cons>
  4012a0:	mov    r14,rax
  4012a3:	xor    eax,eax
  4012a5:	call   401b30 <list_empty>
  4012aa:	mov    r12,rax
  4012ad:	jmp    401220 <group_into_basic_blocks+0x90>
  4012b2:	xor    eax,eax
  4012b4:	xor    r12d,r12d
  4012b7:	call   401b30 <list_empty>
  4012bc:	mov    r14,rax
  4012bf:	nop
  4012c0:	mov    rdi,r12
  4012c3:	call   401ba0 <list_isEmpty>
  4012c8:	test   al,al
  4012ca:	je     401300 <group_into_basic_blocks+0x170>
  4012cc:	mov    rdi,r13
  4012cf:	call   401690 <hash_free>
  4012d4:	mov    rdi,r14
  4012d7:	call   401c50 <list_reverse>
  4012dc:	mov    rdi,r14
  4012df:	mov    rbx,rax
  4012e2:	call   401b70 <list_free>
  4012e7:	add    rsp,0x8
  4012eb:	mov    rax,rbx
  4012ee:	pop    rbx
  4012ef:	pop    rbp
  4012f0:	pop    r12
  4012f2:	pop    r13
  4012f4:	pop    r14
  4012f6:	pop    r15
  4012f8:	ret    
  4012f9:	nop    DWORD PTR [rax+0x0]
  401300:	mov    rdi,r12
  401303:	call   401c50 <list_reverse>
  401308:	mov    rdi,r12
  40130b:	mov    rbx,rax
  40130e:	call   401b70 <list_free>
  401313:	mov    rdi,rbx
  401316:	call   401ba0 <list_isEmpty>
  40131b:	test   al,al
  40131d:	jne    40133c <group_into_basic_blocks+0x1ac>
  40131f:	mov    edi,0x8
  401324:	call   400910 <malloc@plt>
  401329:	mov    rsi,r14
  40132c:	mov    QWORD PTR [rax],rbx
  40132f:	mov    rdi,rax
  401332:	call   401b40 <list_cons>
  401337:	mov    r14,rax
  40133a:	jmp    4012cc <group_into_basic_blocks+0x13c>
  40133c:	call   400970 <bblock_new.part.0>
  401341:	nop    DWORD PTR [rax+rax*1+0x0]
  401346:	nop    WORD PTR cs:[rax+rax*1+0x0]
