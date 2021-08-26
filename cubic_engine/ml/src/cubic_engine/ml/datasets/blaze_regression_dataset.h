#ifndef REGRESSION_DATASET_H
#define REGRESSION_DATASET_H

#include "cubic_engine/base/cubic_engine_types.h"
#include "cubic_engine/ml/datasets/regression_dataset_base.h"
#include "cubic_engine/ml/datasets/dataset_point_type.h"

#include <boost/noncopyable.hpp>
#include <string>
#include <map>

namespace cengine {
namespace ml{



///
/// \brief The BlazeRegressionDataset class
///
class BlazeRegressionDataset: private boost::noncopyable
{
public:

    ///
    /// \brief storage_engine_t
    ///
    typedef DynMat<real_t> storage_engine_t;

    ///
    /// \brief storage_engine_t
    ///
    typedef DynMat<real_t> features_t;

    ///
    /// \brief labels_t
    ///
    typedef DynVec<real_t> labels_t;

    ///
    /// \brief label_value_t
    ///
    typedef real_t label_value_t;

    ///
    /// \brief row_t
    ///
    typedef DynVec<real_t> row_t;

    ///
    /// \brief RegressionDataset
    ///
    BlazeRegressionDataset();

    ///
    /// \brief operator []
    /// \param i
    /// \return
    ///
    std::tuple<row_t, label_value_t> operator[](uint_t i)const;

    ///
    /// \brief empty
    /// \return
    ///
    bool empty()const noexcept{return examples_.rows() == 0;}

    ///
    /// \brief n_rows
    /// \return
    ///
    uint_t n_rows()const noexcept{return examples_.rows();}

    ///
    /// \brief n_features
    /// \return
    ///
    uint_t n_features()const{return examples_.columns();}

    ///
    /// \brief n_examples
    ///
    uint_t n_examples()const{return examples_.rows();}

    ///
    /// \brief feature_matrix
    /// \return
    ///
    const storage_engine_t& feature_matrix()const noexcept{return examples_;}

    ///
    /// \brief get_row
    /// \param idx
    /// \return
    ///
    row_t get_row(uint_t idx)const{return row_t();}

    ///
    /// \brief get_label
    /// \param i
    /// \return
    ///
    label_value_t get_label(uint_t i)const{return labels_[i];}

    ///
    /// \brief labels
    /// \return
    ///
    const labels_t& labels()const noexcept{return labels_;}

    ///
    /// \brief feature_matrix
    /// \return
    ///
    storage_engine_t& feature_matrix()noexcept{return examples_;}

    ///
    /// \brief labels
    /// \return
    ///
    labels_t& labels()noexcept{return labels_;}

    ///
    /// \brief load_from_file
    ///
    template<typename FileReader>
    void load_from_file(FileReader& reader);

    ///
    /// \brief Load the dataset from the given loader
    ///
    template<typename DataLoader>
    void load_from_loader(const DataLoader& loader);

    ///
    /// \brief load_from_data
    /// \param features
    /// \param labels
    ///
    void load_from_data(const features_t& features, const labels_t& labels);

    ///
    /// \brief get_columns
    /// \return
    ///
    std::vector<std::string> get_columns()const;

    ///
    /// \brief set_columns
    /// \param columns
    ///
    void set_columns(const std::vector<std::string>& columns);

    ///
    /// \brief columns
    ///
    auto columns()-> std::map<std::string, uint_t>&{return columns_;}

    ///
    /// \brief columns
    ///
    const std::map<std::string, uint_t>& columns()const noexcept {return columns_;}

    ///
    ///
    ///
    template<typename OperatorTp>
    DynVec<real_t> accumulate(const OperatorTp& op, uint_t limit, PointTp type=PointTp::START)const;

    ///
    ///
    ///
    template<typename OpTp>
    void iterate(const OpTp& op)const;


private:


    ///
    /// \brief features_ The features matrix
    ///
    storage_engine_t examples_;

    ///
    /// \brief labels_
    ///
    labels_t labels_;

    ///
    ///
    ///
    std::map<std::string, uint_t> columns_;


    ///
    ///
    ///
    template<typename OperatorTp>
    friend DynVec<real_t> accumulate_head(const BlazeRegressionDataset& dataset, const OperatorTp& op,
                                    uint_t limit);

    ///
    ///
    ///
    template<typename OperatorTp>
    friend DynVec<real_t> accumulate_tail(const BlazeRegressionDataset& dataset, const OperatorTp& op,
                                    uint_t limit);

};


template<typename DataLoader>
void
BlazeRegressionDataset::load_from_loader(const DataLoader& loader){
    loader.load(examples_, labels_, columns());
}

template<typename FileReader>
void
BlazeRegressionDataset::load_from_file(FileReader& reader){
    reader.read(examples_, labels_, columns());
}

template<typename OperatorTp>
DynVec<real_t>
accumulate_head(const BlazeRegressionDataset& dataset, const OperatorTp& op, uint_t limit){

    if(dataset.empty()){
        return DynVec<real_t>();
    }

    DynVec<real_t> result(limit, 0.0);

    for(uint_t i=0; i < limit; ++i){
        auto[row, label] = dataset[i];
        result[i] = op(row, label);
    }
    return result;
}

template<typename OperatorTp>
DynVec<real_t>
accumulate_tail(const BlazeRegressionDataset& dataset, const OperatorTp& op, uint_t limit){

    if(dataset.empty()){
        return DynVec<real_t>();
    }

    DynVec<real_t> result(limit, 0.0);
    auto start = dataset.n_rows() - 1;
    auto end = dataset.n_rows() - limit;
    for(uint_t i=0; start< end;  --start, i++){
        auto[row, label] = dataset[start];
        result[i] = op(row, label);
    }
    return result;
}


template<typename OperatorTp>
DynVec<real_t>
BlazeRegressionDataset::accumulate(const OperatorTp& op, uint_t limit, PointTp type)const{

    if(type == PointTp::END){
        return accumulate_tail<OperatorTp>(*this, op, limit);
    }

     return accumulate_head<OperatorTp>(*this, op, limit);
}

template<typename OpTp>
void
BlazeRegressionDataset::iterate(const OpTp& op)const{

    for(uint_t i=0; i < n_rows(); ++i){
        auto [row, label] = (*this)[i];
        op(row, label);
    }
}



}
}

#endif // REGRESSION_DATASET_H
