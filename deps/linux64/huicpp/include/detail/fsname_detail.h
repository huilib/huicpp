/*
 * Created: Joseph Hui Sun Sep 26 2021
 * Description: 
  		Wrapper for huicpp file system detail.
*/

#ifndef __H_HUICPP_DETAIL_FSNAME_H__
#define __H_HUICPP_DETAIL_FSNAME_H__


#include "../huicpp.h"

namespace HUICPP {

namespace detail {

/*
    a name refer to a file system item.
*/
class fsname {
public:
    using value_type = HCH;
    using string_type = std::basic_string<value_type>;
    using filesize_t = HULL;
    static constexpr const value_type preferred_separator = '/';
    static constexpr const value_type dos_elem = '.';

public:
    /*
        Construction function. 
    */
    fsname() noexcept { }

    explicit fsname(const string_type& strpath) noexcept
        : m_name(strpath) { }

    fsname(const fsname& other) noexcept
        : m_name(other.m_name) { }

    fsname(fsname&& other) noexcept 
        : m_name(std::move(other.m_name)) { other.Clear(); }

    virtual ~ fsname() noexcept {  }

    fsname& operator=(const fsname& other) noexcept;

public:
    const string_type& GetName() const noexcept { return m_name; }

    void SetName (const string_type& _path) noexcept { m_name = _path; }

    void Clear() noexcept { m_name.clear(); }

public:
    /*
        check exists.
    */
    bool Exists() const noexcept;

    /*
        check whether is direction;
    */
    bool IsDir() const noexcept;

    /*
        check whether is regular file;
    */
    bool IsFile() const noexcept;

    /*
        make a new name for a file.
    */
    HRET Symlink(const string_type& strDstName) const noexcept;
    
    HRET Symlink(const fsname& dst_file) const noexcept;

    /*
        Remove a file or directories.
    */
    HRET Remove() const noexcept;

    /*
        Rename a file.
    */
    HRET Rename(const string_type& strDstName) noexcept;

    /*
        Whether is an absolute dirname.
    */
    bool IsAbsolute() const noexcept;

    /*
        Whether is a relative dirname.
    */
    bool IsRelative() const noexcept;

    /*
        Set as current working directory.
    */
    fsname& SetAsCurrent() noexcept;

    /*
        Get the parent directory.
    */
    fsname GetParentPath() const noexcept;    


    fsname& SetAsAbsolute() noexcept;

    /*
        append path.
    */
    fsname& Append(const string_type& new_path) noexcept;

    /*
        create directory.
    */
    HRET CreateDirectory(HUN modes = 0766) const noexcept;


    /*
        get file length.
    */
    filesize_t GetFileSize() const noexcept;


    string_type FileName() const noexcept;


    string_type FileNameOnly() const noexcept;


    string_type GetFileExternName() const noexcept;

    
    bool IsJustOnlyFileName () const noexcept;
public:
    static bool IsRootBegin(const string_type& strPath) noexcept;

    static bool IsRootEnd(const string_type& strPath) noexcept;

protected:
    bool isRootBegin() const noexcept {
        return IsRootBegin(m_name);
    }

    bool isRootEnd() const noexcept {
        return IsRootEnd(m_name);
    }
    
private:
    string_type m_name;
};


}

}

#endif // __H_HUICPP_DETAIL_FSNAME_H__