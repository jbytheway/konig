#ifndef KONIG_SERVER_PROCESS_HPP
#define KONIG_SERVER_PROCESS_HPP

#include <vector>

#include <boost/filesystem/path.hpp>

namespace konig { namespace server {

class Process : boost::noncopyable {
  public:
    Process();
    Process(
        boost::filesystem::path const&,
        std::vector<std::string> const& args
      );
    Process(Process&&);
    Process& operator=(Process&&);
    ~Process();
    void reset();
    void reset(
        boost::filesystem::path const&,
        std::vector<std::string> const& args
      );
    void swap(Process&);
    void swap(Process&&);
  private:
    pid_t pid_;
};

inline void swap(Process& l, Process& r) {
  l.swap(r);
}

}}

#endif // KONIG_SERVER_PROCESS_HPP

