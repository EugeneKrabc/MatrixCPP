#include "s21_matrix_oop.h"
#include "gtest/gtest.h"


TEST(Constructors, default_constructor) {
    S21Matrix M;
    EXPECT_TRUE(M.get_rows() == 0);
    EXPECT_TRUE(M.get_cols() == 0);
}

TEST(Constructors, indexing_elements) {
    S21Matrix M(2, 3);
    M(1, 1) = 21;
    EXPECT_TRUE(M(1, 1) == 21);
}

TEST(Constructors, parameterized_constructor) {
    S21Matrix M1(2, 3);
    M1(1, 2) = 21;
    EXPECT_TRUE(M1.get_rows() == 2);
    EXPECT_TRUE(M1.get_cols() == 3);
    EXPECT_TRUE(M1(1, 2) == 21);

    try {
        S21Matrix M2(-2, -3);
        EXPECT_TRUE(false);
    } catch(const char* error) {
        EXPECT_TRUE(true);
    }
}

TEST(Constructors, copy_constructor) {
    S21Matrix M1(2, 3);
    M1(1, 2) = 42;
    M1(0, 0) = 21;
    S21Matrix M2(M1);
    EXPECT_TRUE(M2.get_rows() == 2);
    EXPECT_TRUE(M2.get_cols() == 3);
    EXPECT_TRUE(M2(1, 2) == 42);
    EXPECT_TRUE(M2(0, 0) == 21);
}

TEST(Constructors, move_constructor) {
    S21Matrix M1(2, 3);
    M1(1, 1) = 21;
    M1(1, 2) = 42;
    S21Matrix M2(std::move(M1));
    EXPECT_TRUE(M2(1, 1) == 21);
    EXPECT_TRUE(M2(1, 2) == 42);
    EXPECT_TRUE(M1.get_rows() == 0);
    EXPECT_TRUE(M1.get_cols() == 0);
    try {
        S21Matrix doomed_matrix(std::move(doomed_matrix));
        EXPECT_TRUE(false);
    } catch(const char *error) {
        EXPECT_TRUE(true);
    }

}

TEST(Operations, equality) {
    S21Matrix M1(2, 3);
    for (int i = 0; i < M1.get_rows(); i++)
        for (int j = 0; j < M1.get_cols(); j++)
            M1(i, j) = i + j;
    S21Matrix M2(2, 3);
    for (int i = 0; i < M2.get_rows(); i++)
        for (int j = 0; j < M2.get_cols(); j++)
            M2(i, j) = i + j;
    EXPECT_TRUE(M1 == M2);

    S21Matrix M3 = M2;
    EXPECT_TRUE(M2 == M3);

    S21Matrix M4(2, 3);
    M4(0, 0) = 21;
    EXPECT_FALSE(M4 == M3);

    S21Matrix M5(5, 5);
    for (int i = 0; i < M5.get_rows(); i++)
        for (int j = 0; j < M5.get_cols(); j++)
            M5(i, j) = i + j;
    EXPECT_FALSE(M4 == M3);
    S21Matrix M6(2, 2);
    for (int i = 0; i < M6.get_rows(); i++)
        for (int j = 0; j < M6.get_cols(); j++)
            M6(i, j) = i + j;
    EXPECT_FALSE(M6 == M5);
}

TEST(Operations, sum) {
    S21Matrix M1(5, 5);
    M1(1, 1) = 21;
    M1(2, 2) = 42;
    S21Matrix M2(5, 5);
    M2(1, 1) = 5;
    M2(2, 2) = -5;
    S21Matrix M3 = M1 + M2;
    EXPECT_TRUE(M3.get_rows() == 5);
    EXPECT_TRUE(M3.get_cols() == 5);
    EXPECT_TRUE(M3(1, 1) == 26);
    EXPECT_TRUE(M3(2, 2) == 37);
    EXPECT_TRUE(M1(1, 1) == 21);
    EXPECT_TRUE(M2(1, 1) == 5);

    M3+=M2;
    EXPECT_TRUE(M3.get_rows() == 5);
    EXPECT_TRUE(M3.get_cols() == 5);
    EXPECT_TRUE(M3(1, 1) == 31);
    EXPECT_TRUE(M3(2, 2) == 32);
    EXPECT_TRUE(M2(1, 1) == 5);

    S21Matrix M4(2, 3);
    try {
        S21Matrix doomed_matrix = M4 + M1;
        EXPECT_TRUE(false);
    } catch(const char *error) {
        EXPECT_TRUE(true);
    }
}

