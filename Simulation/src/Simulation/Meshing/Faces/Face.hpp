#pragma once

enum class FaceType { Interior, Boundary };

class Face {
protected:
    FaceType m_type;
    double m_viscosity = 0.0;
public:
    Face(FaceType type);

    virtual ~Face() = default;

    FaceType get_type() const;

    double get_viscosity() const;

    virtual double get_pressure() const = 0;

    virtual double get_pressure_correction() const = 0;
};
