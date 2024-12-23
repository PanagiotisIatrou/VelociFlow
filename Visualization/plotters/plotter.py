from copy import copy

import matplotlib
import numpy as np
from matplotlib import pyplot as plt
from abc import ABC, abstractmethod
from enum import Enum

from Visualization.utilities.utilities import normalize_vectors

class ScalarFields(Enum):
    VELOCITY_U = 1
    VELOCITY_V = 2
    PRESSURE = 4
    DYE = 3

class VectorFields(Enum):
    VELOCITY_MAGNITUDE = 1

class Plotter(ABC):
    def __init__(self, data, settings):
        self.data = data
        self.settings = settings

        self.x = np.linspace(self.data.dx / 2, self.data.domain_size_x - self.data.dx / 2, self.data.grid_size_x)
        self.y = np.linspace(self.data.dy / 2, self.data.domain_size_y - self.data.dy / 2, self.data.grid_size_y)

    def create_plot(self):
        plt.figure(figsize=(16, 9))
        plt.gca().set_aspect('equal', adjustable='box')
        plt.title("??? field")
        plt.xlabel("x")
        plt.ylabel("y")

    def create_color_mesh(self, field):
        color_map = copy(plt.colormaps.get_cmap(self.settings.color_map))
        color_map.set_bad(color='black')

        color_mesh = plt.pcolormesh(
            self.x,
            self.y,
            np.array(field).T,
            cmap=color_map,
            shading="gouraud" if self.settings.blur else "auto",
            clim=(self.min_value, self.max_value),
        )
        plt.colorbar(label="Magnitude")

        return color_mesh

    def update_color_mesh(self, color_mesh, field):
        color_mesh.set_array(np.array(field).T.ravel())

    def set_min_max_values(self, field):
        # Find the min and max velocity (from all the timesteps)
        self.min_value = np.nanmin(field)
        self.max_value = np.nanmax(field)

    def create_quiver(self, field1, field2):
        # Normalize the quiver if specified
        if self.settings.normalize_quiver:
            quiver_field1, quiver_field2 = normalize_vectors(field1, field2, 6.5)
        else:
            quiver_field1, quiver_field2 = field1, field2

        # Calculate the quiver density based on the smallest side of the domain
        smallest_side = min(self.data.grid_size_x, self.data.grid_size_y)
        quiver_density = (15.0 / smallest_side) * self.settings.quiver_density_factor
        step = int(1 / quiver_density)

        # Plot quiver (skip some points for better visualization according to the density)
        quiver = plt.quiver(
            self.x[::step],
            self.y[::step],
            quiver_field1.T[::step, ::step],
            quiver_field2.T[::step, ::step],
            scale=175,
            color=self.settings.quiver_color
        )

        return quiver

    def update_quiver(self, quiver, field1, field2):
        # Normalize the quiver if specified
        if self.settings.normalize_quiver:
            quiver_field1, quiver_field2 = normalize_vectors(field1, field2, 6.5)
        else:
            quiver_field1, quiver_field2 = field1, field2

        # Calculate the quiver density based on the smallest side of the domain
        smallest_side = min(self.data.grid_size_x, self.data.grid_size_y)
        quiver_density = (15.0 / smallest_side) * self.settings.quiver_density_factor
        step = int(1 / quiver_density)

        # Plot quiver (skip some points for better visualization according to the density)
        quiver.set_UVC(
            quiver_field1.T[::step, ::step],
            quiver_field2.T[::step, ::step]
        )

    def create_streamlines(self, field1, field2):
        # Calculate the streamplot density based on the smallest side of the domain
        smallest_side = min(self.data.grid_size_x, self.data.grid_size_y)
        streamplot_density = self.settings.streamline_density_factor * smallest_side / 50.0

        # Plot streamlines
        streamplot = plt.streamplot(
            self.x,
            self.y,
            field1.T,
            field2.T,
            color=self.settings.streamline_color,
            density=streamplot_density
        )

        return streamplot

    def update_streamlines(self, field1, field2):
        ax = plt.gca()
        for obj in ax.collections:
            if isinstance(obj, matplotlib.collections.LineCollection):
                obj.remove()
                break
        for patch in ax.patches:
            patch.remove()

        # Create the new streamplot
        return self.create_streamlines(field1, field2)
