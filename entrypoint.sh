#!/bin/bash
 
# Create user
USER_ID=${USER_ID:-1000}
GROUP_ID=${GROUP_ID:-1000}
 
groupadd -g $GROUP_ID devgroup || true
useradd -u $USER_ID -g $GROUP_ID -m -s /bin/bash dev || true
 
mkdir -p /home/dev/.ssh
chown -R dev:devgroup /home/dev/.ssh
chmod 700 /home/dev/.ssh
 
# Set workspace permissions
mkdir -p /workspace
chown dev:devgroup /workspace
 
# Start Jupyter Notebook as dev user
exec su - dev -c "jupyter notebook --notebook-dir=/workspace --ip=0.0.0.0 --no-browser --allow-root"
 
 