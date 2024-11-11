FROM ubuntu:22.04
RUN apt update && apt install make g++ valgrind -y && rm -rf /var/lib/apt/lists/*
RUN mkdir /app
WORKDIR /app
COPY . .
RUN make ci
CMD valgrind --leak-check=yes --track-origins=yes ./bin/weblist