TEST(Operations, sub) {
    S21Matrix M1(5, 5);
    M1(1, 1) = 21;
    M1(2, 2) = 42;
    S21Matrix M2(5, 5);
    M2(1, 1) = -5;
    M2(2, 2) = 5;
    S21Matrix M3 = M1 - M2;
    EXPECT_TRUE(M3.get_rows() == 5);
    EXPECT_TRUE(M3.get_cols() == 5);
    EXPECT_TRUE(M3(1, 1) == 26);
    EXPECT_TRUE(M3(2, 2) == 37);
    EXPECT_TRUE(M1(1, 1) == 21);
    EXPECT_TRUE(M2(1, 1) == -5);

    M3 -= M2;
    EXPECT_TRUE(M3.get_rows() == 5);
    EXPECT_TRUE(M3.get_cols() == 5);
    EXPECT_TRUE(M3(1, 1) == 31);
    EXPECT_TRUE(M3(2, 2) == 32);
    EXPECT_TRUE(M2(1, 1) == -5);

    S21Matrix M4(2, 3);
    try {
        S21Matrix doomed_matrix = M4 - M1;
        EXPECT_TRUE(false);
    } catch(const char *error) {
        EXPECT_TRUE(true);
    }
}

TEST(Operations, mul_nmb) {
    S21Matrix M1(5, 5);
    M1(1, 1) = 21;
    M1(2, 2) = 42;
    M1.mul_number(0.5);
    EXPECT_TRUE(M1(1, 1) == 10.5);
    EXPECT_TRUE(M1(2, 2) == 21);
}

TEST(Operations, mul_matrix) {
    int nmb = 1;
    S21Matrix M1(2, 3);
    for (int i = 0; i < M1.get_rows(); i++)
        for (int j = 0; j < M1.get_cols(); j++)
            M1(i, j) = nmb++;
    S21Matrix M2(3, 2);
    for (int i = 0; i < M2.get_rows(); i++)
        for (int j = 0; j < M2.get_cols(); j++)
            M2(i, j) = nmb++;
    S21Matrix M3 = M1 * M2;
    EXPECT_TRUE(M3.get_rows() == 2);
    EXPECT_TRUE(M3.get_cols() == 2);
    EXPECT_TRUE(M3(0, 0) == 58);
    EXPECT_TRUE(M3(0, 1) == 64);
    EXPECT_TRUE(M3(1, 0) == 139);
    EXPECT_TRUE(M3(1, 1) == 154);

    M1 *= M2;
    EXPECT_TRUE(M1.get_rows() == 2);
    EXPECT_TRUE(M1.get_cols() == 2);
    EXPECT_TRUE(M1(0, 0) == 58);
    EXPECT_TRUE(M1(0, 1) == 64);
    EXPECT_TRUE(M1(1, 0) == 139);
    EXPECT_TRUE(M1(1, 1) == 154);

    S21Matrix M4(3, 3);
    try {
        S21Matrix doomed_matrix = M4 * M1;
        EXPECT_TRUE(false);
    } catch(const char *error) {
        EXPECT_TRUE(true);
    }
}

TEST(Operations, transpose) {
    S21Matrix M1(3, 3);
    int k = 0;
    for (int i = 0; i < M1.get_rows(); i++)
        for (int j = 0; j < M1.get_cols(); j++)
            M1(i, j) = k++;
    S21Matrix M2 = M1.transpose();
    M2(0, 0) = 0;
    M2(0, 1) = 3;
    M2(0, 2) = 6;

    M2(1, 0) = 1;
    M2(1, 1) = 4;
    M2(1, 2) = 7;

    M2(2, 0) = 2;
    M2(2, 1) = 5;
    M2(2, 2) = 8;
}

