CXXFLAGS = -Wall --std=c++17 -O3
CXX = g++

tokenizer.o: lexer/tokenizer.cpp lexer/tokenizer.h
	$(CXX) $(CXXFLAGS) -c $^

lexer.o: lexer/lexer.cpp lexer/lexer.h
	$(CXX) $(CXXFLAGS) -c $^

lexer_test.o: lexer/lexer_test.cpp
	$(CXX) $(CXXFLAGS) -c $^

lexer: tokenizer.o lexer.o lexer_test.o
	$(CXX) -o lexer.exe $^
	.\lexer

clean:
	rm -f *.o *.exe *.gch