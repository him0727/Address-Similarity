#include "../OutOfRangeException.h"

OutOfRangeException::OutOfRangeException(std::string property, float value) {
  message = "OutOfRangeException: " + property + (value < 0 ? " is less than 0" : " is greater than 1");
}

OutOfRangeException::OutOfRangeException(std::string property, int value) {
  message = "OutOfRangeException: " + property + " is less than 1";
}