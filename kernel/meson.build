kernel_src = [
	'kernel.c'
]

klinker_sc = meson.current_source_dir() / 'linker.sc'

kernel_link_arg = [
	'-T' + klinker_sc
]
kernel_link_dep = [ klinker_sc ]

kernel = executable('kernel', kernel_src, link_args: kernel_link_arg, link_depends: kernel_link_dep)