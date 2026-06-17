import sys
import pathlib
import argparse
import numpy as np
import pandas as pd

def main(infile, outfile):

    df = pd.read_csv(infile)
    normed_eff_tot = df.Eff_tot
    
    print(df)


if __name__ == "__main__":
    parser = argparse.ArgumentParser("CSVtoDAT", description="A small utility program to convert resp.csv files to resp.dat files.")
    parser.add_argument("input", nargs=1, type=pathlib.Path)#, required=True)
    parser.add_argument("output", nargs=1, type=pathlib.Path)#, required=True)

    args = parser.parse_args()

    infile = args.input[0]
    outfile = args.output[0]
    main(infile, outfile)