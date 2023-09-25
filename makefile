PROJ_NAME=tempcat

SRC_DIR=src
INC_DIR=inc
OBJ_DIR=obj

RELEASE_DIR=release

# get all source files under $SRC_DIR folder, change file extension from c to o
SRCOBJS=$(patsubst %.cpp, %.o, $(wildcard ./$(SRC_DIR)/*.cpp))

# replace folder name from $SRC_DIR to $OBJ_DIR
OBJS=$(subst /$(SRC_DIR)/,/$(OBJ_DIR)/, $(SRCOBJS))

INCLUDES=\
-I. \
-I$(INC_DIR)

LIBS=

all: $(RELEASE_DIR)/$(PROJ_NAME)

$(RELEASE_DIR)/$(PROJ_NAME): $(RELEASE_DIR) $(OBJ_DIR) $(OBJS)
	@echo "link file: $@"
	$(CXX) -o $(RELEASE_DIR)/$(PROJ_NAME) $(OBJS) $(LIBS)
	@echo "success"
	
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@echo "bulid file: $<"
	$(CXX) -std=c++11 -MMD -MF$(@:%.o=%.d) -c $(INCLUDES) $< -o $@
	@echo "success"

$(OBJ_DIR):
	-mkdir -p $(OBJ_DIR)

$(RELEASE_DIR):
	-mkdir -p $(RELEASE_DIR)
	
clean:
	-rm -rf $(OBJ_DIR)/*.d
	-rm -rf $(OBJ_DIR)/*.o
	-rm -rf $(RELEASE_DIR)/$(PROJ_NAME)

.PHONY: all clean
