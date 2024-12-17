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
        self.velocity_timesteps_u = []
        self.velocity_timesteps_v = []
        self.pressure_timesteps = []

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
                    elif line.startswith("grid_size"):
                        line = f.readline()
                        grid_size_x, grid_size_y = line.split(",")
                        self.grid_size_x = int(grid_size_x)
                        self.grid_size_y = int(grid_size_y)
                    elif line.startswith("domain_size"):
                        line = f.readline()
                        domain_size_x, domain_size_y = line.split(",")
                        self.domain_size_x = float(domain_size_x)
                        self.domain_size_y = float(domain_size_y)
                    elif line.startswith("dt"):
                        line = f.readline()
                        self.dt = float(line)
                    elif line.startswith("execution_time"):
                        line = f.readline()
                        self.execution_time = float(line)
                    elif line.startswith("velocity_u"):
                        self.timesteps += 1
                        velocity_u = np.zeros((self.grid_size_x, self.grid_size_y))
                        for i in range(self.grid_size_x * self.grid_size_y):
                            line = f.readline()
                            i, j, value = line.split(",")
                            if value.strip() == '-':
                                velocity_u[int(i), int(j)] = math.nan
                            else:
                                velocity_u[int(i), int(j)] = float(value.strip())
                        self.velocity_timesteps_u.append(np.array(velocity_u))
                        progress_bar.update(1)
                    elif line.startswith("velocity_v"):
                        velocity_v = np.zeros((self.grid_size_x, self.grid_size_y))
                        for i in range(self.grid_size_x * self.grid_size_y):
                            line = f.readline()
                            i, j, value = line.split(",")
                            if value.strip() == '-':
                                velocity_v[int(i), int(j)] = math.nan
                            else:
                                velocity_v[int(i), int(j)] = float(value.strip())
                        self.velocity_timesteps_v.append(np.array(velocity_v))
                    elif line.startswith("pressure"):
                        pressure = np.zeros((self.grid_size_x, self.grid_size_y))
                        for i in range(self.grid_size_x * self.grid_size_y):
                            line = f.readline()
                            i, j, value = line.split(",")
                            if value.strip() == '-':
                                pressure[int(i), int(j)] = math.nan
                            else:
                                pressure[int(i), int(j)] = float(value.strip())
                        self.pressure_timesteps.append(np.array(pressure))

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
