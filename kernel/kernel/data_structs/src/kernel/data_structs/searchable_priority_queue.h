#ifndef SEARCHABLE_PRIORITY_QUEUE_H
#define SEARCHABLE_PRIORITY_QUEUE_H

#include <queue>
#include <vector>
#include <algorithm>

namespace kernel
{

/// \brief searchable_priority_queue simple exposes a find
/// function to the std::priority_queue function.
template<
    class T,
    class Container = std::vector<T>,
    class Compare = std::less<typename Container::value_type> >
class searchable_priority_queue: public std::priority_queue<T, Container, Compare>
{

public:

    using std::priority_queue<T, Container, Compare>::priority_queue;

   /// \brief Returns true if the given value is contained
   /// internally it calls std::find.
   bool contains(const T& val)const;
};

template<typename T,typename Container,typename Compare>
bool
searchable_priority_queue<T,Container,Compare>::contains(const T& val)const{

   auto itr = std::find(this->c.cbegin(), this->c.cend(),val);
   if(itr == this->c.end())
   {
        return false;
   }

   return true;
}

}

#endif // SEARCHABLE_PRIORITY_QUEUE_H
