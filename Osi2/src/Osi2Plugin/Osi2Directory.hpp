#ifndef OSI2DIRECTORY_H
#define OSI2DIRECTORY_H

//----------------------------------------------------------------------

#include <string>
#if defined(__GNUC__) || defined(__sun)
# include <dirent.h>
# include <sys/stat.h>
#endif

//----------------------------------------------------------------------

class Path;
  
namespace Directory
{
  // check if a directory exists
  bool exists(const std::string & path);

  // get current working directory
  std::string getCWD();

  // set current working directories
  void setCWD(const std::string & path);

  // Copy directory tree rooted in 'source' to 'destination'
  void copyTree(const std::string & source, const std::string & destination);

  // Remove directory tree rooted in 'path'
  void removeTree(const std::string & path);

  // Create directory 'path' including all parent directories if missing
  void create(const std::string & path);

  /*
    This is problematic. The Entry structure wants to include an apr_finfo_t
    structure (a file info structure; analogous to POSIX stat, according to
    APR comments. This will need to be resolved for MSVC.
  */
    struct Entry  
#   if !(defined(__GNUC__) || defined(__sun))
      : public struct apr_finfo_t
#   endif
  {
    enum Type { FILE, DIRECTORY, LINK };

    Type type;
    std::string path;
  };

  class Iterator
  {
  public:

    Iterator(const Path & path);    
    Iterator(const std::string & path);
    ~Iterator();

    // Resets directory to start. Subsequent call to next() 
    // will retrieve the first entry
    void reset();
    // get next directory entry
    Entry * next(Entry & e);

  private:
    Iterator();
    Iterator(const Iterator &);

    void init(const std::string & path);
  private:
    std::string path_;
  /*
    This will also be problematic. We'll need to sort out the corresponding
    structures in MSVC. Looks like the idea is to allocate a private memory
    pool and use it to maintain a directory structure. Need to check the APR
    win32 side.
  */
#if defined(__GNUC__) || defined(__sun)
    DIR * handle_;
#else
    struct apr_dir_t * handle_;
    struct apr_pool_t * pool_;
#endif    
  };
}


#endif // OSI2DIRECTORY_H


