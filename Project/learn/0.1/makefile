
.PHONY: clean

TARGET='./bin/test'

all: 
	cd ./thirdpart && make
	g++ -g -Wall *.cpp ./thirdpart/*.c ./thirdpart/libs/libantlr3c.a -o $(TARGET) -I. -I ./thirdpart/include -I ./thirdpart/
	$(TARGET) ./input

clean:
	rm -rf $(TARGET)
