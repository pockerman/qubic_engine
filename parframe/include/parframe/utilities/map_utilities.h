#ifndef MAP_UTILITIES_H
#define MAP_UTILITIES_H

namespace kernel
{

/**
 * \brief See Scott Meyers Effective STL Item 24
 * for this function. If k isn't in the map
 * efficiently add pair (k,v) otherwise efficiently update
 * to v the value associated with k. Return an interator
 * to the added or modified pair
 **/
template<typename MapType,
         typename KeyArgType,
         typename ValueArgType>
typename MapType::iterator
add_or_update_map(MapType& map, const KeyArgType& k, const ValueArgType& v){

//find where k is or should be
typename MapType::iterator lb = map.lower_bound(k);

//if lb points to a pair whose key is equivalent to k
//update the pair's value and return the iterator
if(lb != map.end() &&
   !(map.key_comp()(k,lb->first))){

         lb->second = v;
         return lb;
 }
 else{  //add pair (k,v) to map and return an iterator
           //to the new map element

  typedef typename MapType::value_type MVT;
  return map.insert(lb,MVT(k,v));

 }
}

/**
  * \brief Same as above but when we update the map we apply operation
  */
template<typename MapType,
         typename KeyArgType,
         typename ValueArgType,typename Op>
typename MapType::iterator
add_or_update_map(MapType& map, const KeyArgType& k,
                  const ValueArgType& v,const Op& op){

//find where k is or should be
typename MapType::iterator lb = map.lower_bound(k);

//if lb points to a pair whose key is equivalent to k
//update the pair's value and return the iterator
if(lb != map.end() &&
   !(map.key_comp()(k,lb->first))){

         //lb->second = v;
         op(lb);
         return lb;
 }
 else{  //add pair (k,v) to map and return an iterator
           //to the new map element

  typedef typename MapType::value_type MVT;
  return map.insert(lb,MVT(k,v));

 }
}

}

#endif // MAP_UTILITIES_H
