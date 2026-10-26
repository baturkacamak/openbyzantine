CXX ?= c++
CXXFLAGS ?= -std=c++17 -O2 -Wall -Wextra -Wpedantic
SDL2_CFLAGS := $(shell pkg-config --cflags sdl2)
SDL2_LIBS := $(shell pkg-config --libs sdl2)

all: byzantine-rle byzantine-all byzantine-cc byzantine-play

byzantine-rle: src/main.cpp src/rle.cpp include/byzantine/rle.hpp
	$(CXX) $(CXXFLAGS) -Iinclude src/main.cpp src/rle.cpp -o $@

byzantine-all: src/all_main.cpp src/all.cpp include/byzantine/all.hpp
	$(CXX) $(CXXFLAGS) -Iinclude src/all_main.cpp src/all.cpp -o $@

byzantine-cc: src/cc_main.cpp src/cc.cpp src/cb.cpp src/variables.cpp src/records.cpp src/save.cpp src/anims.cpp src/compose.cpp src/jpeg.cpp src/panorama.cpp src/panorama_view.cpp src/scene_setup.cpp include/byzantine/variables.hpp include/byzantine/cc.hpp include/byzantine/cb.hpp include/byzantine/records.hpp include/byzantine/save.hpp include/byzantine/anims.hpp include/byzantine/compose.hpp include/byzantine/jpeg.hpp include/byzantine/panorama.hpp include/byzantine/scene_setup.hpp
	$(CXX) $(CXXFLAGS) -Iinclude src/cc_main.cpp src/cc.cpp src/cb.cpp src/variables.cpp src/records.cpp src/save.cpp src/anims.cpp src/compose.cpp src/jpeg.cpp src/panorama.cpp src/panorama_view.cpp src/scene_setup.cpp -o $@

byzantine-play: src/play_main.cpp src/cc.cpp src/cb.cpp src/anims.cpp src/compose.cpp src/jpeg.cpp src/panorama.cpp src/panorama_view.cpp src/save.cpp src/records.cpp src/variables.cpp src/scene_setup.cpp src/text.cpp include/byzantine/cc.hpp include/byzantine/cb.hpp include/byzantine/anims.hpp include/byzantine/compose.hpp include/byzantine/jpeg.hpp include/byzantine/panorama.hpp include/byzantine/panorama_view.hpp include/byzantine/save.hpp include/byzantine/scene_setup.hpp include/byzantine/text.hpp
	$(CXX) $(CXXFLAGS) $(SDL2_CFLAGS) -Iinclude src/play_main.cpp src/cc.cpp src/cb.cpp src/anims.cpp src/compose.cpp src/jpeg.cpp src/panorama.cpp src/panorama_view.cpp src/save.cpp src/records.cpp src/variables.cpp src/scene_setup.cpp src/text.cpp $(SDL2_LIBS) -o $@

clean:
	rm -f byzantine-rle byzantine-all byzantine-cc byzantine-play scene-state-test variable-metadata-test panorama-test panorama-view-test records-test save-test anims-test compose-test jpeg-test

scene-state-test: tests/scene_state.cpp src/cc.cpp src/cb.cpp src/variables.cpp src/anims.cpp include/byzantine/cc.hpp include/byzantine/anims.hpp include/byzantine/compose.hpp
	$(CXX) $(CXXFLAGS) -Iinclude tests/scene_state.cpp src/cc.cpp src/cb.cpp src/variables.cpp src/anims.cpp src/compose.cpp -o $@

variable-metadata-test: tests/variable_metadata.cpp src/variables.cpp src/cc.cpp src/cb.cpp src/anims.cpp include/byzantine/variables.hpp include/byzantine/cc.hpp include/byzantine/cb.hpp include/byzantine/anims.hpp include/byzantine/compose.hpp
	$(CXX) $(CXXFLAGS) -Iinclude tests/variable_metadata.cpp src/variables.cpp src/cc.cpp src/cb.cpp src/anims.cpp src/compose.cpp -o $@

panorama-test: tests/panorama.cpp src/panorama.cpp include/byzantine/panorama.hpp
	$(CXX) $(CXXFLAGS) -Iinclude tests/panorama.cpp src/panorama.cpp -o $@

panorama-view-test: tests/panorama_view.cpp src/panorama_view.cpp src/panorama.cpp src/compose.cpp include/byzantine/panorama_view.hpp include/byzantine/panorama.hpp include/byzantine/compose.hpp
	$(CXX) $(CXXFLAGS) -Iinclude tests/panorama_view.cpp src/panorama_view.cpp src/panorama.cpp src/compose.cpp -o $@

records-test: tests/records.cpp src/records.cpp include/byzantine/records.hpp
	$(CXX) $(CXXFLAGS) -Iinclude tests/records.cpp src/records.cpp -o $@

anims-test: tests/anims.cpp src/anims.cpp include/byzantine/anims.hpp
	$(CXX) $(CXXFLAGS) -Iinclude tests/anims.cpp src/anims.cpp -o $@

compose-test: tests/compose.cpp src/compose.cpp src/anims.cpp include/byzantine/compose.hpp include/byzantine/anims.hpp
	$(CXX) $(CXXFLAGS) -Iinclude tests/compose.cpp src/compose.cpp src/anims.cpp -o $@

jpeg-test: tests/jpeg.cpp src/jpeg.cpp include/byzantine/jpeg.hpp
	$(CXX) $(CXXFLAGS) -Iinclude tests/jpeg.cpp src/jpeg.cpp -o $@

text-test: tests/text.cpp src/text.cpp src/compose.cpp include/byzantine/text.hpp include/byzantine/compose.hpp
	$(CXX) $(CXXFLAGS) -Iinclude tests/text.cpp src/text.cpp src/compose.cpp -o $@

save-test: tests/save.cpp src/save.cpp src/cc.cpp src/cb.cpp src/variables.cpp src/anims.cpp include/byzantine/save.hpp include/byzantine/cc.hpp include/byzantine/cb.hpp include/byzantine/anims.hpp include/byzantine/compose.hpp
	$(CXX) $(CXXFLAGS) -Iinclude tests/save.cpp src/save.cpp src/cc.cpp src/cb.cpp src/variables.cpp src/anims.cpp src/compose.cpp -o $@

test: scene-state-test variable-metadata-test panorama-test panorama-view-test records-test save-test anims-test compose-test jpeg-test text-test
	./scene-state-test output/cc_all/EA output/cb_all/EA output/cc_all/STAR output/cb_all/STAR output/co_all/EA
	./variable-metadata-test ../analysis/rea/engine_sample/copy2hd/engine output/cc_all/EA output/cb_all/EA
	./panorama-test
	./panorama-view-test
	./records-test
	./anims-test
	./compose-test
	./jpeg-test ../analysis/rea/jpeg_sample/BCLINTEL.jpg ../analysis/rea/jpeg_sample/vc05.jpg
	./text-test
	./save-test ../analysis/rea/save_sample ../analysis/rea/engine_sample/copy2hd/engine/SHOW.LCC output/cc_all

.PHONY: all clean test
