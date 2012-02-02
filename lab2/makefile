$(part): lab2-$(part).c GL_utilities.c
	gcc -Wall -std=c99 -o lab2-$(part).elf -lGL -lglut -DGL_GLEXT_PROTOTYPES lab2-$(part).c GL_utilities.c VectorUtils2.c loadobj.c

clean :
	$(RM) lab2-*.elf
	$(RM) lab2-*.o
