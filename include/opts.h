#ifndef OPTS_H
#define OPTS_H

#include <boost/program_options.hpp>

#define DEFAULT_GRAPH "input/worlds/grid100"
#define DEFAULT_INIT "input/config/init0"
#define DEFAULT_OUT "output/hist0"

struct my_opt{
    std::string graph;
    std::string init_config;
    std::string out;
};

namespace po = boost::program_options;

struct my_opt get_options(int argc, char ** argv)
{
    
    struct my_opt opts;

    //Define options
    po::options_description desc("Allowed options");
    desc.add_options()
        ("help", "produce help message")
        ("graph", po::value<std::string>()->default_value(DEFAULT_GRAPH), "Set path to edge-list input graph file with world topology")
        ("init-config", po::value<std::string>()->default_value(DEFAULT_INIT), "Set path to input file with initial world configuration")
        ("out", po::value<std::string>()->default_value(DEFAULT_OUT), "Set path to history output")
    ;

    // Parse command line
    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    // Convert to my options
    opts.graph = vm["graph"].as<std::string>();
    opts.init_config = vm["init-config"].as<std::string>();
    opts.out = vm["out"].as<std::string>();

    return opts;
}

#endif