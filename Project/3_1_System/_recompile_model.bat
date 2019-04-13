@echo OFF
SET MODEL_NAME=BLAZE

cd "C:\Users\nirma\Documents\IE413\Project\3_1_System\"

REM Basic config defines
SET STD_C_INCLUDES=C:\PROGRA~2\BIOPRO~1\Sigma\Libs\
SET STD_C_LIBS=C:\PROGRA~2\BIOPRO~1\Sigma\Libs\std_lib

REM Compile the C code to .obj files
C:\PROGRA~2\BIOPRO~1\Sigma\Libs\wpp386 BLAZE.c -bc -d0 -w3 -xs -ot -bt=nt -3s -mf -q -i=%STD_C_INCLUDES% -i=%STD_C_INCLUDES%std_inc

REM Do the link step
if exist BLAZE.exe del BLAZE.exe
C:\PROGRA~2\BIOPRO~1\Sigma\Libs\wlink @linker_args.lnk

REM CLEANUP TEMP FILE
if exist BLAZE.obj del BLAZE.obj

BLAZE.exe
