#include "typeinfo_wrapper.h"
namespace sql_base
{
  bool 
  operator  < (const typeinfo_wrapper &t_left_wrapp_ , 
               const typeinfo_wrapper &t_right_wrapp_)
  {
    if ( NULL == t_left_wrapp_.type_info_ )   
      EXCEPT_GENERATOR_I( "NULL_POINTER" ); 
    if ( NULL == t_right_wrapp_.type_info_ )   
      EXCEPT_GENERATOR_I( "NULL_POINTER" ); 
    return (
      ((*t_left_wrapp_.type_info_).before( *t_right_wrapp_.type_info_ ))?true:false
        );
  }
//----------------------------------------------------------
  typeinfo_wrapper::
  typeinfo_wrapper(const typeinfo_wrapper &t_wrapp_)
  {
    type_info_ = t_wrapp_.type_info_; 
  }
//----------------------------------------------------------
  typeinfo_wrapper::
  typeinfo_wrapper(const std::type_info& ti_)
  {
    type_info_ = const_cast<std::type_info *>(&ti_);
  }
//----------------------------------------------------------
  bool 
  typeinfo_wrapper::
  operator == ( typeinfo_wrapper &t_wrapp_ )
  {
    if ( NULL == t_wrapp_.type_info_ )   
      EXCEPT_GENERATOR_I( "NULL_POINTER" ); 
    if ( NULL == type_info_ )   
      EXCEPT_GENERATOR_I( "NULL_POINTER" ); 
    return (
            ((*type_info_) == (*t_wrapp_.type_info_))?true:false
        );
  }
//----------------------------------------------------------
  bool 
  typeinfo_wrapper::
  operator != ( typeinfo_wrapper &t_wrapp_ )
  {
    if ( NULL == t_wrapp_.type_info_ )   
      EXCEPT_GENERATOR_I( "NULL_POINTER" ); 
    if ( NULL == type_info_ )   
      EXCEPT_GENERATOR_I( "NULL_POINTER" ); 
    return (((*type_info_) != (*t_wrapp_.type_info_))?true:false);
  }
//----------------------------------------------------------
  bool 
  typeinfo_wrapper::
  operator  > ( typeinfo_wrapper &t_wrapp_ )
  {
    if ( NULL == t_wrapp_.type_info_ )   
      EXCEPT_GENERATOR_I( "NULL_POINTER" ); 
    if ( NULL == type_info_ )   
      EXCEPT_GENERATOR_I( "NULL_POINTER" ); 
    return (  ((*type_info_) != ( *t_wrapp_.type_info_ ))&&
              (!(*type_info_).before( *t_wrapp_.type_info_ ))
            );
  }
//----------------------------------------------------------
  const
  typeinfo_wrapper&
  typeinfo_wrapper::
  operator  = ( const typeinfo_wrapper &t_wrapp_ )
  { 
    type_info_ = t_wrapp_.type_info_;
    return (*this);
  }
//----------------------------------------------------------
};