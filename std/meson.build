std_src = [
	'man.c'
]


std = static_library('std', std_src)

# This is done because we want to have those headers directly
# available at the top level
boot_inc += include_directories('.')
boot_mod += std