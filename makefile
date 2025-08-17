CC=gcc -std=c11
HDRDIR=include
OBJDIR=objects
ALLEGROSDK=/home/alisson/Code/Libs/Allegro/sdk
EXECDIR=exec
LIBS=-lm -lallegro -lallegro_image -lallegro_color -lallegro_primitives -lallegro_font

$(EXECDIR)/sr3d.out: $(OBJDIR)/*.o
	$(CC) -o $@ $^ -L"$(ALLEGROSDK)/lib" $(LIBS)

$(OBJDIR)/%.o: %.c $(HDRDIR)/*
	$(CC) -Wall -c -o $@ $< -I"$(ALLEGROSDK)/include/"

.PHONY: clean

clean:
	rm ./*~ $(EXECDIR)/*


# %.o e %.c só funcionam se os arquivos desprovidos dos sufixos tem o mesmo
# nome. Por isso utilizamos "main.c" e "main.o".
