# Sketchable-Interaction

**Reference implementation of a novel and generic computational environment for defining user interfaces and workflows**

##  Plugins
[Plugin Repository](https://github.com/PDA-UR/Sketchable-Interaction-Plugins)

## Documentation:
* [SIGRun](https://pda-ur.github.io/Sketchable-Interaction/SI-Doc/si/sigrun/html/docs/index.html)
* [PySI](https://pda-ur.github.io/Sketchable-Interaction/SI-Doc/si/pysi/html/docs/index.html)
* [API Examples](https://pda-ur.github.io/Sketchable-Interaction/SI-Doc/si/api_examples/api_examples.html)

## Building SI yourself (currently only available on Linux (Ubuntu/Debian))
In this section, the build process is described which is used to create the current Release of SI.
### Downloading and Building the Dependencies
#### Misc. Dependencies
The dependencies for all components of SI (SIGRun, SIREn, PySI, SIROSe, SIEGen, SITools, SIPlugGen) are:
  * Qt5
  * QML
  * ROS2
  * libglm
  * uuid
  * python3.x-dev (replace x with your desired version)
  * libtbb-dev
  
The recommended way of installing the dependencies except for Boost is:
  * Open your terminal
  * Update your sources.list for installing libtbb with e.g.:
    * echo "deb http://cz.archive.ubuntu.com/ubuntu eoan main universe" | sudo tee -a  /etc/apt/sources.list
  * Update packages list
    * sudo apt update
  * Install Qt5 and QML
    * sudo apt install qtbase5-dev
    * sudo apt install qtdeclarative5-dev
  * Install libglm
    * sudo apt install libglm-dev
  * Install libuuid
    * sudo apt install uuid-dev
  * Install your desired python3 version's dev library (e.g. 3.9)
    * sudo apt install python3.9-dev
  * Install libtbb-dev
    * sudo apt install libtbb-dev
    
#### Boost with components Python, System, Filesystem
These dependencies are unfortunately a little more complex to install than simple **apt** calls.
The following example demonstrates the installation with Python3.7 and Boostv1.69.0.
Using other versions should work, therefore the commands then need adjustments according to that versions:
* Download and extract Boost:
  * wget https://sourceforge.net/projects/boost/files/boost/1.69.0/boost_1_69_0.tar.gz/download
  * tar xzf download
* Configure Boost for use with Python3.9
  * Open your terminal
  * Navigate to your extracted Boost folder
  * Run the command: sudo ./bootstrap.sh --with-python=$(which python3)
* Install Boost with components Python, System, Filesystem
  * Run the command: sudo ./b2 --with-system --with-python --with-filesystem -j 8 -d0 install
  
### Compiling SI
* Download SI and extract the zip file
* Open your terminal and navigate to the Sketchable-Interaction folder
* Source your ROS2 installation
* Source the SIROSe extension: source SI/src/sirose/msg/ros2_si_interface/install/setup.bash
* Run the commands:
  * cmake .
  * cmake --build . --target all -j 8

## Running SI
* Open your Terminal and navigate to the Sketchable-Interaction Folder
* Run the command si.sh or run the command si_test_application
