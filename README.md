Mirrored buffer is a block of physical memory mapped to two consecutive blocks of virtual address spaces.

Use `cat /proc/<pid>/maps` to investigate how `mmap()` works. 

```none
lw@DESKTOP-N9CIN42:~/projects/tests-mmap$ ./build/tests_mmap
whole_buf addr: 0x7fd7328ad000
first_half_buf addr: 0x7fd7328ad000
second_half_buf addr: 0x7fd7328ae00

lw@DESKTOP-N9CIN42:~/projects/tests-mmap$ sudo cat /proc/11673/maps
55d11c62e000-55d11c62f000 r--p 00000000 08:20 146840       /home/lw/projects/tests-mmap/build/tests_mmap
55d11c62f000-55d11c630000 r-xp 00001000 08:20 146840       /home/lw/projects/tests-mmap/build/tests_mmap
55d11c630000-55d11c631000 r--p 00002000 08:20 146840       /home/lw/projects/tests-mmap/build/tests_mmap
55d11c631000-55d11c632000 r--p 00002000 08:20 146840       /home/lw/projects/tests-mmap/build/tests_mmap
55d11c632000-55d11c633000 rw-p 00003000 08:20 146840       /home/lw/projects/tests-mmap/build/tests_mmap
55d11d1bd000-55d11d1de000 rw-p 00000000 00:00 0            [heap]
7fd73267b000-7fd73267e000 rw-p 00000000 00:00 0
7fd73267e000-7fd7326a6000 r--p 00000000 08:20 28625        /usr/lib/x86_64-linux-gnu/libc.so.6
7fd7326a6000-7fd73283b000 r-xp 00028000 08:20 28625        /usr/lib/x86_64-linux-gnu/libc.so.6
7fd73283b000-7fd732893000 r--p 001bd000 08:20 28625        /usr/lib/x86_64-linux-gnu/libc.so.6
7fd732893000-7fd732894000 ---p 00215000 08:20 28625        /usr/lib/x86_64-linux-gnu/libc.so.6
7fd732894000-7fd732898000 r--p 00215000 08:20 28625        /usr/lib/x86_64-linux-gnu/libc.so.6
7fd732898000-7fd73289a000 rw-p 00219000 08:20 28625        /usr/lib/x86_64-linux-gnu/libc.so.6
7fd73289a000-7fd7328a7000 rw-p 00000000 00:00 0
7fd7328ad000-7fd7328ae000 rw-s 00000000 00:01 11264        /memfd:test_buffer (deleted) <----- first buffer
7fd7328ae000-7fd7328af000 rw-s 00000000 00:01 11264        /memfd:test_buffer (deleted) <----- second buffer
7fd7328af000-7fd7328b1000 rw-p 00000000 00:00 0
7fd7328b1000-7fd7328b3000 r--p 00000000 08:20 28588        /usr/lib/x86_64-linux-gnu/ld-linux-x86-64.so.2
7fd7328b3000-7fd7328dd000 r-xp 00002000 08:20 28588        /usr/lib/x86_64-linux-gnu/ld-linux-x86-64.so.2
7fd7328dd000-7fd7328e8000 r--p 0002c000 08:20 28588        /usr/lib/x86_64-linux-gnu/ld-linux-x86-64.so.2
7fd7328e9000-7fd7328eb000 r--p 00037000 08:20 28588        /usr/lib/x86_64-linux-gnu/ld-linux-x86-64.so.2
7fd7328eb000-7fd7328ed000 rw-p 00039000 08:20 28588        /usr/lib/x86_64-linux-gnu/ld-linux-x86-64.so.2
7ffc0f2ad000-7ffc0f2cf000 rw-p 00000000 00:00 0            [stack]
7ffc0f368000-7ffc0f36c000 r--p 00000000 00:00 0            [vvar]
7ffc0f36c000-7ffc0f36e000 r-xp 00000000 00:00 0            [vdso]
```

