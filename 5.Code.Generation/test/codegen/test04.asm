
test04:     file format elf32-i386


Disassembly of section .init:

08048268 <_init>:
 8048268:	53                   	push   %ebx
 8048269:	83 ec 08             	sub    $0x8,%esp
 804826c:	e8 7f 00 00 00       	call   80482f0 <__x86.get_pc_thunk.bx>
 8048271:	81 c3 8f 1d 00 00    	add    $0x1d8f,%ebx
 8048277:	8b 83 fc ff ff ff    	mov    -0x4(%ebx),%eax
 804827d:	85 c0                	test   %eax,%eax
 804827f:	74 05                	je     8048286 <_init+0x1e>
 8048281:	e8 2a 00 00 00       	call   80482b0 <__libc_start_main@plt+0x10>
 8048286:	83 c4 08             	add    $0x8,%esp
 8048289:	5b                   	pop    %ebx
 804828a:	c3                   	ret    

Disassembly of section .plt:

08048290 <__libc_start_main@plt-0x10>:
 8048290:	ff 35 04 a0 04 08    	pushl  0x804a004
 8048296:	ff 25 08 a0 04 08    	jmp    *0x804a008
 804829c:	00 00                	add    %al,(%eax)
	...

080482a0 <__libc_start_main@plt>:
 80482a0:	ff 25 0c a0 04 08    	jmp    *0x804a00c
 80482a6:	68 00 00 00 00       	push   $0x0
 80482ab:	e9 e0 ff ff ff       	jmp    8048290 <_init+0x28>

Disassembly of section .plt.got:

080482b0 <.plt.got>:
 80482b0:	ff 25 fc 9f 04 08    	jmp    *0x8049ffc
 80482b6:	66 90                	xchg   %ax,%ax

Disassembly of section .text:

080482c0 <_start>:
 80482c0:	31 ed                	xor    %ebp,%ebp
 80482c2:	5e                   	pop    %esi
 80482c3:	89 e1                	mov    %esp,%ecx
 80482c5:	83 e4 f0             	and    $0xfffffff0,%esp
 80482c8:	50                   	push   %eax
 80482c9:	54                   	push   %esp
 80482ca:	52                   	push   %edx
 80482cb:	68 30 87 04 08       	push   $0x8048730
 80482d0:	68 c0 86 04 08       	push   $0x80486c0
 80482d5:	51                   	push   %ecx
 80482d6:	56                   	push   %esi
 80482d7:	68 4c 85 04 08       	push   $0x804854c
 80482dc:	e8 bf ff ff ff       	call   80482a0 <__libc_start_main@plt>
 80482e1:	f4                   	hlt    
 80482e2:	66 90                	xchg   %ax,%ax
 80482e4:	66 90                	xchg   %ax,%ax
 80482e6:	66 90                	xchg   %ax,%ax
 80482e8:	66 90                	xchg   %ax,%ax
 80482ea:	66 90                	xchg   %ax,%ax
 80482ec:	66 90                	xchg   %ax,%ax
 80482ee:	66 90                	xchg   %ax,%ax

