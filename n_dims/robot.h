//
// Created by Daniel St. John St. John on 6/20/25.
//

#pragma once
#include "world.h"
#include "state.h"
#include "probablity_distibution.h"
#include "world.h"

template<typename Measurement, typename T = float>
class Robot {
    typedef dsj::probability_distribution<T> Prior;
    typedef dsj::probability_distribution<T> Posterior;

public:
    Robot() = default;

    Robot(dsj::World<Measurement> world) : world_(world) {
        // Initialize the robot with a world
    }

public:
    Posterior sense(Prior p, Measurement m) {
        Posterior q(p.data.shape()); // Create a new posterior distribution with the same shape as prior

        for (auto [coords,a_measurement]: world_) {
            cout << "(";
            for (auto c: coords) {
                cout << c << ",";
            }
            cout << "):";
            cout << a_measurement << endl;
            auto hit = a_measurement == m ? 1 : 0;
            q(coords) = p(coords) * (pReadCorrect_ * hit + (1 - hit) * pReadWrong_);
        }
        q.normalize();
        return q;
    }

    Posterior sense(Prior p, std::vector<Measurement> measurements) {
        for (const auto &m: measurements) {
            p = sense(p, m);
        }
        return p;
    }

    Posterior move(const std::vector<size_t> &moves, dsj::probability_distribution<T> p) {
        Posterior q(world_.dimensions()); // Create a new posterior distribution with the same shape as world

        // if (moves.size() != world_.dimensions().size()) {
        //     throw std::invalid_argument("Number of moves must match the number of dimensions in the world.");
        // }
        // for (size_t i = 0; i < world_.dimensions().size(); ++i) {
        //     size_t movement_in_dim = moves[i];
        //     // cycle
        //     q(i) = p((i - movement_in_dim) % world_.dimensions()[i]);
        // }
        // q.normalize();
        return q;
    }

    [[nodiscard]] float p_read_correct() const {
        return pReadCorrect_;
    }


    [[nodiscard]] float p_read_wrong() const {
        return pReadWrong_;
    }

    [[nodiscard]] dsj::World<Measurement> world() const {
        return world_;
    }

private:
    float pReadCorrect_ = 0.6f; // Probability of correct reading
    float pReadWrong_ = 0.2f; // Probability of wrong reading
    dsj::World<Measurement> world_;
};
