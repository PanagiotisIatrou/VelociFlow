#pragma once

enum class Field;

enum class FaceType { Interior, Boundary };

enum class Orientation { Horizontal, Vertical };

class Face {
protected:
    FaceType m_face_type;
    Orientation m_orientation;
public:
    Face(FaceType type, Orientation orientation);

    virtual ~Face() = default;

    FaceType get_face_type() const;

    Orientation get_orientation() const;

    double get_normal_velocity();

    virtual double get_viscosity() const = 0;

    virtual double get_density() const = 0;

    virtual double get_field_value(Field field) const = 0;
};
