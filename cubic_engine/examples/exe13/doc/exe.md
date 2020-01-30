# Example 13: Logistic Regression

## Contents
* [Overview](#overview) 
	* [Logistic Regression](#logistic_regression)
	* [Error Metrics](#error_metrics)
		* [Confusion Matrix](#confusion_matrix)
		* [Accuracy](#accuracy)
		* [Recall or Sensitivity](#recall_or_Sensitivity)
		* [Precision](#precision)
		* [Specificity](#specificity)
		* [F1-score](#f1_score)
* [Include files](#include_files)
* [The main function](#m_func)
* [Results](#results)
* [Source Code](#source_code)

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

Accuracy answers the following question: Overall how often is the classifier correct? It is defined as
as the sum of the diagonal terms over the total number of examples


The opossite of accuracy is the misclassification rate which answers the question: Overall how often is the classifier
wrong? It is defined as the sum of off diagonal terms over the total number of points. Being however the opposite of accuracy we can also
calculate it as one minus accuracy.

#### <a name="recall_or_Sensitivity"></a> Recall or Sensitivity

#### <a name="precision"></a> Precision

Precision answers the following question. When it predicts Yes how often is it correct?

#### <a name="specificity"></a> Specificity

Specificity or True Negative Rate answers the following question: When it is actually No how often the classifier predicts No?
Specificity is one minus the false Positive Rate


#### <a name="f1_score"></a> F1-score


![Sigmoid Function](sigmoid_func.png)

## <a name="include_files"></a> Include files

```
#include "cubic_engine/base/cubic_engine_types.h"
#include "cubic_engine/ml/supervised_learning/logistic_regression.h"
#include "cubic_engine/optimization/serial_batch_gradient_descent.h"
#include "cubic_engine/optimization/utils/gd_control.h"
#include "cubic_engine/maths/confusion_matrix.h"

#include "kernel/maths/functions/real_vector_polynomial.h"
#include "kernel/maths/errorfunctions/mse_function.h"
#include "kernel/maths/functions/sigmoid_function.h"
#include "kernel/utilities/data_set_loaders.h"

#include <iostream>
```

## <a name="m_func"></a> The main function

```
int main(){

    using cengine::uint_t;
    using cengine::real_t;
    using cengine::DynMat;
    using cengine::DynVec;
    using cengine::GDControl;
    using cengine::Gd;
    using cengine::LogisticRegression;
    using cengine::ConfusionMatrix;
    using kernel::RealVectorPolynomialFunction;
    using kernel::MSEFunction;
    using kernel::SigmoidFunction;

    typedef RealVectorPolynomialFunction hypothesis_t;
    typedef SigmoidFunction<RealVectorPolynomialFunction> transformer_t;

    try{

        auto dataset = kernel::load_reduced_iris_data_set();

        // the classifier to use. use a hypothesis of the form
        // f = w_0 + w_1*x_1 + w_2*x_2 + w_3*x_3 + w_4*x_4;
        // set initial weights to 0
        LogisticRegression<hypothesis_t, transformer_t> classifier({0.0, 0.0, 0.0, 0.0, 0.0});

        transformer_t sigmoid_h(classifier.get_model());

        // the error function to to use for measuring the error
        MSEFunction<transformer_t, DynMat<real_t>, DynVec<uint_t>> mse(sigmoid_h);

        GDControl control(20000, 1.0e-4, 0.005);
        control.show_iterations = true;
        Gd gd(control);

        auto result = classifier.train(dataset.first, dataset.second, gd, mse);
        std::cout<<result<<std::endl;
        std::cout<<classifier<<std::endl;

        DynVec<real_t> point{1.0, 5.7, 2.8, 4.1, 1.3};
        auto class_idx = classifier.predict(point);

        std::cout<<"Class index: "<<class_idx<<std::endl;

        // predictions on the training set
        DynVec<uint_t> predictions(dataset.first.rows(), 0);
        classifier.predict(dataset.first, predictions);

        ConfusionMatrix cmatrix(dataset.second, predictions, 2);

        // let's do some calculations
        std::cout<<"\nConsfusion Matrix...."<<std::endl;
        std::cout<<cmatrix<<std::endl;

        std::cout<<"Accuracy.........."<<cmatrix.accuracy()<<std::endl;
        std::cout<<"True positives...."<<cmatrix.true_positives()<<std::endl;
        std::cout<<"Recall class 0...."<<cmatrix.recall_class(0)<<std::endl;
        std::cout<<"Recall class 1...."<<cmatrix.recall_class(1)<<std::endl;

    }
    catch(std::exception& e){

        std::cerr<<e.what()<<std::endl;
    }
    catch(...){

        std::cerr<<"Unknown exception occured"<<std::endl;
    }

    return 0;
}

```

## <a name="results"></a> Results

When running the driver, we get the following output

```
BatchGD: iteration: 1
        Jold: 0.693147 Jcur: 0.68686 error std::fabs(Jcur-Jold): 0.00628751 exit tolerance: 0.0001
BatchGD: iteration: 2
        Jold: 0.68686 Jcur: 0.681045 error std::fabs(Jcur-Jold): 0.0058148 exit tolerance: 0.0001
BatchGD: iteration: 3
        Jold: 0.681045 Jcur: 0.6756 error std::fabs(Jcur-Jold): 0.00544443 exit tolerance: 0.0001
BatchGD: iteration: 4
        Jold: 0.6756 Jcur: 0.670448 error std::fabs(Jcur-Jold): 0.00515239 exit tolerance: 0.0001
BatchGD: iteration: 5
        Jold: 0.670448 Jcur: 0.665528 error std::fabs(Jcur-Jold): 0.00492023 exit tolerance: 0.0001
BatchGD: iteration: 6
        Jold: 0.665528 Jcur: 0.660794 error std::fabs(Jcur-Jold): 0.00473387 exit tolerance: 0.0001
BatchGD: iteration: 7
        Jold: 0.660794 Jcur: 0.656211 error std::fabs(Jcur-Jold): 0.00458253 exit tolerance: 0.0001
BatchGD: iteration: 8
        Jold: 0.656211 Jcur: 0.651753 error std::fabs(Jcur-Jold): 0.00445799 exit tolerance: 0.0001
BatchGD: iteration: 9
        Jold: 0.651753 Jcur: 0.647399 error std::fabs(Jcur-Jold): 0.00435398 exit tolerance: 0.0001
BatchGD: iteration: 10
        Jold: 0.647399 Jcur: 0.643134 error std::fabs(Jcur-Jold): 0.00426572 exit tolerance: 0.0001
BatchGD: iteration: 11
        Jold: 0.643134 Jcur: 0.638944 error std::fabs(Jcur-Jold): 0.00418957 exit tolerance: 0.0001

...

# iterations:..781
# processors:..1
# threads:.....1
Residual:......9.98194e-05
Tolerance:.....0.0001
Convergence:...Yes
Total time:....0.197053
Learning rate:..0.005

-0.164834,-0.25484,-0.943743,1.44809,0.621685

Class index: 1

Consfusion Matrix....
50 , 0

0 , 50


Accuracy..........1
True positives....100
Recall class 0....1
Recall class 1....1
```

## <a name="source_code"></a> Source Code

<a href="../exe.cpp">exe.cpp</a>
