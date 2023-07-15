{ pkgs }: {
	deps = [
        pkgs.cpplint
        pkgs.clang-tools
        pkgs.gcc
		pkgs.ccls
		pkgs.gdb
		pkgs.gnumake
        pkgs.premake5
        pkgs.neovim
        pkgs.python310
        pkgs.tokei
        pkgs.pstree
	];
}