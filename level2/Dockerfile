FROM --platform=linux/amd64 ubuntu:22.04

RUN dpkg --add-architecture i386 && \
    apt-get update && \
    apt-get install -y \
    libc6:i386 \
    libncurses5:i386 \
    libstdc++6:i386 \
    gdb \
    strace \
    ltrace \
    binutils \
    elfutils \
    git \
    build-essential \
    cmake \
    file \
    python3


COPY level2 /level2
RUN chmod +x /level2

CMD ["/bin/bash"]