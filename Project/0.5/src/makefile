.PHONY: clean
all:
	java -jar ../../../Java/Antlr/antlr-3.4-complete.jar ./hl.g && g++ -g -Wall *.cpp *.c ../include/paramater.cc ../../../Java/Antlr/libantlr3c-3.4/*.o 

clean:
	rm -f *.c *.tokens *.h *.out
