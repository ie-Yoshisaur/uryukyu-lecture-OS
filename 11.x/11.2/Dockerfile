#Stage 1
FROM golang AS build01
RUN mkdir workspace
COPY ./hello_world.go /workspace/
WORKDIR /workspace
RUN go build hello_world.go
#Stage 2
FROM ubuntu
RUN mkdir workspace
COPY --from=build01 workspace/hello_world /workspace
WORKDIR /workspace
ENTRYPOINT ["./hello_world"]
