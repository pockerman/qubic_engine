#ifndef CSV_FILE_READER_H
#define CSV_FILE_READER_H

#include "kernel/base/types.h"

#include <boost/noncopyable.hpp>
#include <string>
#include <vector>
#include <fstream>

namespace kernel
{

class CSVFileReader: private boost::noncopyable
{
public:

	///
    /// \brief The default column delimiter
	///
    static std::string default_delimeter(){return ",";}

	///
    /// \brief Constructor
	///
    CSVFileReader(const std::string& file_path, bool open=true,
                  const std::string delimeter=CSVFileReader::default_delimeter());
				  
	///
    /// \brief Destructor. Close the file if it is empty
	///
    ~CSVFileReader();

	///
    /// \brief Read a line from the file. It attempts to
    /// open the file if the file is not already open
	///
    std::vector<std::string> read_line();

	///
    /// \brief Attempts to open the file for reading
	///
    void open();

	///
    /// \brief Close the file
	///
    void close();

	///
    /// \brief Returns true if the underlying stream
    /// handler has reached the EOF
	///
    bool eof()const;

private:

	///
    /// \brief The filename to read
	///
    const std::string file_name_;

	///
    /// \brief The delimiter
	///
    const std::string delimeter_;

	///
    /// \brief The file reader
	///
    std::ifstream file_reader_;

	///
    /// \brief The current row index
	///
    uint_t current_row_idx_;
};
}

#endif // CSV_FILE_READER_H
