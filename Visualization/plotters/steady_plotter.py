import numpy as np
from matplotlib import pyplot as plt

from Visualization.plotters.plotter import Plotter
from Visualization.utilities.utilities import get_vector_field_magnitudes


class SteadyPlotter(Plotter):
    def __init__(self, data, settings):
        super().__init__(data, settings)

    def velocity(self, show=True, save=False, filename="steady.png"):
        if not show and not save:
            raise Exception("You must either show the plot or save it (or both)")

        # Initialize the plot
        self.create_plot()

        # Calculate the velocity magnitude
        velocity_u = np.array(self.data.velocity_timesteps_u[0])
        velocity_v = np.array(self.data.velocity_timesteps_v[0])
        velocity = get_vector_field_magnitudes(velocity_u, velocity_v)

        # Add the color map and the color bar
        self.create_color_mesh(velocity)

        # Plot quiver
        if self.settings.show_quiver:
            self.create_quiver(velocity_u, velocity_v)

        # Plot the streamlines
        if self.settings.show_streamlines:
            self.create_streamlines(velocity_u, velocity_v)

        if save:
            plt.savefig(filename)

        if show:
            plt.show()

        if not show and save:
            plt.close()
