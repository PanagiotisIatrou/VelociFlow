from copy import copy

import matplotlib
import numpy as np
from matplotlib import pyplot as plt
from abc import ABC, abstractmethod

from Visualization.utilities.utilities import normalize_vectors


class Plotter(ABC):
    def __init__(self, data, settings):
        self.data = data
        self.settings = settings

        self.x = np.linspace(self.data.dx / 2, self.data.domain_size_x - self.data.dx / 2, self.data.grid_size_x)
        self.y = np.linspace(self.data.dy / 2, self.data.domain_size_y - self.data.dy / 2, self.data.grid_size_y)

    def create_plot(self):
        plt.gca().set_aspect('equal', adjustable='box')
        plt.title("Velocity field")
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
            clim=(0.0, np.nanmax(field)),
        )
        plt.colorbar(label="Magnitude")

        return color_mesh

    def update_color_mesh(self, color_mesh, field):
        color_mesh.set_array(np.array(field).T.ravel())
        color_mesh.set_clim(vmin=0.0, vmax=np.nanmax(field))

    def create_quiver(self, velocity_u, velocity_v):
        # Normalize the quiver if specified
        if self.settings.normalize_quiver:
            quiver_velocity_u, quiver_velocity_v = normalize_vectors(velocity_u, velocity_v, 6.5)
        else:
            quiver_velocity_u, quiver_velocity_v = velocity_u, velocity_v

        # Calculate the quiver density based on the smallest side of the domain
        smallest_side = min(self.data.grid_size_x, self.data.grid_size_y)
        quiver_density = (15.0 / smallest_side) * self.settings.quiver_density_factor
        step = int(1 / quiver_density)

        # Plot quiver (skip some points for better visualization according to the density)
        quiver = plt.quiver(
            self.x[::step],
            self.y[::step],
            quiver_velocity_u.T[::step, ::step],
            quiver_velocity_v.T[::step, ::step],
            scale=175,
            color=self.settings.quiver_color
        )

        return quiver

    def update_quiver(self, quiver, velocity_u, velocity_v):
        # Normalize the quiver if specified
        if self.settings.normalize_quiver:
            quiver_velocity_u, quiver_velocity_v = normalize_vectors(velocity_u, velocity_v, 6.5)
        else:
            quiver_velocity_u, quiver_velocity_v = velocity_u, velocity_v

        # Calculate the quiver density based on the smallest side of the domain
        smallest_side = min(self.data.grid_size_x, self.data.grid_size_y)
        quiver_density = (15.0 / smallest_side) * self.settings.quiver_density_factor
        step = int(1 / quiver_density)

        # Plot quiver (skip some points for better visualization according to the density)
        quiver.set_UVC(
            quiver_velocity_u.T[::step, ::step],
            quiver_velocity_v.T[::step, ::step]
        )

    def create_streamlines(self, velocity_u, velocity_v):
        # Calculate the streamplot density based on the smallest side of the domain
        smallest_side = min(self.data.grid_size_x, self.data.grid_size_y)
        streamplot_density = self.settings.streamline_density_factor * smallest_side / 50.0

        # Plot streamlines
        streamplot = plt.streamplot(
            self.x,
            self.y,
            velocity_u.T,
            velocity_v.T,
            color=self.settings.streamline_color,
            density=streamplot_density
        )

        return streamplot

    def update_streamlines(self, streamplot, velocity_u, velocity_v):
        ax = plt.gca()
        for obj in ax.collections:
            if isinstance(obj, matplotlib.collections.LineCollection):
                obj.remove()
                break
        for patch in ax.patches:
            patch.remove()

        # Create the new streamplot
        return self.create_streamlines(velocity_u, velocity_v)
