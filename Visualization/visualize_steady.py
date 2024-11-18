import math
import os
import json
import numpy as np
import matplotlib.pyplot as plt

# Load settings
settings_file = os.path.join(os.path.dirname(__file__), "settings.json")
with open(settings_file, "r") as f:
    data = json.load(f)

    filename = data["filename"]
    blur = data["blur"]
    show_quiver = data["showQuiver"]
    show_streamlines = data["showStreamlines"]
    streamline_density_factor = data["streamlineDensityFactor"]
    streamline_color = data["streamlineColor"]

def import_data(file):
    with open(file, 'r') as f:
        # Read line
        while True:
            line = f.readline()
            if not line:
                break
            elif line.startswith("grid_size"):
                line = f.readline()
                N, M = line.split(",")
                N = int(N)
                M = int(M)
            elif line.startswith("domain_size"):
                line = f.readline()
                domain_size_x, domain_size_y = line.split(",")
                domain_size_x = float(domain_size_x)
                domain_size_y = float(domain_size_y)
            elif line.startswith("execution_time"):
                line = f.readline()
                execution_time = float(line)
            elif line.startswith("velocity_u"):
                velocity_u = np.zeros((N, M))
                for i in range(N * M):
                    line = f.readline()
                    i, j, value = line.split(",")
                    if value.strip() == '-':
                        velocity_u[int(i), int(j)] = math.nan
                    else:
                        velocity_u[int(i), int(j)] = float(value.strip())
            elif line.startswith("velocity_v"):
                velocity_v = np.zeros((N, M))
                for i in range(N * M):
                    line = f.readline()
                    i, j, value = line.split(",")
                    if value.strip() == '-':
                        velocity_v[int(i), int(j)] = math.nan
                    else:
                        velocity_v[int(i), int(j)] = float(value.strip())
            elif line.startswith("pressure"):
                pressure = np.zeros((N, M))
                for i in range(N * M):
                    line = f.readline()
                    i, j, value = line.split(",")
                    if value.strip() == '-':
                        pressure[int(i), int(j)] = math.nan
                    else:
                        pressure[int(i), int(j)] = float(value.strip())

    return N, M, domain_size_x, domain_size_y, execution_time, velocity_u, velocity_v, pressure

# Import the simulation data
file = os.path.join(os.path.dirname(__file__), f"../Results/Steady/{filename}")
N, M, domain_size_x, domain_size_y, execution_time, velocity_u, velocity_v, pressure = import_data(file)
dx = domain_size_x / N
dy = domain_size_y / M
x = np.linspace(dx / 2, domain_size_x - dx / 2, N)
y = np.linspace(dy / 2, domain_size_y - dy / 2, M)

# Calculate the streamplot density
smallest_side = min(N, M)
streamplot_density = streamline_density_factor * smallest_side / 50.0

# Print information about the simulation
print("Domain size: ", (domain_size_x, domain_size_y))
print("Grid size: ", (N, M))
print("Execution time: ", execution_time, "s")

# Calculate the velocity magnitude
u_velocity = np.array(velocity_u)
v_velocity = np.array(velocity_v)
velocity = np.sqrt(u_velocity ** 2 + v_velocity ** 2)

def normalize_vectors(u, v):
    magnitude = np.sqrt(u ** 2 + v ** 2)
    u_normalized = np.divide(u, magnitude, where=magnitude != 0)
    v_normalized = np.divide(v, magnitude, where=magnitude != 0)
    return u_normalized, v_normalized

# Plot velocity
fig, ax = plt.subplots()
ax.set_aspect('equal')
cmap = plt.get_cmap('jet')
cmap.set_bad(color='black')
c1 = ax.pcolormesh(x, y, np.array(velocity).T, cmap=cmap, shading="gouraud" if blur else "auto")
c1.set_clim(vmin=0.0, vmax=np.nanmax(velocity))
plt.colorbar(c1, ax=ax, label="Magnitude")
ax.set_title("Velocity")

# Plot quiver
if show_quiver:
    u_velocity, v_velocity = normalize_vectors(u_velocity, v_velocity)
    ax.quiver(x, y, u_velocity.T, v_velocity.T, scale=35, color='black')

# Plot streamlines
if show_streamlines:
    ax.streamplot(x, y, u_velocity.T, v_velocity.T, color=streamline_color, density=streamplot_density)

# Leave no margin (caused from the streamplot
plt.margins(0)

# Open in maximized
mng = plt.get_current_fig_manager()
mng.window.state('zoomed')
plt.show()
