tasm /la  /zi compi.asm
IF ERRORLEVEL 1 GOTO EXIT
tlink /v compi
IF ERRORLEVEL 1 GOTO EXIT
TD -cspecial compi
:EXIT