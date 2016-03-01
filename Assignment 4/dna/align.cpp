//
//  align.cpp
//  dna_alignment
//
//  Created by Erika DeBenedictis on 1/27/13.
//  Copyright (c) 2014 California Institute of Technology. All rights reserved.
//
#include <iostream>
#include <string>
#include <unordered_map>

using namespace std;

// scoring values
#define GAP_SCORE -5
#define MISMATCH -1
#define MATCHING 2

/**
 * @brief Packages the score and instruction string the align function returns
 */
struct align_result {
    int score;      // score of this alignment
    string inst;    // instruction on how to align the inputs

    align_result(int s, string i) {
        this->score = s;
        this->inst = i;
    }
    align_result() {
        this->score = 0;
        this->inst = "";
    }
};

// memo_type will allow us to hash the string input to align
// with its output for memoization
typedef unordered_map<string, align_result> memo_type;

/**
 * @brief Function takes two strings, s and t, and produces an align_result
 * of the highest alignment score and its corresponding instruction str.
 */
align_result align(string s, string t, memo_type &memo) {
    // if this result is memoized, use recorded result
    string key = s + "," + t;
    if (memo.count(key) > 0){
      return memo[key];
    }

    /*
      DONE: calculate the highest score for an alignment of s and t
      - Base cases: s or t is empty
      - Recursive calls
     */
    // Base cases
    // s is empty, t is not
    if ((s.size() == 0) and (t.size() != 0))
    {
        return align_result((GAP_SCORE * t.size()), string(t.size(), 't'));
    }
    // s is not empty, t is
    else if ((s.size() != 0) and (t.size() == 0))
    {
        return align_result((GAP_SCORE * s.size()), string(s.size(), 's'));
    }
    // s and t are both empty
    else if ((s.size() == 0) and (t.size() == 0))
    {
        return align_result();
    }

    // Recursive calls
    else
    {
        // Case 1: Gap in t, remove char from s
        align_result case1 = align(s.substr(1, -1), t.substr(0, -1), memo);
        case1.score += GAP_SCORE;
        case1.inst.insert(0, "s");

        // Case 2: Gap in s, remove char from t
        align_result case2 = align(s.substr(0, -1), t.substr(1, -1), memo);
        case2.score += GAP_SCORE;
        case2.inst.insert(0,"t");

        // Case 3: Remove char from s and t
        char firstchars = s.at(0);
        char firstchart = t.at(0);
        align_result case3 = align(s.substr(1, -1), t.substr(1, -1), memo);
        // if the deleted chars are the same
        if (firstchars == firstchart)
        {
            case3.score += MATCHING;
            case3.inst.insert(0,"|");
        }
        else
        {
            case3.score += MISMATCH;
            case3.inst.insert(0, "*");
        }
        // Now we see which case results in greatest score and memoize it
        if ((case1.score > case2.score) and
            (case1.score > case3.score))
        {
            memo[key] = case1;
            return case1;
        }
        else if ((case2.score > case1.score) and
            (case2.score > case3.score))
        {
            memo[key] = case2;
            return case2;
        }
        else
        {
            memo[key] = case3;
            return case3;
        }
    }
}

/**
 * @brief Wrapper function to print the results of align
 */
void DNA_align(string s, string t) {
    cout << endl << "Calling DNA align on strings " << s << ", " << t << endl;

    // create the memoization system
    memo_type memo;

    align_result answer = align(s, t, memo);
    string ans = answer.inst;

    // Printing section
    // line where string s will be printed, spaces inserted
    string line1 = "";
    // line where string t will be printed, spaces inserted
    string line2 = "";
    // description of the relationship between s and t here (* | s t)
    string line3 = "";

    int j = 0;      // running index in s
    int k = 0;      // running index in t

    for (unsigned int m = 0; m < ans.length(); m++) {
        // i is the next element in our instruction string ans
        string i = ans.substr(m, 1);

        // only in s
        if(i.compare("s") == 0){
            line1 += s[j]; j++;
            line2 += " ";
            line3 += "s";
        }

        // only in t
        else if (i.compare("t") == 0){
            line1 += " ";
            line2 += t[k]; k++;
            line3 += "t";
        }

        // mismatch
        else if (i.compare("*") == 0){
            line1 += s[j]; j++;
            line2 += t[k]; k++;
            line3 += "*";
        }

        // match
        else {
            line1 += s[j]; j++;
            line2 += t[k]; k++;
            line3 += "|";
        }
    }
    cout << line1 << endl << line2 << endl << line3 << endl;
}

int main(){
    // some test cases to begin with
    DNA_align("",   "a");
    DNA_align("a",   "");
    DNA_align("a",   "b");
    DNA_align("a",   "a");
    DNA_align("b",   "ba");
    DNA_align("b",   "ab");
    DNA_align("ab",   "b");
    DNA_align("ba",   "b");
    DNA_align("ab",   "ba");
    DNA_align("abc",   "ac");
    DNA_align("abc",   "adc");

    // H3N2 v. H1N1 (PA gene)
    // DNA_align("ATGGAAGACTTTGTGCGACAGTGCTTCAATCCAATGATTGTCGAGCTTGCGGAAAAGGCAATGAAAGAATATGGGGAAGATCCGAAAATCGAAACCAACAAATTTGCTGCAATATG",
    //           "ATGGAAGACTTTGTGCGACAATGCTTCAATCCAATGATCGTCGAGCTTGCGGAAAAGGCAATGAAAGAATATGGGGAAGATCCGAAAATCGAAACTAACAAGTTTGCTGCAATATG");

    // Human 18S v. E. Coli 16S
    // DNA_align("TCGGCCCCGCCGGGGTCGGCCCACGGCCCTGGCGGAGCGCTGAGAAGACGGTCGAACTTGACTATCTAGAGGAAGTAAAAGTCGTAACAAGGTTTCCGTAGGTGAACCTGCGGAAGGATCATTA",
    //           "GTGGGTTGCAAAAGAAGTAGGTAGCTTAACCTTCGGGAGGGCGCTTACCACTTTGTGATTCATGACTGGGGTGAAGTCGTAACAAGGTAACCGTAGGGGAACCTGCGGTTGGATCACCTCCTTA");
    return 0;
}
