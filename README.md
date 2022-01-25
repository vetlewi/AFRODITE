
# AFRODITE [![MacOS](https://github.com/vetlewi/AFRODITE/actions/workflows/macos.yaml/badge.svg?branch=master)](https://github.com/vetlewi/AFRODITE/actions/workflows/macos.yaml) [![Ubuntu](https://github.com/vetlewi/AFRODITE/actions/workflows/ubuntu.yaml/badge.svg)](https://github.com/vetlewi/AFRODITE/actions/workflows/ubuntu.yaml) [![codecov](https://codecov.io/gh/vetlewi/AFRODITE/branch/master/graph/badge.svg?token=Mp6QwsKF7j)](https://codecov.io/gh/vetlewi/AFRODITE) [![DOI](https://zenodo.org/badge/348759776.svg)](https://zenodo.org/badge/latestdoi/348759776)
A GEANT4 simulation of the AFRODITE vault at iThemba LABS, composed of the AFRODITE array of HPGe clover detectors along with various ancillary detectors

This package can be used for realistic simulation of typical experimental setups. This can be useful
to determine important information about the setup such as the gamma energy response, etc.

Currently this package implements the following:
- CLOVER HPGe detectors (Eurogam type), manufactured by Canberra.
- Scattering chamber built for the AFRODITE frame
- Large volume LaBr3:Ce detectors (3.5x8")
- Fast timing LaBr3:Ce detectors (2x2")
- Silicon particle detectors of type S2 from Micron Semiconductor Ltd. (geometry is somewhat approximate)
- Aluminium shield and tunnel for the silicon detectors
- The AFRODITE frame (To be added)

## Installation

### Dependencies
This packages depends on the following packages:
- `Geant4`, for installation see [here](https://geant4-userdoc.web.cern.ch/UsersGuides/InstallationGuide/html/index.html)
- `CADMesh <= 1.1.0`, included in the build tree (no need for manual install)
- `tetgen`, also included in the build tree (no need for manual install)
- `assimp`, available in most package managers

### Build
1) Clone this project
    ```
    git clone https://github.com/vetlewi/AFRODITE
    ```
2) Configure using CMake
    ```
   cd AFRODITE
   cmake -DCMAKE_BUILD_TYPE=Release -H. -Bbuild
   ```
3) Build
    ```
   cd build
   make -j4
   ```
4) Run
    ```
   ./AFRODITE
   ```
   
## Limitation
Currently the setup hasn't 
