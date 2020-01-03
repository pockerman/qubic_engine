# Example 12: Logistic Regression 

## Contents
* [Acknowledgements](#ackw)
* [Overview](#overview) 
	* [Logistic Regression](#logistic_regression)
	* [Error Metrics](#error_metrics)
		*[Confusion Matrix](#confusion_matrix)
		*[Accuracy](#accuracy)
		*[Recall or Sensitivity](#recall_or_Sensitivity)
		*[Precision](#precision)
		*[Specificity](#specificity)
		*[F1-score](#f1_score)
* [Include files](#include_files)
* [Program structure](#prg_struct)
* [The main function](#m_func)
* [Results](#results)
* [Source Code](#source_code)

## <a name="ackw"></a>  Acknowledgements

## <a name="overview"></a> Overview

Example 11 discussed the linear regression model. This example is concerned with classification and in particular classification using
the so-called logistic regression model. Classification is merely a technique that allows us to distinguish or classify items into
classes (checkout the wikipedia article <a href="https://en.wikipedia.org/wiki/Statistical_classification">statistical classification</a>).
There are several models in order to classify items in a data set. Perhaps one of the simplest  is the logistic regression 
model (see wikipedia entry <a href="https://en.wikipedia.org/wiki/Logistic_regression">logistic regression</a> for more details). 

### <a name="logistic_regression"></a> Logistic Regression

### <a name="error_metrics"></a> Error Metrics

So we established the logistic regression model but how can we measure how good it is? There are various metrics to do so

- Confusion matrix
- Accuracy
- Recall or sensitivity
- Precision
- Specificity
- F1-score

Let's see what all these are. 

#### <a name="confusion_matrix"></a> Confusion Matrix

A confusion matrix is simply  a table layout that allows visualization of the performance of an algorithm. 
Each row of the matrix represents the instances in a predicted class while each column represents the instances in an actual class (or vice versa).
For more information, see the wikipedia entry <a href="https://en.wikipedia.org/wiki/Confusion_matrix">Confusion matrix</a> 

#### <a name="accuracy"></a> Accuracy

#### <a name="recall_or_Sensitivity"></a> Recall or Sensitivity

#### <a name="precision"></a> Precision

#### <a name="specificity"></a> Specificity


#### <a name="f1_score"></a> F1-score


![Sigmoid Function](sigmoid_func.png)

## <a name="include_files"></a> Include files

```
#include "kernel/base/config.h"

#ifdef USE_OPENMP
#include "kernel/parallel/threading/openmp_executor.h"
#endif

#include "kernel/base/kernel_consts.h"
#include "kernel/parallel/threading/thread_pool.h"
#include "kernel/maths/functions/real_vector_polynomial.h"
#include "kernel/maths/errorfunctions/mse_function.h"
#include "kernel/utilities/data_set_loaders.h"

#include "cubic_engine/base/cubic_engine_types.h"
#include "cubic_engine/optimization/threaded_batch_gradient_descent.h"
#include "cubic_engine/optimization/utils/gd_control.h"

#include <iostream>

```
## <a name="prg_struct"></a> Program structure

## <a name="m_func"></a> The main function

```
int main(){

    using cengine::uint_t;
    using cengine::real_t;
    using cengine::DynMat;
    using cengine::DynVec;
    using cengine::ThreadedGd;
    using cengine::GDControl;
    using kernel::RealVectorPolynomialFunction;
    using kernel::MSEFunction;
    using kernel::PartitionedType;

    const uint_t NUM_THREADS = 2;

    auto dataset = kernel::load_car_plant_dataset_with_partitions(2);

    GDControl control(10000, kernel::KernelConsts::tolerance(), GDControl::DEFAULT_LEARNING_RATE);
    control.show_iterations = true;

    ThreadedGd gd(control);

#ifdef USE_OPENMP
    {

        kernel::OMPExecutor executor(NUM_THREADS);

        //hypothesis of the form f = w_0 + w_1*x
        // where w_0 = w_1 = 0.0
        RealVectorPolynomialFunction hypothesis({0.0, 0.0});

        // the error functionto to use for measuring the error
        MSEFunction<RealVectorPolynomialFunction,
                PartitionedType<DynMat<real_t>>,
                PartitionedType<DynVec<real_t>>> mse(hypothesis);

        gd.solve(dataset.first, dataset.second, mse, hypothesis, executor, kernel::OMPOptions());
    }
#endif
    {
        kernel::ThreadPool executor(NUM_THREADS);

        //hypothesis of the form f = w_0 + w_1*x
        // where w_0 = w_1 = 0.0
        RealVectorPolynomialFunction hypothesis({0.0, 0.0});

        // the error functionto to use for measuring the error
        MSEFunction<RealVectorPolynomialFunction,
                PartitionedType<DynMat<real_t>>,
                PartitionedType<DynVec<real_t>>> mse(hypothesis);

        gd.solve(dataset.first, dataset.second, mse, hypothesis, executor, kernel::Null());

    }
        
    return 0;
}

```

## <a name="results"></a> Results

## <a name="source_code"></a> Source Code

<a href="../exe.cpp">exe.cpp</a>
