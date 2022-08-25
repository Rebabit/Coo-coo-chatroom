#ifndef __LTest_H__
#define __LTest_H__

#include <iostream>
#include <string>
#include <vector>

class LTestCase;
std::vector<LTestCase *> ltest_cases;
#define TESTSUITE(name) \
  class name : public LTestCase { \
    public: \
      name() { \
        ltest_cases.push_back(this); \
        suitename = #name; \
      } 


#define ENDSUITE \
  } caseEntity##__LINE__;


class LTestCase {
  public:
    virtual void execute() = 0;
    void operator() (bool simplify_output = false) {
      test_num = test_pass = test_error = 0;

      std::cout << "\nSuite: " << suitename << std::endl;
      if (!simplify_output) {
        std::cout << "##################" << std::endl;
      }
      
      execute();
      if (!simplify_output)
        std::cout << "##################" << std::endl;

      if (test_num == test_pass) {
        std::cout << "Test Passed" << std::endl;
      }
      else {
        std::cout << "Pass " << test_pass << ", Fail " << test_num - test_error - test_pass;
        std::cout << ", Error " << test_error << std::endl << std::endl;
      }
    }
#define CurPos "( " << __FILE__ << " : " << __LINE__ << " )"

#define ERRMSG(msg)\
  std::cout << "Error " << CurPos << ": " << msg << "." << std::endl; \
  running_result = -1; \
  return;

#define ENV( statements ) \
  try {\
    statements \
  } catch(exception & e) {\
    ERRMSG(e.what()) \
  } catch(std::string & e) {\
    ERRMSG(e) \
  } catch(const char * e) {\
    ERRMSG(e) \
  } catch(int e) {\
    ERRMSG("ErrCode = " << e) \
  } catch(...) {\
    ERRMSG("Unknown Error") \
  }

#define CASE(name) void SUITE_##name()

#define EXE(testname) \
    std::cout << "Running: case=" << #testname << "... " << std::endl; \
    running_result = 0; \
    SUITE_##testname(); \
    test_pass += running_result == 0; \
    test_num++; \
    test_error += running_result == -1;

#define assertTrue(cond) \
  ENV(\
    if (!(cond)) {\
      std::cout << "Assertion Failed" << CurPos << " : suppose " << #cond << " == true, but got false instead." << std::endl; \
      running_result = 1; \
      return; \
    })
  protected:
    int test_num, test_pass, test_error;
    int running_result;
    std::string suitename;
};

int RunAllTests() {
  for (auto c : ltest_cases)  {
    (*c) ();
  }
  return 0;
}

#endif
