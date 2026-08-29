CXX ?= g++

out: src/main.cpp src/PreCompileStd.hpp.gch src/scene_sets.hpp \
 src/camera.hpp src/math_util.hpp src/ray.hpp src/vec3.hpp \
 src/hittables.hpp src/aabb.hpp src/range.hpp src/hittable.hpp \
 src/vec2.hpp src/material.hpp src/texture.hpp src/obj_loader.hpp \
 src/triangle.hpp src/rendrer.hpp src/bvh.hpp src/color.hpp src/timer.hpp \
 src/sphere.hpp
	$(CXX) -include src/PreCompileStd.hpp -march=native -O3 -fopenmp ./src/main.cpp -o ./out

src/PreCompileStd.hpp.gch: src/PreCompileStd.hpp
	$(CXX) -x c++-header -march=native -O3 -fopenmp  ./src/PreCompileStd.hpp   
