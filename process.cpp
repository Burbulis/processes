// time_experiment.cpp: определяет точку входа для консольного приложения.
//


//#include <sstream>
//#include <windows.h>
#include "proc_mini_lib.h"
//#include "sql_helper.h"
//#include <tchar.h>




int main()
{

 // sql_helper sql_hlp;
 
	GetProcessSeq::PList list;
	GetProcessSeq gps;
	gps.get(list);
//	std::string res_ = GetProcessName(list[131].Uid);
 // GetProcessBasicInfo(list[130].Uid);
//	printf("res = %s\n", res_.c_str());
//	res_ = GetProcessName(list[141].Uid);
//	printf("res = %s\n", res_.c_str());
//	res_ = GetProcessName(list[41].Uid);
//	printf("res = %s\n", res_.c_str());
  for (size_t i = 0; i < list.size(); ++i) {
	  printf("name:%s pid=%d\n", list[i].ProcessName.c_str(),list[i].Uid);
    GetProcessBasicInfo pbi(list[i]);
    _Process p_ = pbi.get();
    if (!p_.image_path.empty())
    {
	//explorer	hash_in_disk = "ed6b4c95e2a6d67480b9dbb8a8e7d9b4"
      __asm {nop}
    }
	if (p_.commandline.length()>200)
	{
		__asm {nop}
	}

   // sql_hlp.insert_to_list(list[i].Uid,p_.hash_in_disk,p_.ProcessName,p_.image_path ,p_.commandline);

  }
	getc(stdin);
	return 0;
}

