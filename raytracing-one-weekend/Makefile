CXX ?= g++

CXXFLAGS += -march=native -O3 -g -fopenmp 

out: src/main.cpp src/PreCompileStd.hpp.gch src/scene_sets.hpp \
 src/camera.hpp src/math_util.hpp src/ray.hpp src/vec3.hpp \
 src/hittables.hpp src/aabb.hpp src/range.hpp src/hittable.hpp \
 src/vec2.hpp src/material.hpp src/texture.hpp src/obj_loader.hpp \
 src/triangle.hpp src/rendrer.hpp src/bvh.hpp src/color.hpp src/timer.hpp \
 src/sphere.hpp src/image/image_file.o
	$(CXX) -include src/PreCompileStd.hpp $(CXXFLAGS) ./src/main.cpp ./src/image/image_file.o -o ./out

src/image/image_file.o: src/image/image_file.cpp src/image/image_file.hpp
	$(CXX) $(CXXFLAGS) -c src/image/image_file.cpp  -o ./src/image/image_file.o

src/PreCompileStd.hpp.gch: src/PreCompileStd.hpp
	$(CXX) -x c++-header $(CXXFLAGS) ./src/PreCompileStd.hpp   
