ObjDir = obj
SrcDir = src
ExeDir = bin
VendorDir = vendor
ExeName = test.out
IncludeDir = include

MKDIR = mkdir

build: $(ExeDir)/$(ExeName)


$(ObjDir)/d2d.o: $(VendorDir)/ryu/d2d.c $(VendorDir)/ryu/ryu.h $(VendorDir)/ryu/common.h $(VendorDir)/ryu/d2d_intrinsics.h $(VendorDir)/ryu/d2d_full_table.h
	$(MKDIR) -p $(ObjDir)
	gcc -c $< -o $@

$(ObjDir)/printf.o: $(SrcDir)/printf.c $(IncludeDir)/printf.h $(VendorDir)/ryu/ryu.h
	$(MKDIR) -p $(ObjDir)
	gcc -I $(VendorDir) -I $(IncludeDir) -c $< -o $@

$(ObjDir)/test.o: $(SrcDir)/test.c $(IncludeDir)/printf.h
	$(MKDIR) -p $(ObjDir)
	gcc -I $(VendorDir) -I $(IncludeDir) -c $< -o $@

$(ExeDir)/$(ExeName): $(ObjDir)/test.o $(ObjDir)/printf.o $(ObjDir)/d2d.o
	$(MKDIR) -p $(ExeDir)
	gcc -o $@ $^

.PHONY: clean
clean:
	rm -f $(ObjDir)/*.o
	rm -f $(ExeDir)/*
