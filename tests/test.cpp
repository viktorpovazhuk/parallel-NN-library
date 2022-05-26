#include <gtest/gtest.h>
#include <Model.h>
#include <lossfunction/MSELossFunction.h>
#include <matrix/MatrixD.h>

#include <memory>
#include <cmath> // for std::log(), std::exp()
#include <vector>

TEST(TestSuiteName, TestName) {
    EXPECT_STRNE("hello", "world");
    EXPECT_EQ(1 + 1, 2);
}

TEST(ModelTest, Xor) {
    Model model{2, std::make_shared<MSELossFunction>()};
}

//TEST(LossTest, SoftMaxLossFunctionTest) {
//    using Eigen::MatrixXd;
//    SoftMaxLossFunction loss;
//    double actual = loss.forwardPropagate(MatrixXd{{2, 3},
//                                                   {1, -3}},
//                                          MatrixXd{{0, 1},
//                                                   {1, 0}});
//    using std::exp, std::log;
//    double expected = - 1.0 / 2 * (log(exp(1) / (exp(2) + exp(1))) +
//            log(exp(3) / (exp(3) + exp(-3))));
//    EXPECT_DOUBLE_EQ(actual, expected);
//}

TEST(MatrixTest, Constructor) {
    MatrixD m1{3, 2};
    ASSERT_EQ(m1(0, 0), double{});
    ASSERT_EQ(m1(0, 1), double{});
    ASSERT_EQ(m1(1, 0), double{});
    ASSERT_EQ(m1(1, 1), double{});
    ASSERT_EQ(m1(2, 0), double{});
    ASSERT_EQ(m1(2, 1), double{});

    MatrixD m2(4, 2, std::vector<double>{1, 2,
                                         3, 4,
                                         5, 6,
                                         7, 8});
    MatrixD m3(4, 2, std::vector<double>{1, 2,
                                         4, 4,
                                         5, 6,
                                         7, 8});
    ASSERT_NE(m2, m3);
    m3(1, 0) = 3;
    ASSERT_EQ(m2, m3);

    ASSERT_NE(MatrixD(1, 1), m2);


    MatrixD m4{{1}};
    MatrixD m5{{1, 2}};
    MatrixD m6{{3}, {4}};
    MatrixD m7{{1, 2}, {3, 4}};
    MatrixD m8{};

    EXPECT_EQ(m4.rows(), 1);
    EXPECT_EQ(m4.cols(), 1);
    EXPECT_EQ(m5.rows(), 1);
    EXPECT_EQ(m5.cols(), 2);
    EXPECT_EQ(m6.rows(), 2);
    EXPECT_EQ(m6.cols(), 1);
    EXPECT_EQ(m7.rows(), 2);
    EXPECT_EQ(m7.cols(), 2);
    EXPECT_EQ(m8.rows(), 0);
    EXPECT_EQ(m8.cols(), 0);

    MatrixD m9{{}, {}}; // reduces to {}
    EXPECT_EQ(m9.rows(), 0);
    EXPECT_EQ(m9.rows(), 0);

    EXPECT_EQ(m4(0, 0), 1);

    EXPECT_EQ(m5(0, 0), 1);
    EXPECT_EQ(m5(0, 1), 2);

    EXPECT_EQ(m6(0, 0), 3);
    EXPECT_EQ(m6(1, 0), 4);

    EXPECT_EQ(m7(0, 0), 1);
    EXPECT_EQ(m7(0, 1), 2);
    EXPECT_EQ(m7(1, 0), 3);
    EXPECT_EQ(m7(1, 1), 4);

    EXPECT_EQ(m8, MatrixD());
    EXPECT_EQ(m9, MatrixD());
}

TEST(MatrixTest, Multiplication_1) {
    size_t mat_m = 5, mat_n = 4;
    std::vector<double> v1{1, 5, 8, 6,
                           1, 3, 7, 6,
                           1, 3, 4, 6,
                           1, 2, 7, 6,
                           7, 2, 3, 4};
    std::vector<double> v2{1, 3, 8, 6,
                           1, 3, 9, 6,
                           1, 1, 1, 1,
                           1, 2, 7, 6};
    std::vector<double> v3{20, 38, 103, 80,
                           17, 31, 84, 67,
                           14, 28, 81, 64,
                           16, 28, 75, 61,
                           16, 38, 105, 81};
    MatrixD mA(mat_m, mat_n, v1);
    MatrixD mB(mat_n, mat_n, v2);
    MatrixD mC(mat_m, mat_n, v3);
    EXPECT_EQ(mA * mB, mC);
}

TEST(MatrixTest, Multiplication_2) {
    std::vector<double> v1{-2};
    std::vector<double> v2{5};
    std::vector<double> v3{-10};
    MatrixD mA(1, 1, v1);
    MatrixD mB(1, 1, v2);
    MatrixD mC(1, 1, v3);
    EXPECT_EQ(mA * mB, mC);
}

TEST(MatrixTest, Multiplication_3) {
    std::vector<double> v1{-2, 0.5};
    std::vector<double> v2{4, 6};
    std::vector<double> v3{-5.0}; // TODO: Fix operator== with doubles
    MatrixD mA(1, 2, std::move(v1));
    MatrixD mB(2, 1, std::move(v2));
    MatrixD mC(1, 1, std::move(v3));
    EXPECT_EQ(mA * mB, mC);
}

TEST(MatrixTest, Transpose) {
    std::vector<double> v1{1, 5, 8, 6,
                           1, 3, 7, 6,
                           1, 3, 4, 6,
                           1, 2, 7, 6,
                           7, 2, 3, 4};
    std::vector<double> v2{1, 3, 8, 6,
                           1, 3, 9, 6,
                           1, 1, 1, 1,
                           1, 2, 7, 6};
    std::vector<double> v3{20, 38, 103, 80};
    MatrixD mA(5, 4, v1);
    MatrixD mB(4, 4, v2);
    MatrixD mC(1, 4, v3);
    mA.transposeInPlace();
    mA.transposeInPlace();
    mA.transposeInPlace();
    mB.transposeInPlace();
    mC.transposeInPlace();
    EXPECT_EQ(mA.rows(), 4);
    EXPECT_EQ(mA.cols(), 5);
    EXPECT_EQ(mB.rows(), 4);
    EXPECT_EQ(mB.cols(), 4);
    EXPECT_EQ(mC.rows(), 4);
    EXPECT_EQ(mC.cols(), 1);

    EXPECT_EQ(mA, mA.transposeInPlace().transposeInPlace());
    EXPECT_EQ(mB, mB.transposeInPlace().transposeInPlace());
    EXPECT_EQ(mC, mC.transposeInPlace().transposeInPlace());
    EXPECT_EQ(mA, MatrixD(4, 5, std::vector<double>{1, 1, 1, 1, 7,
                                            5, 3, 3, 2, 2,
                                            8, 7, 4, 7, 3,
                                            6, 6, 6, 6, 4}));
    EXPECT_EQ(mB, MatrixD(4, 4, std::vector<double>{1, 1, 1, 1,
                                                    3, 3, 1, 2,
                                                    8, 9, 1, 7,
                                                    6, 6, 1, 6}));
    EXPECT_EQ(mC, MatrixD(4, 1, std::vector<double>{20,
                                                    38,
                                                    103,
                                                    80}));
}