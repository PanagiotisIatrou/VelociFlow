# cfd

<a target="_blank" href="https://www.paypal.com/donate/?hosted_button_id=J65KNQYEK88ML">
  <img src="https://img.shields.io/badge/Donate-PayPal-green.svg">
</a>

This project serves as part of my BsC thesis in Computer Science. It is a CFD (Computational Fluid Dynamics) solver for
the 2D incompressible Navier-Stokes equations. The solver is based on the SIMPLE
algorithm for pressure-velocity coupling, solved in an orthogonal cartesian colocated grid. The simulation is written in
C++, while the visualizations are conducted in Python.

## Equations

The following system of equations are discretized and solved using the finite volume method (FVM):

- $\nabla \cdot \vec{v} = 0 $
- $\rho \bigg[ \dfrac{\partial \vec{v}}{\partial t} + (\vec{v} \cdot \nabla)\vec{v} \bigg] = -\nabla p + \mu \, \nabla^2\vec{v}$

Expanding the equations in 2D gives the following system of 3 equations:

- $\dfrac{\partial u}{\partial x} + \dfrac{\partial v}{\partial y} = 0$
- $\dfrac{\partial(\rho u)}{\partial t} + \dfrac{\partial(\rho uu)}{\partial x} + \dfrac{\partial(\rho vu)}{\partial y} = -\dfrac{\partial p}{\partial x} + \dfrac{\partial}{\partial x}\bigg(\mu\dfrac{\partial u}{\partial x}\bigg) + \dfrac{\partial}{\partial y}\bigg(\mu\dfrac{\partial u}{\partial y}\bigg)$
- $\dfrac{\partial(\rho v)}{\partial t} + \dfrac{\partial(\rho uv)}{\partial x} + \dfrac{\partial(\rho vv)}{\partial y} = -\dfrac{\partial p}{\partial y} + \dfrac{\partial}{\partial x}\bigg(\mu\dfrac{\partial v}{\partial x}\bigg) + \dfrac{\partial}{\partial y}\bigg(\mu\dfrac{\partial v}{\partial y}\bigg)$