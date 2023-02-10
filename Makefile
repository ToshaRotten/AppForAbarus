setup: listener.o sender.o
	./listener && ./sender

listener.o:
	g++ -o listener/listener listener/listener.cpp
sender.o:
	g++ -o sender/sender sender/sender.cpp

listener: listener.o
	cd listener/ && ./listener
sender: sender.o
	cd sender/ && ./sender
