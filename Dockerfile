# tgpet
FROM alpine:latest
RUN apk update && apk upgrade && apk add --no-cache linux-headers g++ make gdb cmake git openssl-dev ccache zlib-dev gperf jsoncpp-dev ossp-uuid-dev
WORKDIR /tiko
RUN mkdir -p logs storage build bin
COPY . ./

RUN cd build && cmake ..
RUN cd build && cmake --build . -j4

RUN mv build/tiko build/tgen bin
RUN rm -rf tiko.cpp tgen.cpp CMakeLists.txt tg utils diverse Dockerfile
ENTRYPOINT [ "bin/tiko" ]
