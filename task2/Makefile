libPass.so: pass.cpp
	clang++ pass.cpp -fPIC -shared -o libPass.so

app.ll: app.c libPass.so
	clang -fpass-plugin=./libPass.so app.c -emit-llvm -S -o app.ll
app-opt2.ll: app.c libPass.so
	clang -fpass-plugin=./libPass.so app.c -emit-llvm -S -O2 -o app-opt2.ll
app-opts.ll: app.c libPass.so
	clang -fpass-plugin=./libPass.so app.c -emit-llvm -S -Os -o app-opts.ll

app: app-opts.ll start.c sim.c log.c
	clang $^ -lSDL2 -o $@

clean:
	rm app *.ll full.bc *.so

.PHONY: all clean
