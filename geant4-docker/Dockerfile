FROM ubuntu:noble
LABEL authors="vetlewi"

# Set environment variables
ENV G4INSTALL=/opt/geant4
ENV G4DATA_DIR=$G4INSTALL/share/Geant4/data
ENV G4LIB_DIR=$G4INSTALL/lib
ENV PATH=$G4INSTALL/bin:$PATH
ENV LD_LIBRARY_PATH=$G4INSTALL/lib:$LD_LIBRARY_PATH
ENV G4GDMLROOT=$G4INSTALL
ENV G4_VERSION=11.4.0

RUN apt-get update && apt-get install -y \
        build-essential cmake wget libxerces-c-dev \
        libexpat1-dev qt6-base-dev mesa-utils \
        libglu1-mesa-dev freeglut3-dev mesa-common-dev

WORKDIR /opt
RUN wget https://github.com/Geant4/geant4/archive/refs/tags/v${G4_VERSION}.tar.gz
RUN tar -xf v${G4_VERSION}.tar.gz
RUN mv geant4-${G4_VERSION} geant4-source
RUN mkdir -p geant4-build geant4-install
WORKDIR /opt/geant4-build
RUN cmake ../geant4-source  \
           -DCMAKE_INSTALL_PREFIX=/usr/local \
           -DGEANT4_BUILD_MULTITHREADED=ON \
           -DGEANT4_USE_GDML=ON \
           -DGEANT4_USE_SYSTEM_EXPAT=ON \
           -DGEANT4_USE_SYSTEM_XERCESC=ON \
           -DGEANT4_INSTALL_DATA=ON \
           -DGEANT4_INSTALL_EXAMPLES=OFF \
           -DGEANT4_USE_OPENGL_X11=OFF \
           -DGEANT4_USE_QT=ON \
           -DGEANT4_USE_XM=OFF \
           -DGEANT4_USE_RAYTRACER_X11=OFF
RUN make -j$(nproc)
RUN make install
RUN apt-get clean

