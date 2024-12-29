#include "Saver.hpp"

Saver::Saver(Mesh *mesh, std::string path) {
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

void Saver::write_domain_size() const {
    fprintf(m_file, "domain_size\n");
    fprintf(m_file, "%f,%f\n", m_mesh->get_domain_size_x(), m_mesh->get_domain_size_y());
}

void Saver::write_grid_size() const {
    fprintf(m_file, "grid_size\n");
    fprintf(m_file, "%d,%d\n", m_mesh->get_size_x(), m_mesh->get_size_y());

}

void Saver::write_execution_time(const double execution_time) const {
    fprintf(m_file, "execution_time\n");
    fprintf(m_file, "%.2f\n", execution_time);
}

void Saver::write_dt() const {
    fprintf(m_file, "dt\n");
    fprintf(m_file, "%f\n", m_mesh->get_dt());
}

void Saver::write_timesteps_count(const int timesteps) const {
    fprintf(m_file, "timesteps\n");
    fprintf(m_file, "%d\n", timesteps);
}

void Saver::write_field(const Field field) const {
    // Write the title of the field
    std::string title;
    if (field == Field::VelocityX) {
        title = "velocity_u";
    } else if (field == Field::VelocityY) {
        title = "velocity_v";
    } else if (field == Field::Pressure) {
        title = "pressure";
    } else if (field == Field::Dye) {
        title = "dye";
    }
    fprintf(m_file, "%s\n", title.c_str());

    // Write the values of the field
    for (int i = 0; i < m_mesh->get_size_x(); i++) {
        for (int j = 0; j < m_mesh->get_size_y(); j++) {
            const Node *node_P = m_mesh->get_node(i, j);
            if (node_P != nullptr) {
                double value;
                if (field == Field::VelocityX) {
                    value = node_P->get_velocity_x();
                } else if (field == Field::VelocityY) {
                    value = node_P->get_velocity_y();
                } else if (field == Field::Pressure) {
                    value = node_P->get_pressure();
                } else if (field == Field::Dye) {
                    value = node_P->get_dye();
                }
                fprintf(m_file, "%d,%d,%f\n", i, j, value);
            } else {
                fprintf(m_file, "%d,%d,-\n", i, j);
            }
        }
    }
}