The `whole_buf` is split into two halves. There is no virtual memory that ranges 0x7fd7328ad000 - 0x7fd7328af000.
This suggests we don't need to `munmap()` the original `whole_buf`.

If we comment out `mmap()`ing of the second half of `whole_buf`.

```none
lw@DESKTOP-N9CIN42:~/projects/tests-mmap$ ./build/tests_mmap
whole_buf addr: 0x7fe082304000
first_half_buf addr: 0x7fe082304000

lw@DESKTOP-N9CIN42:~/projects/tests-mmap$ sudo cat /proc/11797/maps
557813ab4000-557813ab5000 r--p 00000000 08:20 146840       /home/lw/projects/tests-mmap/build/tests_mmap
557813ab5000-557813ab6000 r-xp 00001000 08:20 146840       /home/lw/projects/tests-mmap/build/tests_mmap
557813ab6000-557813ab7000 r--p 00002000 08:20 146840       /home/lw/projects/tests-mmap/build/tests_mmap
557813ab7000-557813ab8000 r--p 00002000 08:20 146840       /home/lw/projects/tests-mmap/build/tests_mmap
557813ab8000-557813ab9000 rw-p 00003000 08:20 146840       /home/lw/projects/tests-mmap/build/tests_mmap
5578159ed000-557815a0e000 rw-p 00000000 00:00 0            [heap]
7fe0820d2000-7fe0820d5000 rw-p 00000000 00:00 0
7fe0820d5000-7fe0820fd000 r--p 00000000 08:20 28625        /usr/lib/x86_64-linux-gnu/libc.so.6
7fe0820fd000-7fe082292000 r-xp 00028000 08:20 28625        /usr/lib/x86_64-linux-gnu/libc.so.6
7fe082292000-7fe0822ea000 r--p 001bd000 08:20 28625        /usr/lib/x86_64-linux-gnu/libc.so.6
7fe0822ea000-7fe0822eb000 ---p 00215000 08:20 28625        /usr/lib/x86_64-linux-gnu/libc.so.6
7fe0822eb000-7fe0822ef000 r--p 00215000 08:20 28625        /usr/lib/x86_64-linux-gnu/libc.so.6
7fe0822ef000-7fe0822f1000 rw-p 00219000 08:20 28625        /usr/lib/x86_64-linux-gnu/libc.so.6
7fe0822f1000-7fe0822fe000 rw-p 00000000 00:00 0
7fe082304000-7fe082305000 rw-s 00000000 00:01 2050         /memfd:test_buffer (deleted) <---- first half mapped
7fe082305000-7fe082306000 ---p 00000000 00:00 0                                         <---- second half remains
7fe082306000-7fe082308000 rw-p 00000000 00:00 0
7fe082308000-7fe08230a000 r--p 00000000 08:20 28588        /usr/lib/x86_64-linux-gnu/ld-linux-x86-64.so.2
7fe08230a000-7fe082334000 r-xp 00002000 08:20 28588        /usr/lib/x86_64-linux-gnu/ld-linux-x86-64.so.2
7fe082334000-7fe08233f000 r--p 0002c000 08:20 28588        /usr/lib/x86_64-linux-gnu/ld-linux-x86-64.so.2
7fe082340000-7fe082342000 r--p 00037000 08:20 28588        /usr/lib/x86_64-linux-gnu/ld-linux-x86-64.so.2
7fe082342000-7fe082344000 rw-p 00039000 08:20 28588        /usr/lib/x86_64-linux-gnu/ld-linux-x86-64.so.2
7fff312cb000-7fff312ed000 rw-p 00000000 00:00 0            [stack]
7fff31364000-7fff31368000 r--p 00000000 00:00 0            [vvar]
7fff31368000-7fff3136a000 r-xp 00000000 00:00 0            [vdso]
```

The second half is still there, with permission `p` (private). This suggests that we don't need to worry about
the race condition where another thread `mmap()`s the second half of `whole_buf` from underneath us.