080482f0 <__x86.get_pc_thunk.bx>:
 80482f0:	8b 1c 24             	mov    (%esp),%ebx
 80482f3:	c3                   	ret    
 80482f4:	66 90                	xchg   %ax,%ax
 80482f6:	66 90                	xchg   %ax,%ax
 80482f8:	66 90                	xchg   %ax,%ax
 80482fa:	66 90                	xchg   %ax,%ax
 80482fc:	66 90                	xchg   %ax,%ax
 80482fe:	66 90                	xchg   %ax,%ax
 8048300:	b8 33 a0 04 08       	mov    $0x804a033,%eax
 8048305:	2d 30 a0 04 08       	sub    $0x804a030,%eax
 804830a:	83 f8 06             	cmp    $0x6,%eax
 804830d:	76 1a                	jbe    8048329 <__x86.get_pc_thunk.bx+0x39>
 804830f:	b8 00 00 00 00       	mov    $0x0,%eax
 8048314:	85 c0                	test   %eax,%eax
 8048316:	74 11                	je     8048329 <__x86.get_pc_thunk.bx+0x39>
 8048318:	55                   	push   %ebp
 8048319:	89 e5                	mov    %esp,%ebp
 804831b:	83 ec 14             	sub    $0x14,%esp
 804831e:	68 30 a0 04 08       	push   $0x804a030
 8048323:	ff d0                	call   *%eax
 8048325:	83 c4 10             	add    $0x10,%esp
 8048328:	c9                   	leave  
 8048329:	c3                   	ret    
 804832a:	8d b6 00 00 00 00    	lea    0x0(%esi),%esi
 8048330:	b8 30 a0 04 08       	mov    $0x804a030,%eax
 8048335:	2d 30 a0 04 08       	sub    $0x804a030,%eax
 804833a:	c1 f8 02             	sar    $0x2,%eax
 804833d:	89 c2                	mov    %eax,%edx
 804833f:	c1 ea 1f             	shr    $0x1f,%edx
 8048342:	01 d0                	add    %edx,%eax
 8048344:	d1 f8                	sar    %eax
 8048346:	74 1b                	je     8048363 <__x86.get_pc_thunk.bx+0x73>
 8048348:	ba 00 00 00 00       	mov    $0x0,%edx
 804834d:	85 d2                	test   %edx,%edx
 804834f:	74 12                	je     8048363 <__x86.get_pc_thunk.bx+0x73>
 8048351:	55                   	push   %ebp
 8048352:	89 e5                	mov    %esp,%ebp
 8048354:	83 ec 10             	sub    $0x10,%esp
 8048357:	50                   	push   %eax
 8048358:	68 30 a0 04 08       	push   $0x804a030
 804835d:	ff d2                	call   *%edx
 804835f:	83 c4 10             	add    $0x10,%esp
 8048362:	c9                   	leave  
 8048363:	c3                   	ret    
 8048364:	8d b6 00 00 00 00    	lea    0x0(%esi),%esi
 804836a:	8d bf 00 00 00 00    	lea    0x0(%edi),%edi
 8048370:	80 3d 30 a0 04 08 00 	cmpb   $0x0,0x804a030
 8048377:	75 13                	jne    804838c <__x86.get_pc_thunk.bx+0x9c>
 8048379:	55                   	push   %ebp
 804837a:	89 e5                	mov    %esp,%ebp
 804837c:	83 ec 08             	sub    $0x8,%esp
 804837f:	e8 7c ff ff ff       	call   8048300 <__x86.get_pc_thunk.bx+0x10>
 8048384:	c6 05 30 a0 04 08 01 	movb   $0x1,0x804a030
 804838b:	c9                   	leave  
 804838c:	c3                   	ret    
 804838d:	8d 76 00             	lea    0x0(%esi),%esi
 8048390:	b8 10 9f 04 08       	mov    $0x8049f10,%eax
 8048395:	8b 10                	mov    (%eax),%edx
 8048397:	85 d2                	test   %edx,%edx
 8048399:	75 05                	jne    80483a0 <__x86.get_pc_thunk.bx+0xb0>
 804839b:	eb 93                	jmp    8048330 <__x86.get_pc_thunk.bx+0x40>
 804839d:	8d 76 00             	lea    0x0(%esi),%esi
 80483a0:	ba 00 00 00 00       	mov    $0x0,%edx
 80483a5:	85 d2                	test   %edx,%edx
 80483a7:	74 f2                	je     804839b <__x86.get_pc_thunk.bx+0xab>
 80483a9:	55                   	push   %ebp
 80483aa:	89 e5                	mov    %esp,%ebp
 80483ac:	83 ec 14             	sub    $0x14,%esp
 80483af:	50                   	push   %eax
 80483b0:	ff d2                	call   *%edx
 80483b2:	83 c4 10             	add    $0x10,%esp
 80483b5:	c9                   	leave  
 80483b6:	e9 75 ff ff ff       	jmp    8048330 <__x86.get_pc_thunk.bx+0x40>
 80483bb:	90                   	nop

