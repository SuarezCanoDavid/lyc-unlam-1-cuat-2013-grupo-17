tasm /la  /zi compi.asm
IF ERRORLEVEL 1 GOTO EXIT
tlink /3 /v compi
IF ERRORLEVEL 1 GOTO EXIT
TD -cspecial compi
:EXIT