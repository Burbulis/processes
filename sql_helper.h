#include "sql_connector.h"

class sql_helper
{
public:
  sql_helper()
  {
    sql_base::sql_init(sql_obj, "localhost", "root", "1234554321", "mysql", 1000);
    sql_obj.prepare("use proc_lists");
    sql_obj.execute();
  }

  void
    to_sql_str(__in std::string tmp_str, __out std::vector<SQLCHAR>& tmp_);


  void  
  insert_to_list (
    size_t pid, std::string md5,std::string imagename, std::string path, std::string image_cmdline);

private:
  sql_base::sql sql_obj;

  
 };
