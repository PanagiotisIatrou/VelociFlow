<h1 align="center">VelociFlow | CFD Solver</h1>

<p align="center">[WORK IN PROGRESS]</p>

<a target="_blank" href="https://www.paypal.com/donate/?hosted_button_id=J65KNQYEK88ML">
  <img src="https://img.shields.io/badge/Donate-PayPal-green.svg">
</a>

<img src="logo.png" width="auto">

This project serves as part of my [BSc thesis](thesis.pdf) in Computer Science. It is a CFD (Computational Fluid Dynamics) solver for
the 2D incompressible Navier-Stokes equations. The solver is based on the SIMPLE
algorithm for pressure-velocity coupling, solved in an orthogonal cartesian colocated grid. The simulation library is written in
C++, while the visualizations are written in Python.

## How to run

1. Clone the repository
2. Select an example to run
   - Open `VelociFlow-main/CMakeLists.txt`
   - Change the name of the `.cpp` file in the `add_executable` line to the `.cpp` file you want to run
3. Compile
   - `cd VelociFlow-main`
   - `mkdir build`
   - `cd build`
   - `cmake ..`
   - `make`
4. Run
   - `./Simulation`
   - Wait for the simulation to finish (or press `q` to stop it earlier)
5. Visualize
   - `cd VelociFlow-main/Visualization`
   - `python -m venv venv`
   - `venv/bin/pip install -r requirements.txt`
   - Create a `settings.json` file (look at `settings-steady.json` and `settings-steady.json` examples)
   - Open `visualize.py`
   - Change the name of the `.json` file in the `settings_file` variable to the name of your `settings.json` file
   - `venv/bin/python3 visualize.py`

## Equations

Equations being discretized and solved using the Finite Volume Method (FVM) in 2D:

- Incompressible Navier Stokes

$$
\begin{aligned}
    \begin{cases}
        \large \nabla \cdot \vec{v} = 0 \\ \\
        \large \rho \bigg[ \dfrac{\partial \vec{v}}{\partial t} + (\vec{v} \cdot \nabla)\vec{v} \bigg] = -\nabla p + \mu \nabla^2\vec{v}
    \end{cases}
\end{aligned}
$$

- Convection - Diffusion

$$
\begin{aligned}
    \large \dfrac{\partial \vec{v}}{\partial t} + (\vec{v} \cdot \nabla)\vec{v} = \mu \nabla^2\vec{v}
\end{aligned}
$$

- Single Convection - Diffusion

$$
\begin{aligned}
    \large \dfrac{\partial \phi}{\partial t} + (\vec{v} \cdot \nabla)\phi = \Gamma \nabla^2\phi
\end{aligned}
$$

- Diffusion

$$
\begin{aligned}
    \large \dfrac{\partial \phi}{\partial t} = \Gamma \nabla^2 \phi
\end{aligned}
$$

## Gallery

<table>
  <tbody>
    <tr>
      <td>
        <figure>
          <p align="center" width="300px">
            <figcaption><p align="center">Lid-driven cavity</p></figcaption>
            <img src="Gallery/lid-driven-cavity.png" width="300px">
          </p>
        </figure>
      </td>
      <td>
        <figure>
          <p align="center" width="300px">
            <figcaption><p align="center">Kelvin-Helmholtz</p></figcaption>
            <img src="Gallery/kelvin-helmholtz.gif" width="300px">
          </p>
        </figure>
      </td>
    </tr>
    <tr>
      <td>
        <figure>
          <p align="center" width="300px">
            <figcaption><p align="center">Pipe</p></figcaption>
            <img src="Gallery/pipe.png" width="300px">
          </p>
        </figure>
      </td>
      <td>
        <figure>
          <p align="center" width="300px">
            <figcaption><p align="center">Backward-facing step</p></figcaption>
            <img src="Gallery/backward-facing-step.png" width="300px">
          </p>
        </figure>
      </td>
    </tr>
    <tr>
      <td>
        <figure>
          <p align="center" width="300px">
            <figcaption><p align="center">Pipe with obstacles</p></figcaption>
            <img src="Gallery/pipe-with-obstacles.png" width="300px">
          </p>
        </figure>
      </td>
      <td>
        <figure>
          <p align="center" width="300px">
            <figcaption><p align="center">Von Karman</p></figcaption>
            <img src="Gallery/von-karman.gif" width="300px">
          </p>
        </figure>
      </td>
    </tr>
  </tbody>
</table>

## Features

### Creating simulations

- 2D Simulations (both steady and unsteady)
    - Incompressible Navier Stokes (using the SIMPLE algorithm)
    - Convection - Diffusion
    - Single Convection - Diffusion
    - Diffusion
- Boundary conditions
    - Velocity inlet
    - Fixed pressure
    - No-slip wall
    - Slip wall
    - Moving wall
    - Periodic
    - Free
- Grid types
    - Orthogonal cartesian colocated
- Diffusion schemes
    - Central differencing
- Convection schemes
    - Upwind
    - Central differencing
    - QUICK Hayase
- Time schemes
    - Implicit Euler
- Dye injection (for tracing the flow)
- Ability for early stopping and restarting of the Incompressible Navier-Stokes simulations (by pressing `q`)
- Residual types
    - Unscaled
    - Scaled
    - Normalized
- Residual norm types
    - $\large L^1$
    - $\large L^2$
    - $\large L^\infty$
- Stopping rules (based on tolerance)
    - Absolute
    - Relative

### Visualizing the results

- Create single images for steady simulations
- Create video animations for unsteady simulations
- Selecting a field to plot
    - Velocity Magnitude
    - Velocity X
    - Velocity Y
    - Pressure
    - Dye
    - Phi
    - Vorticity
    - Streamlines
    - Quivers
- Setting min and max values
- Selecting a color map
- Select a specific frame of an unsteady simulation
- Ability to blur and smooth out the result
- Produce real time animations based on $\Delta t$
- Set the frames per second (FPS) of an animation
- Include streamlines (only on velocity magnitude)
- Include quivers (only on velocity magnitude)
- Set density of streamlines and quivers
- Set color of streamlines and quivers
- Option to only show the graphics without the grid

## Things to implement in the future

- Code documentation
- Features usage documentation
- Parallelizing code execution (CPU or GPU)
- Simulate real dye color mixing
- Time dependent boundary conditions
- SIMPLE algorithm variations (SIMPLER, SIMPLEC, etc.)
- Multiphase flow
- Convergence monitors
