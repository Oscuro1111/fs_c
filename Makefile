VERSION=1.0
LIB_NAME=libfs_c.so
MAJER_VERSION=1

build: 
	@cd objs&&make build
	@./build_dep.sh $(LIB_NAME) $(VERSION) $(MAJER_VERSION)
	@echo "build"
clean:
	rm -f objs/*.o
