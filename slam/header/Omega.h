//
// Created by Daniel St. John St. John on 6/21/25.
//

#pragma once

#include <xtensor/xarray.hpp>
#include <xtensor/xadapt.hpp>
#include <unordered_map>

// Each measurement [landmark_index, dx, dy] gives a constraint of the form:
// [robot_x - landmark_x = dx]
// [robot_y - landmark_y = dy]
// Ω[robot_x][robot_x] += 1
// Ω[robot_x][landmark_x] += -1
// Ω[landmark_x][robot_x] += -1
// Ω[landmark_x][landmark_x] += 1
namespace dsj {
    class OmegaMatrix {
    public:
        OmegaMatrix(size_t num_poses, size_t num_landmarks)
            : n_pose_vars(num_poses * 2), // x and y per pose
              n_landmark_vars(num_landmarks * 2),
              total_vars(n_pose_vars + n_landmark_vars),
              omega(xt::zeros<float>({total_vars, total_vars})) {
        }

        void add_pose_constraint(size_t pose_index, float strength = 1.0f) {
            size_t xi = pose_index * 2;
            omega(xi, xi) += strength;
            omega(xi + 1, xi + 1) += strength;
        }

        void add_measurement_constraint(size_t pose_index, size_t landmark_index, float strength = 1.0f) {
            size_t xi = pose_index * 2;
            size_t li = n_pose_vars + landmark_index * 2;

            omega(xi, xi) += strength;
            omega(xi + 1, xi + 1) += strength;
            omega(li, li) += strength;
            omega(li + 1, li + 1) += strength;

            omega(xi, li) -= strength;
            omega(li, xi) -= strength;

            omega(xi + 1, li + 1) -= strength;
            omega(li + 1, xi + 1) -= strength;
        }

        const xt::xarray<float> &matrix() const {
            return omega;
        }

    private:
        size_t n_pose_vars;
        size_t n_landmark_vars;
        size_t total_vars;
        xt::xarray<float> omega;
    };
}

