#ifndef __TYPEINFO_WRAPPER__
#define __TYPEINFO_WRAPPER__
#include <typeinfo.h>
#include "deffer_excpt.h"
namespace sql_base
{
// type info wrapper
  class typeinfo_wrapper
  {
  public:
    typeinfo_wrapper(const typeinfo_wrapper &);
    typeinfo_wrapper(const std::type_info& ti_);
    bool operator == (typeinfo_wrapper &);
    bool operator != (typeinfo_wrapper &);
    bool operator  > (typeinfo_wrapper &);
    friend
    bool
    operator  < (const typeinfo_wrapper &t_left_wrapp_ ,
                 const typeinfo_wrapper &t_right_wrapp_);
    const typeinfo_wrapper&
    operator  = (const typeinfo_wrapper &);
  private:
    std::type_info *type_info_;
  };
};

#endif