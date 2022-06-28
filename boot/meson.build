bootas = as.process(
    'bootsec.S',
    'stage1.S',
	extra_args : ['-i', meson.current_source_dir()]
)

bootelfsrc = [
    bootas
]

bootelfdeps = meson.project_source_root() / 'linker.ld'
bootelf = executable('bootelf', bootelfsrc, link_args : ['-T' + bootelfdeps], link_depends : bootelfdeps)
bootbin = custom_target('bootbin',
    output : 'bootbin',
    input : bootelf,
    command : [objcopy,
        '-I', 'elf64-x86-64', '-O', 'binary',
        '--binary-architecture=i386:x86-64',
        '@INPUT@', '@OUTPUT@'],
    install : true,
    install_dir : 'subdir')
