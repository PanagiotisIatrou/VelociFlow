import numpy as np
from matplotlib import pyplot as plt

from Visualization.plotters.plotter import Plotter, ScalarFields, VectorFields
from Visualization.utilities.utilities import get_vector_field_magnitudes


class SteadyPlotter(Plotter):
    def __init__(self, data, settings):
        super().__init__(data, settings)

    def __scalar_field(self, field):
        # Initialize the plot
        self.create_plot()

        # Find the min and max velocity (from all the timesteps)
        self.set_min_max_values(field)

        # Add the color map and the color bar
        self.create_color_mesh(field)

    def __vector_field(self, field1, field2):
        # Initialize the plot
        self.create_plot()

        # Calculate the velocity magnitude
        velocity = get_vector_field_magnitudes(field1, field2)
        self.set_min_max_values(velocity)

        # Add the color map and the color bar
        self.create_color_mesh(velocity)

        # Plot quiver
        if self.settings.show_quiver:
            self.create_quiver(field1, field2)

        # Plot the streamlines
        if self.settings.show_streamlines:
            self.create_streamlines(field1, field2)

    def __apply_scalar_field(self, field):
        if field == ScalarFields.VELOCITY_X:
            self.velocity_x = np.array(self.data.timesteps_velocity_x[-1])
            self.__scalar_field(self.velocity_x)
        elif field == ScalarFields.VELOCITY_Y:
            self.velocity_y = np.array(self.data.timesteps_velocity_y[-1])
            self.__scalar_field(self.velocity_y)
        elif field == ScalarFields.PRESSURE:
            self.pressure = np.array(self.data.pressure_timesteps[-1])
            self.__scalar_field(self.pressure)
        elif field == ScalarFields.DYE:
            self.dye = np.array(self.data.dye_timesteps[-1])
            self.__scalar_field(self.dye)
        elif field == ScalarFields.PHI:
            self.phi = np.array(self.data.phi_timesteps[-1])
            self.__scalar_field(self.phi)
        elif field == ScalarFields.VORTICITY:
            self.vorticity = np.gradient(self.data.timesteps_velocity_x[-1], axis=0) - np.gradient(
                self.data.timesteps_velocity_y[-1], axis=1)
            self.__scalar_field(self.vorticity)

    def __apply_vector_field(self, field):
        if field == VectorFields.VELOCITY_MAGNITUDE:
            self.velocity_x = np.array(self.data.timesteps_velocity_x[-1])
            self.velocity_y = np.array(self.data.timesteps_velocity_y[-1])
            self.__vector_field(self.velocity_x, self.velocity_y)

    def __apply_plot(self, field):
        if field in ScalarFields:
            self.__apply_scalar_field(field)
        elif field in VectorFields:
            self.__apply_vector_field(field)

    def save_field(self, field, filename="steady.png"):
        self.__apply_plot(field)
        print("Saving...")
        plt.savefig(filename)
        plt.close()

    def plot_and_save_field(self, field, filename="steady.png"):
        self.__apply_plot(field)
        print("Saving...")
        plt.savefig(filename)
        print("Plotting...")
        plt.show()
