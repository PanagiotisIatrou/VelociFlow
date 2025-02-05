#include "Saver.hpp"

#include "Simulation/Discretization/Equations/PressureCorrection.hpp"

Saver::Saver(Mesh *mesh, const std::string path) {
    m_mesh = mesh;
    m_path = path;
}

void Saver::open_and_clear_file() {
    m_file = fopen(m_path.c_str(), "w");
}

void Saver::open_append_file() {
    m_file = fopen(m_path.c_str(), "a");
}

void Saver::close_file() {
    fclose(m_file);
    m_file = nullptr;
}

void Saver::write_simulation_name(const SimulationName simulation_name) const {
    fprintf(m_file, "[simulation_name]\n");
    fprintf(m_file, "%s\n", simulation_name_to_str(simulation_name).c_str());
}

void Saver::write_domain_size(const double size_x, const double size_y) const {
    fprintf(m_file, "[domain_size]\n");
    fprintf(m_file, "%.15f,%.15f\n", size_x, size_y);
}

void Saver::write_grid_size(const int size_x, const int size_y) const {
    fprintf(m_file, "[grid_size]\n");
    fprintf(m_file, "%d,%d\n", size_x, size_y);
}

void Saver::write_execution_time(const double execution_time) const {
    fprintf(m_file, "[execution_time]\n");
    fprintf(m_file, "%.2f\n", execution_time);
}

void Saver::write_dt(const double dt) const {
    fprintf(m_file, "[dt]\n");
    fprintf(m_file, "%.15f\n", dt);
}

void Saver::write_density(const double density) const {
    fprintf(m_file, "[density]\n");
    fprintf(m_file, "%.15f\n", density);
}

void Saver::write_viscosity(const double viscosity) const {
    fprintf(m_file, "[viscosity]\n");
    fprintf(m_file, "%.15f\n", viscosity);
}

void Saver::write_tolerance(const EquationType equation_type, const double value) const {
    fprintf(m_file, "[tolerance]\n");
    fprintf(m_file, "%s,%.15f\n", equation_type_to_str(equation_type).c_str(), value);
}

void Saver::write_timesteps_count(const int timesteps) const {
    fprintf(m_file, "[timesteps]\n");
    fprintf(m_file, "%d\n", timesteps);
}

void Saver::write_field(const Field field) const {
    // Write the title of the field
    std::string title;
    switch (field) {
        case Field::VelocityX: {
            title = "velocity_x";
            break;
        }
        case Field::VelocityY: {
            title = "velocity_y";
            break;
        }
        case Field::Pressure: {
            title = "pressure";
            break;
        }
        case Field::Dye: {
            title = "dye";
            break;
        }
        case Field::Phi: {
            title = "phi";
            break;
        }
        default: {
            std::cerr << "Invalid field" << std::endl;
            exit(1);
        }
    }

    fprintf(m_file, "[%s]\n", title.c_str());

    // Write the values of the field
    for (int i = 0; i < m_mesh->get_size_x(); i++) {
        for (int j = 0; j < m_mesh->get_size_y(); j++) {
            const Node *node_P = m_mesh->get_node(i, j);
            if (node_P != nullptr) {
                const double value = node_P->get_field_value(field);
                fprintf(m_file, "%d,%d,%.15f\n", i, j, value);
            } else {
                fprintf(m_file, "%d,%d,-\n", i, j);
            }
        }
    }
}

void Saver::write_normalization_values(const EquationType equation_type, Equation *equation) const {
    fprintf(m_file, "[normalization]\n");
    fprintf(m_file, "%s,%.15f", equation_type_to_str(equation_type).c_str(),
            equation->get_imbalance_normalization_factor());

    // Extra normalization value only for the pressure correction equation
    // (mass imbalance)
    if (equation_type == EquationType::PressureCorrection) {
        fprintf(m_file, ",%.15f",
                static_cast<PressureCorrection *>(equation)->get_mass_imbalance_normalization_factor());
    }

    fprintf(m_file, "\n");
}
