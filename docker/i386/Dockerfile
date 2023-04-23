FROM i386/debian:11

RUN apt-get update && \
    apt-get install -y build-essential gcc-multilib

VOLUME /shared

COPY .bashrc /root/
COPY .vimrc /root/

# Build the container with:
# docker build -t debian32-gcc-v1.0 .

# to run it with a shared directory:
# docker run -it --name debian32-gcc-container-1 -v `pwd`:/shared debian32-gcc-v1.0

# Attach another terminal to container:
# docker exec -it debian32-gcc-container-1 bash