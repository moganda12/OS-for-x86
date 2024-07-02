export PREFIX="$HOME/OS-for-x86/opt/cross"
export TARGET=i686-elf
export PATH="$PREFIX/bin:$PATH"

$TARGET-as boot.asm -o boot.o

$TARGET-g++ -c kernel.cpp -o kernel.o -ffreestanding -O2 -Wall -Wextra -fno-exceptions -fno-rtti

$TARGET-gcc -T linker.ld -o myos.bin -ffreestanding -O2 -nostdlib boot.o kernel.o
grub-file --is-x86-multiboot myos.bin
echo $?

mkdir -p isodir/boot/grub
cp myos.bin isodir/boot/myos.bin
cp grub.cfg isodir/boot/grub/grub.cfg
grub-mkrescue -o myos.iso isodir
#qemu-system-i386 -cdrom myos.iso
qemu-system-i386 -kernel myos.bin