080483bc <ReadInt>:
 80483bc:	55                   	push   %ebp
 80483bd:	89 e5                	mov    %esp,%ebp
 80483bf:	53                   	push   %ebx
 80483c0:	56                   	push   %esi
 80483c1:	57                   	push   %edi
 80483c2:	83 ec 2c             	sub    $0x2c,%esp
 80483c5:	8d 4d f4             	lea    -0xc(%ebp),%ecx
 80483c8:	8d 75 ff             	lea    -0x1(%ebp),%esi
 80483cb:	ba 01 00 00 00       	mov    $0x1,%edx
 80483d0:	bb 00 00 00 00       	mov    $0x0,%ebx
 80483d5:	b8 03 00 00 00       	mov    $0x3,%eax
 80483da:	cd 80                	int    $0x80
 80483dc:	85 c0                	test   %eax,%eax
 80483de:	74 42                	je     8048422 <ReadInt+0x66>
 80483e0:	8a 01                	mov    (%ecx),%al
 80483e2:	3c 0a                	cmp    $0xa,%al
 80483e4:	74 10                	je     80483f6 <ReadInt+0x3a>
 80483e6:	41                   	inc    %ecx
 80483e7:	39 ce                	cmp    %ecx,%esi
 80483e9:	74 0b                	je     80483f6 <ReadInt+0x3a>
 80483eb:	3c 2d                	cmp    $0x2d,%al
 80483ed:	74 dc                	je     80483cb <ReadInt+0xf>
 80483ef:	2c 30                	sub    $0x30,%al
 80483f1:	3c 0a                	cmp    $0xa,%al
 80483f3:	72 d6                	jb     80483cb <ReadInt+0xf>
 80483f5:	49                   	dec    %ecx
 80483f6:	89 ce                	mov    %ecx,%esi
 80483f8:	8a 01                	mov    (%ecx),%al
 80483fa:	3c 0a                	cmp    $0xa,%al
 80483fc:	74 22                	je     8048420 <ReadInt+0x64>
 80483fe:	3c 09                	cmp    $0x9,%al
 8048400:	74 1e                	je     8048420 <ReadInt+0x64>
 8048402:	3c 20                	cmp    $0x20,%al
 8048404:	74 1a                	je     8048420 <ReadInt+0x64>
 8048406:	ba 01 00 00 00       	mov    $0x1,%edx
 804840b:	8d 4d f0             	lea    -0x10(%ebp),%ecx
 804840e:	bb 00 00 00 00       	mov    $0x0,%ebx
 8048413:	b8 03 00 00 00       	mov    $0x3,%eax
 8048418:	cd 80                	int    $0x80
 804841a:	85 c0                	test   %eax,%eax
 804841c:	74 02                	je     8048420 <ReadInt+0x64>
 804841e:	eb d8                	jmp    80483f8 <ReadInt+0x3c>
 8048420:	89 f1                	mov    %esi,%ecx
 8048422:	c6 01 00             	movb   $0x0,(%ecx)
 8048425:	b8 00 00 00 00       	mov    $0x0,%eax
 804842a:	31 db                	xor    %ebx,%ebx
 804842c:	8d 4d f4             	lea    -0xc(%ebp),%ecx
 804842f:	bf 0a 00 00 00       	mov    $0xa,%edi
 8048434:	80 39 2d             	cmpb   $0x2d,(%ecx)
 8048437:	75 06                	jne    804843f <ReadInt+0x83>
 8048439:	bb 01 00 00 00       	mov    $0x1,%ebx
 804843e:	41                   	inc    %ecx
 804843f:	0f b6 31             	movzbl (%ecx),%esi
 8048442:	41                   	inc    %ecx
 8048443:	83 ee 30             	sub    $0x30,%esi
 8048446:	83 fe 09             	cmp    $0x9,%esi
 8048449:	77 06                	ja     8048451 <ReadInt+0x95>
 804844b:	f7 e7                	mul    %edi
 804844d:	01 f0                	add    %esi,%eax
 804844f:	eb ee                	jmp    804843f <ReadInt+0x83>
 8048451:	85 db                	test   %ebx,%ebx
 8048453:	74 02                	je     8048457 <ReadInt+0x9b>
 8048455:	f7 d8                	neg    %eax
 8048457:	83 c4 2c             	add    $0x2c,%esp
 804845a:	5f                   	pop    %edi
 804845b:	5e                   	pop    %esi
 804845c:	5b                   	pop    %ebx
 804845d:	5d                   	pop    %ebp
 804845e:	c3                   	ret    

