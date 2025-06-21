FROM ubuntu:22.04
 
# Prevent interactive prompts
ENV DEBIAN_FRONTEND=noninteractive
 
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    g++ \
    git \
    valgrind \
    curl \
    wget \
    unzip \
    pkg-config \
    libssl-dev 
 
RUN apt-get update && apt-get install -y libopencv-dev
RUN apt-get update && apt-get install -y libboost-all-dev
RUN apt-get update && apt-get install -y libeigen3-devRUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    g++ \
    git \
    valgrind \
    curl \
    wget \
    unzip \
    pkg-config \
    libssl-dev \
    libopencv-dev \
    libboost-all-dev \
    libeigen3-dev \
    libyaml-cpp-dev \
    nlohmann-json3-dev \
    libcgal-dev \
    # libgdal-dev 
 
 
# Install Crow (header-only)
RUN git clone https://github.com/CrowCpp/crow.git /opt/crow \
    && ln -s /opt/crow/include/crow /usr/local/include/crow
 
# Install GoogleTest (build from source)
RUN git clone https://github.com/google/googletest.git /opt/googletest \
    && cd /opt/googletest && cmake -DCMAKE_INSTALL_PREFIX=/usr/local . \
    && make -j$(nproc) && make install
 
# Install Python and tools
RUN apt-get update && apt-get install -y \
    python3 \
    python3-pip \
    python3-venv \
    python3-dev \
    ipython \
    && rm -rf /var/lib/apt/lists/*
 
# Upgrade pip and install scientific packages
RUN pip3 install --upgrade pip setuptools wheel
 
# Install core scientific stack
RUN pip3 install numpy scipy matplotlib pandas scikit-learn jupyter
 
# Install PyTorch (CPU-only version)
RUN pip3 install torch torchvision torchaudio --index-url https://download.pytorch.org/whl/cpu
 
# Install TensorFlow (CPU-only version)
RUN pip3 install tensorflow
 
# Install Jupyter Notebook
RUN pip3 install notebook
 
# Create a workspace directory (optional)
RUN mkdir -p /workspace && chown -R root:root /workspace
 
# Expose Jupyter port
EXPOSE 8888
 
# Install Node.js (LTS)
RUN curl -fsSL https://deb.nodesource.com/setup_18.x | bash - && \
    apt-get install -y nodejs && \
    npm install -g npm
RUN npm install -g yarn typescript vite
 
# Install MongoDB
RUN apt-get update && apt-get install -y gnupg && \
    curl -fsSL https://pgp.mongodb.com/server-6.0.asc | gpg --dearmor -o /usr/share/keyrings/mongodb-server-6.0.gpg && \
    echo "deb [ arch=amd64 signed-by=/usr/share/keyrings/mongodb-server-6.0.gpg ] https://repo.mongodb.org/apt/ubuntu jammy/mongodb-org/6.0 multiverse" | tee /etc/apt/sources.list.d/mongodb-org-6.0.list && \
    apt-get update && apt-get install -y mongodb-org && \
    mkdir -p /data/db
 
# Expose MongoDB port
EXPOSE 27017