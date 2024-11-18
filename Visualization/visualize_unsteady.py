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
    normalize_quiver = data["normalizeQuiver"]
    show_streamlines = data["showStreamlines"]
    streamline_density_factor = data["streamlineDensityFactor"]
    streamline_color = data["streamlineColor"]

def import_data(file):
    velocity_timesteps_u = []
    velocity_timesteps_v = []
    pressure_timesteps = []
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
            elif line.startswith("dt"):
                line = f.readline()
                dt = float(line)
            elif line.startswith("timesteps"):
                line = f.readline()
                timesteps = int(line)
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
                velocity_timesteps_u.append(np.array(velocity_u))
            elif line.startswith("velocity_v"):
                velocity_v = np.zeros((N, M))
                for i in range(N * M):
                    line = f.readline()
                    i, j, value = line.split(",")
                    if value.strip() == '-':
                        velocity_v[int(i), int(j)] = math.nan
                    else:
                        velocity_v[int(i), int(j)] = float(value.strip())
                velocity_timesteps_v.append(np.array(velocity_v))
            elif line.startswith("pressure"):
                pressure = np.zeros((N, M))
                for i in range(N * M):
                    line = f.readline()
                    i, j, value = line.split(",")
                    if value.strip() == '-':
                        pressure[int(i), int(j)] = math.nan
                    else:
                        pressure[int(i), int(j)] = float(value.strip())
                pressure_timesteps.append(np.array(pressure))

    return N, M, domain_size_x, domain_size_y, dt, timesteps, execution_time, velocity_timesteps_u, velocity_timesteps_v, pressure_timesteps

# Import the simulation data
file = os.path.join(os.path.dirname(__file__), f"../Results/Unsteady/{filename}")
N, M, domain_size_x, domain_size_y, dt, timesteps, execution_time, velocity_timesteps_u, velocity_timesteps_v, pressure_timesteps = import_data(file)
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
print("dx: ", dx)
print("dy: ", dy)
print("dt: ", dt)
print("Timesteps: ", timesteps)
print("Simulation time: ", timesteps * dt)
print("Execution time: ", execution_time, "s")

# Calculate the velocity magnitude
velocity_timesteps = []
for k in range(timesteps):
    velocity_timesteps.append(np.sqrt(velocity_timesteps_u[k] ** 2 + velocity_timesteps_v[k] ** 2))

def normalize_vectors(u, v):
    magnitude = np.sqrt(u ** 2 + v ** 2)
    u_normalized = np.divide(u, magnitude, where=magnitude != 0)
    v_normalized = np.divide(v, magnitude, where=magnitude != 0)
    return u_normalized, v_normalized

plt.ion()
fig, ax = plt.subplots(1, 1, figsize=(13, 5))
pcolor1 = ax.pcolormesh(x, y, np.array(velocity_timesteps[0]).T, cmap='jet', shading="gouraud" if blur else "auto")
quiver1 = ax.quiver(x, y, velocity_timesteps[0].T, velocity_timesteps[0].T, color="#00000000" if not show_quiver else "black",
                       scale=35)
fig.colorbar(pcolor1, ax=ax)
ax.set_xlabel('x')
ax.set_ylabel('y')
ax.set_xlim(0, domain_size_x)
ax.set_ylim(0, domain_size_y)
ax.set_title('Velocity field')
ax.axis("off")
ax.set_aspect('equal')

# Remove left, right and middle margins
plt.subplots_adjust(left=0.05, right=0.95, top=0.85, bottom=0.05, wspace=0.0)

# Play the animation
for k in range(timesteps):
    # Velocity
    pcolor1.set_array(np.array(velocity_timesteps[k]).T.ravel())
    pcolor1.set_clim(vmin=0.0, vmax=np.nanmax(velocity_timesteps[k]))

    # Quiver
    if normalize_quiver:
        velocity_timesteps_u[k], velocity_timesteps_v[k] = normalize_vectors(velocity_timesteps_u[k], velocity_timesteps_v[k])
    quiver1.set_UVC(velocity_timesteps_u[k].T, velocity_timesteps_v[k].T)

    fig.canvas.draw_idle()
    fig.suptitle(f'Time: {k * dt:.5f}')
    plt.pause(0.1)

# Plot
plt.ioff()
plt.show()
