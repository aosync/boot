project('cross', 'c')

objcopy = find_program('llvm-objcopy')

bootelf_link_deps = meson.project_source_root() / 'linker.ld'

bootelf = executable('bootelf', 'boot.c', link_args : ['-T' + bootelf_link_deps], link_depends : bootelf_link_deps)
bootbin = custom_target('bootbin',
    output : 'bootbin',
    input : bootelf,
    command : [objcopy,
        '-I', 'elf64-x86-64', '-O', 'binary',
        '--binary-architecture=i386:x86-64',
        '@INPUT@', '@OUTPUT@'],
    install : true,
    install_dir : 'subdir')

