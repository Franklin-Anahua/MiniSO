cc = gcc
cflags = -Wall -Wextra -O2 -pthread
include = -Iinclude

src = main.c src/procesos.c src/hilos.c src/memoria/particionamiento.c src/memoria/paginacion.c
obj = $(src:.c=.o)
bin = miniso

all: $(bin)

$(bin): $(obj)
	$(cc) $(cflags) -o $@ $(obj)

%.o: %.c
	$(cc) $(cflags) $(include) -c $< -o $@

clean:
	rm -f $(obj) $(bin)

.phony: all clean

