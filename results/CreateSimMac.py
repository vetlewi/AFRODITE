import datetime
import os
import platform
import shlex
import multiprocessing
from subprocess import Popen, PIPE, STDOUT
from pathlib import Path

import numpy as np


def MakeHeader(filename, num_energies):
    return f"""#############################################################################
# File: {filename}
# Automatically created at {datetime.datetime.now().strftime('%c')}
# Number of runs in macro: {num_energies}
# Please do not edit unless you know what you do!
#############################################################################"""


def Common():
    body = "## Particle type, position, energy...\n"
    body += "/gps/number 1\n"
    body += "/gps/particle gamma\n"
    body += "/gps/ang/type iso\n"
    body += "/gps/ang/mintheta 0 degree\n"
    body += "/gps/ang/maxtheta 180 degree\n"
    body += "/run/printProgress 10000"
    return body


def Run_body(energy, num_events, output_base_name):
    output_file = f"{output_base_name+f'{int(energy):d}keV'}.root"
    body = "#######################################################\n"
    body += f"#\tEnergy:      {energy} keV\n"
    body += f"#\tOutput file: {output_file}\n"
    body += "#######################################################\n"
    body += "\n"
    body += f"/FilePath/output {output_file}\n"
    body += f"/gps/energy {energy:.3f} keV\n"
    body += f"/run/beamOn {int(num_events):d}\n"
    return body


def MakeMacFile(energies, num_events, filename, output_base):
    file = MakeHeader(filename, len(energies)) + "\n\n\n"
    file += Common() + "\n\n"
    for energy in energies:
        file += Run_body(energy, num_events, output_base) + "\n\n"
    return file


def RunGeant4(kwargs):
    bin_dir, workdir, macro_name, log_name = kwargs
    env = os.environ
    if platform.system() == 'Darwin':
        env['DYLD_LIBRARY_PATH'] = '/Users/vetlewi/prog/lib'
    args = shlex.split(f"{bin_dir} -m {macro_name}")
    process = Popen(args, stdout=PIPE, stderr=STDOUT, env=env, cwd=workdir)
    with open(workdir/log_name, "w") as logfile:
        for line in iter(process.stdout.readline, b''):
            logfile.write(line.decode('utf-8'))
    exitcode = process.wait()
    return exitcode


class Geant4_RunManager:

    def __init__(self, energies, num_events,
                 num_workers=multiprocessing.cpu_count(),
                 output_base="run",
                 bin_dir=os.getcwd()+"/../build/AFRODITE"):
        """ Initialize the RunManager.
        Args:
            energies (list or array): List of energies that should be simulated
            num_events (int): Total number of events per energy
            n_workers: Number of parallel workers to spawn
        """

        self.energies = energies
        self.num_events = num_events
        self.num_workers = num_workers
        self.output_base = output_base
        self.bin_dir = bin_dir
        self.path = Path(f"run-{datetime.datetime.now().strftime('%d-%m-%Y-%H-%M-%S')}")
        self.macros = [f"worker_{i}.mac" for i in range(self.num_workers)]
        self.logs = [f"worker_{i}.log" for i in range(self.num_workers)]

        # create the output folder if not existing
        self.path.mkdir(exist_ok=True, parents=True)

    def generate_all(self):
        list_energies = np.array_split(self.energies, self.num_workers)
        for worker_no, (energies, macro) in enumerate(zip(list_energies,
                                                          self.macros)):
            macro_content = MakeMacFile(energies, self.num_events,
                                        macro, self.output_base)
            with open(self.path/macro, 'w') as macro_file:
                macro_file.write(macro_content)

    def run_one(self, macro):
        # We will now start the Geant4 process
        print(f"Running macro file: {macro}")
        env = os.environ
        env['DYLD_LIBRARY_PATH'] = '/Users/vetlewi/prog/lib'
        args = shlex.split(f"../build/AFRODITE -m {macro}")
        process = Popen(args, stdout=PIPE, stderr=STDOUT, env=env)
        with open(f"{macro}.log", "w") as logfile:
            for line in iter(process.stdout.readline, b''):
                logfile.write(line.decode('utf-8'))
        exitcode = process.wait()
        return exitcode

    def run_all(self):
        # Start a process pool
        print(f"Starting processing pool with {self.num_workers} workers")
        with multiprocessing.Pool(self.num_workers) as p:
            p.map(RunGeant4, [(self.bin_dir, self.path, m, l) for m, l in zip(self.macros, self.logs)])


if __name__ == '__main__':
    #rm = Geant4_RunManager(np.arange(50, 10050, 50), 10000, 1)
    energies = [10, 20, 30, 40]
    energies += list(np.arange(50, 10050, 50))
    rm = Geant4_RunManager(np.array(energies), 10000)
    rm.generate_all()
    rm.run_all()
