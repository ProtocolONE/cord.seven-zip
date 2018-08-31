C_OBJS = $(C_OBJS) \
  $O\7zCrc.obj

!IF "$(CPU)" == "IA64" || "$(CPU)" == "MIPS" || "$(CPU)" == "ARM" || "$(CPU)" == "ARM64"
  C_OBJS = $(C_OBJS) \
  $O\7zCrcOpt.obj
!ELSE
ASM_OBJS = $(ASM_OBJS) \
   $O\7zCrcOpt.obj
!ENDIF
