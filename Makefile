# 1d53675bf3349c0cc20c48f3f251c235
CFLAGS=-O$(O) -std=c++11
O=2
LFLAGS=
OBJS=objs/main.o


.PHONY: all
all: objs bfc

./bfc: $(OBJS)
	@ echo "    LINK ./bfc"
	@ $(CXX) $(OBJS) -o "./bfc" $(LFLAGS)

objs/main.o: main.cpp
	@ echo "    CXX  ./main.cpp"
	@ $(CXX) $(CFLAGS) -c "./main.cpp" -o $@

objs:
	@ mkdir "objs"
.PHONY: c clean
c: clean
clean:
	@ if [ -d "objs" ]; then rm -r "objs"; fi
	@ rm -f "./bfc"
	@ echo "    CLEAN"
.PHONY: f fresh
f: fresh
fresh: clean
	@ make all --no-print-directory
.PHONY: r run
r: run
run: all
	@ ././bfc

.PHONY: d debug
d: debug
debug: CFLAGS += -DDEBUG -g3 -Wall -Wextra
debug: O=0
debug: all

.PHONY: check-syntax
check-syntax:
	$(CXX) $(CFLAGS) -fsyntax-only -Wall -o /dev/null -S $(CHK_SOURCES)
