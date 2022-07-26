project('boot', 'c')

fs = import('fs')

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

# Boot sources
boot_src = [
    'boot.c'
]
boot_mod = []
boot_inc = [include_directories('.')]
boot_link_arg = []
boot_link_dep = []

# Modules
subdir('rt')
subdir('bios')
subdir('biosdrv')
subdir('bs')
subdir('elf')
subdir('fmt')
subdir('ini')
subdir('io')
subdir('mem')
subdir('str')
subdir('sys')
subdir('text')
subdir('vmap')

# Create the elf
boot = executable('boot', boot_src, link_with: boot_mod, include_directories: boot_inc, link_args: boot_link_arg, link_depends: boot_link_dep)

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