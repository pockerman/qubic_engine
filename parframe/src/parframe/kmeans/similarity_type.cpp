#include "kmeans/similarity_type.h"

namespace kernel
{

	std::string 
	SimilarityType::type_as_string(SimilarityType::Type t){
		
		switch(t){
		
			case SimilarityType::Type::EUCLIDEAN:
				return "EUCLIDEAN";
			case SimilarityType::Type::MANHATTAN:
				return "MANHATTAN";
		}
		
		return "INVALID";
	}
	
	SimilarityType::Type 
	SimilarityType::string_as_type(const std::string& type){
		
		if(type == "EUCLIDEAN"){
			return SimilarityType::Type::EUCLIDEAN;
		}
		else if(type == "MANHATTAN"){
			return SimilarityType::Type::MANHATTAN;
		}
		
		return SimilarityType::Type::INVALID;
	}
	
	std::string 
	SimilarityType::types_as_string(){
		return "[EUCLIDEAN, MANHATTAN, INVALID]";
	}

}
