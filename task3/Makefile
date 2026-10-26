app-opts.ll: app.c
	clang -emit-llvm -S $^ -Os -o $@

LLVM_FLAGS = $(shell llvm-config --cppflags --ldflags --libs)

ir_gen: sim.c app_ir_gen.cpp
	clang++ $(LLVM_FLAGS) $^ -lSDL2 -o $@

clean:
	rm *.ll ir_gen