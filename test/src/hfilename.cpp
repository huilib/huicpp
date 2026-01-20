

#include "hfilename.h"
#include <dirent.h>
#include "htodoholder.h"

namespace HUICPP {


HFileNameBase::HFileNameBase(HFileNameBase&& fname) noexcept 
	: base_class(std::move(fname)) {


}


HDirection& HDirection::SetAsCurrent() noexcept {

	base_class::SetAsCurrent();
	
	return *this;

}


void HDirection::GoToParent () noexcept {

	SetName(base_class::GetParentPath().GetName());

}


HDirection HDirection::GetParent () const noexcept {

	return HDirection (base_class::GetParentPath().GetName());

}


HRET HDirection::CreateDic() const noexcept {

	return base_class::CreateDirectory();

}


HFileName HDirection::GetDicFile(const string_type& strFileName) const noexcept {

	return HFileName(GetName()).Append(strFileName);

}


void HDirection::GetSubFiles (std::vector<string_type>& fs, std::vector<string_type>& ds) const {
	
	DIR* dir_ptr = opendir(GetName().c_str());

    if (dir_ptr == nullptr) {
        return;
    }

	HSCOPE_EXIT { closedir(dir_ptr); };

    struct dirent* deptr = nullptr;

    while((deptr = readdir(dir_ptr)) != nullptr) {

        if (deptr->d_type & DT_REG) {
            fs.push_back(deptr->d_name);
        }

        if (deptr->d_type & DT_DIR) {
            ds.push_back(deptr->d_name);
        }
		
    }

}


HFileName& HFileName::Append (const string_type& new_path) noexcept {

	base_class::Append(new_path);

	return *this;

}


HDirection HFileName::GetParentDic () const noexcept {

	return HDirection (base_class::GetParentPath().GetName());

}


HFileName::string_type HFileName::GetExtenName () const noexcept {

	return base_class::GetFileExternName();

}


HFileName::string_type HFileName::JustFileName () const noexcept {

	return base_class::FileNameOnly();

}


}
