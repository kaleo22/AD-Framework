g++ -c plugin_maf.cpp
g++ -o plugin_maf.dll plugin_maf.o -shared -Wl,--subsystem,windows
g++ -c plugin_counter.cpp
g++ -o plugin_counter.dll plugin_counter.o -shared -Wl,--subsystem,windows
g++ main.cpp -o main.exe -lboost_filesystem-mt
