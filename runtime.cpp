/*
 * @Description: runtime lib of MC/DC coverage pass
 * @Author: Fishermanykx
 * @Date: 2021-04-12 21:52:34
 * @LastEditors: Fishermanykx
 * @LastEditTime: 2021-05-27 16:05:50
 */

#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>

// #define T 1   // True
// #define F 0   // False
// #define N -1  // NULL

using namespace std;

int totalCondNum = 0;
int first_scan = 1;
string terminLabel = "if.end";
string thenLabel = "if.then";

map<string, vector<vector<string> > > nameTable;
// map<string, vector<vector<string> > > execCondTabe;
map<string, vector<map<string, vector<int> > > > condTable;

bool hasSubStr(string s, string pat) { return !(s.find(pat) == string::npos); }

void printNameTable() {
  cout << "---------------------------" << endl;
  for (map<string, vector<vector<string> > >::iterator it = nameTable.begin();
       it != nameTable.end(); ++it) {
    cout << "In function " << it->first << ":\n";
    vector<vector<string> > funcDecision = it->second;
    for (vector<vector<string> >::iterator vv_it = funcDecision.begin();
         vv_it != funcDecision.end(); ++vv_it) {
      vector<string> singleDecision = *vv_it;
      cout << "single decision: ";
      for (vector<string>::iterator v_it = singleDecision.begin();
           v_it != singleDecision.end(); ++v_it) {
        cout << *v_it << "\t";
      }
      cout << endl;
    }
    cout << "---------------------------" << endl;
  }
}

void printCondTable() {
  cout << "---------------------------" << endl;
  cout << "Cond Table" << endl;
  cout << endl;

  for (map<string, vector<map<string, vector<int> > > >::iterator it =
           condTable.begin();
       it != condTable.end(); ++it) {
    cout << "In function " << it->first << ":" << endl;
    cout << endl;

    vector<map<string, vector<int> > > singFunc = it->second;
    int size = singFunc.size();
    // cout << size << endl;
    for (int i = 0; i < size; ++i) {
      cout << "single decision: \n";
      map<string, vector<int> > m_cond = singFunc[i];
      for (map<string, vector<int> >::iterator m_it = m_cond.begin();
           m_it != m_cond.end(); ++m_it) {
        const char* bb_name = m_it->first.c_str();
        printf("%-20s", bb_name);
        vector<int> vals = m_it->second;
        for (vector<int>::iterator v_it = vals.begin(); v_it != vals.end();
             ++v_it) {
          printf("%-5d", *v_it);
        }
        cout << endl;
      }
      cout << endl;
    }
    cout << endl;
  }

  cout << "---------------------------" << endl;
}

// cal MC/DC coverage
void calMCDCCov() {
  int indiCondNum = 0;

  // Calculate number of conditions that fits the requirements (each condition
  // individually affects blabla)

  // Iterate over each function to be tested
  for (map<string, vector<vector<string> > >::iterator it = nameTable.begin();
       it != nameTable.end(); ++it) {
    string funcNameStr = it->first;
    vector<vector<string> > singleFuncNames = it->second;
    vector<map<string, vector<int> > > singleFuncCondVals =
        condTable[funcNameStr];
    // Iterate over each decision of the function
    int deciNum = singleFuncNames.size();
    for (int i = 0; i < deciNum; ++i) {
      map<string, vector<int> > singleDeci = singleFuncCondVals[i];
      vector<string> singleDeciNames = singleFuncNames[i];
      // Deduplicate
      set<vector<int> > solutions;
      int sol_num = singleDeci[singleDeciNames[0]].size();
      for (int ii = 0; ii < sol_num; ++ii) {
        vector<int> tmp;
        for (vector<string>::iterator name = singleDeciNames.begin();
             name != singleDeciNames.end(); ++name) {
          tmp.push_back(singleDeci[*name][ii]);
        }
        solutions.insert(tmp);
      }
      int valid_sol_num = solutions.size();

      cout << "For decision " << i + 1
           << ", valid solution num is: " << valid_sol_num << endl;
      indiCondNum += valid_sol_num;
    }
  }
  double coverage = indiCondNum * 1.0 / totalCondNum;
  cout << "MC/DC Coverage of current test case is: " << 100 * coverage << "\%"
       << endl;
}

int getVal(char* handler, int lhs, int rhs) {
  // cout << "handler is: " << handler << endl;
  // cout << "Value of lhs is: " << lhs << endl;
  // cout << "Value of rhs is: " << rhs << endl;
  // Calculate value of the cmp func
  string handlerStr(handler);
  int res = 0;
  if (handlerStr == "eq") {
    res = (lhs == rhs);
  } else if (handlerStr == "ne") {
    res = (lhs != rhs);
  } else if (handlerStr == "sge") {
    res = (lhs >= rhs);
  } else if (handlerStr == "sgt") {
    res = (lhs > rhs);
  } else if (handlerStr == "slt") {
    res = (lhs < rhs);
  } else if (handlerStr == "sle") {
    res = (lhs <= rhs);
  }
  return res;
}

extern "C" void getScanSig() { first_scan = 0; }

extern "C" void mainInitExit() {
  printNameTable();
  printCondTable();
  calMCDCCov();

  printf("\n Main function Exits\n");
}

extern "C" void mainInit() {
  printf("In function main\n\n");

  atexit(mainInitExit);
}

