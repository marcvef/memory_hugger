#include <condition_variable>
#include <iostream>
#include <csignal>

#include <boost/program_options.hpp>

namespace po = boost::program_options;

using namespace std;

static condition_variable shutdown_please;
static mutex mtx;

constexpr size_t one_gb = 1073741824;

void shutdown_handler(int dummy) {
    shutdown_please.notify_all();
}

int main(int argc, const char* argv[]) {

    size_t hug_cnt = 0;
    // Parse input
    po::options_description desc("Allowed options");
    desc.add_options()
            ("help,h", "Help message")
            // TODO smaller sizes
            ("hug,H", po::value<size_t>()->required(), "Amount of memory to hug in total (in GB)");
    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);

    if (vm.count("help")) {
        cout << desc << "\n";
        return 1;
    }
    try {
        po::notify(vm);
    } catch (po::required_option& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }
    if (vm.count("hug")) {
        hug_cnt = vm["hug"].as<size_t>();
    }

    // memory allocated here
    vector<unique_ptr<char[]>> hug_v{};
    for (size_t i = 0; i < hug_cnt; ++i) {
        auto hug_ptr = make_unique<char[]>(one_gb);
        hug_v.emplace_back(move(hug_ptr));
    }
    cout << hug_cnt << "GB of memory is hugged. Waiting for shutdown signal to free resources ..." << endl;
    // wait until shutdown command is sent
    signal(SIGINT, shutdown_handler);
    signal(SIGTERM, shutdown_handler);
    signal(SIGKILL, shutdown_handler);

    unique_lock<mutex> lk(mtx);
    // Wait for shutdown signal to initiate shutdown protocols
    shutdown_please.wait(lk);

    cout << "Signal encountered. Shutting down ..." << endl;

    return 0;
}