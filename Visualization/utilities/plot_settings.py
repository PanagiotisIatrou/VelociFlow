import json

from matplotlib import pyplot as plt

from Visualization.plotters.plotter import VectorFields, ScalarFields


class PlotSettings:
    def __init__(self):
        # Required fields
        self.state = None
        self.filename = None
        self.color_map = None
        self.field = None

        # Optional fields
        self.min_value = None
        self.max_value = None
        self.blur = False
        self.real_time = False
        self.only_last_frame = False
        self.only_specific_frame = None
        self.fps = None
        self.show_quiver = False
        self.quiver_density_factor = 1.0
        self.quiver_color = "black"
        self.normalize_quiver = None
        self.show_streamlines = False
        self.streamline_density_factor = 1.0
        self.streamline_color = "black"

    def import_file(self, file):
        # Clear the data
        self.__init__()

        # Open the file
        with open(file, "r") as f:
            data = json.load(f)

            # Required fields
            self.state = data["state"]
            self.filename = data["filename"]
            self.field = data["field"]
            self.color_map = data["colorMap"]
            if self.color_map not in plt.colormaps():
                raise Exception("Invalid color map")

            if self.field == "velocity_magnitude":
                self.field = VectorFields.VELOCITY_MAGNITUDE
            elif self.field == "velocity_x":
                self.field = ScalarFields.VELOCITY_X
            elif self.field == "velocity_y":
                self.field = ScalarFields.VELOCITY_Y
            elif self.field == "pressure":
                self.field = ScalarFields.PRESSURE
            elif self.field == "dye":
                self.field = ScalarFields.DYE
            elif self.field == "phi":
                self.field = ScalarFields.PHI
            elif self.field == "vorticity":
                self.field = ScalarFields.VORTICITY
            else:
                raise Exception("Invalid field")

            # Optional fields
            if "minValue" in data:
                self.min_value = data["minValue"]
            if "maxValue" in data:
                self.max_value = data["maxValue"]
            if "blur" in data:
                self.blur = data["blur"]
            if "realTime" in data:
                self.real_time = data["realTime"]
            if "onlyLastFrame" in data:
                self.only_last_frame = data["onlyLastFrame"]
            if "onlySpecificFrame" in data:
                self.only_specific_frame = data["onlySpecificFrame"]
            if "fps" in data:
                self.fps = data["fps"]
            if "showQuiver" in data:
                self.show_quiver = data["showQuiver"]
            if "quiverDensityFactor" in data:
                self.quiver_density_factor = data["quiverDensityFactor"]
            if "quiverColor" in data:
                self.quiver_color = data["quiverColor"]
            if "normalizeQuiver" in data:
                self.normalize_quiver = data["normalizeQuiver"]
            if "showStreamlines" in data:
                self.show_streamlines = data["showStreamlines"]
            if "streamlineDensityFactor" in data:
                self.streamline_density_factor = data["streamlineDensityFactor"]
            if "streamlineColor" in data:
                self.streamline_color = data["streamlineColor"]

            # Constraints

            if (self.fps is None and not self.real_time) or (self.fps is not None and self.real_time):
                raise Exception("You must ether specify the frames per second or enable real_time (not both")

            if self.only_last_frame and self.only_specific_frame is not None:
                raise Exception("You must either specify onlyLastFrame or onlySpecifyFrame (not both)")

