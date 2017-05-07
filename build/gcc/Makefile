# Project: S-GUI Demo
# Makefile created by Wenliang Guan.

ifneq ($V,1)
Q ?= @
endif

CPP      =	g++
CC       =	gcc
UNAME	 = 	$($Q shell uname -s)
RM       =	rm -f

BIN      =	demo
CFLAGS   =	$(INCS) -Wall -Ofast

ifeq ($(UNAME), Linux)
LIBS	 =	../../Codes/sdl/linux/libSDL.a -lpthread -ldl -lm
SDLINC	 =	-I"../../Codes/sdl/include/linux"
else
LIBS	 =	../../Codes/sdl/win32/libSDL.dll.a
SDLINC	 =	-I"../../Codes/sdl/include/win32"
endif

APPDIR	 =	../../Codes/App/
CFGDIR	 = 	../../Codes/GUI/Config/
AADIR	 =	../../Codes/GUI/AntiAliasing/src/
COREDIR	 =	../../Codes/GUI/Core/src/
WDGTDIR	 =	../../Codes/GUI/Widget/
FONTDIR	 =	../../Codes/GUI/Fonts/

INCS	 =	-I"../../Codes/App"			\
		-I"../../Codes/GUI/AntiAliasing/inc"	\
		-I"../../Codes/GUI/Config"		\
		-I"../../Codes/GUI/Core/inc"		\
		-I"../../Codes/GUI/Fonts"		\
		-I"../../Codes/GUI/Widget"		\
		$(SDLINC)

OBJ      = 	$(APPDIR)game.o				\
		$(APPDIR)GUI_Test.o			\
		$(APPDIR)main.o				\
		$(APPDIR)pic_rootwin.o			\
		$(APPDIR)sdlscreen.o			\
		$(CFGDIR)GUI_Bottom.o			\
		$(COREDIR)GUI_Bitmap.o			\
		$(COREDIR)GUI_ClipArea.o		\
		$(COREDIR)GUI_ClipAreaHeap.o		\
		$(COREDIR)GUI_Color.o			\
		$(COREDIR)GUI_Context.o			\
		$(COREDIR)GUI_Core.o			\
		$(COREDIR)GUI_Device.o			\
		$(COREDIR)GUI_DispChar.o		\
		$(COREDIR)GUI_DrawBase.o		\
		$(COREDIR)GUI_GL.o			\
		$(COREDIR)GUI_KeyBoard.o		\
		$(COREDIR)GUI_Malloc.o			\
		$(COREDIR)GUI_Math.o			\
		$(COREDIR)GUI_MemDev.o			\
		$(COREDIR)GUI_PaintArea.o		\
		$(COREDIR)GUI_Queue.o			\
		$(COREDIR)GUI_Rect.o			\
		$(COREDIR)GUI_RootWin.o			\
		$(COREDIR)GUI_String.o			\
		$(COREDIR)GUI_Timer.o			\
		$(COREDIR)GUI_TouchPad.o		\
		$(COREDIR)GUI_WM.o			\
		$(COREDIR)linked_list.o			\
		$(WDGTDIR)WIDGET.o			\
		$(WDGTDIR)BUTTON.o			\
		$(WDGTDIR)WINDOW.o			\
		$(FONTDIR)ASCII_8x16.o			\
		$(FONTDIR)UI17_4pp.o			\
		$(AADIR)GUI_Rasterizer_AA.o

$(BIN): $(OBJ)
	$(Q) echo [Linking...]
	$(Q) $(CC) $(OBJ) -o $(BIN) $(LIBS)
ifneq ($(UNAME), Linux)
	$Q echo [Copy files...]
	$Q copy "..\..\Codes\sdl\win32\SDL.dll" ".\"
endif

.c.o:
	$Q echo [Compile] $<
	$Q $(CC) -c $(CFLAGS) $< -o $@

clean:
	$(Q) echo [Clean...]
	$(Q) $(RM) $(OBJ)
