SOURCES_CPP := UniversityCollider_2_0.cpp FeedWriter.cpp FeedReader.cpp MultiMedidor.cpp Medidor.cpp
SOURCES_C := $(shell find libmodbus/ -name '*.c')
OBJECTS := $(SOURCES_CPP:.cpp=.o)
OBJECTS += $(SOURCES_C:.c=.o)
CPPFLAGS := -g -Wall -Wextra -g -I pwd/UniversityCollider_2_0.h -I /usr/include/cppconn
LDFLAGS = -L/usr/lib/x86_64-linux-gnu -lmysqlcppconn

universitycollider: $(OBJECTS)
        g++ $(OBJECTS) -o $@ $(LDFLAGS)

%.o: %.cpp
        g++ $(CPPFLAGS) -c $< -o $@

%.o: %.c
        gcc $(CPPFLAGS) -c $< -o $@

.PHONY: clean
clean:
        rm -r *.o
