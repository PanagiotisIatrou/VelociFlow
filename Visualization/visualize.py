import os

from Visualization.utilities.plot_settings import PlotSettings
from Visualization.utilities.simulation_data import SimulationData
from Visualization.plotters.steady_plotter import SteadyPlotter
from Visualization.plotters.unsteady_plotter import UnsteadyPlotter

root_path = os.path.dirname(__file__)

# Load the settings
settings_file = os.path.join(root_path, "settings_steady.json")
# settings_file = os.path.join(root_path, "settings_unsteady.json")
plot_settings = PlotSettings()
plot_settings.import_file(settings_file)

# Import the data from the simulation
if plot_settings.transient:
    folder = "Unsteady"
else:
    folder = "Steady"
data_file = os.path.join(root_path, f"../Results/{folder}/{plot_settings.filename}")
simulation_data = SimulationData()
simulation_data.import_file(data_file)

if plot_settings.transient:
    plotter = UnsteadyPlotter(simulation_data, plot_settings)
    plotter.save_velocity()
else:
    plotter = SteadyPlotter(simulation_data, plot_settings)
    plotter.plot_and_save_velocity()
