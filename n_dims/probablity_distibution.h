#pragma once
#include <xtensor/containers/xtensor.hpp>
#include <xtensor/containers/xarray.hpp>
#include <xtensor/io/xio.hpp>
#include <xtensor/generators/xrandom.hpp>


namespace dsj {
    // In Bayesian probability theory, when you're combining a prior belief with
    // new evidence to get a posterior belief, you multiply the distributions
    // element-wise and then normalize the result. This is based on Bayes' theorem:
    // P(A|B) ∝ P(B|A) * P(A)
    // We need to add a multiplication operator to your probability distribution
    // class that does element-wise multiplication and then normalizes the result.

    template<typename T>
    class iprobability_distribution {
    };

    template<typename T>
    class probability_distribution {
    public:
        probability_distribution() = default;

        probability_distribution(const std::vector<size_t> &dimensions)
            : data(xt::xarray<T>::from_shape(dimensions)) {
            this->data = xt::ones<T>(dimensions);
            this->normalize();
        }


        probability_distribution(const xt::xarray<T>::shape_type &shape)
            : data(xt::ones<T>(shape)) {
            normalize();
        }

        // Create a uniform distribution
        static probability_distribution uniform(const std::vector<size_t> &dimensions) {
            probability_distribution dist(dimensions);
            dist.data = xt::ones<T>(dimensions);
            dist.normalize();
            return dist;
        }

        // Create a random distribution
        static probability_distribution random(const std::vector<size_t> &dimensions) {
            probability_distribution dist(dimensions);
            dist.data = xt::random::rand<T>(dimensions);
            dist.normalize();
            return dist;
        }

        // Get the maximum probability and its location
        std::pair<T, std::vector<size_t> > max_probability() const {
            auto max_element = std::max_element(data.begin(), data.end());
            T max_prob = *max_element;

            // Find the index of the maximum element
            size_t flat_index = std::distance(data.begin(), max_element);
            std::vector<size_t> coords = {flat_index}; // For 1D case

            return {max_prob, coords};
        }

        // Add a method to get raw data for debugging/visualization
        const xt::xarray<T> &get_data() const {
            return data;
        }


        /*
            Example usage:
            probability_distribution<double> dist({3, 4, 2});  // 3D distribution
            // Using vector of coordinates
            std::vector<size_t> coords = {1, 2, 0};
            double val1 = dist(coords);
            // Using direct indices
            double val2 = dist(1, 2, 0);  // same position as above
        */
        T &operator()(const std::vector<size_t> &coords) {
            size_t flat_index = 0;
            auto shape = dimensions();
            size_t stride = 1;

            for (int i = coords.size() - 1; i >= 0; --i) {
                flat_index += coords[i] * stride;
                stride *= shape[i];
            }
            return data[flat_index];
        }

        const T &operator()(const std::vector<size_t> &coords) const {
            size_t flat_index = 0;
            auto shape = dimensions();
            size_t stride = 1;

            for (int i = coords.size() - 1; i >= 0; --i) {
                flat_index += coords[i] * stride;
                stride *= shape[i];
            }
            return data[flat_index];
        }


        std::vector<size_t> dimensions() const {
            return std::vector<size_t>(data.shape().begin(), data.shape().end());
        }


        void normalize() {
            T sum = std::accumulate(data.begin(), data.end(), T(0));
            if (sum != T(0)) {
                data /= sum;
            }
        }


        // Element-wise multiplication of distributions
        probability_distribution operator*(const probability_distribution &other) const {
            // Create a new distribution with same shape
            probability_distribution result(xt::shape(data));
            // Element-wise multiplication
            result.data = data * other.data;
            // Normalize the result
            result.normalize();
            return result;
        }

        // Add these methods to the probability_distribution class:

        // Add these methods to the probability_distribution class:

        // Helper function to convert flat index to n-dimensional coordinates
        std::vector<size_t> flat_to_coords(size_t flat_idx) const {
            std::vector<size_t> shape = dimensions();
            std::vector<size_t> coords(shape.size());
            for (int dim = shape.size() - 1; dim >= 0; --dim) {
                coords[dim] = flat_idx % shape[dim];
                flat_idx /= shape[dim];
            }
            return coords;
        }

        // Calculate mean vector (one mean per dimension)
        std::vector<T> mean() const {
            std::vector<size_t> shape = dimensions();
            std::vector<T> means(shape.size(), T(0));
            // For each point in the distribution
            for (size_t i = 0; i < data.size(); ++i) {
                std::vector<size_t> coords = flat_to_coords(i);
                T prob = data[i];
                // Accumulate weighted position for each dimension
                for (size_t dim = 0; dim < shape.size(); ++dim) {
                    means[dim] += coords[dim] * prob;
                }
            }
            return means;
        }

        // Calculate full covariance matrix for N dimensions
        xt::xarray<T> covariance_matrix() const {
            std::vector<size_t> shape = dimensions();
            size_t n_dims = shape.size();
            // Get means for each dimension
            std::vector<T> means = mean();
            // Initialize covariance matrix
            xt::xarray<T> cov = xt::zeros<T>({n_dims, n_dims});
            // For each point in the distribution
            for (size_t i = 0; i < data.size(); ++i) {
                std::vector<size_t> coords = flat_to_coords(i);
                T prob = data[i];
                // Calculate covariances between all pairs of dimensions
                for (size_t dim1 = 0; dim1 < n_dims; ++dim1) {
                    T diff1 = static_cast<T>(coords[dim1]) - means[dim1];
                    for (size_t dim2 = 0; dim2 <= dim1; ++dim2) {
                        T diff2 = static_cast<T>(coords[dim2]) - means[dim2];
                        T cov_value = diff1 * diff2 * prob;
                        // Covariance matrix is symmetric
                        cov(dim1, dim2) += cov_value;
                        if (dim1 != dim2) {
                            cov(dim2, dim1) += cov_value;
                        }
                    }
                }
            }
            return cov;
        }

        // Get variance vector (diagonal of covariance matrix)
        std::vector<T> variance() const {
            auto cov = covariance_matrix();
            std::vector<T> var(cov.shape()[0]);

            for (size_t i = 0; i < var.size(); ++i) {
                var[i] = cov(i, i);
            }

            return var;
        }

        // Helper method to get entropy (works for any dimension)
        T entropy() const {
            T h = T(0);
            for (const auto &p: data) {
                if (p > T(0)) {
                    h -= p * std::log2(p);
                }
            }
            return h;
        }


        // Calculate the covariance for 1D case (returns variance)
        T covariance() const {
            return variance();
        }


        // Access element by multi-index
        template<typename... Indices>
        T &at(Indices... idxs) {
            return data(idxs...);
        }

        size_t size() const { return data.size(); }

        xt::xarray<T> data;
    };
}
