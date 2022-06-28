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

# Subdirectories
subdir('bootstrap')


# Boot sources
boot_src = [
    'boot.c',
    bootstrap_as
]

# Create the elf
boot_elf_deps = linker_sc
boot_elf = executable('boot', boot_src, link_args: ['-T' + linker_sc], link_depends: boot_elf_deps)

# Create the flat binary
boot_bin = custom_target('boot.bin',
    output : 'boot.bin',
    input : boot_elf,
    command : [objcopy,
        '-I', 'elf64-x86-64', '-O', 'binary',
        '--binary-architecture=i386:x86-64',
        '@INPUT@', '@OUTPUT@'],
    install : true,
    install_dir : 'subdir')
