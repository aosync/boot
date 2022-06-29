# Generate the assembly objects
bios_as = as.process(
    'bootsec.S',
    'stage1.S',
	extra_args : ['-i', meson.current_source_dir()]
)

bios = static_library('bios', bios_as)

boot_mod += bios