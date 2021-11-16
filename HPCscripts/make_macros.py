import numpy as np


def make_block(energy, count):
    """Formats the string to include
    the desired energy and number of events to simulate"""

    outname = f"energy_{energy}keV.root"

    base_string = [
        f"/gps/energy {energy} keV",
        f"/FilePath/output {outname}",
        f"/run/beamOn {count}",
    ]

    return base_string


def make_file(fname, strings):

    strng = [
        "/run/initialize",
        "/gps/position 0 0 0",
        "/gps/ang/type iso"
    ]

    strng += strings

    with open(fname, "w") as file:
        for cmd in strng:
            file.write(f"{cmd}\n")


if __name__ == "__main__":

    energy_grid = np.arange(50, 1e4, 10, dtype=int)
    nevents = np.linspace(6e5, 3e6, len(energy_grid), dtype=int)

    energy_grid = np.append(energy_grid, [int(1.2e4), int(1.5e4), int(2e4)])
    nevents = np.append(nevents, [int(3e6), int(3e6), int(3e6)])

    strings = []

    for i, (energy, nevent) in enumerate(zip(energy_grid, nevents)):
        # print(f"Simulating gridpoint {i}")
        strings += make_block(energy, nevent)

    make_file("grid_response.mac", strings)
