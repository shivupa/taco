#ifndef TACO_BACKEND_C_H
#define TACO_BACKEND_C_H

#include <map>

#include "ir.h"
#include "ir_printer.h"

namespace taco {
namespace internal {

class Module {
public:
  /** Create a module for some source code */
  Module(std::string source);

  /** Compile the source into a library, returning
   * its full path */
  std::string compile();
  
  /** Get a function pointer to a compiled function.
   * This returns a void* pointer, which the caller is
   * required to cast to the correct function type before
   * calling.
   */
  void *get_func(std::string name);
  
  /** Call a function in this module and return the result */
  template <typename... Args>
  int call_func(std::string name, Args... args) {
    typedef int (*fnptr_t)(Args...);
    fnptr_t func_ptr = (fnptr_t)get_func(name);
    return func_ptr(args...);
  }
  
private:
  std::string source;
  std::string libname;
  std::string tmpdir;
  void* lib_handle;
  
  void set_libname();
};

class CodeGen_C : public IRPrinter {
public:
  /** Initialize a code generator that generates code to an
   * output stream.
   */
  CodeGen_C(std::ostream &dest);
  ~CodeGen_C();
  
  /** Compile a lowered function */
  void compile(const Function* func);

  static std::string gen_unique_name(std::string var_name="");
  
protected:
  void visit(const Var*);
  void visit(const For*);
  void visit(const While*);
  void visit(const Block*);
  
  bool func_block;
  std::string func_decls;
  
  static int unique_name_counter;
  
  std::map<Expr, std::string, ExprCompare> var_map;
  std::ostream &out;

};

} // namespace internal
} // namespace taco
#endif