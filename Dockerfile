ARG UBUNTU_VERSION=bionic
FROM ubuntu:${UBUNTU_VERSION}

RUN apt-get update && apt-get upgrade -y

RUN apt-get install -y build-essential unzip zip

# Source: https://github.com/VCVRack/manual/blob/master/Building.md#linux
RUN apt-get install -y curl cmake zlib1g-dev libasound2-dev libjack-jackd2-dev

# I suspect I don't need these because BBI isn't doing anything fancy and I'm not building all of Rack
# RUN apt-get install libx11-dev libglu1-mesa-dev libxrandr-dev libxinerama-dev libxcursor-dev libgtk2.0-dev libxi-dev

WORKDIR /src
ARG RACK_SDK_URL=https://vcvrack.com/downloads/Rack-SDK-0.6.1.zip
RUN curl ${RACK_SDK_URL} --output /src/Rack-SDK.zip
RUN unzip Rack-SDK.zip

WORKDIR /src/plugin

ENTRYPOINT [ "make" ]
CMD [ "clean", "dist" ]

# $ docker build -t rack-linux-plugin .
# $ docker run -v $(pwd):/src/plugin rack-linux-plugin