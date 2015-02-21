CXX = g++
CXXFLAGS = -std=c++11 -Wall --pedantic -I ../Building-the-Matrix/Dependencies
DEPS = Address.hpp Socket.hpp GameObject.hpp UpdateManager.hpp Sender.hpp SafeQueue.hpp SafeMap.hpp Update.hpp Controls.hpp Message.hpp CommonMinimal.hpp
OBJ = Address.o Socket.o GameObject.o UpdateManager.o Sender.o SafeQueue.o SafeMap.o Update.o Controls.o Message.o

all: Server test

test Server: %: %.cpp $(OBJ) 
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp $(DEPS)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	rm *.o ServerMain
