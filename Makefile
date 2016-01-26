CFLAGS = -O2 -Wall -Werror -std=c99
ifeq ($(debug),1)
	CFLAGS += -DPRINT_DEBUG=1
endif

pact: pact.c
	gcc pact.c ${CFLAGS} -o pact

clean:
	rm -f pact
