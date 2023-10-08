ObjDir = obj
SrcDir = src
ExeDir = bin
VendorDir = vendor
ExeName = printf_example.out
TestName = printf_test.out
IncludeDir = include

MKDIR = mkdir
FLAGS=
DECLARES=
INCLUDES=-I $(VendorDir) -I $(IncludeDir)
MUNIT_PATH=../munit
ObjFiles = $(ObjDir)/d2d.o $(ObjDir)/printf.o $(ObjDir)/run.o
CC = clang

build: $(ExeDir)/$(ExeName)

release: clean $(ExeDir)/$(ExeName)

test: DECLARES += -DTEST
test: INCLUDES += -I ../munit
test: clean $(ExeDir)/$(TestName)

$(ObjDir)/d2d.o: $(VendorDir)/ryu/d2d.c $(VendorDir)/ryu/ryu.h $(VendorDir)/ryu/common.h $(VendorDir)/ryu/d2d_intrinsics.h $(VendorDir)/ryu/d2d_full_table.h
	$(MKDIR) -p $(ObjDir)
	$(CC) $(DECLARES) -c $< -o $@

$(ObjDir)/printf.o: $(SrcDir)/printf.c $(IncludeDir)/printf.h $(VendorDir)/ryu/ryu.h
	$(MKDIR) -p $(ObjDir)
	$(CC) $(FLAGS) $(INCLUDES) $(DECLARES) -c $< -o $@

$(ObjDir)/run.o: $(SrcDir)/run.c $(IncludeDir)/printf.h
	$(MKDIR) -p $(ObjDir)
	$(CC) $(FLAGS) $(INCLUDES) $(DECLARES) -c $< -o $@

$(ObjDir)/munit.o: $(MUNIT_PATH)/munit.c
	$(MKDIR) -p $(ObjDir)
	$(CC) $(FLAGS) -c $< -o $@

$(ExeDir)/$(ExeName): $(ObjFiles)
	$(MKDIR) -p $(ExeDir)
	$(CC) -o $@ $^

$(ExeDir)/$(TestName): $(ObjFiles) $(ObjDir)/munit.o
	$(MKDIR) -p $(ExeDir)
	$(CC) -o $@ $^

.PHONY: clean
clean:
	rm -f $(ObjDir)/*.o
	rm -f $(ExeDir)/*
