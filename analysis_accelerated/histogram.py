"""
Histogram utillities

---

This file is part of the oclpy - a library for nuclear physics at the University of Oslo

Copyright (C) 2020 Vetle Wegner Ingeberg
Oslo Cyclotron Laboratory
vetlewi@fys.uio.no

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

"""

from __future__ import annotations
from typing import Optional, Iterable, Union, Any, Tuple, Dict
import numpy as np
from ompy.vector import Vector
from ompy.matrix import Matrix

def Histogram1D(values : Iterable[float], **kwargs) -> Vector:
    counts, bin_edges = np.histogram(values, **kwargs)
    return Vector(values=counts, E=[0.5*(bin_edges[i+1]+bin_edges[i]) for i in range(len(bin_edges)-1)])

def Histogram2D(x : Iterable[float],
                y : Iterable[float], **kwargs) -> Matrix:
    counts, xedges, yedges = np.histogram2d(x, y, **kwargs)
    return Matrix(values=counts.T,
                  Eg=np.array([0.5*(xedges[i+1]+xedges[i]) for i in range(len(xedges)-1)]),
                  Ex=np.array([0.5*(yedges[i+1]+yedges[i]) for i in range(len(yedges)-1)])) 