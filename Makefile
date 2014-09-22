all: matching

matching: windowMatch.cpp
	g++ -g $^ -o $@ -Wall `pkg-config opencv --cflags --libs`

road_seper: road_seper_HSV.cpp
	g++ -g $^ -o $@ -Wall `pkg-config opencv --cflags --libs`
road_isolation:road_isolate.cpp
	g++ -g $^ -o $@ -Wall `pkg-config opencv --cflags --libs`

luminance: luminance_check.cpp
	g++ -g $^ -o $@ -Wall `pkg-config opencv --cflags --libs`	

road: road_identify.cpp
	g++ -g $^ -o $@ -I/home/arnatubai/fuerte_workspace/sandbox/road/include -Wall `pkg-config opencv --cflags --libs`
