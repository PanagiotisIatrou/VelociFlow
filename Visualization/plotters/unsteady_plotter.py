import sys

import numpy as np
from matplotlib import pyplot as plt, animation
from tqdm import tqdm

from Visualization.plotters.plotter import Plotter
from Visualization.utilities.utilities import get_vector_field_magnitudes


class UnsteadyPlotter(Plotter):
    def __init__(self, data, settings):
        super().__init__(data, settings)

    def save_dye(self, filename="unsteady.mp4"):
        # Initialize the plot
        self.create_plot()

        # Find the min and max dye (from all the timesteps)
        # all_dyes = np.array(self.data.dye_timesteps)
        # self.set_min_max_values(all_dyes)
        self.min_value = 0.0
        self.max_value = 1.0

        # Add the color map and the color bar
        dye = np.array(self.data.dye_timesteps[0])
        color_mesh = self.create_color_mesh(dye)

        def animate(k):
            dye = np.array(self.data.dye_timesteps[k])

            # Update the color mesh
            self.update_color_mesh(color_mesh, dye)

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
        with tqdm(total=self.data.timesteps, bar_format="Making animation {l_bar}{bar:10}| Elapsed: {elapsed}") as progress_bar:
            def progress_callback(i, n):
                progress_bar.update(1)
            anim.save(filename, fps=animation_fps, progress_callback=progress_callback)
        plt.close()

    def save_velocity(self, filename="unsteady.mp4"):
        # Initialize the plot
        self.create_plot()

        # Find the min and max velocity (from all the timesteps)
        all_velocities = get_vector_field_magnitudes(np.array(self.data.velocity_timesteps_u), np.array(self.data.velocity_timesteps_v))
        self.set_min_max_values(all_velocities)

        # Calculate the velocity magnitude
        velocity_u = np.array(self.data.velocity_timesteps_u[0])
        velocity_v = np.array(self.data.velocity_timesteps_v[0])
        velocity = get_vector_field_magnitudes(velocity_u, velocity_v)

        # Add the color map and the color bar
        color_mesh = self.create_color_mesh(velocity)

        # Plot quiver
        if self.settings.show_quiver:
            quiver = self.create_quiver(velocity_u, velocity_v)

        # Plot the streamlines
        if self.settings.show_streamlines:
            streamplot = self.create_streamlines(velocity_u, velocity_v)

        def animate(k):
            # Calculate the velocity magnitude
            velocity_u = np.array(self.data.velocity_timesteps_u[k])
            velocity_v = np.array(self.data.velocity_timesteps_v[k])
            velocity = get_vector_field_magnitudes(velocity_u, velocity_v)

            # Update the color mesh
            self.update_color_mesh(color_mesh, velocity)

            # Update the quiver
            if self.settings.show_quiver:
                self.update_quiver(quiver, velocity_u, velocity_v)

            # Update the streamlines
            if self.settings.show_streamlines:
                self.update_streamlines(velocity_u, velocity_v)

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
        with tqdm(total=self.data.timesteps, bar_format="Making animation {l_bar}{bar:10}| Elapsed: {elapsed}") as progress_bar:
            def progress_callback(i, n):
                progress_bar.update(1)
            anim.save(filename, fps=animation_fps, progress_callback=progress_callback)
        plt.close()
