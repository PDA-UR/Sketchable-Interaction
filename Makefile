build_all: build_debug build_release

build_debug:
	#xterm -e "(cd ${CURDIR}/src/sirose/msg/ros2_si_interface;. ~/ros2_galactic/install/local_setup.bash; colcon build --packages-select ros2_si_interface)"
	cmake -S .. -B ${CURDIR}/build/Debug -D CMAKE_BUILD_TYPE=Debug
	cmake --build ${CURDIR}/build/Debug --target si_test_application -j 8

debug: build_debug
	xterm -hold -e "(cd ${CURDIR};. ~/ros2_galactic/install/local_setup.bash; . ${CURDIR}/src/sirose/msg/ros2_si_interface/install/local_setup.bash; ./build/Debug/si_test_application)"

build_release:
	cmake -S .. -B ${CURDIR}/build/Release -D CMAKE_BUILD_TYPE=Release
	cmake --build ${CURDIR}/build/Release --target si_test_application -j 8

release: build_release
	xterm -e "(cd ${CURDIR};. ~/ros2_galactic/install/local_setup.bash;./build/Release/si_test_application)"

clean_all: clean_debug clean_release

clean_debug:
	(cd ${CURDIR}/build; rm -rf Debug)

clean_release:
	(cd ${CURDIR}/build; rm -rf Release)
