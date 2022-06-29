project('boot', 'c')

# Program definitions
nasm    = find_program('nasm')
objcopy = find_program('llvm-objcopy')

# Generators definitions
as      = generator(
    nasm,
    output : '@BASENAME@.o',
    arguments : ['-f', 'elf64',
                 '@INPUT@',
                 '-o', '@OUTPUT@',
                 '@EXTRA_ARGS@']
)

# Linker script
linker_sc = meson.project_source_root() / 'linker.ld'

# Boot sources
boot_src = [
    'boot.c',
    'pmem.c'
]
boot_mod = []
boot_inc = [include_directories('.')]
boot_link_dep = [linker_sc]

# Modules
subdir('bootstrap')
subdir('stddef')
subdir('mem')

# Create the elf
boot = executable('boot', boot_src, link_with: boot_mod, include_directories: boot_inc, link_args: ['-T' + linker_sc], link_depends: boot_link_dep)

# Create the flat binary
boot_bin = custom_target('boot.bin',
    output : 'boot.bin',
    input : boot,
    command : [objcopy,
        '-I', 'elf64-x86-64', '-O', 'binary',
        '--binary-architecture=i386:x86-64',
        '@INPUT@', '@OUTPUT@'],
    install : true,
    install_dir : 'subdir')
