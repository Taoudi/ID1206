cmd_/home/taoudi/Assignments/lkm/skynet/skynet.ko := ld -r -m elf_x86_64 -z max-page-size=0x200000 -T ./scripts/module-common.lds --build-id  -o /home/taoudi/Assignments/lkm/skynet/skynet.ko /home/taoudi/Assignments/lkm/skynet/skynet.o /home/taoudi/Assignments/lkm/skynet/skynet.mod.o ;  true