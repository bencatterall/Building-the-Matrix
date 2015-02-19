CXX = g++
CXXFLAGS = -std=c++11 -Wall --pedantic
DEPS = Address.hpp Socket.hpp GameObject.hpp UpdateManager.hpp Sender.hpp SafeQueue.hpp SafeMap.hpp Update.hpp Controls.hpp Message.hpp CommonMinimal.hpp
OBJ = Address.o Socket.o GameObject.o UpdateManager.o Sender.o SafeQueue.o SafeMap.o Update.o Controls.o Message.o

all: Server

Server: %: %.cpp $(OBJ) 
	$(CXX) -o $@ $^ $(CXXFLAGS)

%.o: %.cpp $(DEPS)
	$(CXX) -c -o $@ $< $(CXXFLAGS)

clean:
	rm *.o ServerMain
