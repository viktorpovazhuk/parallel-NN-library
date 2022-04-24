//
// Created by vityha on 22.04.22.
//

#include "layer/FCLayer.h"

#include <Eigen/Dense>

using Eigen::VectorXd;
using Eigen::MatrixXd;

// Initialization

FCLayer::FCLayer(int nodesNumber, int layerInputsNumber) :
                                                Layer{nodesNumber, Layer::LayerType::FC},
                                                weights(MatrixXd::Random(nodesNumber, layerInputsNumber + 1))
{}

// Forward propagation

MatrixXd FCLayer::forwardPropagate(const MatrixXd &inputs) {
    MatrixXd inputsWithBias(inputs.rows() + 1, inputs.cols());
    inputsWithBias << MatrixXd::Ones(1, inputs.cols()), inputs;
    layerInputs = inputsWithBias;
    return weights * layerInputs;;
}

// Backward propagation

MatrixXd FCLayer::calculateGradientsWrtInputs(const MatrixXd &topDerivatives) {
    using Eigen::placeholders::last, Eigen::placeholders::all;
    MatrixXd bottomDerivatives = weights(all, Eigen::seq(1, last)).transpose() *
                                                                        topDerivatives;
    return bottomDerivatives;
}

MatrixXd FCLayer::calculateGradientsWrtWeights(const MatrixXd &topDerivatives) {
    int examplesNum = layerInputs.cols();
#if 0
    MatrixXd meanBatchDerivativesByWeights(weights.rows(), weights.cols());
    for (int i = 0; i < examplesNum; i++) {
        meanBatchDerivativesByWeights += topDerivatives.col(i) * layerInputs.col(i).transpose();
    }
    meanBatchDerivativesByWeights = meanBatchDerivativesByWeights / examplesNum;
    return meanBatchDerivativesByWeights;
#else
    return topDerivatives * layerInputs.transpose();
#endif
}

// Weights

void FCLayer::updateWeights(const MatrixXd &newWeights) {
    weights = newWeights;
}

MatrixXd FCLayer::getWeights() const {
    return weights;
}