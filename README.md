# 伙伴堆内存分配

参考资料 [wiki buddy memory allocation](<https://en.wikipedia.org/wiki/Buddy_memory_allocation>)



实现了自己的malloc 和free函数, malloc并不实际分配内存,内存的分配用地址号代替.

维护了一个空闲链表数组,每条链可分配的内存块不同,都是2的幂次个页.

实现了一个简单的QT GUI, 可以分配/释放内存, 查看分配内存的情况.



## 分配内存

输入需要内存块的size,根据size计算出最适合的块在空闲链表中的index,进行分配.

如果分配成功,将内存块从空闲链表中删除,返回该内存的首地址;

否则,从更大的空闲块获取一块内存,divide 到本空闲链表中,然后分配.

## 释放内存

输入要释放的内存首地址, 查询空闲块中是否存在可合并的块,如果存在,递归地执行合并操作;否则,添加到对应链表,首地址升序对应的位置.