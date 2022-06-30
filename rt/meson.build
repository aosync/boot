rt_src = [
	'man.c'
]


rt = static_library('rt', rt_src)

# This is done because we want to have those headers directly
# available at the top level
boot_inc += include_directories('.')
boot_mod += rt