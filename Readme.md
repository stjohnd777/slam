
1. **N-dimensional State Space**
    - Handles arbitrary dimensions
    - Generic state representation
    - Flexible probability distributions

2. **Discrete Probability Foundation**
    - Proper probability distribution handling
    - Statistical measures (entropy, variance, covariance)
    - Proper indexing for hyper-dimensional spaces

3. **Generic Implementation**
    - Template-based design for flexibility
    - Can work with different state types
    - Extensible for different sensor models

This architecture will make it easier to:
- Add different types of sensors
- Implement various motion models
- Handle complex state representations
- Add continuous state space later if needed
- Implement different SLAM algorithms (FastSLAM, GraphSLAM, etc.)

Would you like to explore any particular aspect next, such as:
- Adding more sensor types?
- Implementing motion models?
- Working on map representation?
- Adding particle filter capabilities?







🧹 Docker
You now have a modern multi-language dev image with:
 
✅ C++: g++, cmake, valgrind, Boost, Eigen, OpenCV, Crow, yaml-cpp, nlohmann/json
 
✅ Geometry/GIS: CGAL, GDAL
 
✅ Unit Testing: GoogleTest
 
✅ Python Dev: python3, pip, venv, ipython
 
✅ ML Frameworks: PyTorch, TensorFlow
 
✅ Container Behavior: UID/GID mapping, mounted .ssh, REST port forwarding


| Port  | Purpose            | Example Use Case              |
| ----- | ------------------ | ----------------------------- |
| 8080  | C++ REST API       | Crow / cpp-httplib endpoints  |
| 8888  | Jupyter Notebook   | Interactive Python / ML       |
| 3000  | Node.js Web Server | Express / React / Vite Dev    |
| 27017 | MongoDB            | Database (Mongo Compass etc.) |
 

start.ps1
param (
    [Parameter(Mandatory=$true)]
    [string]$ImageName
)
 
# Get UID/GID equivalents for Windows (emulated as 1000/1000)
$USER_ID = 1000
$GROUP_ID = 1000
 
# Get user profile and working directory
$HOME_DIR = $env:USERPROFILE
$PWD = Get-Location
 
# Docker run command
docker run -it --rm `
    -e USER_ID=$USER_ID `
    -e GROUP_ID=$GROUP_ID `
    -v "$HOME_DIR\.ssh:/home/dev/.ssh:ro" `
    -v "$PWD:/workspace" `
    -p 8080:8080 `
    --name cpp-dev `
    $ImageName
 
# If you get an execution policy error, allow script execution temporarily:
# Set-ExecutionPolicy -Scope Process -ExecutionPolicy Bypass
 
Start.sh
#!/bin/bash
 
# Usage: ./run_dev_container.sh your-docker-image-name
IMAGE_NAME="$1"
 
if [ -z "$IMAGE_NAME" ]; then
  echo "Usage: $0 your-docker-image-name"
  exit 1
fi
 
USER_ID=$(id -u)
GROUP_ID=$(id -g)
 
docker run -it --rm \
  -e USER_ID="$USER_ID" \
  -e GROUP_ID="$GROUP_ID" \
  -v "$HOME/.ssh:/home/dev/.ssh:ro" \
  -v "$(pwd):/workspace" \
  -p 8080:8080 \
  --name cpp-dev \
  "$IMAGE_NAME"
 
Up.sh

UID=$(id -u) GID=$(id -g) docker-compose up

```aiignore


├── Dockerfile-base
├── Parent
│   ├── Main
│   │   ├── CmakeLists.txt
│   │   ├── Readme.md
│   │   ├── Requirments.md
│   │   ├── cpp
│   │   ├── doc
│   │   ├── include
│   │   └── test
│   ├── doc
│   ├── lib_common
│   │   ├── CmakeLists.txt
│   │   ├── Readme.md
│   │   ├── Requirments.md
│   │   ├── cpp
│   │   ├── doc
│   │   ├── include
│   │   │   ├── private
│   │   │   └── public
│   │   └── test
│   ├── liba
│   │   ├── CmakeLists.txt
│   │   ├── Readme.md
│   │   ├── Requirments.md
│   │   ├── cpp
│   │   ├── doc
│   │   ├── hello_world
│   │   ├── include
│   │   │   ├── private
│   │   │   └── public
│   │   └── test
│   ├── libb
│   │   ├── CmakeLists.txt
│   │   ├── Readme.md
│   │   ├── Requirments.md
│   │   ├── cpp
│   │   ├── doc
│   │   ├── hello_world
│   │   ├── include
│   │   │   ├── private
│   │   │   └── public
│   │   └── test
│   └── libc
│       ├── CmakeLists.txt
│       ├── Readme.md
│       ├── Requirments.md
│       ├── cpp
│       ├── doc
│       ├── hello_world
│       ├── include
│       │   ├── private
│       │   └── public
│       └── test
├── Readme.md
├── docker-compose.yml
├── entrypoint.sh
├── github
├── github.pub
├── start.ps1
├── start.sh
└── up.sh

```
