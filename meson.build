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

# 
subdir('boot')