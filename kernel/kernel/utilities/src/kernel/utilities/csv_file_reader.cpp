#include "kernel/utilities/csv_file_reader.h"
#include "kernel/base/kernel_consts.h"
#include "kernel/base/config.h"

#include <boost/algorithm/string.hpp>

#ifdef KERNEL_DEBUG
#include <cassert>
#endif

namespace kernel
{

CSVFileReader::CSVFileReader(const std::string& file_name, bool do_open,
                             const std::string delimeter)
    :
      file_name_(file_name),
      delimeter_(delimeter),
      file_reader_(),
      current_row_idx_(0)
{

    if(do_open){
       open();
    }
}

CSVFileReader::~CSVFileReader()
{
    close();
}

void
CSVFileReader::open(){

    if(!file_reader_.is_open()){

        try{
            file_reader_.open(file_name_, std::ios_base::in);

#ifdef KERNEL_DEBUG

            if(!file_reader_.good()){
                std::string msg("Failed to open file: ");
                msg += file_name_;
                assert(false && msg.c_str());
            }
#endif

        }
        catch(...){

#ifdef KERNEL_DEBUG
            std::string msg("Failed to open file: ");
            msg += file_name_;
            assert(false && msg.c_str());
#else
            throw;
#endif

        }
    }
}

void
CSVFileReader::close(){

    if(file_reader_.is_open()){
        file_reader_.close();
    }
}

std::vector<std::string>
CSVFileReader::read_line(){

    if(!file_reader_.is_open()){
        return std::vector<std::string>(1, "FILE_NOT_OPEN");
    }

#ifdef KERNEL_DEBUG

            if(!file_reader_.good()){
                std::string msg("Failed to open file: ");
                msg += file_name_;
                assert(false && msg.c_str());
            }
#endif
	
    std::vector<std::string> result;
    if(file_reader_.eof()){
		
        result.push_back(KernelConsts::eof_string());
        return result;
    }

    std::string line = "";
    std::getline(file_reader_, line);
    
    boost::algorithm::split(result, line, boost::is_any_of(delimeter_));
    current_row_idx_++;
    return result;
}

std::vector<uint_t>
CSVFileReader::read_line_as_uint(){

    auto line = read_line();
    std::vector<uint_t> line_int(line.size());

    for(uint_t i=0; i<line.size(); ++i){
        line_int[i] = std::stoull(line[i]);
    }

    return line_int;
}

bool
CSVFileReader::eof()const{
    return file_reader_.eof();
}
}
