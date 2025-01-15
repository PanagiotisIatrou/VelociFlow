import math
import numpy as np
from tqdm import tqdm


class SimulationData:
    def __init__(self):
        self.grid_size_x = None
        self.grid_size_y = None
        self.domain_size_x = None
        self.domain_size_y = None
        self.dx = None
        self.dy = None
        self.dt = None
        self.timesteps = 0
        self.execution_time = None
        self.density = None
        self.viscosity = None
        self.timesteps_velocity_x = []
        self.timesteps_velocity_y = []
        self.pressure_timesteps = []
        self.dye_timesteps = []

    def import_file(self, file):
        # Clear the data
        self.__init__()

        # Open the file
        print("Loading simulation data...")
        with tqdm(total=1, bar_format="Timesteps loaded: {n} | Elapsed: {elapsed}", ) as progress_bar:
            with open(file, 'r') as f:
                # Read line by line
                while True:
                    line = f.readline()
                    if not line:
                        break
                    elif not line.startswith("["):
                        continue

                    header_value = line[1:-2]
                    if header_value == "grid_size":
                        line = f.readline()
                        grid_size_x, grid_size_y = line.split(",")
                        self.grid_size_x = int(grid_size_x)
                        self.grid_size_y = int(grid_size_y)
                    elif header_value == "domain_size":
                        line = f.readline()
                        domain_size_x, domain_size_y = line.split(",")
                        self.domain_size_x = float(domain_size_x)
                        self.domain_size_y = float(domain_size_y)
                    elif header_value == "dt":
                        line = f.readline()
                        self.dt = float(line)
                    elif header_value == "execution_time":
                        line = f.readline()
                        self.execution_time = float(line)
                    elif header_value == "density":
                        line = f.readline()
                        self.density = float(line)
                    elif header_value == "viscosity":
                        line = f.readline()
                        self.viscosity = float(line)
                    elif header_value == "velocity_x":
                        self.timesteps += 1
                        velocity_x = np.zeros((self.grid_size_x, self.grid_size_y))
                        for i in range(self.grid_size_x * self.grid_size_y):
                            line = f.readline()
                            i, j, value = line.split(",")
                            if value.strip() == '-':
                                velocity_x[int(i), int(j)] = math.nan
                            else:
                                velocity_x[int(i), int(j)] = float(value.strip())
                        self.timesteps_velocity_x.append(np.array(velocity_x))
                        progress_bar.update(1)
                    elif header_value == "velocity_y":
                        velocity_y = np.zeros((self.grid_size_x, self.grid_size_y))
                        for i in range(self.grid_size_x * self.grid_size_y):
                            line = f.readline()
                            i, j, value = line.split(",")
                            if value.strip() == '-':
                                velocity_y[int(i), int(j)] = math.nan
                            else:
                                velocity_y[int(i), int(j)] = float(value.strip())
                        self.timesteps_velocity_y.append(np.array(velocity_y))
                    elif header_value == "pressure":
                        pressure = np.zeros((self.grid_size_x, self.grid_size_y))
                        for i in range(self.grid_size_x * self.grid_size_y):
                            line = f.readline()
                            i, j, value = line.split(",")
                            if value.strip() == '-':
                                pressure[int(i), int(j)] = math.nan
                            else:
                                pressure[int(i), int(j)] = float(value.strip())
                        self.pressure_timesteps.append(np.array(pressure))
                    elif header_value == "dye":
                        dye = np.zeros((self.grid_size_x, self.grid_size_y))
                        for i in range(self.grid_size_x * self.grid_size_y):
                            line = f.readline()
                            i, j, value = line.split(",")
                            if value.strip() == '-':
                                dye[int(i), int(j)] = math.nan
                            else:
                                dye[int(i), int(j)] = float(value.strip())
                        self.dye_timesteps.append(np.array(dye))

            self.dx = self.domain_size_x / self.grid_size_x
            self.dy = self.domain_size_y / self.grid_size_y

    def print_info(self):
        print("Domain size:", (self.domain_size_x, self.domain_size_y))
        print("Grid size:", (self.grid_size_x, self.grid_size_y))
        print("dx:", self.dx)
        print("dy:", self.dy)
        if self.dt is not None:
            print("dt:", self.dt)
        if self.timesteps is not None:
            print("Timesteps:", self.timesteps)
        if self.dt is not None and self.timesteps is not None:
            print("Simulation time:", self.timesteps * self.dt, "s")
        print("Execution time:", self.execution_time, "s")
        if self.density is not None:
            print("Density:", self.density)
        if self.viscosity is not None:
            print("Viscosity:", self.viscosity)
