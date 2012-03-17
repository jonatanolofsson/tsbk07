CC			:= g++
LDFLAGS		:= -I/usr/include/nvidia-current/ -lGL -lglfw -lGLEW
CFLAGS		:= -O0 -Wall -Wextra -ggdb -std=c++0x

# Manually added subfolders
MODULES		:= geometry .

SRC_DIR		:= $(addprefix src/, $(MODULES))
BIN_DIR		:= ./build
OBJ_DIR		:= ./build

SOURCES		:= $(foreach sdir, $(SRC_DIR), $(wildcard $(sdir)/*.cpp))
OBJECTS		:= $(patsubst src/%.cpp, $(OBJ_DIR)/%.o, $(SOURCES))

OUT			:= grass

all:	$(OUT)

objs:	$(OBJECTS)

clean:
		$(RM) $(OBJECTS)
		$(RM) $(BIN_DIR)/$(OUT)
		
run:	$(OUT)
		$(BIN_DIR)/$(OUT)
		
optirun: $(OUT)
		 optirun $(BIN_DIR)/$(OUT)

$(OUT):	$(OBJECTS)
		@test -d $(BIN_DIR) || mkdir -pm 775 $(BIN_DIR)
		$(CC) $(CFLAGS) $(OBJECTS) -o $(BIN_DIR)/$(OUT) $(LDFLAGS)

.PHONY:	all clean run optirun objs

# Special rule makes sure all object files compile from the correct source file
VPATH = ./src
$(OBJ_DIR)/%.o:	%.cpp
		@test -d $(@D) || mkdir -pm 775 $(@D)
		$(CC) -c $(CFLAGS) $< -o $(OBJ_DIR/)$@ $(LDFLAGS)
