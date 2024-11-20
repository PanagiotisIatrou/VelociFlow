import numpy as np


def get_vector_field_magnitudes(x_field, y_field):
    """
    Calculates the magnitude of the vector field
    :param x_field: The x component of the field
    :param y_field: The y component of the field
    :return: The magnitude of the vector field
    """

    return np.sqrt(x_field ** 2 + y_field ** 2)


def normalize_vectors(x_field, y_field, magnitude=1.0):
    """
    Normalizes all the vectors
    :param factor: The magnitude to normalize the vectors to
    :param x_field: The x component of the field
    :param y_field: The y component of the field
    :return: The normalized x and y components of the field
    """

    magnitudes = get_vector_field_magnitudes(x_field, y_field) / magnitude
    x_field_normalized = np.divide(x_field, magnitudes, where=magnitudes != 0)
    y_field_normalized = np.divide(y_field, magnitudes, where=magnitudes != 0)
    return x_field_normalized, y_field_normalized
