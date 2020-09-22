#include "stdafx.h"
#include "deffer_excpt.h"
namespace sql_base
{ 
  void
  except( long line_ , const std::string &file_ ,
         const std::string &func_
      )
  {
    std::ostringstream stream_;
    stream_ << "excpt throw [file:" << file_ << " func: " << func_ << " line: "
       << line_ <<  "]" << std::endl;
    throw (std::logic_error(stream_.str())); 
   }  
}

void
dbg_message(sql_base::sql_exceptions t,
            std::string file_,size_t prj_line,
            std::string& meth_name
            )
{
  if (100 == t.code())
  {
    //throw(t);   
    return;
  }
  std::ostringstream dbg_msg;           
  dbg_msg <<  file_ 
         << ":" 
         << meth_name 
         << ":" 
         << prj_line 
         << ":" 
         << t.what(); 
  MessageBox(NULL,dbg_msg.str().c_str(),"DEBUG_MESSAGE",NULL);
}