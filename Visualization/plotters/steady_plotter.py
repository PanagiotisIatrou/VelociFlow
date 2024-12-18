import numpy as np
from matplotlib import pyplot as plt

from Visualization.plotters.plotter import Plotter
from Visualization.utilities.utilities import get_vector_field_magnitudes


class SteadyPlotter(Plotter):
    def __init__(self, data, settings):
        super().__init__(data, settings)

        # Keep only the last timestep (in case of multiple timesteps)
        self.data.velocity_timesteps_u = [self.data.velocity_timesteps_u[-1]]
        self.data.velocity_timesteps_v = [self.data.velocity_timesteps_v[-1]]

    def __velocity(self):
        # Initialize the plot
        self.create_plot()

        # Calculate the velocity magnitude
        velocity_u = np.array(self.data.velocity_timesteps_u[0])
        velocity_v = np.array(self.data.velocity_timesteps_v[0])
        velocity = get_vector_field_magnitudes(velocity_u, velocity_v)
        self.set_min_max_values(velocity)

        # Add the color map and the color bar
        self.create_color_mesh(velocity)

        # Plot quiver
        if self.settings.show_quiver:
            self.create_quiver(velocity_u, velocity_v)

        # Plot the streamlines
        if self.settings.show_streamlines:
            self.create_streamlines(velocity_u, velocity_v)

    def plot_velocity(self):
        print("Plotting...")
        self.__velocity()
        plt.show()

    def save_velocity(self, filename="steady.png"):
        print("Saving...")
        self.__velocity()
        plt.savefig(filename)
        plt.close()

    def plot_and_save_velocity(self, filename="steady.png"):
        self.__velocity()
        print("Saving...")
        plt.savefig(filename)
        print("Plotting...")
        plt.show()
