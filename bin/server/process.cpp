#include "process.hpp"

#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

#include <limits>
#include <cstring>
#include <iostream>

#include <boost/system/system_error.hpp>

namespace konig { namespace server {

Process::Process() :
  pid_(0)
{}

static char* new_c_str(std::string const& s)
{
  char* const c_str = new char[s.size() + 1];
  std::memcpy(c_str, s.c_str(), s.size() + 1); // nothrow
  return c_str;
}

Process::Process(
    boost::filesystem::path const& exe,
    std::vector<std::string> const& args
  )
{
  if (args.size() >= size_t(std::numeric_limits<int>::max()-1)) {
    throw std::runtime_error("too many args");
  }
  pid_t child = fork();
  if (child == -1) {
    throw boost::system::system_error(
        errno, boost::system::get_system_category()
      );
  }
  if (child != 0) {
    pid_ = child;
    return;
  }
  // We are in the child process here
  int const argc = args.size() + 1;
  char** const argv = new char*[argc+1];
  std::fill(argv, argv+argc+1, static_cast<char*>(NULL)); // nothrow
  try {
    // Program name
    argv[0] = new_c_str(exe.filename());
    // Args
    for (int i=1; i<argc; ++i) {
      argv[i] = new_c_str(args[i-1]);
    }
    execv(exe.file_string().c_str(), argv);
    // execv failed if it returns
    std::perror(("execv: "+exe.file_string()).c_str());
    std::exit(1);
  } catch (...) {
    // Extreme badness!
    std::cerr << "Child process suffered fatal error\n";
    std::exit(1);
  }
}

Process::Process(Process&& p)
{
  reset();
  swap(p);
}

Process& Process::operator=(Process&& p)
{
  swap(Process(std::move(p)));
  return *this;
}

Process::~Process()
{
  // BUG: This could kill some other process if the one we generated is long
  // gone and has been replaced.  Probably this shouldn't happen because the
  // children will hang around as zombies indefinitely
  if (pid_) {
    assert(pid_>0);
    int r = kill(pid_, SIGTERM);
    if (r) {
      // Badness, but this is a destructor so we can't throw
      // Most likely kill failed because the process was already dead
      std::perror("~Process: kill");
    }
    // Clean up zombie
    r = waitpid(pid_, NULL, 0);
    if (r == -1) {
      std::perror("~Process: waitpid");
    }
  }
}

void Process::reset()
{
  swap(Process());
}

void Process::reset(
    boost::filesystem::path const& exe,
    std::vector<std::string> const& args
  )
{
  swap(Process(exe, args));
}

void Process::swap(Process& p)
{
  using std::swap;
  swap(pid_, p.pid_);
}

void Process::swap(Process&& p)
{
  using std::swap;
  swap(pid_, p.pid_);
}

}}

