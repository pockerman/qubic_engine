#ifndef SIMILARITY_TYPE_H
#define SIMILARITY_TYPE_H

#include <string>

namespace kernel
{

	//Holds an enum of the similarity types supported
	struct SimilarityType
	{
	
		enum class Type {EUCLIDEAN, MANHATTAN, INVALID};
		
		//Returns the assumed initialization type for variables of this type
		static SimilarityType::Type init(){return SimilarityType::Type::INVALID;}
		
		//Returns a string representation of the given type
		static std::string type_as_string(SimilarityType::Type t);
		
		//Returns the type representation from the given string
		static SimilarityType::Type string_as_type(const std::string& type);
		
		//return a string representation of all the similarity types supported
		static std::string types_as_string();
	};


}
#endif
