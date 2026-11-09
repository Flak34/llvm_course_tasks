app-opts.ll: app.c
	clang -emit-llvm -S $^ -Os -o $@

LLVM_FLAGS = $(shell llvm-config --cppflags --ldflags --libs)

asm_2_IRgen_1: sim.c asm_2_IRgen_1.cpp
	clang++ $(LLVM_FLAGS) $^ -lSDL2 -o $@

asm_2_IRgen_2: sim.c asm_2_IRgen_2.cpp
	clang++ $(LLVM_FLAGS) $^ -lSDL2 -o $@

all: asm_2_IRgen_1 asm_2_IRgen_2

clean:
	rm -f app-opts.ll asm_2_IRgen_1 asm_2_IRgen_2
