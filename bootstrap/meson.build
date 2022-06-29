# Generate the assembly objects
bootstrap_as = as.process(
    'bootsec.S',
    'stage1.S',
	extra_args : ['-i', meson.current_source_dir()]
)

bootstrap = static_library('bootstrap', bootstrap_as)

boot_mod += bootstrap