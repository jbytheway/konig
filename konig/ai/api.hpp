#ifndef KONIG_AI_API_HPP
#define KONIG_AI_API_HPP

#if defined(_MSC_VER)
  /* The __declspec stuff for ensuring symbols are exported from DLLs and
   * imported back into libraries */
  #ifdef KONIG_EXPORTS
    #define KONIG_AI_API __declspec(dllexport)
  #else
    #define KONIG_AI_API __declspec(dllimport)
  #endif //KONIG_EXPORTS
#else
  #if defined(__GNUC__) && (__GNUC__ >= 4)
    #define KONIG_AI_API __attribute__ ((visibility ("default")))
  #else
    #define KONIG_AI_API
  #endif
#endif

#endif // KONIG_AI_API_HPP

