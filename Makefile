#OBJS specifies which files to compile as part of the project
OBJS = main.cpp bit.cpp bios.cpp cpu.cpp flags.cpp interrupt.cpp lcd.cpp log.cpp memory.cpp ops.cpp rom.cpp timer.cpp unitTest.cpp imgui/imgui.cpp imgui/imgui_draw.cpp imgui/imgui_impl_sdl.cpp imgui/imgui_custom_extensions.cpp tinyfiledialogs/tinyfiledialogs.cpp

#CC specifies which compiler we're using
CC = g++ --std=c++11 -funroll-loops -O2 -fsanitize=address -fno-omit-frame-pointer
#COMPILER_FLAGS specifies the additional compilation options we're using
COMPILER_FLAGS = -w

#LINKER_FLAGS specifies the libraries we're linking against
LINKER_FLAGS = -lSDL2 -lGL

# OBJ_NAME specifies the name of our exectuable
OBJ_NAME = cBoy

#This is the target that compiles our executable
all : $(OBJS)
	$(CC) $(OBJS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)
