#include <iostream>
using namespace std;

#include "n_dims/probablity_distibution.h"
#include "n_dims/world.h"
#include "n_dims/robot.h"
#include "n_dims/state.h"
// TIP To <b>Run</b> code, press <shortcut actionId="Run"/> or
// click the <icon src="AllIcons.Actions.Execute"/> icon in the gutter.
int main() {
    using namespace dsj;

    // one dimensional probability distribution 5 states

    probability_distribution<float> init_prob_dist(std::vector<size_t>{5});

    cout << init_prob_dist({0}) << endl;
    cout << init_prob_dist({1}) << endl;
    cout << init_prob_dist({2}) << endl;
    cout << init_prob_dist({3}) << endl;
    cout << init_prob_dist({4}) << endl;

    // one dimensional world five cells
    World<color_measurement> world({5});

    // Use array for indexing instead of vector
    world.set_cell(std::vector<size_t>{0}, color_measurement{ColorStates::GREEN});
    world.set_cell(std::vector<size_t>{1}, color_measurement{ColorStates::RED});
    world.set_cell(std::vector<size_t>{2}, color_measurement{ColorStates::RED});
    world.set_cell(std::vector<size_t>{3}, color_measurement{ColorStates::GREEN});
    world.set_cell(std::vector<size_t>{4}, color_measurement{ColorStates::GREEN});


    cout << world.get_cell({0}) << endl;
    cout << world.get_cell({1}) << endl;
    cout << world.get_cell({2}) << endl;
    cout << world.get_cell({3}) << endl;
    cout << world.get_cell({4}) << endl;

    for (auto [coords,measurement]: world) {
        for (auto c: coords) {
            cout << c << ":";
        }

        cout << measurement << endl;
    }
    // Changed from 5 to 4

    Robot<color_measurement> robot(world);
    //auto posterior = robot.sense(pd, color_measurement{ColorStates::GREEN});

    auto posterior = robot.sense(init_prob_dist, {
                                     color_measurement{ColorStates::RED}, color_measurement{ColorStates::GREEN}
                                 });


    cout << posterior({0}) << endl;
    cout << posterior({1}) << endl;
    cout << posterior({2}) << endl;
    cout << posterior({3}) << endl;
    cout << posterior({4}) << endl;


    std::vector<size_t> moves{1};

    probability_distribution<float> test(std::vector<size_t>{5});


   // posterior = robot.move(moves, prior);

    return 0;
}

