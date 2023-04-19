FROM debian:11

RUN apt-get update && \
    apt-get install -y vim git man-db bc procps \
    build-essential valgrind && \
    rm -rf /var/lib/apt/lists/*

VOLUME /shared

COPY .bashrc /root/
COPY .vimrc /root/


# Build the container with:
# docker build -t nm-image-v1.0 .

# to run it with a shared directory:
# docker run -it --name nm-container-1 -v /path/to/host/dir:/shared nm-image-v1.0

# Attach another terminal to container:
# docker exec -it nm-container-1 bash