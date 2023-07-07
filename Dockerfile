FROM alpine:3.18.2

RUN apk update
RUN apk add --no-cache vim
RUN apk add --no-cache g++
RUN apk add --no-cache make
RUN apk add --no-cache cmake
RUN apk add --no-cache git
RUN apk add --no-cache glu-dev
RUN apk add --no-cache mesa-dev
RUN apk add --no-cache boost-dev
RUN apk add --no-cache libxml2-dev

RUN mkdir SUNLIGHT
WORKDIR /SUNLIGHT
COPY CMakeLists.txt .
COPY datas datas
COPY src src
