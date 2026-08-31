FROM geant4:11.4.0
LABEL authors="vetlewi"

WORKDIR /
COPY . /afrodite-source
WORKDIR /afrodite-build

RUN apt-get update && apt-get install -y git gdb

RUN cmake -DCMAKE_BUILD_TYPE=Debug -S /afrodite-source/all -B ./
RUN make -j$(nproc)

WORKDIR /afrodite-build/app
#ENTRYPOINT ["AFRODITE"]

