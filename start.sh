#Start.sh
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