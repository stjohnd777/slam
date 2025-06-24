//
// Created by Daniel St. John St. John on 6/20/25.
//

#pragma once
#include "world_tensor.h"
#include "state.h"
#include "probability_distribution.h"
#include "world_tensor.h"
#include <xtensor/misc/xmanipulation.hpp>


// robot moves and observes.
// Each robot pose is represented as a point in N-dimensional space.
// Each pose gets N entries (e.g., x₀, x₁, ..., x_{N−1}).
template<typename Measurement, typename T = float>
class Robot {
    typedef dsj::probability_distribution<T> Prior;
    typedef dsj::probability_distribution<T> Posterior;

public:
    Robot() = default;

    Robot(dsj::World<Measurement> world) : world_(world) {
        // Initialize the robot with a world
    }


    void set_motion_model(float under, float exact, float over) {
        probablityMoveUnderShoot_ = under;
        probabilityMoveExact_ = exact;
        probabilityMoveOverShoot_ = over;
    }

    // prob reading correct
    // prob reading wrong
    // p(x|z) ∝ p(z|x) * p(x)
    Posterior sense(Measurement m, Prior p) {
        Posterior q(p.data.shape()); // Create a new posterior distribution with the same shape as prior

        for (auto [coords,a_measurement]: world_) {
            // cout << "(";
            // for (auto c: coords) {
            //     cout << c << ",";
            // }
            // cout << "):"; cout << a_measurement << endl;
            auto hit = a_measurement == m ? 1 : 0;
            q(coords) = p(coords) * (probablityReadCorrect_ * hit + (1 - hit) * proabilityReadWrong_);
        }
        q.normalize();
        return q;
    }

    // prob reading correct
    // prob reading wrong
    Posterior sense(std::vector<Measurement> measurements, Prior p) {
        for (const auto &m: measurements) {
            p = sense(m, p);
        }
        return p;
    }

    // prob exact
    // prob undershoot
    // prob overshoot
    // tensor cyclic shift delta times along dim axis
    Posterior move(const std::vector<size_t> &motion, Prior p) {
        if (motion.size() != world_.dimensions().size()) {
            throw std::invalid_argument("Motion vector must match world dimensions.");
        }

        Posterior q(p.data.shape());
        q.data = xt::zeros<T>(p.data.shape());

        for (size_t axis = 0; axis < motion.size(); ++axis) {
            std::ptrdiff_t move = static_cast<std::ptrdiff_t>(motion[axis]);

            // Apply undershooting
            q.data += xt::roll(p.data, move - 1, axis) * static_cast<T>(probablityMoveUnderShoot_);
            // Apply the exact move
            q.data += xt::roll(p.data, move, axis) * static_cast<T>(probabilityMoveExact_);
            // Apply overshoot
            q.data += xt::roll(p.data, move + 1, axis) * static_cast<T>(probabilityMoveOverShoot_);
        }

        q.normalize();
        return q;
    }

    [[nodiscard]] float p_read_correct() const {
        return probablityReadCorrect_;
    }


    [[nodiscard]] float p_read_wrong() const {
        return proabilityReadWrong_;
    }

    [[nodiscard]] dsj::World<Measurement> world() const {
        return world_;
    }

private:
    // sense uncertainty
    float probablityReadCorrect_ = 0.6f; // Probability of correct reading
    float proabilityReadWrong_ = 0.2f; // Probability of wrong reading

    // move uncertainty
    float probablityMoveUnderShoot_ = 0.1f;
    float probabilityMoveExact_ = 0.8f;
    float probabilityMoveOverShoot_ = 0.1f;
    dsj::World<Measurement> world_;
};
