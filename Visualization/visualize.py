import os

from Visualization.plotters.plotter import ScalarFields, VectorFields
from Visualization.utilities.plot_settings import PlotSettings
from Visualization.utilities.simulation_data import SimulationData
from Visualization.plotters.steady_plotter import SteadyPlotter
from Visualization.plotters.unsteady_plotter import UnsteadyPlotter

root_path = os.path.dirname(__file__)

# Load the settings
# settings_file = os.path.join(root_path, "settings_steady.json")
settings_file = os.path.join(root_path, "settings_unsteady.json")
plot_settings = PlotSettings()
plot_settings.import_file(settings_file)

# Import the data from the simulation
if plot_settings.state == "unsteady":
    folder = "Unsteady"
else:
    folder = "Steady"
data_file = os.path.join(root_path, f"../Results/{folder}/{plot_settings.filename}")
simulation_data = SimulationData()
simulation_data.import_file(data_file)

if plot_settings.state == "unsteady" and not plot_settings.only_last_frame and plot_settings.only_specific_frame is None:
    plotter = UnsteadyPlotter(simulation_data, plot_settings)
    plotter.save_field(plot_settings.field)
else:
    plotter = SteadyPlotter(simulation_data, plot_settings)
    plotter.plot_and_save_field(plot_settings.field)
