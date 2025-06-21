//
// Created by Daniel St. John St. John on 6/21/25.
//
#include <gtest/gtest.h>
#include <iostream>
using namespace std;
#include "probability_distribution.h"
#include "world_tensor.h"
#include "robot.h"
#include "state.h"
using namespace dsj;

TEST(one_dimension, probability_distribution) {
    // one dimensional probability distribution 5 states
    probability_distribution<float> init_prob_dist(std::vector<size_t>{5});

    cout << init_prob_dist({0}) << endl;
    cout << init_prob_dist({1}) << endl;
    cout << init_prob_dist({2}) << endl;
    cout << init_prob_dist({3}) << endl;
    cout << init_prob_dist({4}) << endl;

    EXPECT_EQ(1, 1);
}

TEST(one_dimension, sense) {
    probability_distribution<float> init_prob_dist(std::vector<size_t>{5});
    cout << init_prob_dist({0}) << endl;
    cout << init_prob_dist({1}) << endl;
    cout << init_prob_dist({2}) << endl;
    cout << init_prob_dist({3}) << endl;
    cout << init_prob_dist({4}) << endl;

    World<color_measurement> world({5});
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


    Robot<color_measurement> robot(world);
    auto posterior = robot.sense(color_measurement{ColorStates::GREEN}, init_prob_dist);

    cout << posterior({0}) << endl;
    cout << posterior({1}) << endl;
    cout << posterior({2}) << endl;
    cout << posterior({3}) << endl;
    cout << posterior({4}) << endl;
}

TEST(one_dimension, move) {
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

    World<color_measurement> world({5});
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


    Robot<color_measurement> robot(world);
    auto post = robot.move({1}, dirac);

    cout << post({0}) << endl;
    cout << post({1}) << endl;
    cout << post({2}) << endl;
    cout << post({3}) << endl;
    cout << post({4}) << endl;
}

TEST(one_dimension, move_sense_twice_red_red) {
    probability_distribution<float> init_prob_dist(std::vector<size_t>{5});
    cout << init_prob_dist({0}) << endl;
    cout << init_prob_dist({1}) << endl;
    cout << init_prob_dist({2}) << endl;
    cout << init_prob_dist({3}) << endl;
    cout << init_prob_dist({4}) << endl;

    World<color_measurement> world({5});
    world.set_cell(std::vector<size_t>{0}, color_measurement{ColorStates::GREEN});
    world.set_cell(std::vector<size_t>{1}, color_measurement{ColorStates::RED});
    world.set_cell(std::vector<size_t>{2}, color_measurement{ColorStates::RED});
    world.set_cell(std::vector<size_t>{3}, color_measurement{ColorStates::GREEN});
    world.set_cell(std::vector<size_t>{4}, color_measurement{ColorStates::GREEN});

    Robot<color_measurement> robot(world);

    std::vector<color_measurement> color_measurements{
        color_measurement{ColorStates::RED},
        color_measurement{ColorStates::RED}
    };

    std::vector<size_t> move_one{1};
    std::vector<std::vector<size_t> > movements = {
        move_one,
        move_one
    };

    probability_distribution<float> p = init_prob_dist;
    for (auto i = 0; i < movements.size(); ++i) {
        color_measurement m = color_measurements[i];
        std::vector<size_t> move = movements[i];

        p = robot.sense(m, p);
        cout << p({0}) << endl;
        cout << p({1}) << endl;
        cout << p({2}) << endl;
        cout << p({3}) << endl;
        cout << p({4}) << endl;
        p = robot.move(move, p);
        cout << p({0}) << endl;
        cout << p({1}) << endl;
        cout << p({2}) << endl;
        cout << p({3}) << endl;
        cout << p({4}) << endl;
    }

    cout << p({0}) << endl;
    cout << p({1}) << endl;
    cout << p({2}) << endl;
    cout << p({3}) << endl;
    cout << p({4}) << endl;
}

TEST(one_dimension, move_sense_twice_red_green) {
    probability_distribution<float> init_prob_dist(std::vector<size_t>{5});
    cout << init_prob_dist({0}) << endl;
    cout << init_prob_dist({1}) << endl;
    cout << init_prob_dist({2}) << endl;
    cout << init_prob_dist({3}) << endl;
    cout << init_prob_dist({4}) << endl;

    World<color_measurement> world({5});
    world.set_cell(std::vector<size_t>{0}, color_measurement{ColorStates::GREEN});
    world.set_cell(std::vector<size_t>{1}, color_measurement{ColorStates::RED});
    world.set_cell(std::vector<size_t>{2}, color_measurement{ColorStates::RED});
    world.set_cell(std::vector<size_t>{3}, color_measurement{ColorStates::GREEN});
    world.set_cell(std::vector<size_t>{4}, color_measurement{ColorStates::GREEN});

    Robot<color_measurement> robot(world);

    std::vector<color_measurement> color_measurements{
        color_measurement{ColorStates::RED},
        color_measurement{ColorStates::GREEN}
    };

    std::vector<size_t> move_one{1};
    std::vector<std::vector<size_t> > movements = {
        move_one,
        move_one
    };

    probability_distribution<float> p = init_prob_dist;
    for (auto i = 0; i < movements.size(); ++i) {
        color_measurement m = color_measurements[i];
        std::vector<size_t> move = movements[i];

        p = robot.sense(m, p);
        cout << p({0}) << endl;
        cout << p({1}) << endl;
        cout << p({2}) << endl;
        cout << p({3}) << endl;
        cout << p({4}) << endl;
        p = robot.move(move, p);
        cout << p({0}) << endl;
        cout << p({1}) << endl;
        cout << p({2}) << endl;
        cout << p({3}) << endl;
        cout << p({4}) << endl;
    }

    cout << p({0}) << endl;
    cout << p({1}) << endl;
    cout << p({2}) << endl;
    cout << p({3}) << endl;
    cout << p({4}) << endl;
}

