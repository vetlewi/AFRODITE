import uproot as up
import numpy as np
import ompy as om
from typing import Iterable, Optional
from pathlib import Path
from tqdm import tqdm

def Histogram1D(values : Iterable[float], **kwargs) -> om.Vector:
    counts, bin_edges = np.histogram(values, **kwargs)
    return om.Vector(values=counts, E=[0.5*(bin_edges[i+1]+bin_edges[i]) for i in range(len(bin_edges)-1)])

def get_energy(path):
    """ Extract the energy from the filename."""
    name = Path(path).name
    energy = int(name[name.find("_")+1:name.find("keV")])
    return energy

def get_ocl_labr_events(filename):
    tree = up.open(f"{filename}:DataTreeSim")
    labr = []
    for name in [f'OCLLABR_Energy{i}' for i in range(6)]:
        labr.append(tree[name].array(library="np"))
    labr = np.concatenate(labr)
    return tree.num_entries, Histogram1D(labr[labr > 0], 
        range=[-0.5, get_energy(filename)+100.5], bins=get_energy(filename)+101)

if __name__ == '__main__':
    files = [str(p) for p in sorted(Path("/Volumes/PR271/afrodite6LaBr3").glob("energy_*.root"))]
    for file in tqdm(files, total=len(files)):
        num_counts, spect = get_ocl_labr_events(file)
        spect.save(f"mama_spectra/energy_ocl_{int(get_energy(file))}keV_{int(num_counts)}entries.m", _assert=False)