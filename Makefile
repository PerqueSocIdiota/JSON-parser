# Call nmake makefile \
!ifndef 0 # \
!include scripts\\nmake.mk # \
!else
# Call make makefile
include scripts/make.mk
# \
!endif

