//
// Created by Daniel St. John St. John on 6/20/25.
//

#pragma once
#include "../n_dims/world_tensor.h"
#include "../n_dims/state.h"
#include "probability_distribution.h"
#include "../n_dims/world_tensor.h"
#include <xtensor/misc/xmanipulation.hpp>

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
        pMoveUnderShoot_ = under;
        pMoveExact_ = exact;
        pMoveOverShoot_ = over;
    }

public:
    // p(x|z) ∝ p(z|x) * p(x)
    Posterior sense(Prior p, Measurement m) {
        Posterior q(p.data.shape()); // Create a new posterior distribution with the same shape as prior

        for (auto [coords,a_measurement]: world_) {
            // cout << "(";
            // for (auto c: coords) {
            //     cout << c << ",";
            // }
            // cout << "):";
            // cout << a_measurement << endl;
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

    Posterior move(const std::vector<size_t> &motion, Prior p) {
        if (motion.size() != world_.dimensions().size()) {
            throw std::invalid_argument("Motion vector must match world dimensions.");
        }

        Posterior q(p.data.shape());
        q.data = xt::zeros<T>(p.data.shape());

        for (size_t axis = 0; axis < motion.size(); ++axis) {
            std::ptrdiff_t move = static_cast<std::ptrdiff_t>(motion[axis]);

            // Apply undershooting
            q.data += xt::roll(p.data, move - 1, axis) * static_cast<T>(pMoveUnderShoot_);
            // Apply the exact move
            q.data += xt::roll(p.data, move, axis) * static_cast<T>(pMoveExact_);
            // Apply overshoot
            q.data += xt::roll(p.data, move + 1, axis) * static_cast<T>(pMoveOverShoot_);
        }

        q.normalize();
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
    // sense uncertainty
    float pReadCorrect_ = 0.6f; // Probability of correct reading
    float pReadWrong_ = 0.2f; // Probability of wrong reading

    // move uncertainty
    float pMoveUnderShoot_ = 0.1f;
    float pMoveExact_ = 0.8f;
    float pMoveOverShoot_ = 0.1f;
    dsj::World<Measurement> world_;
};