0804845f <WriteInt>:
 804845f:	55                   	push   %ebp
 8048460:	89 e5                	mov    %esp,%ebp
 8048462:	53                   	push   %ebx
 8048463:	56                   	push   %esi
 8048464:	83 ec 10             	sub    $0x10,%esp
 8048467:	8b 45 08             	mov    0x8(%ebp),%eax
 804846a:	bb 0a 00 00 00       	mov    $0xa,%ebx
 804846f:	8d 4d f8             	lea    -0x8(%ebp),%ecx
 8048472:	89 c6                	mov    %eax,%esi
 8048474:	c1 ee 1f             	shr    $0x1f,%esi
 8048477:	74 02                	je     804847b <WriteInt+0x1c>
 8048479:	f7 d8                	neg    %eax
 804847b:	31 d2                	xor    %edx,%edx
 804847d:	f7 f3                	div    %ebx
 804847f:	83 c2 30             	add    $0x30,%edx
 8048482:	49                   	dec    %ecx
 8048483:	88 11                	mov    %dl,(%ecx)
 8048485:	85 c0                	test   %eax,%eax
 8048487:	75 f2                	jne    804847b <WriteInt+0x1c>
 8048489:	85 f6                	test   %esi,%esi
 804848b:	74 04                	je     8048491 <WriteInt+0x32>
 804848d:	49                   	dec    %ecx
 804848e:	c6 01 2d             	movb   $0x2d,(%ecx)
 8048491:	8d 55 f8             	lea    -0x8(%ebp),%edx
 8048494:	29 ca                	sub    %ecx,%edx
 8048496:	bb 01 00 00 00       	mov    $0x1,%ebx
 804849b:	b8 04 00 00 00       	mov    $0x4,%eax
 80484a0:	cd 80                	int    $0x80
 80484a2:	83 c4 10             	add    $0x10,%esp
 80484a5:	5e                   	pop    %esi
 80484a6:	5b                   	pop    %ebx
 80484a7:	5d                   	pop    %ebp
 80484a8:	c3                   	ret    

080484a9 <WriteStr>:
 80484a9:	55                   	push   %ebp
 80484aa:	89 e5                	mov    %esp,%ebp
 80484ac:	53                   	push   %ebx
 80484ad:	57                   	push   %edi
 80484ae:	83 ec 10             	sub    $0x10,%esp
 80484b1:	8b 5d 08             	mov    0x8(%ebp),%ebx
 80484b4:	89 1c 24             	mov    %ebx,(%esp)
 80484b7:	e8 82 00 00 00       	call   804853e <DOFS>
 80484bc:	01 c3                	add    %eax,%ebx
 80484be:	89 df                	mov    %ebx,%edi
 80484c0:	b9 ff ff ff ff       	mov    $0xffffffff,%ecx
 80484c5:	31 c0                	xor    %eax,%eax
 80484c7:	fc                   	cld    
 80484c8:	f2 ae                	repnz scas %es:(%edi),%al
 80484ca:	f7 d1                	not    %ecx
 80484cc:	49                   	dec    %ecx
 80484cd:	89 ca                	mov    %ecx,%edx
 80484cf:	89 d9                	mov    %ebx,%ecx
 80484d1:	bb 01 00 00 00       	mov    $0x1,%ebx
 80484d6:	b8 04 00 00 00       	mov    $0x4,%eax
 80484db:	cd 80                	int    $0x80
 80484dd:	83 c4 10             	add    $0x10,%esp
 80484e0:	5f                   	pop    %edi
 80484e1:	5b                   	pop    %ebx
 80484e2:	5d                   	pop    %ebp
 80484e3:	c3                   	ret    

080484e4 <WriteChar>:
 80484e4:	55                   	push   %ebp
 80484e5:	89 e5                	mov    %esp,%ebp
 80484e7:	53                   	push   %ebx
 80484e8:	83 ec 14             	sub    $0x14,%esp
 80484eb:	8a 45 08             	mov    0x8(%ebp),%al
 80484ee:	88 04 24             	mov    %al,(%esp)
 80484f1:	ba 01 00 00 00       	mov    $0x1,%edx
 80484f6:	89 e1                	mov    %esp,%ecx
 80484f8:	bb 01 00 00 00       	mov    $0x1,%ebx
 80484fd:	b8 04 00 00 00       	mov    $0x4,%eax
 8048502:	cd 80                	int    $0x80
 8048504:	83 c4 14             	add    $0x14,%esp
 8048507:	5b                   	pop    %ebx
 8048508:	5d                   	pop    %ebp
 8048509:	c3                   	ret    

