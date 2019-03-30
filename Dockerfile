# BUILD mkthumb

FROM debian as build-mkthumb

RUN apt-get update
RUN apt-get install -qy build-essential pkg-config
RUN apt-get install -qy libseccomp-dev libwebp-dev libpng-dev libjpeg-dev

WORKDIR /build/mkthumb
ADD mkthumb /build/mkthumb
RUN gcc -Wall -O3 `find src -name *.c` `pkg-config --cflags --libs libseccomp libwebp libpng libjpeg`

# BUILD server

FROM golang:1.12 as build-server

WORKDIR /build/server
ADD server /build/server
RUN go build -v
