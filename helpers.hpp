#pragma once
#include <string>
namespace helpers
{


  template
    <
      typename T
    >
  std::string
  to_str(const T& container)
  {
    std::string str;
    str.insert(str.begin(), container.begin(), container.end());
    return (str);
  }




}