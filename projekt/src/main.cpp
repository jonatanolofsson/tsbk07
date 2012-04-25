#include <boost/thread.hpp>
#include "cpgl/cpgl.hpp"
#include "yaml-cpp/yaml.h"
int main(int argc, char* argv[])
{
    YAML::Node config = YAML::LoadFile(argv[1]);
    CPGL::init(argc, argv, config);
    CPGL::wait();
}
