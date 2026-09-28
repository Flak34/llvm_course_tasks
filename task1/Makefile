all: app

start.ll: start.c
	clang -emit-llvm -S $^
sim.ll: sim.c
	clang -emit-llvm -S $^
app.ll: app.c
	clang -emit-llvm -S $^
app-opt2.ll: app.c
	clang -emit-llvm -S $^ -O2 -o $@
app-opts.ll: app.c
	clang -emit-llvm -S $^ -Os -o $@
full.bc: start.ll sim.ll app.ll
	llvm-link $^ -o full.bc

app: app.ll start.c sim.c
	clang $^ -lSDL2 -o $@

clean:
	rm app *.ll full.bc

.PHONY: all clean
