CC = g++
CFLAGS = -g -Wall

BIN = ./bin
SRC = ./src

TARGET = $(BIN)/time
SRCS = $(SRC)/main.cpp \
	$(SRC)/get.cpp \
	$(SRC)/gengraph.cpp \
	$(SRC)/first.cpp \
	$(SRC)/second.cpp \
	$(SRC)/third.cpp \
	$(SRC)/fourth.cpp \
	$(SRC)/batch.cpp \
	$(SRC)/unwind.cpp \
	$(SRC)/loadcsv.cpp

LFLAGS = -L../lib -L/usr/local/lib -L/usr/lib
LIBS = -lneo4j-client

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -O2 -o $(TARGET) $(SRCS) $(LFLAGS) $(LIBS)

clean:
	rm $(TARGET)
