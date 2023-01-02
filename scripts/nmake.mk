SRC_DIR = $(abspath src)
TEST_DIR = $(abspath test)
INCLUDE_DIR = $(abspath include)

BUILD_DIR = $(abspath build)
BIN_DIR = $(abspath bin)
LIB_DIR = $(abspath lib)

SCRIPTS_DIR = $(abspath scripts)

# CPP defined by default as cl
CPPFLAGS = -EHsc -std:c++14 -W4 -WX -c -nologo

!include $(SCRIPTS_DIR)/source.mk

# Source and object files for the library
SRC_FILES = $(patsubst %,$(SRC_DIR)\\%,$(SOURCES))
OBJ_FILES = $(patsubst $(SRC_DIR)\\%.cpp,$(BUILD_DIR)\\%.obj,$(SRC_FILES))

# Source and object files for the tests (not part of the library)
TEST_SRC_FILES = $(patsubst %,$(TEST_DIR)\\%,$(TEST_SOURCES))
TEST_OBJ_FILES = $(patsubst $(TEST_DIR)\\%.cpp,$(BUILD_DIR)\\%.obj,$(TEST_SRC_FILES))

TEST_LINK_FLAGS = /INCREMENTAL

!ifdef DEBUG
# To debug in windows add /Zi to compile with debug information
# then, add /DEBUG to create the pdb file
# Using /Z7, the debug information is embedded within the lib
# TODO: check if this works like this for libraries too
CPPFLAGS = $(CPPFLAGS) -DDEBUG /Z7

# Having separate names (adding 'd' to the name) for debug and relase version of the library
# is it worth it?
JSON_LIB = $(LIB_DIR)/idiota_json_parserd.lib
TEST_LINK_FLAGS = $(TEST_LINK_FLAGS) /DEBUG:FULL /PDB:$(BUILD_DIR)\test_json.pdb
!else
CPPFLAGS = $(CPPFLAGS) -DRELEASE
JSON_LIB = $(LIB_DIR)/idiota_json_parser.lib
!endif

# Libraries used for the tests, in this case only one is used
# but more complex tests could use more libraries
TEST_LIBS = $(JSON_LIB)

!ifdef VERBOSE
CPPFLAGS = $(CPPFLAGS) -DIDIOTA_JSON_VERBOSE
!endif

all: lib test

lib: $(JSON_LIB)

obj: dirs $(OBJ_FILES)

$(JSON_LIB): dirs $(OBJ_FILES)
	lib $(OBJ_FILES) /NOLOGO /VERBOSE /OUT:$@

{$(SRC_DIR)\}.cpp{$(BUILD_DIR)\}.obj:
	$(CPP) $(CPPFLAGS) $< -Fo"$@" -I$(INCLUDE_DIR)

dirs:
	if not exist $(BUILD_DIR) mkdir $(BUILD_DIR)
	if not exist $(BIN_DIR) mkdir $(BIN_DIR)
	if not exist $(LIB_DIR) mkdir $(LIB_DIR)

clean:
	if exist "$(BUILD_DIR)" rmdir /q /s $(BUILD_DIR)
	if exist "$(BIN_DIR)" rmdir /q /s $(BIN_DIR)
	if exist "$(LIB_DIR)" rmdir /q /s $(LIB_DIR)

test: $(BIN_DIR)/json_test.exe

test-obj: dirs $(TEST_OBJ_FILES)

$(BIN_DIR)/json_test.exe: dirs lib $(TEST_OBJ_FILES)
	link $(TEST_OBJ_FILES) $(TEST_LIBS) $(TEST_LINK_FLAGS) /NOLOGO /OUT:$@

{$(TEST_DIR)\}.cpp{$(BUILD_DIR)\}.obj:
	$(CPP) $(CPPFLAGS) $< -Fo"$@" -I$(INCLUDE_DIR)