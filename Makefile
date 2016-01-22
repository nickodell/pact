ifeq ($(debug),1)
	CFLAGS= -Wall -Werror -std=c99 -DPRINT_DEBUG=1
else
	CFLAGS= -Wall -Werror -std=c99
endif

pact: pact.c
	gcc pact.c ${CFLAGS} -o pact

