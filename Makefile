#OBJS specifies which files to compile as part of the project
OBJS = main.cpp bit.cpp bios.cpp cpu.cpp interrupt.cpp lcd.cpp log.cpp memory.cpp rom.cpp timer.cpp imgui/imgui.cpp imgui/imgui_draw.cpp imgui/imgui_impl_sdl.cpp imgui/imgui_custom_extensions.cpp tinyfiledialogs/tinyfiledialogs.cpp

#CC specifies which compiler we're using
CC = g++ --std=c++11 -funroll-loops -O2 
#-s -DNDEBUG
#-Wall -Wextra
#COMPILER_FLAGS specifies the additional compilation options we're using
# -w suppresses all warnings
#COMPILER_FLAGS = 

#LINKER_FLAGS specifies the libraries we're linking against
LINKER_FLAGS = -lSDL2 -lGL

#SDL_CFLAGS := $(shell sdl-config --cflags)
#SDL_LDFLAGS := $(shell sdl-config --libs)

#OBJ_NAME specifies the name of our exectuable
OBJ_NAME = cBoy

#This is the target that compiles our executable
all : $(OBJS)
	$(CC) $(OBJS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)

clean : rm -f *.o rm $(BIN)