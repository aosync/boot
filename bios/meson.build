# Generate the assembly objects
bios_as = as.process(
    'bootsec.S',
    'stage1.S',
	extra_args : ['-i', meson.current_source_dir()]
)

bios_src = [
    'quirks.c',
    bios_as
]

bios = static_library('bios', bios_src, include_directories: boot_inc)

boot_mod += bios

# Linker script:
# since [bios] defines how to boot when used,
# we add these to the parent [boot] program
linker_sc = meson.current_source_dir() / 'linker.sc'

boot_link_arg += '-T' + linker_sc
boot_link_dep += linker_sc