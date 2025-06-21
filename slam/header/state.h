//
// Created by Daniel St. John St. John on 6/20/25.
//

#pragma once
#include <ostream>


namespace dsj {
    enum class ColorStates {
        RED,
        GREEN,
    };

    inline std::ostream &operator<<(std::ostream &os, const ColorStates &state) {
        switch (state) {
            case ColorStates::RED:
                os << "\033[31m●\033[0m"; // Red circle with ANSI color
                break;
            case ColorStates::GREEN:
                os << "\033[32m●\033[0m"; // Green circle with ANSI color
                break;
            default:
                os << "?";
                break;
        }
        return os;
    }


    // State enum to represent different cell states
    enum class MoveViewState {
        OPEN, // Robot can move here
        BLOCKED, // Obstacle or wall
        LANDMARK, // Contains a landmark
        ROBOT, // Robot's current position
    };

    inline std::ostream &operator<<(std::ostream &os, const MoveViewState &state) {
        switch (state) {
            case MoveViewState::OPEN:
                os << "."; // Open space
                break;
            case MoveViewState::BLOCKED:
                os << "#"; // Wall/obstacle
                break;
            case MoveViewState::LANDMARK:
                os << "L"; // Landmark
                break;
            case MoveViewState::ROBOT:
                os << "R"; // Robot
                break;
            default:
                os << "?"; // Unknown state
                break;
        }
        return os;
    }


    template<typename states_space>
    struct measurement {
        friend std::ostream &operator<<(std::ostream &os, const measurement &obj) {
            return os << "m: " << obj.m;
        }

        states_space m;

        friend bool operator==(const measurement &lhs, const measurement &rhs) {
            return lhs.m == rhs.m;
        }

        friend bool operator!=(const measurement &lhs, const measurement &rhs) {
            return !(lhs == rhs);
        }
    };

    typedef measurement<ColorStates> color_measurement;
    typedef measurement<MoveViewState> move_view_measurement;


    // State class that can hold both the type and additional metadata
    template<typename STATES>
    class State {
    public:
        STATES type;
        int id; // Can be used for landmark or robot identification

        State(STATES t, int identifier = 0)
            : type(t), id(identifier) {
        }

        // Operators for comparison
        bool operator==(const State &other) const {
            return type == other.type && id == other.id;
        }

        bool operator!=(const State &other) const {
            return !(*this == other);
        }
    };
}
