all: sourceline tracemap
sourceline: pass_source_line.c
	gcc pass_source_line.c -o $@

tracemap: parser.cpp readOutput.cpp
	g++ parser.cpp readOutput.cpp -o $@

.PHONY: clean
clean:
	rm sourceline tracemap
