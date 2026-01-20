

#include "fsname_detail.h"
#include "../hstr.h"
#include "../htodoholder.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <libgen.h>


namespace HUICPP {


namespace detail {

// non-named namespace 
namespace {

    // get file status
    int get_file_stat(HCSTRR strFilePathName, struct stat& buf) {
        
        ZERO_MEM(&buf, sizeof(struct stat));
        return ::stat(strFilePathName.c_str(), &buf);        
        
    }

}


fsname& fsname::operator=(const fsname& other) noexcept { 

    SetName(other.GetName()); 
    return *this; 
    
}


bool fsname::Exists() const noexcept {

    struct stat _st;
    return get_file_stat(m_name, _st) == 0;

}


bool fsname::IsDir() const noexcept {

    struct stat _st;

    // file is not exists.
    if (get_file_stat(m_name, _st) != 0) {
        return false;
    }

    return (_st.st_mode & S_IFMT) == S_IFDIR;
}


bool fsname::IsFile() const noexcept {

    struct stat _st;
    
    // file is not exists.
    if (get_file_stat(m_name, _st) != 0) {
        return false;
    }

    return (_st.st_mode & S_IFMT) == S_IFREG;

}


HRET fsname::Symlink(const string_type& strDstName) const noexcept {

    HASSERT_RETURN(::symlink(m_name.c_str(), strDstName.c_str()) == 0, SYS_FAILED);

    HRETURN_OK;

}


HRET fsname::Symlink(const fsname& dst_file) const noexcept {

    return Symlink(dst_file.m_name);

}


HRET fsname::Remove() const noexcept {

    HASSERT_RETURN(::remove(m_name.c_str()) == 0, SYS_FAILED);
    HRETURN_OK;

}


HRET fsname::Rename(const string_type& strDstName) noexcept {

    /*
        We need to make sure the file is renamed before assign the m_name to the new name.
    */
    HASSERT_RETURN(::rename(m_name.c_str(), strDstName.c_str()) == 0, INVL_PARA);
    m_name = strDstName;
    HRETURN_OK;

}


bool fsname::IsAbsolute() const noexcept {

    return isRootBegin();

}


bool fsname::IsRelative() const noexcept {

    return not IsAbsolute();

}


fsname& fsname::SetAsCurrent () noexcept {

    HPATH_BUF buf = {0};
    ::getcwd(buf, HPATH_LEN);
    m_name.assign(buf);
    return *this;

}


fsname fsname::GetParentPath() const noexcept {
    
    fsname res(m_name);
    if (not res.IsAbsolute()) {
        res.SetAsAbsolute();
    }

    if (res.isRootEnd()) {
        HStr::RemoveRight(res.m_name, 1);
    }

    res.m_name.assign(::dirname(const_cast<HSZ>(res.m_name.c_str())));
    return res;

}


fsname& fsname::SetAsAbsolute() noexcept {

    if (IsAbsolute()) {
        return *this;
    }

    if (IsJustOnlyFileName()) {
        // just only filename, we return "current-direction / filename" as path
        m_name = fsname().SetAsCurrent().Append(m_name).m_name;
        return *this;
    }

    HPATH_BUF buf = {0};
    ::realpath(m_name.c_str(), buf);
    
    m_name.assign(buf);
    return *this;

}


fsname& fsname::Append (const string_type& new_path) noexcept {
    
    // return if the new_path is null.
    if (new_path.empty()) {
        return *this;
    }

    // if m_name is null-string, setas current working directory.
    if (m_name.empty()) {
        SetAsCurrent();
    }

    string_type newpath = new_path;
    if (IsRootBegin(newpath)) {
        HStr::RemoveLeft(newpath, 1);
    }
    if (IsRootEnd(m_name)) {
        HStr::RemoveRight(m_name, 1);
    }

    m_name.push_back(preferred_separator);
    m_name.append(newpath);

    return *this;
}


HRET fsname::CreateDirectory(HUN modes) const noexcept {

    SYS_T ret = ::mkdir(m_name.c_str(), modes);
    HASSERT_RETURN(ret == 0 or errno == EEXIST, SYS_FAILED);
    HRETURN_OK;

}


fsname::filesize_t fsname::GetFileSize() const noexcept{

    struct stat buf;
    ZERO_MEM(&buf, sizeof(struct stat));
    if (::stat(m_name.c_str(), &buf) != 0) {
        return 0;
    }
    return static_cast<filesize_t>(buf.st_size);

}


fsname::string_type fsname::FileName() const noexcept {
    
    HCSZ pos = strrchr(m_name.c_str(), preferred_separator);
    if (pos == nullptr) {
        return m_name;
    }

    return string_type(++pos);
}


fsname::string_type fsname::FileNameOnly() const noexcept {

    string_type strFileName = FileName();
    HCSZ pos = strrchr(strFileName.c_str(), dos_elem);
    if (pos == nullptr) {
        return strFileName;
    } 

    return string_type(strFileName.c_str(), pos);
}


fsname::string_type fsname::GetFileExternName() const noexcept {

    HCSZ po_pos = strrchr(m_name.c_str(), dos_elem);
    if (po_pos == nullptr) {
        return string_type();
    }

    HCSZ root_pos = strchr(m_name.c_str(), preferred_separator);
    if (po_pos < root_pos) {
        return string_type();
    }

    return string_type(++po_pos);

}


bool fsname::IsJustOnlyFileName() const noexcept {

    return m_name.find(preferred_separator) == string_type::npos;

}


bool fsname::IsRootBegin(const string_type& strPath) noexcept {

    if (strPath.empty()) {
        return false;
    }

    return strPath[0] == preferred_separator;
}


bool fsname::IsRootEnd(const string_type& strPath) noexcept {

    if (strPath.empty()) {
        return false;
    }

    return strPath[strPath.length() - 1] == preferred_separator;
}


}

}