{ pkgs }: {
    deps = [
        pkgs.zip
        pkgs.wget
        pkgs.flex
        pkgs.yacc
        pkgs.objconv
        pkgs.nasm
        pkgs.qemu
        pkgs.gmp
        pkgs.mpfr
        pkgs.texinfo
        pkgs.libmpc
        pkgs.ccls
        pkgs.grub2
        pkgs.libisoburn
    ];
}