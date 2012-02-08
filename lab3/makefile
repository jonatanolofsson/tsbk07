$(part): lab3-$(part).c GL_utilities.c
	gcc -Wall -std=c99 -ggdb -o lab3-$(part).elf -lGL -lglut -DGL_GLEXT_PROTOTYPES lab3-$(part).c GL_utilities.c VectorUtils2.c loadobj.c

clean :
	$(RM) lab3-*.elf
	$(RM) lab3-*.o
