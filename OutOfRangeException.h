#ifndef _ADDRESSSIMILARITY_ASEXCEPTIONS_OUTOFRANGEEXCEPTION_H
#define _ADDRESSSIMILARITY_ASEXCEPTIONS_OUTOFRANGEEXCEPTION_H
#include <iostream>
#include <exception>
#include <string>

/*
 * This header file declares the public methods for OutOfRangeException class, which is intended to be thrown by AddressSimilarity class. 
 * This exception is explicitly for the invalid range of arguments of AddressSimilarity when setting them, preventing from 
 * instantizing AddressSimilarity class with wrong weights and causing runtime error and wrong results. 
 * For the more details, refer to ASExceptions/OutOfRangeException.cpp and AddressSimilarity.h.
 */
class OutOfRangeException : public std::exception {

  private:
    std::string message;
  
  public:
    OutOfRangeException(std::string property, int value);
    
    OutOfRangeException(std::string property, float value);
    
    virtual const char* what() const throw () {
      return message.c_str();
    }
  
};

#endif