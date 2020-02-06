#ifndef FILE_WRITER_BASE_H
#define FILE_WRITER_BASE_H

#include "kernel/utilities/file_formats.h"

#include <fstream>
#include <string>

namespace kernel
{

class FileWriterBase
{

public:

    /**
     * @brief The default comment mark
     */
    static char default_comment_mark(){return '#';}

    /// \brief Default ctor
    FileWriterBase()=default;

    /**
      *@brief Constructor. Construct by passing the name
      * of the file to write into.
      */
    FileWriterBase(const std::string& file_name, FileFormats::Type t, bool open_file=false, const std::ios_base::openmode mode=std::ios_base::out);

    /**
      *@brief Destructor
    */
    virtual ~FileWriterBase();

    /**
      *@brief Open the file for writing
      */
    virtual void open(std::ios_base::openmode mode);

    /**
      * @brief Return true if and only if the file is open
      */
    bool is_open()const{return file_.is_open();}

    /**
      *@brief Close the file. Return true if and only if the file
      * was closed successfully false otherwise
      */
    virtual void close();

    /**
      *@brief Return the mark that signifies the beginning of a comment line
      */
    char get_comment_mark()const{return comment_mark_;}

    /**
      *@brief Set the comment mark
      */
    void set_comment_mark(char mark){comment_mark_ = mark;}

    /**
     * @brief Write the header of the file. By default some information
     * such as date and time the file was created is written
     */
    virtual void write_header();

    /**
     * @brief Returns the type of the file
     */
    FileFormats::Type get_type()const{return t_;}

protected:

    /**
      *@brief The name of the file to write
      */
    std::string file_name_;

    /**
      *@brief The mark that signifies the beginning of a comment line. The default is #
          */
    char comment_mark_;

    /**
      *@brief The format of the file
      */
    const FileFormats::Type t_;

    /**
     *@brief The stream that holds the file
     */
    std::ofstream file_;
};

}

#endif // FILE_WRITER_BASE_H
