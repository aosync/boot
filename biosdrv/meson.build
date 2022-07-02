biosdrv_src = [
	'disk.c',
	'mmap.c'
]

biosdrv = static_library('biosdrv', biosdrv_src, include_directories: boot_inc)

boot_mod += biosdrv