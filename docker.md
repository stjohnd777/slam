🧹 Docker
Modern multi-language dev image with:

✅ C++: g++, cmake, valgrind, Boost, Eigen, OpenCV, Crow, yaml-cpp, nlohmann/json

✅ Geometry/GIS: CGAL, GDAL

✅ Unit Testing: GoogleTest

✅ Python Dev: python3, pip, venv, ipython

✅ ML Frameworks: PyTorch, TensorFlow

✅ Container Behavior: UID/GID mapping, mounted .ssh, REST port forwarding

| Port  | Purpose            | Example Use Case              |
|-------|--------------------|-------------------------------|
| 8080  | C++ REST API       | Crow / cpp-httplib endpoints  |
| 8888  | Jupyter Notebook   | Interactive Python / ML       |
| 3000  | Node.js Web Server | Express / React / Vite Dev    |
| 27017 | MongoDB            | Database (Mongo Compass etc.) |

# Docker run command

docker run -it --rm `
    -e USER_ID=$USER_ID `
-e GROUP_ID=$GROUP_ID `
    -v "$HOME_DIR\.ssh:/home/dev/.ssh:ro" `
-v "$PWD:/workspace" `
    -p 8080:8080 `
--name cpp-dev `
$ImageName

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
