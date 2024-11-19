#pragma once

enum class FaceType { Interior, Boundary };

class Face {
protected:
    FaceType m_face_type;
public:
    Face(FaceType type);

    virtual ~Face() = default;

    FaceType get_face_type() const;

    virtual double get_viscosity() const = 0;

    virtual double get_density() const = 0;

    virtual double get_pressure() const = 0;

    virtual double get_pressure_correction() const = 0;
};
