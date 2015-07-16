CXX = clang++
CXXFLAGS = -Iinclude -std=c++14 -stdlib=libc++ -c -g -O0 -Wall -Wextra
LDFLAGS = -std=c++14 -stdlib=libc++ -lc++abi

ifdef SANITIZE
CXXFLAGS += -fsanitize=$(SANITIZE)
LDFLAGS += -fsanitize=$(SANITIZE)
endif

DECODER = decoder
ENCODER = encoder
PRINTER = print_as_ascii

EXES = $(DECODER) $(ENCODER) $(PRINTER)

ENC_OBJS = huffman_tree.o frequency.o encoder.o encoder_prog.o binary_file_writer.o binary_file_reader.o
DEC_OBJS = huffman_tree.o frequency.o decoder.o decoder_prog.o binary_file_writer.o binary_file_reader.o
PRINT_OBJS = binary_file_reader.o print_as_ascii.o

.PHONY: all clean tidy

all: $(EXES)

$(ENCODER): $(ENC_OBJS)
	$(CXX) $(ENC_OBJS) $(LDFLAGS) -o $(ENCODER)

$(DECODER): $(DEC_OBJS)
	$(CXX) $(DEC_OBJS) $(LDFLAGS) -o $(DECODER)

$(PRINTER): $(PRINT_OBJS)
	$(CXX) $(PRINT_OBJS) $(LDFLAGS) -o $(PRINTER)

frequency.o: src/frequency.cpp include/frequency.h
	$(CXX) $(CXXFLAGS) $<

huffman_tree.o: src/huffman_tree.cpp include/binary_file_reader.h \
	include/binary_file_writer.h include/frequency.h include/huffman_tree.h
	$(CXX) $(CXXFLAGS) $<

binary_file_reader.o: src/binary_file_reader.cpp include/binary_file_reader.h
	$(CXX) $(CXXFLAGS) $<

binary_file_writer.o: src/binary_file_writer.cpp include/binary_file_writer.h
	$(CXX) $(CXXFLAGS) $<

encoder.o: src/encoder.cpp include/encoder.h include/frequency.h \
	include/binary_file_writer.h
	$(CXX) $(CXXFLAGS) $<

encoder_prog.o: src/encoder_prog.cpp include/encoder.h
	$(CXX) $(CXXFLAGS) $<

decoder.o: src/decoder.cpp include/decoder.h include/frequency.h \
	include/binary_file_reader.h
	$(CXX) $(CXXFLAGS) $<

decoder_prog.o: src/decoder_prog.cpp include/decoder.h
	$(CXX) $(CXXFLAGS) $<

print_as_ascii.o: src/print_as_ascii.cpp include/binary_file_reader.h
	$(CXX) $(CXXFLAGS) $<

doc: lab_huffman.doxygen $(wildcard include/*) $(wildcard src/*)
	doxygen lab_huffman.doxygen

tidy:
	-rm -rf *.bin *.txt *.huff

clean:
	-rm -rf *.o $(EXES) doc
