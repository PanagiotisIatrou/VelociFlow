# cfd

<a target="_blank" href="https://www.paypal.com/donate/?hosted_button_id=J65KNQYEK88ML">
  <img src="https://img.shields.io/badge/Donate-PayPal-green.svg">
</a>

This project serves as part of my BsC thesis in Computer Science. It is a CFD (Computational Fluid Dynamics) solver for
the 2D incompressible Navier-Stokes equations. The solver is based on the SIMPLE
algorithm for pressure-velocity coupling. The code is written in C++.

## Equations

The following system of equations are discretized and solved using the finite volume method (FVM):  
$\large \nabla \cdot (\vec{\rho v}) = 0$  
$\large \rho \cdot [\frac{\partial \vec{v}}{\partial t} + (\vec{v} \cdot \nabla)\vec{v}] = -\nabla p + \mu \cdot \nabla^2\vec{v}$

Expanding the equations in 2D gives the following system of 3 equations:

$\large \frac{\partial(\rho u)}{\partial x} + \frac{\partial(\rho v)}{\partial y} = 0$  
$\large \frac{\partial(\rho u)}{\partial t} + \frac{\partial(\rho u^2)}{\partial x} + \frac{\partial(\rho uv)}{\partial y} = -\frac{\partial p}{\partial x} + \frac{\partial}{\partial x}(\mu\frac{\partial u}{\partial x})$  
$\large \frac{\partial(\rho v)}{\partial t} + \frac{\partial(\rho u v)}{\partial x} + \frac{\partial(\rho v^2)}{\partial y} = -\frac{\partial p}{\partial y} + \frac{\partial}{\partial y}(\mu\frac{\partial v}{\partial y})$