#include "kernel/utilities/csv_file_reader.h"
#include <boost/algorithm/string.hpp>
namespace kernel
{

CSVFileReader::CSVFileReader(const std::string& file_name, bool open,
                             const std::string delimeter)
    :
      file_name_(file_name),
      delimeter_(delimeter),
      file_reader_(),
      current_row_idx_(0)
{

    if(open){
       file_reader_.open(file_name_, std::ios_base::in);
    }
}

CSVFileReader::~CSVFileReader()
{
    close();
}

void
CSVFileReader::open(){

    if(!file_reader_.is_open()){
        file_reader_.open(file_name_, std::ios_base::in);
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

    open();

    std::string line = "";
    std::getline(file_reader_, line);
    std::vector<std::string> result;
    boost::algorithm::split(result, line, boost::is_any_of(delimeter_));
    current_row_idx_++;
    return result;
}

bool
CSVFileReader::eof()const{
    return file_reader_.eof();
}
}
