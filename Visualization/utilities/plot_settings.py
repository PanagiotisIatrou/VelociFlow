import json


class PlotSettings:
    def __init__(self):
        # Required fields
        self.state = None
        self.filename = None
        self.color_map = None

        # Optional fields
        self.blur = False
        self.real_time = False
        self.only_last_frame = False
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
            self.color_map = data["colorMap"]

            # Optional fields
            if "blur" in data:
                self.blur = data["blur"]
            if "realTime" in data:
                self.real_time = data["realTime"]
            if "onlyLastFrame" in data:
                self.only_last_frame = data["onlyLastFrame"]
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
            if self.state == "unsteady" and ((self.fps is None and not self.real_time) or (self.fps is not None and self.real_time)):
                raise Exception("You must ether specify the frames per second or enable real_time (not both")
