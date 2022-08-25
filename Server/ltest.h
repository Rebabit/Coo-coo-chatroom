#ifndef __LTest_H__
#define __LTest_H__

#include <iostream>
#include <string>
#include <vector>
#include <cstdio>

class LTestCase;

struct TestSuiteResult {
    int test_num, test_error, test_fail, test_pass;
    TestSuiteResult& operator += (TestSuiteResult & other) {
        test_num += other.test_num, test_fail += other.test_fail;
        test_error += other.test_error, test_pass += other.test_pass;
        return *this;
    }
};

std::vector<LTestCase *> ltest_cases;

#define TESTSUITE(name) \
class name : public LTestCase { \
public: \
    name() { \
        ltest_cases.push_back(this); \
        suitename = #name; \
    } \
    ~ name() {\
        dtor(); \
    }


#define ENDSUITE(name) \
} caseEntity##name;

#define CurPos "(" << __FILE__ << ":" << __LINE__ << ")"


#define ERRMSG(msg)\
    std::cout << "#... Error " << CurPos << ": " << msg << "." << std::endl; \
    running_result = -1; \
    return;

#define ENV( statements ) \
    try {\
        statements \
    } catch(std::exception & e) {\
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
    std::cout << "# Running: case=" << #testname << "... " << std::endl; \
    running_result = 0; \
    SUITE_##testname(); \
    test_pass += running_result == 0; \
    test_num++; \
    test_error += running_result == -1;

#define assertTrue_1(cond) \
ENV(\
    if (!(cond)) {\
        std::cout << "#... Assertion Failed" << CurPos << " : suppose " << #cond << " == true, but got false instead." << std::endl; \
        running_result = 1; \
        return; \
    })

#define assertTrue_2(cond, printable) \
ENV(\
    if (!(cond)) {\
        std::cout << "#... Assertion Failed" << CurPos << " : suppose " << #cond << " == true, but got false instead." << std::endl; \
        running_result = 1; \
        if (printable) \
            std::cout << "#...... Info: " << printable << std::endl; \
        return; \
    })
#define GET_MACRO_AssertTrue(_1,_2,NAME,...) NAME
#define assertTrue(...) GET_MACRO_AssertTrue(__VA_ARGS__, assertTrue_2, assertTrue_1)(__VA_ARGS__)

#define assertEqual_3(a, b, printable)\
ENV(\
    if ((a) != (b)) {\
        std::cout << "#... Assertion Failed" << CurPos <<": " \
                    << #a << " != " << #b << std::endl;	\
        running_result = 1;\
        std::cout << "#...... info: " << printable << std::endl; \
        return;\
    } \
)

#define assertEqual_2(a, b)\
ENV(\
    if ((a) != (b)) {\
        std::cout << "#... Assertion Failed" << CurPos <<": " \
                    << #a << " != " << #b << std::endl;	\
        running_result = 1;\
        return;\
    } \
)
#define GET_MACRO_AssertEqual(_1,_2,_3,NAME,...) NAME
#define assertEqual(...) GET_MACRO_AssertEqual(__VA_ARGS__, assertEqual_3, assertEqual_2)(__VA_ARGS__)

#define assertNoException(statement) \
ENV(					\
    statement;			\
    running_result = 0;	\
)

#define assertException(statement) 	\
ENV(								\
    running_result = 1;				\
    try {							\
        statement;					\
    } catch (...) {					\
        running_result = 0; 		\
    } 								\
    if (running_result) {			\
        std::cout << "Assertion Failed" << CurPos << ": expect exception, but not" << std::endl; \
    } 								\
)

#define testlog(msgStr, ...) \
    std::cout << "#... LOG " << CurPos << " ";\
    printf(msgStr "\n", ##__VA_ARGS__)

class LTestCase {
public:
    virtual void execute() = 0;

    virtual void ctor() {}
    virtual void dtor() {}

    TestSuiteResult operator() (bool simplify_output = true) {
        test_num = test_pass = test_error = 0;
        
        
        std::cout << "\nSuite: " << suitename << std::endl;
        if (!simplify_output) {
            std::cout << "##################" << std::endl;
        }
      
        ctor();
        execute();
        if (!simplify_output)
            std::cout << "##################" << std::endl;
        
        if (test_num == test_pass) {
            std::cout << test_pass << " Tests Passed In Total" << std::endl;
        }
        else {
            std::cout << "Pass " << test_pass << ", Fail " << test_num - test_error - test_pass;
            std::cout << ", Error " << test_error << std::endl;
        }
        return { test_num, test_error, test_num - test_pass - test_error, test_pass };
    }

protected:
    int test_num, test_pass, test_error;
    int running_result;
    std::string suitename;
};

int RunAllTests() {
    TestSuiteResult sum {0, 0, 0, 0};
    for (auto c : ltest_cases)  {
        TestSuiteResult result = (*c) ();
        sum += result;
    }

    std::cout << std::endl << "## Run All Test: Test Result ##" << std::endl;
    std::cout << "Pass = " << sum.test_pass << ", Fail = " << sum.test_fail;
    std::cout << ", Error = " << sum.test_error << std::endl;
    std::cout << std::endl << sum.test_num << " test cases (" << ltest_cases.size();
    std::cout << " suites) executed in total." << std::endl;
    std::cout << "Test Completed !" << std::endl;

    return 0;
}

#endif
