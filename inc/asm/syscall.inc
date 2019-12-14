%ifndef _SYSCALL_INC_
%define _SYSCALL_INC_

; SYSCALL HELPER
%define SYS_READ  0x0

%define SYS_WRITE 0x1

%define SYS_OPEN  0x2
    %define O_RDONLY 0x000
    %define O_WRONLY 0x001
    %define O_RDWR   0x004
    %define O_CREAT  0x040
    %define S_IRWXU  00700
    %define S_IRUSR  00400
    %define S_IWUSR  00200
    %define S_IXUSR  00100
    %define S_IRWXG  00070
    %define S_IRGRP  00040
    %define S_IWGRP  00020
    %define S_IXGRP  00010
    %define S_IRWXO  00007
    %define S_IROTH  00004
    %define S_IWOTH  00002

%define SYS_CLOSE 0x3


%endif
