# Project: S-GUI Demo
# Makefile created by Wenliang Guan.

ifneq ($V,1)
Q ?= @
endif

CPP      =	g++
CC       =	gcc
BIN      =	demo
LIBS	 =	../../Codes/sdl/win32/libSDL.dll.a
CFLAGS   =	$(INCS) -Wall -Ofast -g
RM       =	rm -f

APPDIR	 =	../../Codes/App/
CFGDIR	 = 	../../Codes/GUI/Config/
COREDIR	 =	../../Codes/GUI/Core/src/
WDGTDIR	 =	../../Codes/GUI/Widget/
FONTDIR	 =	../../Codes/GUI/Fonts/

INCS     =	-I"../../Codes/App"			\
		-I"../../Codes/GUI/Config"		\
		-I"../../Codes/GUI/Core/inc"		\
		-I"../../Codes/GUI/Fonts"		\
		-I"../../Codes/GUI/Widget"		\
		-I"../../Codes/sdl/include/win32"

OBJ      = 	$(APPDIR)game.o				\
		$(APPDIR)GUI_Test.o			\
		$(APPDIR)main.o				\
		$(APPDIR)pic_rootwin.o			\
		$(APPDIR)sdlscreen.o			\
		$(CFGDIR)GUI_Botton.o			\
		$(COREDIR)GUI_Bitmap.o			\
		$(COREDIR)GUI_ClipArea.o		\
		$(COREDIR)GUI_Color.o			\
		$(COREDIR)GUI_Core.o			\
		$(COREDIR)GUI_Device.o			\
		$(COREDIR)GUI_DispChar.o		\
		$(COREDIR)GUI_GL.o			\
		$(COREDIR)GUI_GL_AA.o			\
		$(COREDIR)GUI_KeyBoard.o		\
		$(COREDIR)GUI_Malloc.o			\
		$(COREDIR)GUI_Math.o			\
		$(COREDIR)GUI_Queue.o			\
		$(COREDIR)GUI_Rect.o			\
		$(COREDIR)GUI_String.o			\
		$(COREDIR)GUI_Surface.o			\
		$(COREDIR)GUI_Timer.o			\
		$(COREDIR)GUI_TouchPad.o		\
		$(COREDIR)GUI_WM.o			\
		$(COREDIR)linked_list.o			\
		$(COREDIR)RootWindow.o			\
		$(WDGTDIR)WIDGET.o			\
		$(WDGTDIR)BUTTON.o			\
		$(WDGTDIR)WINDOW.o			\
		$(FONTDIR)ASCII_8x16.o			\
		$(FONTDIR)UI17_4pp.o

all: $(BIN)

clean:
	$(Q) echo clean
	$(Q) $(RM) $(OBJ)

$(BIN): $(OBJ)
	$(Q) echo [Linking...]
	$(Q) $(CC) $(OBJ) -o $(BIN) $(LIBS)

.c.o:
	$Q echo [Compile] $<
	$Q $(CC) -c $(CFLAGS) $< -o $@
