#pragma once
#include <xtensor/containers/xtensor.hpp>
#include <xtensor/containers/xarray.hpp>
#include <xtensor/containers/xadapt.hpp>

#include <iterator>
#include <vector>
#include <stdexcept>

namespace dsj {
    template<typename T>
    class WorldIterator;

    template<typename T>
    class World {
    public:
        friend class WorldIterator<T>;
        using shape_type = typename xt::xarray<T>::shape_type;

        World() = default;

        explicit World(const std::vector<size_t> &dimensions)
            : space(xt::xarray<T>::from_shape(dimensions)) {
        }

        bool is_valid_position(const std::vector<size_t> &position) const {
            if (position.size() != space.dimension()) {
                return false;
            }
            for (size_t i = 0; i < position.size(); ++i) {
                if (position[i] >= space.shape()[i]) {
                    return false;
                }
            }
            return true;
        }

        void set_cell(const std::vector<size_t> &coords, const T &value) {
            if (!is_valid_position(coords)) {
                throw std::out_of_range("Position is out of world bounds");
            }
            space.element(coords.begin(), coords.end()) = value;
        }

        T get_cell(const std::vector<size_t> &coords) const {
            if (!is_valid_position(coords)) {
                throw std::out_of_range("Position is out of world bounds");
            }
            return space.element(coords.begin(), coords.end());
        }

        std::vector<size_t> dimensions() const {
            return std::vector<size_t>(space.shape().begin(), space.shape().end());
        }

        WorldIterator<T> begin() {
            return WorldIterator<T>(space, false);
        }

        WorldIterator<T> end() {
            return WorldIterator<T>(space, true);
        }

    private:
        xt::xarray<T> space;
    };


    template<typename T>
    class WorldIterator {
    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = std::pair<std::vector<size_t>, T>;
        using pointer = value_type *;
        using reference = value_type &;

        WorldIterator(xt::xarray<T> &space, bool is_end = false)
            : space_(&space),
              it_(is_end ? space_->end() : space_->begin()),
              end_(space_->end()) {
            if (!is_end) {
                updateCurrentValue();
            }
        }

        WorldIterator &operator++() {
            ++it_;
            if (it_ != end_) {
                updateCurrentValue();
            }
            return *this;
        }

        bool operator==(const WorldIterator &other) const {
            return it_ == other.it_;
        }

        bool operator!=(const WorldIterator &other) const {
            return it_ != other.it_;
        }

        value_type operator*() const {
            return current_value_;
        }

    private:
        xt::xarray<T> *space_; // Pointer to space
        typename xt::xarray<T>::iterator it_;
        typename xt::xarray<T>::iterator end_;
        value_type current_value_;

        void updateCurrentValue() {
            auto offset = std::distance(space_->begin(), it_);
            auto shape = space_->shape();
            auto unravel_result = xt::unravel_index(offset, shape);
            std::vector<size_t> coords(unravel_result.begin(), unravel_result.end());
            current_value_ = std::make_pair(coords, *it_);
        }
    };
}
