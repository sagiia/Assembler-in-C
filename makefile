GCC = gcc -Wall -ansi -pedantic
OBJ = error_tool.o file_tool.o first_pass.o label_list.o macro_list.o main.o pre_assembly.o second_pass.o setting.o text_tool.o

my_project: $(OBJ)
	$(GCC) -o my_project $(OBJ)

%.o: %.c
	$(GCC) -c $< -o $@

clean: $(OBJ)
	rm -f $(OBJ)

clang: *.c
	clang --analyze ./*.c && rm -f ./*.plist
