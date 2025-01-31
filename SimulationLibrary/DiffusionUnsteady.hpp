#pragma once

#include "Simulation/DiffusionSimulation.hpp"

class DiffusionUnsteady : public DiffusionSimulation {
private:
    double m_dt;
    int m_timesteps;
    int m_reached_timesteps;

public:
    DiffusionUnsteady(Mesh *mesh, double viscosity, double dt, int timesteps, double tolerance_x, double tolerance_y,
                      std::string output_file);

    void solve() override;

    int get_reached_timesteps() const;
};
