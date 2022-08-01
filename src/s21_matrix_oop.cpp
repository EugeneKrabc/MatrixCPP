#include "s21_matrix_oop.h"

void S21Matrix::destroy_matrix() {
    if (_matrix) {
        for (int i = 0; i < _rows; i++)
            delete [] _matrix[i];
        delete [] _matrix;
        _matrix = nullptr;
    }
    _rows = 0;
    _cols = 0;
}

void S21Matrix::allocate_matrix(int rows, int cols) {
    if (rows < 0 || cols < 0)
        throw "Matrix creation error: Rows and columns must be greater than zero";
    _rows = rows;
    _cols = cols;
    _matrix = new double *[_rows];
    for (int i = 0; i < rows; i++)
        _matrix[i] = new double[cols]();
}

void S21Matrix::copy_matrix_elements(const S21Matrix& other) {
    for (int i = 0; i < other._rows && i < _rows; i++)
        for (int j = 0; j < other._cols && j < _cols; j++)
            _matrix[i][j] = other._matrix[i][j];
}



S21Matrix::S21Matrix() {
    _rows = 0;
    _cols = 0;
    _matrix = nullptr;
}

S21Matrix::S21Matrix(int rows, int cols) {
    allocate_matrix(rows, cols);
}

S21Matrix::S21Matrix(const S21Matrix& other) {
    allocate_matrix(other._rows, other._cols);
    copy_matrix_elements(other);
}

S21Matrix::S21Matrix(S21Matrix&& other) {
    if (this == &other)
        throw "Error: trying to move from the same matrix";
    allocate_matrix(other._rows, other._cols);
    copy_matrix_elements(other);
    other.destroy_matrix();
}

S21Matrix::~S21Matrix() {
    destroy_matrix();
}

bool S21Matrix::eq_matrix(const S21Matrix& other) const {
    bool result = true;
    if (_rows != other._rows || _cols != other._cols)
        result = false;
    for (int i = 0; i < _rows && result; i++)
        for (int j = 0; j < _cols && result; j++)
            if (fabs(_matrix[i][j] - other._matrix[i][j]) > EPSILON) result = false;
    return result;
}

void S21Matrix::sum_matrix(const S21Matrix& other) {
    if (_rows != other._rows || _cols != other._cols)
        throw "Sum error: dimensions of the matrices must be the same";
    for (int i = 0; i < _rows; i++)
        for (int j = 0; j < _cols; j++)
            (*this)(i, j) += other._matrix[i][j];
}

void S21Matrix::sub_matrix(const S21Matrix& other) {
    if (_rows != other._rows || _cols != other._cols)
        throw "Sub error: dimensions of the matrices must be the same";
    for (int i = 0; i < _rows; i++)
        for (int j = 0; j < _cols; j++)
            _matrix[i][j] -= other._matrix[i][j];
}

void S21Matrix::mul_number(const double num) {
    for (int i = 0; i < _rows; i++)
        for (int j = 0; j < _cols; j++)
            _matrix[i][j] *= num;
}

void S21Matrix::mul_matrix(const S21Matrix& other) {
    if (_rows != other._cols || _cols != other._rows) {
        throw "Mult error: Number of rows of the first matrix"
        "must be equal to number of columns of the second matrix";
    }
    S21Matrix result_matrix(_rows, other._cols);
    for (int i = 0; i < result_matrix._rows; i++)
        for (int j = 0; j < result_matrix._cols; j++)
            for (int k = 0; k < _cols; k++)
                result_matrix._matrix[i][j] += _matrix[i][k] * other._matrix[k][j];
    (*this) = result_matrix;
}

S21Matrix S21Matrix::transpose() {
    S21Matrix result(_cols, _rows);
    for (int i = 0; i < result._rows; i++)
        for (int j = 0; j < result._cols; j++)
            result(i, j) = (*this)(j, i);
    return result;
}

S21Matrix S21Matrix::get_minor(int i, int j) {
    S21Matrix result(_rows -1, _cols - 1);
    for (int r = 0, k = 0; r < _rows; r++) {
        if (r == i) continue;
        for (int c = 0, m = 0; c < _cols; c++) {
            if (c == j) continue;
            result(k, m) = (*this)(r, c);
            m++;
        }
        k++;
    }
    return result;
}

double S21Matrix::determinant() {
    if (_rows != _cols)
        throw "Determinant error: Matrix is not square";
    double result = 0;
    if (_cols == 2) {
        result = ((*this)(0, 0) * (*this)(1, 1)) - ((*this)(0, 1) * (*this)(1, 0));
    } else if (_cols > 2) {
        for (int i = 0; i < _cols; i++) {
            double tmp = get_minor(0, i).determinant() * (*this)(0, i);
            int sign = (i % 2 == 0) ? 1 : -1;
            result += tmp * sign;
        }
    } else {
        result = (*this)(0, 0);
    }
    return result;
}

S21Matrix S21Matrix::calc_complements() {
    S21Matrix result(_rows, _cols);
    for (int i = 0; i < _rows; i++)
        for (int j = 0; j < _cols; j++)
            result(i, j) = get_minor(i, j).determinant() * pow(-1.0, i + j);
    return result;
}

S21Matrix S21Matrix::inverse_matrix() {
    double det = determinant();
    if (fabs(det - 0) <= EPSILON)
        throw "Inerse error: Matrix determinant is 0";
    S21Matrix result = calc_complements().transpose();
    result.mul_number(1.0 / fabs(det));
    for (int i = 0; i < result._rows; i++)
        for (int j = 0; j < result._cols; j++)
            result(i, j) *= -1;
    return result;
}

S21Matrix S21Matrix::operator+(const S21Matrix& other) {
    S21Matrix result(*this);
    result.sum_matrix(other);
    return result;
}

S21Matrix S21Matrix::operator-(const S21Matrix& other) {
    S21Matrix result(*this);
    result.sub_matrix(other);
    return result;
}

S21Matrix S21Matrix::operator*(const S21Matrix& other) {
    S21Matrix result(*this);
    result.mul_matrix(other);
    return result;
}

bool S21Matrix::operator==(const S21Matrix& other) const {
    return eq_matrix(other);
}

S21Matrix& S21Matrix::operator=(const S21Matrix& other) {
    if (this != &other) {
        destroy_matrix();
        allocate_matrix(other._rows, other._cols);
        copy_matrix_elements(other);
    }
    return *this;
}

void S21Matrix::operator+=(const S21Matrix& other) {
    sum_matrix(other);
}

void S21Matrix::operator-=(const S21Matrix& other) {
    sub_matrix(other);
}

void S21Matrix::operator*=(const S21Matrix& other) {
    mul_matrix(other);
}
double& S21Matrix::operator()(const int i, const int j) {
    if (i >= _rows || i < 0 || j >= _cols || j < 0)
        throw "Error. Trying to access non-existent element";
    return _matrix[i][j];
}

int S21Matrix::get_rows() {
    return _rows;
}

int S21Matrix::get_cols() {
    return _cols;
}

void S21Matrix::set_rows(int new_rows) {
    if (new_rows <= 0)
        throw "Set rows error: rows must be greater than 0";
    S21Matrix new_matrix(new_rows, _cols);
    new_matrix.copy_matrix_elements(*this);
    (*this) = new_matrix;
}

void S21Matrix::set_columns(int new_cols) {
    if (new_cols <= 0)
        throw "Set rows error: rows must be greater than 0";
    S21Matrix new_matrix(_rows, new_cols);
    new_matrix.copy_matrix_elements(*this);
    (*this) = new_matrix;
}