TEST(Operations, calc_complements) {
    S21Matrix M1(2, 3);
    try {
        S21Matrix doomed_matrix = M1.calc_complements();
        EXPECT_TRUE(false);
    } catch(const char *error) {
        EXPECT_TRUE(true);
    }
    S21Matrix M2(3, 3);
    M2(0, 0) = 1;
    M2(0, 1) = 2;
    M2(0, 2) = 3;

    M2(1, 0) = 0;
    M2(1, 1) = 4;
    M2(1, 2) = 2;

    M2(2, 0) = 5;
    M2(2, 1) = 2;
    M2(2, 2) = 1;
    S21Matrix result_matrix = M2.calc_complements();
    S21Matrix expected_result(3, 3);
    expected_result(0, 0) = 0;
    expected_result(0, 1) = 10;
    expected_result(0, 2) = -20;

    expected_result(1, 0) = 4;
    expected_result(1, 1) = -14;
    expected_result(1, 2) = 8;

    expected_result(2, 0) = -8;
    expected_result(2, 1) = -2;
    expected_result(2, 2) = 4;

    EXPECT_TRUE(result_matrix == expected_result);
}

TEST(Operations, determinant) {
    S21Matrix M1(2, 3);
    try {
        M1.determinant();
        EXPECT_TRUE(false);
    } catch(const char *error) {
        EXPECT_TRUE(true);
    }

    S21Matrix M2(3, 3);
    M2(0, 0) = 0;
    M2(0, 1) = 9;
    M2(0, 2) = 5;

    M2(1, 0) = 4;
    M2(1, 1) = 3;
    M2(1, 2) = -5;

    M2(2, 0) = -1;
    M2(2, 1) = 6;
    M2(2, 2) = -4;
    EXPECT_TRUE(M2.determinant() == 324);

    S21Matrix M3(3, 3);
    int k = 0;
    for (int i = 0; i < M3.get_rows(); i++)
        for (int j = 0; j < M3.get_cols(); j++)
            M3(i, j) = k++;
    EXPECT_TRUE(M3.determinant() == 0);
}

TEST(Operations, inverse_matrix) {
    S21Matrix M1(2, 3);
    try {
        S21Matrix doomed_matrix = M1.inverse_matrix();
        EXPECT_TRUE(false);
    } catch(const char *error) {
        EXPECT_TRUE(true);
    }

    S21Matrix M2(3, 3);
    M2(0, 0) = 2;
    M2(0, 1) = 5;
    M2(0, 2) = 7;

    M2(1, 0) = 6;
    M2(1, 1) = 3;
    M2(1, 2) = 4;

    M2(2, 0) = 5;
    M2(2, 1) = -2;
    M2(2, 2) = -3;
    S21Matrix result_matrix = M2.inverse_matrix();

    S21Matrix expected_matrix(3, 3);
    expected_matrix(0, 0) = 1;
    expected_matrix(0, 1) = -1;
    expected_matrix(0, 2) = 1;

    expected_matrix(1, 0) = -38;
    expected_matrix(1, 1) = 41;
    expected_matrix(1, 2) = -34;

    expected_matrix(2, 0) = 27;
    expected_matrix(2, 1) = -29;
    expected_matrix(2, 2) = 24;

    EXPECT_TRUE(expected_matrix == result_matrix);

    S21Matrix M3(3, 3);
    int k = 0;
    for (int i = 0; i < M3.get_rows(); i++)
        for (int j = 0; j < M3.get_cols(); j++)
            M3(i, j) = k++;
    try {
        S21Matrix doomed_matrix_2 = M3.inverse_matrix();
        EXPECT_TRUE(false);
    } catch(const char *error) {
        EXPECT_TRUE(true);
    }
}

 TEST(Operations, setter) {
    S21Matrix M1(3, 3);
    int k = 0;
    for (int i = 0; i < M1.get_rows(); i++)
        for (int j = 0; j < M1.get_cols(); j++)
            M1(i, j) = k++;
    M1.set_rows(5);
    M1.set_columns(10);
    EXPECT_TRUE(fabs(M1(4, 8) - 0.0) < EPSILON);

    S21Matrix M2(5, 5);
    k = 0;
    for (int i = 0; i < M2.get_rows(); i++)
        for (int j = 0; j < M2.get_cols(); j++)
            M2(i, j) = k++;
    M2.set_rows(2);
    M2.set_columns(2);
    try {
        M2(3, 3);
        EXPECT_TRUE(false);
    } catch(const char *error) {
        EXPECT_TRUE(true);
    }
}

int main(int argc, char *argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
