#include "Saver.h"

void Saver::save_mesh_data(const std::string &path, const double execution_time, const Mesh *mesh) {
    FILE *output_file = fopen(path.c_str(), "w");

    // Write the domain size
    fprintf(output_file, "domain_size\n");
    fprintf(output_file, "%f,%f\n", mesh->get_domain_size_x(), mesh->get_domain_size_y());

    // Write the grid size
    fprintf(output_file, "grid_size\n");
    fprintf(output_file, "%d,%d\n", mesh->get_size_x(), mesh->get_size_y());

    // Write the exceution time
    fprintf(output_file, "execution_time\n");
    fprintf(output_file, "%.2f\n", execution_time);

    // Write velocity u
    fprintf(output_file, "velocity_u\n");
    for (int i = 0; i < mesh->get_size_x(); i++) {
        for (int j = 0; j < mesh->get_size_y(); j++) {
            Node *node_P = mesh->get_node(i, j);
            if (node_P != nullptr) {
                fprintf(output_file, "%d,%d,%f\n", i, j, node_P->get_velocity_u());
            } else {
                fprintf(output_file, "%d,%d,-\n", i, j);
            }
        }
    }

    // Write velocity v
    fprintf(output_file, "velocity_v\n");
    for (int i = 0; i < mesh->get_size_x(); i++) {
        for (int j = 0; j < mesh->get_size_y(); j++) {
            Node *node_P = mesh->get_node(i, j);
            if (node_P != nullptr) {
                fprintf(output_file, "%d,%d,%f\n", i, j, node_P->get_velocity_v());
            } else {
                fprintf(output_file, "%d,%d,-\n", i, j);
            }
        }
    }

    // Write pressure
    fprintf(output_file, "pressure\n");
    for (int i = 0; i < mesh->get_size_x(); i++) {
        for (int j = 0; j < mesh->get_size_y(); j++) {
            Node *node_P = mesh->get_node(i, j);
            if (node_P != nullptr) {
                fprintf(output_file, "%d,%d,%f\n", i, j, node_P->get_pressure());
            } else {
                fprintf(output_file, "%d,%d,-\n", i, j);
            }
        }
    }

    // Close the file
    fclose(output_file);
}
