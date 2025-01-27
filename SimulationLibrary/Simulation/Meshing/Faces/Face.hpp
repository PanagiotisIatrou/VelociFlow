#pragma once

enum class Field;

enum class FaceType { Interior, Boundary };

enum class Orientation { Horizontal, Vertical };

class Face {
protected:
    FaceType m_face_type;
    Orientation m_orientation;

    double m_dt = 1.0;

public:
    Face(FaceType type, Orientation orientation);

    virtual ~Face() = default;

    FaceType get_face_type() const;

    Orientation get_orientation() const;

    virtual double get_normal_velocity() = 0;

    virtual double get_field_value(Field field) const = 0;

    void set_dt(double dt);
};
