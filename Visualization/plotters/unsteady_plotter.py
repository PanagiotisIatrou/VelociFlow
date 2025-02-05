import numpy as np
from matplotlib import pyplot as plt, animation
from tqdm import tqdm

from Visualization.plotters.plotter import Plotter, ScalarFields, VectorFields
from Visualization.utilities.utilities import get_vector_field_magnitudes


class UnsteadyPlotter(Plotter):
    def __init__(self, data, settings):
        super().__init__(data, settings)

    def __save_vector_field(self, field):
        if field == VectorFields.VELOCITY_MAGNITUDE:
            field1 = np.array(self.data.timesteps_velocity_x)
            field2 = np.array(self.data.timesteps_velocity_y)

        # Initialize the plot
        self.create_plot()

        # Find the min and max of the magnitudes of the vector field (from all the timesteps)
        vector_field = get_vector_field_magnitudes(field1, field2)
        self.set_min_max_values(vector_field)

        # Add the color map and the color bar
        color_mesh = self.create_color_mesh(vector_field[0])

        # Plot quiver
        if self.settings.show_quiver:
            quiver = self.create_quiver(field1[0], field2[0])

        # Plot the streamlines
        if self.settings.show_streamlines:
            streamplot = self.create_streamlines(field1[0], field2[0])

        def animate(k):
            # Calculate the velocity magnitude
            field1_timestep = field1[k]
            field2_timestep = field2[k]
            vector_field_timestep = vector_field[k]

            # Update the color mesh
            self.update_color_mesh(color_mesh, vector_field_timestep)

            # Update the quiver
            if self.settings.show_quiver:
                self.update_quiver(quiver, field1_timestep, field2_timestep)

            # Update the streamlines
            if self.settings.show_streamlines:
                self.update_streamlines(field1_timestep, field2_timestep)

            # Update the time
            decimals_count = str(self.data.dt)[::-1].find('.')
            plt.suptitle(f"Time: {k * self.data.dt:.{decimals_count}f}")

        # Calculate the frame rate
        anim = animation.FuncAnimation(plt.gcf(), animate, frames=self.data.timesteps, repeat=False)
        animation_fps = None
        if self.settings.real_time:
            animation_fps = 1.0 / self.data.dt
        else:
            animation_fps = self.settings.fps

        # Save the animation
        print("Saving the animation...")
        with tqdm(total=self.data.timesteps,
                  bar_format="Making animation {l_bar}{bar:10}| Elapsed: {elapsed}") as progress_bar:
            def progress_callback(i, n):
                progress_bar.update(1)

            anim.save("unsteady.mp4", fps=animation_fps, progress_callback=progress_callback)
        plt.close()

    def __save_scalar_field(self, field, filename="unsteady.mp4"):
        if field == ScalarFields.VELOCITY_X:
            scalar_field = np.array(self.data.timesteps_velocity_x)
        elif field == ScalarFields.VELOCITY_Y:
            scalar_field = np.array(self.data.timesteps_velocity_y)
        elif field == ScalarFields.PRESSURE:
            scalar_field = np.array(self.data.pressure_timesteps)
        elif field == ScalarFields.DYE:
            scalar_field = np.array(self.data.dye_timesteps)
        elif field == ScalarFields.PHI:
            scalar_field = np.array(self.data.phi_timesteps)
        elif field == ScalarFields.VORTICITY:
            scalar_field = np.gradient(self.data.timesteps_velocity_x, axis=0) - np.gradient(
                self.data.timesteps_velocity_y, axis=1)

        # Initialize the plot
        self.create_plot()

        # Find the min and max scalar field (from all the timesteps)
        self.set_min_max_values(scalar_field)

        # Add the color map and the color bar
        color_mesh = self.create_color_mesh(scalar_field[0])

        def animate(k):
            scalar_field_timestep = scalar_field[k]

            # Update the color mesh
            self.update_color_mesh(color_mesh, scalar_field_timestep)

            # Update the time
            decimals_count = str(self.data.dt)[::-1].find('.')
            plt.suptitle(f"Time: {k * self.data.dt:.{decimals_count}f}")

        # Calculate the frame rate
        anim = animation.FuncAnimation(plt.gcf(), animate, frames=self.data.timesteps, repeat=False)
        animation_fps = None
        if self.settings.real_time:
            animation_fps = 1.0 / self.data.dt
        else:
            animation_fps = self.settings.fps

        # Save the animation
        print("Saving the animation...")
        with tqdm(total=self.data.timesteps,
                  bar_format="Making animation {l_bar}{bar:10}| Elapsed: {elapsed}") as progress_bar:
            def progress_callback(i, n):
                progress_bar.update(1)

            anim.save(filename, fps=animation_fps, progress_callback=progress_callback)
        plt.close()

    def save_field(self, field):
        if field in ScalarFields:
            self.__save_scalar_field(field)
        elif field in VectorFields:
            self.__save_vector_field(field)
