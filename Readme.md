Architecture Overview SLAM

- SLAM
    - The World<Measurement> encapsulates the map structure.
        - The probability_distribution<T> models a belief over robot pose.
        - The Robot::sense() step performs a Bayesian update, which is the essence of measurement correction in SLAM.

Conceptually treating the robot's pose as a discrete state in a high-dimensional space
(e.g., grid cells in 2D/3D/ND), and that aligns well with histogram filters or particle filters.
Kalman filters will require additional state (e.g., velocity), but this is a clean start.

- Detailes
    - World<T>
        - Uses xt::xarray<T> correctly as a flexible N-dimensional grid.
        - Iteration is robust via WorldIterator, and updateCurrentValue() correctly converts flat index to coordinates.
        - set_cell() and get_cell() are safe with bounds checking.
        - The interface is dimension-agnostic and clear.
        - probability_distribution<T>
            - Well-suited for belief modeling over a discrete pose space.
            - Has rich functionality: mean, covariance, entropy — useful for introspection.
            - Clean support for initialization (uniform, random), Bayes’ update (operator*), and shape-aware
              normalization.
    - 1**N-dimensional State Space**
        - Handles arbitrary dimensions
        - Generic state representation
        - Flexible probability distributions
    - **Discrete Probability Foundation**
        - Proper probability distribution handling
        - Statistical measures (entropy, variance, covariance)
        - Proper indexing for hyper-dimensional spaces
    - **Generic Implementation**
        - Template-based design for flexibility
        - Can work with different state types
        - Extensible for different sensor models



