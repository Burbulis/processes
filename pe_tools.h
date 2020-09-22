#pragma once
#include "memorizer.hpp"


struct get_pe_ptr
{
  get_pe_ptr(PBYTE img_base)
  {
    base_ = img_base;
    dos_header = reinterpret_cast<PIMAGE_DOS_HEADER>(img_base);
  }

  PBYTE get(void)
  {
    return (base_ + dos_header->e_lfanew);
  }
private:
  PBYTE base_;
  PIMAGE_DOS_HEADER dos_header;
};


struct section_tokens
{
  section_tokens(std::string name, PIMAGE_SECTION_HEADER p_section):_name(name), _p_section(p_section){}
  std::string _name;
  PIMAGE_SECTION_HEADER _p_section;
};


template
<
  typename PIMAGE_NT_TYPE
>
struct GetPeObject
{

  GetPeObject(BYTE* _Image) :p_ntimage(reinterpret_cast<PIMAGE_NT_TYPE>(_Image)) 
  {
    sections_count = p_ntimage->FileHeader.NumberOfSections;
  }
  
  PIMAGE_SECTION_HEADER
  get_section(std::string _name) 
  {
    p_section = IMAGE_FIRST_SECTION(p_ntimage);
    for (size_t i = 0; i < sections_count; ++i)
    {
      if (
        !strncmp(_name.c_str(), reinterpret_cast<char *>(p_section->Name), _name.length())
        )
      {
        break;
      }
      ++p_section;
    }
    return (p_section);
  }

  void
    import()
  {
    PIMAGE_DATA_DIRECTORY pide = &p_ntimage->OptionalHeader.DataDirectory[0];
    if (pide->VirtualAddress)
    {
      printf("import");

    }
  }


private:
  size_t sections_count;
  PIMAGE_NT_TYPE p_ntimage;
  PIMAGE_SECTION_HEADER p_section;
};