0804850a <WriteLn>:
 804850a:	55                   	push   %ebp
 804850b:	89 e5                	mov    %esp,%ebp
 804850d:	53                   	push   %ebx
 804850e:	83 ec 14             	sub    $0x14,%esp
 8048511:	c6 04 24 0a          	movb   $0xa,(%esp)
 8048515:	ba 01 00 00 00       	mov    $0x1,%edx
 804851a:	89 e1                	mov    %esp,%ecx
 804851c:	bb 01 00 00 00       	mov    $0x1,%ebx
 8048521:	b8 04 00 00 00       	mov    $0x4,%eax
 8048526:	cd 80                	int    $0x80
 8048528:	83 c4 14             	add    $0x14,%esp
 804852b:	5b                   	pop    %ebx
 804852c:	5d                   	pop    %ebp
 804852d:	c3                   	ret    
 804852e:	66 90                	xchg   %ax,%ax

08048530 <DIM>:
 8048530:	8b 44 24 08          	mov    0x8(%esp),%eax
 8048534:	c1 e0 02             	shl    $0x2,%eax
 8048537:	03 44 24 04          	add    0x4(%esp),%eax
 804853b:	8b 00                	mov    (%eax),%eax
 804853d:	c3                   	ret    

0804853e <DOFS>:
 804853e:	8b 44 24 04          	mov    0x4(%esp),%eax
 8048542:	8b 00                	mov    (%eax),%eax
 8048544:	8d 04 85 04 00 00 00 	lea    0x4(,%eax,4),%eax
 804854b:	c3                   	ret    

0804854c <main>:
 804854c:	55                   	push   %ebp
 804854d:	89 e5                	mov    %esp,%ebp
 804854f:	53                   	push   %ebx
 8048550:	56                   	push   %esi
 8048551:	57                   	push   %edi
 8048552:	83 ec 3c             	sub    $0x3c,%esp
 8048555:	b8 00 00 00 00       	mov    $0x0,%eax
 804855a:	a3 2c a0 04 08       	mov    %eax,0x804a02c

0804855f <l_test04_2__while_cond>:
 804855f:	a1 2c a0 04 08       	mov    0x804a02c,%eax
 8048564:	bb 0a 00 00 00       	mov    $0xa,%ebx
 8048569:	39 d8                	cmp    %ebx,%eax
 804856b:	7c 05                	jl     8048572 <l_test04_3__while_body>
 804856d:	e9 8d 00 00 00       	jmp    80485ff <l_test04_1>

08048572 <l_test04_3__while_body>:
 8048572:	a1 2c a0 04 08       	mov    0x804a02c,%eax
 8048577:	bb 02 00 00 00       	mov    $0x2,%ebx
 804857c:	39 d8                	cmp    %ebx,%eax
 804857e:	7f 02                	jg     8048582 <l_test04_6_true>
 8048580:	eb 0a                	jmp    804858c <l_test04_7_false>

08048582 <l_test04_6_true>:
 8048582:	b8 01 00 00 00       	mov    $0x1,%eax
 8048587:	88 45 f3             	mov    %al,-0xd(%ebp)
 804858a:	eb 08                	jmp    8048594 <l_test04_8>

0804858c <l_test04_7_false>:
 804858c:	b8 00 00 00 00       	mov    $0x0,%eax
 8048591:	88 45 f3             	mov    %al,-0xd(%ebp)

08048594 <l_test04_8>:
 8048594:	8d 05 18 a0 04 08    	lea    0x804a018,%eax
 804859a:	89 45 ec             	mov    %eax,-0x14(%ebp)
 804859d:	a1 2c a0 04 08       	mov    0x804a02c,%eax
 80485a2:	bb 01 00 00 00       	mov    $0x1,%ebx
 80485a7:	f7 eb                	imul   %ebx
 80485a9:	89 45 d4             	mov    %eax,-0x2c(%ebp)
 80485ac:	8d 05 18 a0 04 08    	lea    0x804a018,%eax
 80485b2:	89 45 d0             	mov    %eax,-0x30(%ebp)
 80485b5:	8b 45 d0             	mov    -0x30(%ebp),%eax
 80485b8:	50                   	push   %eax
 80485b9:	e8 80 ff ff ff       	call   804853e <DOFS>
 80485be:	83 c4 04             	add    $0x4,%esp
 80485c1:	89 45 cc             	mov    %eax,-0x34(%ebp)
 80485c4:	8b 45 d4             	mov    -0x2c(%ebp),%eax
 80485c7:	8b 5d cc             	mov    -0x34(%ebp),%ebx
 80485ca:	01 d8                	add    %ebx,%eax
 80485cc:	89 45 c8             	mov    %eax,-0x38(%ebp)
 80485cf:	8b 45 ec             	mov    -0x14(%ebp),%eax
 80485d2:	8b 5d c8             	mov    -0x38(%ebp),%ebx
 80485d5:	01 d8                	add    %ebx,%eax
 80485d7:	89 45 c4             	mov    %eax,-0x3c(%ebp)
 80485da:	0f b6 45 f3          	movzbl -0xd(%ebp),%eax
 80485de:	8b 7d c4             	mov    -0x3c(%ebp),%edi
 80485e1:	89 07                	mov    %eax,(%edi)
 80485e3:	a1 2c a0 04 08       	mov    0x804a02c,%eax
 80485e8:	bb 01 00 00 00       	mov    $0x1,%ebx
 80485ed:	01 d8                	add    %ebx,%eax
 80485ef:	89 45 c0             	mov    %eax,-0x40(%ebp)
 80485f2:	8b 45 c0             	mov    -0x40(%ebp),%eax
 80485f5:	a3 2c a0 04 08       	mov    %eax,0x804a02c
 80485fa:	e9 60 ff ff ff       	jmp    804855f <l_test04_2__while_cond>

