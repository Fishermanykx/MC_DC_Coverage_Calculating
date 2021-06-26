/*
 * @Description: runtime lib of MC/DC coverage pass
 * @Author: Fishermanykx
 * @Date: 2021-04-12 21:52:34
 * @LastEditors: Fishermanykx
 * @LastEditTime: 2021-06-25 11:14:33
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
int scan_num = 1;

string ifTermLabel = "if.end";
string thenLabel = "if.then";
string whileBeginLabel = "while.cond";
string whileBody = "while.body";
string forBeginLabel = "for.cond";
string forBody = "for.body";
string entryLabel = "entry";

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

  // Preprocess, del empty entries
  for (map<string, vector<map<string, vector<int> > > >::iterator it =
           condTable.begin();
       it != condTable.end(); ++it) {
    string funcNameStr = it->first;
    if (condTable[funcNameStr][0]["entry"].empty()) {
      totalCondNum--;
      condTable[funcNameStr][0].erase("entry");
      if (condTable[funcNameStr][0].empty()) {
        condTable[funcNameStr].erase(condTable[funcNameStr].begin());
      }
      nameTable[funcNameStr].erase(nameTable[funcNameStr].begin());
    }
  }

  // printCondTable();

  // Calculate number of conditions that fits the requirements (each condition
  // individually affects blabla)

  // Iterate over each function to be tested
  for (map<string, vector<vector<string> > >::iterator it = nameTable.begin();
       it != nameTable.end(); ++it) {
    string funcNameStr = it->first;
    vector<vector<string> > singleFuncNames = it->second;
    vector<map<string, vector<int> > > singleFuncCondVals =
        condTable[funcNameStr];
    int deciNum = singleFuncNames.size();
    // Update condTable, fill in all the blanks with -1
    // Iterate over nameTable
    for (int i = 0; i < deciNum; ++i) {
      map<string, vector<int> > singleDeci = singleFuncCondVals[i];
      vector<string> singleDeciNames = singleFuncNames[i];
      int maxLen = singleDeci[singleDeciNames[0]].size();
      for (vector<string>::iterator name = singleDeciNames.begin();
           name != singleDeciNames.end(); ++name) {
        while (condTable[funcNameStr][i][*name].size() < maxLen) {
          condTable[funcNameStr][i][*name].push_back(-1);
        }
      }
    }
  }
  printCondTable();
  printNameTable();

  for (map<string, vector<vector<string> > >::iterator it = nameTable.begin();
       it != nameTable.end(); ++it) {
    string funcNameStr = it->first;
    vector<vector<string> > singleFuncNames = it->second;
    vector<map<string, vector<int> > > singleFuncCondVals =
        condTable[funcNameStr];
    int deciNum = singleFuncNames.size();

    // Iterate over each decision of the function
    cout << "In function " << funcNameStr << ":" << endl;
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
    cout << endl;
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

void mergeCondTable() {
  // Iterate over nameTable
  for (map<string, vector<vector<string> > >::iterator it = nameTable.begin();
       it != nameTable.end(); ++it) {
    string funcNameStr = it->first;
    vector<vector<string> > allDecisions = it->second;

    for (vector<vector<string> >::iterator vv_it = allDecisions.begin();
         vv_it != allDecisions.end(); ++vv_it) {
      vector<map<string, vector<int> > > condAllDecis = condTable[funcNameStr];
      vector<string> singleDecision = *vv_it;

      string startBBName = singleDecision[0];
      // Find the according decision in condTable
      int condDeciIndex = 0;
      int found = 0;
      for (vector<map<string, vector<int> > >::iterator m_it =
               condAllDecis.begin();
           m_it != condAllDecis.end(); ++m_it) {
        map<string, vector<int> > condSingDeci = *m_it;

        for (map<string, vector<int> >::iterator name_it = condSingDeci.begin();
             name_it != condSingDeci.end(); ++name_it) {
          string name = name_it->first;
          if (name == startBBName) {
            found = 1;
            break;
          }
        }
        if (found) {
          break;
        }

        ++condDeciIndex;
      }

      int isNewDeci = 0;
      for (vector<string>::iterator v_it = singleDecision.begin();
           v_it != singleDecision.end(); ++v_it) {
        string name = *v_it;
        if (found) {
          // If the cond of the deci is in condTable
          if (isNewDeci || !condAllDecis[condDeciIndex].count(name)) {
            // If cur name is not a key in the map, add
            vector<int> tmp;
            condTable[funcNameStr][condDeciIndex][name] = tmp;
          }
        } else {
          isNewDeci = 1;
          found = 1;
          map<string, vector<int> > tmp_m;
          vector<int> tmp;
          tmp_m[name] = tmp;
          condTable[funcNameStr].push_back(tmp_m);
        }
      }
    }
  }
  // printCondTable();
  // printNameTable();
  // exit(0);
}

extern "C" void getScanSig() {
  first_scan = 0;
  scan_num++;
}

extern "C" void mainInitExit() {
  calMCDCCov();

  printf("\n Main function Exits\n");
}

extern "C" void mainInit() {
  printf("In function main\n\n");

  atexit(mainInitExit);
}

extern "C" void funcInit() {
  if (scan_num == 2) {
    mergeCondTable();
  }
}

extern "C" void updateNameTable(char* funcName, char* bbName,
                                int isBeginLabel) {
  string funcNameStr(funcName);
  string bbNameStr(bbName);

  // Check if the part is already scanned
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

  int isIfEnd = (bbNameStr.find(ifTermLabel) != string::npos);
  int isIfThen = (bbNameStr.find(thenLabel) != string::npos);
  int isWhileBegin = (bbNameStr.find(whileBeginLabel) != string::npos);
  int isWhileBody = (bbNameStr.find(whileBody) != string::npos);
  int isForBegin = (bbNameStr.find(forBeginLabel) != string::npos);
  int isForBody = (bbNameStr.find(forBody) != string::npos);
  int isEntry = (bbNameStr.find(entryLabel) != string::npos);

  int newDeciStart = (isWhileBody || isForBody || isIfEnd || isIfThen ||
                      isWhileBegin || isForBegin || isEntry);

  if (nameTable.count(funcNameStr)) {
    // If funcName already exists in the table
    // Check if a new decision begins
    if (isBeginLabel || newDeciStart) {
      // Is new decision begins
      vector<string> tmp;
      tmp.push_back(bbNameStr);
      nameTable[funcNameStr].push_back(tmp);
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

/* // Insert after each cmpInst, update condTable
void updateCondTable(char* funcName, char* bbName, char* handler, int lhs,
                     int rhs) {
  string funcNameStr(funcName);
  string bbNameStr(bbName);

  int isIfEnd = (bbNameStr.find(ifTermLabel) != string::npos);
  int isIfThen = (bbNameStr.find(thenLabel) != string::npos);
  int isWhileBegin = (bbNameStr.find(whileBeginLabel) != string::npos);
  int isWhileBody = (bbNameStr.find(whileBody) != string::npos);
  int isForBegin = (bbNameStr.find(forBeginLabel) != string::npos);
  int isForBody = (bbNameStr.find(forBody) != string::npos);
  int isEntry = (bbNameStr.find(entryLabel) != string::npos);

  int newDeciStart = (isWhileBody || isForBody || isIfEnd || isIfThen ||
                      isWhileBegin || isForBegin || isEntry);

  int max_len = -1;

  // cout << bbNameStr << endl;

  if (condTable.count(funcNameStr)) {
    // If funcName already exists in the table

    if (newDeciStart) {
      // If a new decision starts
      // FIXME:Check if the tag already exist (for and while, loop many times)
      int loopScanned = 0;
      vector<map<string, vector<int> > > allDecis = condTable[funcNameStr];
      int vecSize = allDecis.size();
      for (int i = 0; i < vecSize; ++i) {
        for (map<string, vector<int> >::iterator m_it = allDecis[i].begin();
             m_it != allDecis[i].end(); ++m_it) {
          if (m_it->first == bbNameStr) {
            condTable[funcNameStr][i][bbNameStr].push_back(
                getVal(handler, lhs, rhs));
            loopScanned = 1;
            break;
          }
        }
        if (loopScanned) break;
      }

      if (!loopScanned) {
        map<string, vector<int> > newDeci;
        vector<int> tmp;
        tmp.push_back(getVal(handler, lhs, rhs));
        newDeci[bbNameStr] = tmp;
        condTable[funcNameStr].push_back(newDeci);
      }

    } else {
      // If is first scan
      if (first_scan) {
        // In loop cases, there will be a bug
        int deciIndex = condTable[funcNameStr].size() - 1;
        vector<int> tmp;
        tmp.push_back(getVal(handler, lhs, rhs));
        condTable[funcNameStr][deciIndex][bbNameStr] = tmp;
      } else {
        // If it's not first scan
        // Traverse the nameTable and find which decision bbName belongs to
        vector<vector<string> > allDeciNames = nameTable[funcNameStr];
        int decIndex = -1;
        string beginBBName;

        int found = 0;
        for (vector<vector<string> >::iterator dec_it = allDeciNames.begin();
             dec_it != allDeciNames.end(); ++dec_it) {
          vector<string> singDec = *dec_it;

          ++decIndex;

          for (vector<string>::iterator s_it = singDec.begin();
               s_it != singDec.end(); ++s_it) {
            if (*s_it == bbNameStr) {
              found = 1;
              beginBBName = singDec[0];
              break;
            }
          }
          if (found) {
            break;
          }
        }
        if (!found) {
          cout << "Cannot find decision" << endl;
          cout << bbNameStr << endl;
          printNameTable();
          exit(1);
        }

        // FIXME: Get max len of all bbNames
        map<string, vector<int> > curDeci = condTable[funcNameStr][decIndex];
        max_len = (curDeci[beginBBName]).size();
        // cout << beginBBName << endl;

        if (!curDeci.count(bbNameStr)) {
          // If not exist such key
          vector<int> tmp;
          for (int i = 0; i < max_len - 1; ++i) {
            tmp.push_back(-1);
          }
          tmp.push_back(getVal(handler, lhs, rhs));
          condTable[funcNameStr][decIndex][bbNameStr] = tmp;
        } else {
          // Update all the conds in the deci
          vector<string> curDeciNames = allDeciNames[decIndex];
          for (vector<string>::iterator name = curDeciNames.begin();
               name != curDeciNames.end(); ++name) {
            if (curDeci[*name].size() != (max_len - 1)) {
              int loopNum =
                  max_len - condTable[funcNameStr][decIndex][*name].size() - 1;
              for (int i = 0; i < loopNum; ++i) {
                condTable[funcNameStr][decIndex][*name].push_back(-1);
              }
            }
          }
          // Insert current node
          condTable[funcNameStr][decIndex][bbNameStr].push_back(
              getVal(handler, lhs, rhs));
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
  // printCondTable();
} */

