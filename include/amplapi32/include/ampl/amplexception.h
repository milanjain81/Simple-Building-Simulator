#ifndef AMPL_AMPLEXCEPTION_H
#define AMPL_AMPLEXCEPTION_H

#include <stdexcept>
#include <string>

#include "ampl/format.h"

namespace ampl {


/**
 * Represent an exception generated by %AMPL
 */
class AMPLException : public std::runtime_error {
public:
  /**
  Constructor
  \param cause Cause of the exception
  */
  AMPLException(fmt::CStringRef cause = fmt::CStringRef(""))
    : std::runtime_error(cause.c_str()) {
    line_ = -1;
    offset_ = -1;
  }
  /**
  Constructor for %AMPL errors
  \param filename Name of the file in which the exception occurred
  \param row Row where the error is detected
  \param offset Offset where the error is detected
  \param message Message to be embedded in the exception
  */
  AMPLException(fmt::CStringRef filename, int row, int offset,
    fmt::CStringRef message)
    : std::runtime_error(getWhat(filename.c_str(), row, offset, message.c_str())),
    filename_(filename.c_str()),
    message_(message.c_str()) {
    line_ = row;
    offset_ = offset;
  }
  /**
  Here to avoid the error looser throw specifier for virtual
  AMPLException::~AMPLException()
  in some old c++ compilers
  */
  ~AMPLException() throw() {}
  /**
  Get the name of the file where the error was detected
  */
  const std::string &getSourceName() const { return filename_; }
  /**
  Get the row where the error is located
  */
  int getLineNumber() const { return line_; }
  /**
  Get the offset where the error is located
  */
  int getOffset() const { return offset_; }
  /**
  Get the error message
  */
  const std::string &getMessage() const { return message_; }

private:
  std::string filename_;
  int line_;
  int offset_;
  std::string message_;

  static std::string getWhat(fmt::StringRef filename, int row, int offset,
    fmt::StringRef message) {
    fmt::MemoryWriter out;
    if (filename.size() > 0) out << "Filename: " << filename << " ";
    out << "Row: " << row << " Offset: " << offset << "\n";
    out << message << "\n";
    return out.str();
  }
};
}
#endif
