# cfd

<a target="_blank" href="https://www.paypal.com/donate/?hosted_button_id=J65KNQYEK88ML">
  <img src="https://img.shields.io/badge/Donate-PayPal-green.svg">
</a>

This project serves as part of my BSc thesis in Computer Science. It is a CFD (Computational Fluid Dynamics) solver for
the 2D incompressible Navier-Stokes equations. The solver is based on the SIMPLE
algorithm for pressure-velocity coupling, solved in an orthogonal cartesian colocated grid. The simulation is written in
C++, while the visualizations are written in Python.

## Equations

Equations being discretized and solved using the finite volume method (FVM):

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

- Diffusion

$$
\begin{aligned}
    \large \mu \nabla^2\vec{v} = 0
\end{aligned}
$$

## Gallery

## Features

- 2D Simulations (both steady and unsteady)
    - Incompressible Navier Stokes (using the SIMPLE algorithm)
    - Convection - Diffusion
    - Diffusion
- Boundary conditions
    - Velocity inlet
    - Fixed pressure
    - No-slip wall
    - Slip wall
    - Moving wall
    - Periodic
    - Free
- Plots
    - Velocity Magnitude
    - Velocity X
    - Velocity Y
    - Pressure
    - Vorticity
    - Dye
    - Streamlines
    - Quivers
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
- Ability for early stopping and restarting of the Incompressible Navier Stokes simulations
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