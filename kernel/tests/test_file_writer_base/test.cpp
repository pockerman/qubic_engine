#include "kernel/utilities/file_writer_base.h"
#include "kernel/utilities/file_formats.h"
#include <gtest/gtest.h>

namespace{

}


/***
 * Test Scenario:   The application attempts to open a file that suffixes don't match
 * Expected Output:	std::logic_error should be thrown
 **/

TEST(FileWriterBase, RunWithSuffixesDontMatch) {

    using kernel::FileWriterBase;
    using kernel::FileFormats;

    /// TODO: Is there a better way to do this?
    try{

        FileWriterBase file_writer("dummy_name.txt", FileFormats::Type::CSV);
        file_writer.open(std::ios_base::out);
    }
    catch(std::logic_error& e){
        std::string msg = e.what();
        ASSERT_EQ(msg, "File already has a suffix but this does not match with what was given txt != csv");
    }
    catch(...){

        ASSERT_FALSE("A non expected exception was thrown for test FileWriterBase::RunWithSuffixesDontMatch");
    }
}


/***
 * Test Scenario:   The application attempts to open a valid file
 * Expected Output:	The file should open properly
 **/

TEST(FileWriterBase, RunWithValidFile) {

    using kernel::FileWriterBase;
    using kernel::FileFormats;

    /// TODO: Is there a better way to do this?
    try{

        FileWriterBase file_writer("dummy_name.csv", FileFormats::Type::CSV);
        file_writer.open(std::ios_base::out);

        ASSERT_TRUE(file_writer.is_open());

    }
    catch(...){

        ASSERT_FALSE("A non expected exception was thrown for test FileWriterBase::RunWithValidFile");
    }
}

/***
 * Test Scenario:   The application attempts to open a file with multiple suffixes
 * Expected Output:	std::logic_error should be thrown
 **/


TEST(FileWriterBase, RunWithMultipleSuffixes) {

    using kernel::FileWriterBase;
    using kernel::FileFormats;

    /// TODO: Is there a better way to do this?
    try{

        FileWriterBase file_writer("dummy_name.txt.csv", FileFormats::Type::CSV);
        file_writer.open(std::ios_base::out);

    }
    catch(std::logic_error& e){
        std::string msg = e.what();
        ASSERT_EQ(msg, "More than one suffixes found in the file name");
    }
    catch(...){

        ASSERT_FALSE("A non expected exception was thrown for test FileWriterBase::RunWithMultipleSuffixes");
    }
}


/***
 * Test Scenario:   The application attempts to open a file with no suffix given at the file name
 * Expected Output:	The file should open properly
 **/


TEST(FileWriterBase, RunWithWithoutSuffix) {

    using kernel::FileWriterBase;
    using kernel::FileFormats;

    /// TODO: Is there a better way to do this?
    try{

        FileWriterBase file_writer("dummy_name", FileFormats::Type::CSV);
        file_writer.open(std::ios_base::out);
        ASSERT_TRUE(file_writer.is_open());
    }
    catch(...){

        ASSERT_FALSE("A non expected exception was thrown for test FileWriterBase::RunWithWithoutSuffix");
    }
}






