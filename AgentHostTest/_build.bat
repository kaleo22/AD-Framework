g++ -c simple_agent.cpp
g++ -o simple_agent.dll simple_agent.o -shared -Wl,--subsystem,windows
g++ -c agent_loader.cpp
g++ -c simple_host.cpp
g++ simple_host.o agent_loader.o -o simple_host.exe