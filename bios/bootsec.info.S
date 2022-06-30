; Definition of the bootsector info struct

global bios_info
bios_info:
bios_info.bootdrive.no:
db 0
bios_info.bootdrive.spt:
db 0
bios_info.bootdrive.heads:
db 0
bios_info.what:
dw 0
global bios_info.bioslidtr
bios_info.bioslidtr:
dq 0
dw 0