#include "sql_helper.h"

void
sql_helper::
to_sql_str(__in std::string tmp_str,__out std::vector<SQLCHAR>& tmp_)
{
 // std::vector<SQLCHAR> tmp_;
  tmp_.resize(tmp_str.length());
  memset(&tmp_[0], 0, tmp_.size());
  memcpy(&tmp_[0], tmp_str.c_str(), tmp_str.length());
  return ;
}

void
sql_helper::
insert_to_list(
  size_t pid, std::string md5, 
  std::string imagename,
  std::string path, 
  std::string image_cmdline
)
{
   sql_obj.bind_reset();
  SQLINTEGER _pid = static_cast<SQLINTEGER>(pid);
  sql_obj.sql_bind_param( _pid, 1);
  
  size_t sz_imagename = imagename.length();
  std::vector<SQLCHAR> tmp_image_name;
  to_sql_str(imagename, tmp_image_name);
  
  sql_obj.sql_bind_param( reinterpret_cast<SQLCHAR *>(&tmp_image_name[0]), 2 ,&sz_imagename);


  if (image_cmdline.empty())
  {
    image_cmdline = imagename;
  }
  else
  {
    __asm {nop}
  }

  std::vector<SQLCHAR> tmp_cmd_line;
  to_sql_str(image_cmdline, tmp_cmd_line);
  size_t sz_cmd_line = tmp_cmd_line.size();
  if (sz_cmd_line > 290)
  {
	  __asm {nop}
  }
  sql_obj.sql_bind_param( reinterpret_cast<SQLCHAR *>(&tmp_cmd_line[0]) , 3 ,&sz_cmd_line);

  if (path.empty())
  {
    path = imagename;
  }


  std::vector<SQLCHAR> tmp_path;
  to_sql_str(path, tmp_path);
  size_t tmp_path_sz_ = path.length();
  sql_obj.sql_bind_param(reinterpret_cast<SQLCHAR *>(&tmp_path[0])  , 4, &tmp_path_sz_);


  if (md5.empty())
  {
    md5 = "00000000000000000000000000000000";
  }
  
  
  std::vector<SQLCHAR> tmp_md5;
  to_sql_str(md5, tmp_md5);
  size_t tmp_md5_sz_ = md5.length();
  sql_obj.sql_bind_param(reinterpret_cast<SQLCHAR *>(&tmp_md5[0]), 5, &tmp_md5_sz_);



  sql_obj.prepare("INSERT INTO list(pid,imagename,cmdstr,path,md5,time) VALUES(?,?,?,?,?,NOW())");
  try
  {
    sql_obj.execute();
  }
  catch (sql_base::sql_exceptions& excpt)
  {
    printf("what:%s\n", excpt.what());

    getc(stdin);
  }
}