080485ff <l_test04_1>:
 80485ff:	b8 00 00 00 00       	mov    $0x0,%eax
 8048604:	a3 2c a0 04 08       	mov    %eax,0x804a02c

08048609 <l_test04_13__while_cond>:
 8048609:	a1 2c a0 04 08       	mov    0x804a02c,%eax
 804860e:	bb 0a 00 00 00       	mov    $0xa,%ebx
 8048613:	39 d8                	cmp    %ebx,%eax
 8048615:	7c 05                	jl     804861c <l_test04_14__while_body>
 8048617:	e9 90 00 00 00       	jmp    80486ac <l_test04_12>

0804861c <l_test04_14__while_body>:
 804861c:	8d 05 18 a0 04 08    	lea    0x804a018,%eax
 8048622:	89 45 bc             	mov    %eax,-0x44(%ebp)
 8048625:	a1 2c a0 04 08       	mov    0x804a02c,%eax
 804862a:	bb 01 00 00 00       	mov    $0x1,%ebx
 804862f:	f7 eb                	imul   %ebx
 8048631:	89 45 b8             	mov    %eax,-0x48(%ebp)
 8048634:	8d 05 18 a0 04 08    	lea    0x804a018,%eax
 804863a:	89 45 e8             	mov    %eax,-0x18(%ebp)
 804863d:	8b 45 e8             	mov    -0x18(%ebp),%eax
 8048640:	50                   	push   %eax
 8048641:	e8 f8 fe ff ff       	call   804853e <DOFS>
 8048646:	83 c4 04             	add    $0x4,%esp
 8048649:	89 45 e4             	mov    %eax,-0x1c(%ebp)
 804864c:	8b 45 b8             	mov    -0x48(%ebp),%eax
 804864f:	8b 5d e4             	mov    -0x1c(%ebp),%ebx
 8048652:	01 d8                	add    %ebx,%eax
 8048654:	89 45 e0             	mov    %eax,-0x20(%ebp)
 8048657:	8b 45 bc             	mov    -0x44(%ebp),%eax
 804865a:	8b 5d e0             	mov    -0x20(%ebp),%ebx
 804865d:	01 d8                	add    %ebx,%eax
 804865f:	89 45 dc             	mov    %eax,-0x24(%ebp)
 8048662:	8b 7d dc             	mov    -0x24(%ebp),%edi
 8048665:	8b 07                	mov    (%edi),%eax
 8048667:	bb 01 00 00 00       	mov    $0x1,%ebx
 804866c:	39 d8                	cmp    %ebx,%eax
 804866e:	74 02                	je     8048672 <l_test04_17_if_true>
 8048670:	eb 10                	jmp    8048682 <l_test04_18_if_false>

08048672 <l_test04_17_if_true>:
 8048672:	b8 01 00 00 00       	mov    $0x1,%eax
 8048677:	50                   	push   %eax
 8048678:	e8 e2 fd ff ff       	call   804845f <WriteInt>
 804867d:	83 c4 04             	add    $0x4,%esp
 8048680:	eb 0e                	jmp    8048690 <l_test04_16>

08048682 <l_test04_18_if_false>:
 8048682:	b8 00 00 00 00       	mov    $0x0,%eax
 8048687:	50                   	push   %eax
 8048688:	e8 d2 fd ff ff       	call   804845f <WriteInt>
 804868d:	83 c4 04             	add    $0x4,%esp

