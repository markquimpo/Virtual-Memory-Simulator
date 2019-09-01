all: vm inputgen

vm: vm.c vm.h replacement.c pagetable.c pagetable.h disk.h disk.c
		gcc vm.c replacement.c pagetable.c disk.c -g -o vm

input_gen: inputgen.c vm.h
		gcc inputgen.c -g -o inputgen

clean:
		rm -f vm inputgen