extern "C" void updateNameTable(char* funcName, char* bbName) {
  string funcNameStr(funcName);
  string bbNameStr(bbName);

  // Judge if the function is already scanned
  // If scanned, nameTable need not to be further modified
  if (nameTable.count(funcNameStr)) {
    vector<vector<string> > singleFuncTable = nameTable[funcNameStr];
    for (vector<vector<string> >::iterator v_it = singleFuncTable.begin();
         v_it != singleFuncTable.end(); ++v_it) {
      vector<string> singleDecision = *v_it;
      if (find(singleDecision.begin(), singleDecision.end(), bbNameStr) !=
          singleDecision.end()) {
        /* cout << "FuncName: " << funcNameStr << endl;
        cout << "bbName: " << bbNameStr << endl;
        cout << endl; */
        // If some vector already contains the bbName
        return;
      }
    }
  }
  // cout << bbNameStr << endl;

  if (nameTable.count(funcNameStr)) {
    // If funcName already exists in the table
    auto isIfEnd = bbNameStr.find(terminLabel);
    auto isIfThen = bbNameStr.find(thenLabel);

    if (isIfEnd != string::npos) {
      // Is if.end
      vector<string> tmp;
      tmp.push_back(bbNameStr);
      nameTable[funcNameStr].push_back(tmp);
    } else if (isIfThen != string::npos) {
      // Is if.then, do nothing as it will go back to if.end
    } else {
      int index = nameTable[funcNameStr].size() - 1;
      nameTable[funcNameStr][index].push_back(bbNameStr);
    }
  } else {
    vector<vector<string> > singleFuncTable;
    vector<string> tmp;
    tmp.push_back(bbNameStr);
    singleFuncTable.push_back(tmp);
    nameTable[funcNameStr] = singleFuncTable;
  }
}

// Insert after each cmpInst, update condTable
extern "C" void updateCondTable(char* funcName, char* bbName, char* handler,
                                int lhs, int rhs) {
  string funcNameStr(funcName);
  string bbNameStr(bbName);

  auto isIfEnd = bbNameStr.find(terminLabel);
  auto isIfThen = bbNameStr.find(thenLabel);
  int max_len = -1;

  // cout << bbNameStr << endl;

  if (condTable.count(funcNameStr)) {
    // If funcName already exists in the table

    if (isIfEnd != string::npos) {
      // If is if.end*
      if (first_scan) {
        map<string, vector<int> > newDeci;
        vector<int> tmp;
        tmp.push_back(getVal(handler, lhs, rhs));
        newDeci[bbNameStr] = tmp;
        condTable[funcNameStr].push_back(newDeci);
      } else {
        // Locate the decision
        vector<map<string, vector<int> > > allDecis = condTable[funcNameStr];
        int vecSize = allDecis.size();
        for (int i = 0; i < vecSize; ++i) {
          if (allDecis[i].begin()->first == bbNameStr) {
            condTable[funcNameStr][i][bbNameStr].push_back(
                getVal(handler, lhs, rhs));
          }
        }
      }
    } else {
      // If is first scan
      if (first_scan) {
        int deciIndex = condTable[funcNameStr].size() - 1;
        vector<int> tmp;
        tmp.push_back(getVal(handler, lhs, rhs));
        condTable[funcNameStr][deciIndex][bbNameStr] = tmp;
      } else {
        // If it's not first scan
        // Traverse the nameTable and find which decision bbName belongs to
        vector<vector<string> > allDeciNames = nameTable[funcNameStr];
        int decIndex = -1;

        for (vector<vector<string> >::iterator dec_it = allDeciNames.begin();
             dec_it != allDeciNames.end(); ++dec_it) {
          vector<string> singDec = *dec_it;

          ++decIndex;
          if (find(singDec.begin(), singDec.end(), bbNameStr) !=
              singDec.end()) {
            break;
          }
        }

        // Get max len of all bbNames
        map<string, vector<int> > curDeci = condTable[funcNameStr][decIndex];
        max_len = (curDeci.begin()->second).size();

        if (!curDeci.count(bbNameStr)) {
          // If not exist such key
          vector<int> tmp;
          for (int i = 0; i < max_len - 1; ++i) {
            tmp.push_back(-1);
          }
          tmp.push_back(getVal(handler, lhs, rhs));
          condTable[funcNameStr][decIndex][bbNameStr] = tmp;
        } else {
          // Need not to insert NULL
          if (curDeci[bbNameStr].size() == (max_len - 1)) {
            condTable[funcNameStr][decIndex][bbNameStr].push_back(
                getVal(handler, lhs, rhs));

          } else {  // Need to insert NULL
            int loopNum = max_len -
                          condTable[funcNameStr][decIndex][bbNameStr].size() -
                          1;
            for (int i = 0; i < loopNum; ++i) {
              condTable[funcNameStr][decIndex][bbNameStr].push_back(-1);
            }
            condTable[funcNameStr][decIndex][bbNameStr].push_back(
                getVal(handler, lhs, rhs));
          }
        }
      }
    }
  } else {
    vector<map<string, vector<int> > > singleFuncTable;
    map<string, vector<int> > tmp;
    vector<int> v_tmp;
    v_tmp.push_back(getVal(handler, lhs, rhs));
    tmp[bbNameStr] = v_tmp;
    singleFuncTable.push_back(tmp);
    condTable[funcNameStr] = singleFuncTable;
  }
}

extern "C" void getTotalConditions(int totalConditions) {
  totalCondNum = totalConditions;
  // cout << totalCondNum << endl;
}