08048690 <l_test04_16>:
 8048690:	a1 2c a0 04 08       	mov    0x804a02c,%eax
 8048695:	bb 01 00 00 00       	mov    $0x1,%ebx
 804869a:	01 d8                	add    %ebx,%eax
 804869c:	89 45 d8             	mov    %eax,-0x28(%ebp)
 804869f:	8b 45 d8             	mov    -0x28(%ebp),%eax
 80486a2:	a3 2c a0 04 08       	mov    %eax,0x804a02c
 80486a7:	e9 5d ff ff ff       	jmp    8048609 <l_test04_13__while_cond>

080486ac <l_test04_12>:
 80486ac:	83 c4 3c             	add    $0x3c,%esp
 80486af:	5f                   	pop    %edi
 80486b0:	5e                   	pop    %esi
 80486b1:	5b                   	pop    %ebx
 80486b2:	5d                   	pop    %ebp
 80486b3:	c3                   	ret    
 80486b4:	66 90                	xchg   %ax,%ax
 80486b6:	66 90                	xchg   %ax,%ax
 80486b8:	66 90                	xchg   %ax,%ax
 80486ba:	66 90                	xchg   %ax,%ax
 80486bc:	66 90                	xchg   %ax,%ax
 80486be:	66 90                	xchg   %ax,%ax

080486c0 <__libc_csu_init>:
 80486c0:	55                   	push   %ebp
 80486c1:	57                   	push   %edi
 80486c2:	56                   	push   %esi
 80486c3:	31 f6                	xor    %esi,%esi
 80486c5:	53                   	push   %ebx
 80486c6:	e8 25 fc ff ff       	call   80482f0 <__x86.get_pc_thunk.bx>
 80486cb:	81 c3 35 19 00 00    	add    $0x1935,%ebx
 80486d1:	83 ec 1c             	sub    $0x1c,%esp
 80486d4:	8b 6c 24 30          	mov    0x30(%esp),%ebp
 80486d8:	8b 7c 24 34          	mov    0x34(%esp),%edi
 80486dc:	e8 87 fb ff ff       	call   8048268 <_init>
 80486e1:	8d 83 08 ff ff ff    	lea    -0xf8(%ebx),%eax
 80486e7:	8d 93 0c ff ff ff    	lea    -0xf4(%ebx),%edx
 80486ed:	29 c2                	sub    %eax,%edx
 80486ef:	c1 fa 02             	sar    $0x2,%edx
 80486f2:	89 54 24 0c          	mov    %edx,0xc(%esp)
 80486f6:	74 24                	je     804871c <__libc_csu_init+0x5c>
 80486f8:	90                   	nop
 80486f9:	8d b4 26 00 00 00 00 	lea    0x0(%esi,%eiz,1),%esi
 8048700:	83 ec 04             	sub    $0x4,%esp
 8048703:	ff 74 24 3c          	pushl  0x3c(%esp)
 8048707:	57                   	push   %edi
 8048708:	55                   	push   %ebp
 8048709:	ff 94 b3 08 ff ff ff 	call   *-0xf8(%ebx,%esi,4)
 8048710:	83 c4 10             	add    $0x10,%esp
 8048713:	83 c6 01             	add    $0x1,%esi
 8048716:	3b 74 24 0c          	cmp    0xc(%esp),%esi
 804871a:	75 e4                	jne    8048700 <__libc_csu_init+0x40>
 804871c:	83 c4 1c             	add    $0x1c,%esp
 804871f:	5b                   	pop    %ebx
 8048720:	5e                   	pop    %esi
 8048721:	5f                   	pop    %edi
 8048722:	5d                   	pop    %ebp
 8048723:	c3                   	ret    
 8048724:	8d b6 00 00 00 00    	lea    0x0(%esi),%esi
 804872a:	8d bf 00 00 00 00    	lea    0x0(%edi),%edi

08048730 <__libc_csu_fini>:
 8048730:	c3                   	ret    

Disassembly of section .fini:

08048734 <_fini>:
 8048734:	53                   	push   %ebx
 8048735:	83 ec 08             	sub    $0x8,%esp
 8048738:	e8 b3 fb ff ff       	call   80482f0 <__x86.get_pc_thunk.bx>
 804873d:	81 c3 c3 18 00 00    	add    $0x18c3,%ebx
 8048743:	83 c4 08             	add    $0x8,%esp
 8048746:	5b                   	pop    %ebx
 8048747:	c3                   	ret    
