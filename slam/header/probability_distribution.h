#pragma once
#include <xtensor/containers/xarray.hpp>
// #include <xtensor/xarray.hpp>
#include <xtensor/generators/xrandom.hpp>
// #include <xtensor/xio.hpp>
// #include <xtensor/xmath.hpp>
// #include <xtensor/xadapt.hpp>
#include <numeric>
#include <vector>
#include <cmath>
#include <algorithm>
#include <stdexcept>

namespace dsj {
    template<typename T>
    class probability_distribution {
    public:
        probability_distribution() = default;

        explicit probability_distribution(const std::vector<size_t> &dimensions)
            : data(xt::ones<T>(dimensions)) {
            normalize();
        }

        explicit probability_distribution(const typename xt::xarray<T>::shape_type &shape)
            : data(xt::ones<T>(shape)) {
            normalize();
        }

        static probability_distribution uniform(const std::vector<size_t> &dimensions) {
            return probability_distribution(dimensions);
        }

        static probability_distribution random(const std::vector<size_t> &dimensions) {
            probability_distribution dist(dimensions);
            dist.data = xt::random::rand<T>(dimensions);
            dist.normalize();
            return dist;
        }

        std::pair<T, std::vector<size_t> > max_probability() const {
            auto max_it = std::max_element(data.begin(), data.end());
            T max_val = *max_it;
            size_t flat_index = std::distance(data.begin(), max_it);
            std::vector<size_t> coords = flat_to_coords(flat_index);
            return {max_val, coords};
        }

        const xt::xarray<T> &get_data() const {
            return data;
        }

        T &operator()(const std::vector<size_t> &coords) {
            return data.element(coords.begin(), coords.end());
        }

        const T &operator()(const std::vector<size_t> &coords) const {
            return data.element(coords.begin(), coords.end());
        }

        // Access element by multi-index (variadic)
        template<typename... Indices>
        T &at(Indices... idxs) {
            return data(idxs...);
        }

        template<typename... Indices>
        T &operator()(Indices... idxs) {
            return data(idxs...);
        }

        template<typename... Indices>
        const T &operator()(Indices... idxs) const {
            return data(idxs...);
        }


        std::vector<size_t> dimensions() const {
            return std::vector<size_t>(data.shape().begin(), data.shape().end());
        }

        size_t size() const { return data.size(); }

        void normalize() {
            T sum = std::accumulate(data.begin(), data.end(), T(0));
            if (sum != T(0)) {
                data /= sum;
            }
        }

        probability_distribution operator*(const probability_distribution &other) const {
            if (data.shape() != other.data.shape()) {
                throw std::invalid_argument("Shapes do not match for element-wise multiplication.");
            }
            probability_distribution result(data.shape());
            result.data = data * other.data;
            result.normalize();
            return result;
        }

        std::vector<size_t> flat_to_coords(size_t flat_idx) const {
            const auto &shape = data.shape();
            std::vector<size_t> coords(shape.size());
            for (int i = static_cast<int>(shape.size()) - 1; i >= 0; --i) {
                coords[i] = flat_idx % shape[i];
                flat_idx /= shape[i];
            }
            return coords;
        }

        std::vector<T> mean() const {
            const auto &shape = data.shape();
            std::vector<T> means(shape.size(), T(0));

            for (size_t i = 0; i < data.size(); ++i) {
                auto coords = flat_to_coords(i);
                T prob = data[i];
                for (size_t dim = 0; dim < shape.size(); ++dim) {
                    means[dim] += coords[dim] * prob;
                }
            }
            return means;
        }

        xt::xarray<T> covariance_matrix() const {
            const auto &shape = data.shape();
            size_t n_dims = shape.size();
            auto means = mean();
            xt::xarray<T> cov = xt::zeros<T>({n_dims, n_dims});

            for (size_t i = 0; i < data.size(); ++i) {
                auto coords = flat_to_coords(i);
                T prob = data[i];
                for (size_t dim1 = 0; dim1 < n_dims; ++dim1) {
                    T diff1 = static_cast<T>(coords[dim1]) - means[dim1];
                    for (size_t dim2 = 0; dim2 <= dim1; ++dim2) {
                        T diff2 = static_cast<T>(coords[dim2]) - means[dim2];
                        T cov_val = diff1 * diff2 * prob;
                        cov(dim1, dim2) += cov_val;
                        if (dim1 != dim2) {
                            cov(dim2, dim1) += cov_val;
                        }
                    }
                }
            }
            return cov;
        }

        std::vector<T> variance() const {
            auto cov = covariance_matrix();
            std::vector<T> var(cov.shape()[0]);
            for (size_t i = 0; i < var.size(); ++i) {
                var[i] = cov(i, i);
            }
            return var;
        }

        T entropy() const {
            T h = T(0);
            for (const auto &p: data) {
                if (p > T(0)) {
                    h -= p * std::log2(p);
                }
            }
            return h;
        }

        // 1D only: return scalar variance
        T variance_1d() const {
            auto var = variance();
            if (var.size() != 1) {
                throw std::logic_error("variance_1d() called on multi-dimensional distribution.");
            }
            return var[0];
        }

        xt::xarray<T> data;
    };
} // namespace dsj