extern "C" void updateCondTable(char* funcName, char* bbName, char* handler,
                                int lhs, int rhs) {
  string funcNameStr(funcName);
  string bbNameStr(bbName);

  int max_len = -1;

  if (scan_num < 2) {
    // updateCondTable(funcName, bbName, handler, lhs, rhs);
  } else {
    // Get max len
    // Find beginBBName
    string beginBBName;
    int found = 0;
    int decIndex = -1;
    vector<vector<string> > allDeciNames = nameTable[funcNameStr];
    for (vector<vector<string> >::iterator dec_it = allDeciNames.begin();
         dec_it != allDeciNames.end(); ++dec_it) {
      vector<string> singDec = *dec_it;
      ++decIndex;

      for (vector<string>::iterator s_it = singDec.begin();
           s_it != singDec.end(); ++s_it) {
        if (*s_it == bbNameStr) {
          found = 1;
          beginBBName = singDec[0];
          break;
        }
      }
      if (found) {
        break;
      }
    }
    if (!found) {
      cout << "Cannot find decision" << endl;
      cout << bbNameStr << endl;
      printNameTable();
      exit(1);
    }

    max_len = condTable[funcNameStr][decIndex][beginBBName].size();

    int cur_size = condTable[funcName][decIndex][bbName].size();
    for (int i = 0; i < max_len - cur_size - 1; ++i) {
      condTable[funcName][decIndex][bbName].push_back(-1);
    }
    condTable[funcName][decIndex][bbName].push_back(getVal(handler, lhs, rhs));
  }
}

extern "C" void getTotalConditions(int totalConditions) {
  totalCondNum = totalConditions;
  // cout << totalCondNum << endl;
}