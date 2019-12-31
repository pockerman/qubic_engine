#ifndef GENERIC_LINE_H
#define GENERIC_LINE_H

#include "kernel/base/types.h"
#include "kernel/base/kernel_consts.h"

namespace kernel
{

  namespace kernel_detail
  {

  template<typename NodeTp>
  struct generic_line_base
  {
      typedef NodeTp node_type;

      /// \brief The starting point
      const NodeTp* start_;

      /// \brief The ending point
      const NodeTp* end_;

      /// \brief The id of the line
      uint_t id_;

      /// \brief Constructor
      generic_line_base();

      /// \brief Constructor
      generic_line_base(const NodeTp& p1,const NodeTp& p2, uint_t id);

      /// \brief Returns true iff id_ != kernel_consts::invalid_size_type()
      bool has_valid_id()const noexcept{return (id_ != KernelConsts::invalid_size_type());}

      /// \brief Returns the id of the line
      uint_t get_id()const noexcept{return id_;}

      /// \brief Set the id of the line
      void set_id(uint_t id)noexcept{id_ = id;}

      /// \brief Returns the starting point of the line
      NodeTp& start(){return *start_;}

      /// \brief Returns the starting point of the line
      const NodeTp& start()const{return *start_;}

      /// \brief Returns the ending point of the line
      NodeTp& end(){return *end_;}

      /// \brief Returns the ending point of the line
      const NodeTp& end()const{return *end_;}

  };

  template<typename NodeTp>
  generic_line_base<NodeTp>::generic_line_base()
  :
  start_(),
  end_(),
  id_(KernelConsts::invalid_size_type())
  {}

  template<typename NodeTp>
  generic_line_base<NodeTp>::generic_line_base(const NodeTp& p1,const NodeTp& p2, uint_t id)
  :
  start_(&p1),
  end_(&p2),
  id_(id)
  {}

}// kernel_detail

/// \detailed Generic representation of a line
/// representing two objects of type NodeT having
/// cost CostTp. If the line does not have a cost then
/// we can use kernel_types::dummy_tag to simulate this.
/// By default it id assumed that the type of the cost
/// is kernel::real_type which depending on the configuration may be
/// either single or double precision floating point number.

template<typename NodeTp, typename CostTp=real_t>
class GenericLine: private kernel_detail::generic_line_base<NodeTp>
{

public:

    typedef CostTp cost_type;
    typedef kernel_detail::generic_line_base<NodeTp> base;
    typedef typename base::node_type node_type;

    using base::start;
    using base::end;
    using base::get_id;
    using base::set_id;
    using base::has_valid_id;

    /// \brief Constructor
    GenericLine();

    /// \brief Constructor
    GenericLine(const NodeTp& p1, const NodeTp& p2, uint_t id, const CostTp& cost = CostTp());

    /// \brief Returns the cost of the line
    const CostTp& get_cost()const{return cost_;}

    /// \brief Returns the cost of the line
    CostTp& get_cost(){return cost_;}

    /// \brief Set the cost of the line
    void set_cost(const CostTp& cost){cost_= cost;}

private:

        /// \brief The cost
    CostTp cost_;

};

template<typename NodeTp,typename CostTp>
GenericLine<NodeTp,CostTp>::GenericLine()
:
kernel_detail::generic_line_base<NodeTp>(),
cost_()
{}

template<typename NodeTp,typename CostTp>
GenericLine<NodeTp, CostTp>::GenericLine(const NodeTp& p1,const NodeTp& p2, uint_t id, const CostTp& cost)
:
kernel_detail::generic_line_base<NodeTp>(p1,p2,id),
cost_(cost)
{}


/// \brief Partial specialization for lines having no cost
template<typename NodeTp>
class GenericLine<NodeTp, void>: private kernel_detail::generic_line_base<NodeTp>
{
public:

    typedef kernel_detail::generic_line_base<NodeTp> base;
    typedef typename base::node_type node_type;

    using base::start;
    using base::end;
    using base::get_id;
    using base::set_id;
    using base::has_valid_id;

    /// \brief Constructor
    GenericLine();

    /// \brief Constructor
    GenericLine(const NodeTp& p1, const NodeTp& p2, uint_t id);
};

template<typename NodeTp>
GenericLine<NodeTp,void>::GenericLine()
:
kernel_detail::generic_line_base<NodeTp>()
{}

template<typename NodeTp>
GenericLine<NodeTp,void>::GenericLine(const NodeTp& p1, const NodeTp& p2, uint_t id)
:
kernel_detail::generic_line_base<NodeTp>(p1,p2,id)
{}


}

#endif // GENERIC_LINE_H
