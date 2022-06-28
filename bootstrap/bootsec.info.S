; Definition of the bootsector info struct

global info
info:
info.bootdrive.no:
db 0
info.bootdrive.spt:
db 0
info.bootdrive.heads:
db 0
global info.bioslidtr
info.bioslidtr:
dq 0
dw 0