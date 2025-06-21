#include <iostream>
using namespace std;

#include "../../header/probability_distribution.h"
#include "../../header/world_tensor.h"
#include "../n_dims/robot.h"
#include "../../header/state.h"
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

    probability_distribution<float> dirac(std::vector<size_t>{5});
    dirac(0) = 1;
    dirac(1) = 0;
    dirac(2) = 0;
    dirac(3) = 0;
    dirac(4) = 0;
    cout << dirac({0}) << endl;
    cout << dirac({1}) << endl;
    cout << dirac({2}) << endl;
    cout << dirac({3}) << endl;
    cout << dirac({4}) << endl;
    auto post = robot.move({1}, dirac);

    cout << post({0}) << endl;
    cout << post({1}) << endl;
    cout << post({2}) << endl;
    cout << post({3}) << endl;
    cout << post({4}) << endl;


    // posterior = robot.move(moves, prior);
    return 0;
}

