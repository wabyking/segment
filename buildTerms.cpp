/*
 * buildTerms.cpp
 *
 *  Created on: Dec 21, 2011
 *      Author: lai_conglin
 */

# include <iostream>
# include <map>
# include <string>
# include <fstream>
# include <set>
# include <vector>
# include <algorithm>
# include <time.h>
#include <stdio.h>
using namespace std;
map<string, set<string> > content;
map<string, int> freq;
size_t ChineseLength;
void initial(){
	string testch="测试";
//	cout<<testch.length()<<endl;
	ChineseLength=testch.length()/2;
}
struct Word {
private:
	string value;
	int frequence;
public:
	Word() {

	}
	void setValue(string src) {
		value = src;
		/**
		 * a single character will need to find its frequence , and a term will be 1 default
		 */
		if (value.length() == ChineseLength) {
			map<string, int>::iterator it = freq.find(value);
			if (it == freq.end()) {
				frequence = 1;
			} else {
				frequence = freq[value];
			}
		} else {
			frequence = 1;
		}
	}
	string getValue() {
		return value;
	}
	int getLength() {
		return value.length();
	}
	int getFrequency() {
		return frequence;
	}
};
struct Chunk {
public:
	vector<Word> list;

	Chunk() {

	}
	/**
	 * add a word to the list
	 */
	void addWord(Word w) {
		list.push_back(w);
	}
	int getWordNumber() {
		return list.size();
	}
	double getVariance() {
		double avgLen = 0.0;
		int listSize = list.size();
		for (int i = 0; i < listSize; i++) {
			avgLen += list.at(i).getLength();
		}
		avgLen = 1.0 * avgLen / listSize;
		double variance = 1.0;
		for (int i = 0; i < listSize; i++) {
			double temp = (avgLen - list.at(i).getLength());
			variance += temp * temp;
		}
		return variance;
	}
	long getFreq() {
		long freqValue = 0;
		int listSize = list.size();
		for (int i = 0; i < listSize; i++) {
			freqValue += list.at(i).getFrequency();
		}
		return freqValue;
	}
	vector<string> getVectorString() {
		vector<string> res;
		int size = list.size();
		for (int i = 0; i < size; i++) {
			res.push_back(list.at(i).getValue());
		}
		return res;
	}
};
void read_terms_from_Lexicon() {
	ifstream fin("Lexicon_full_words.txt");
//	string test="一·二八";
//	cout<<test.substr(3,2);
	string dot = "·";
//	cout<<test.find(dot.c_str(),3);
	string s;
	string temp;
	map<string, set<string> >::iterator lexiconIterator;
	s.clear();
	//ignore the first one string, I don't know why
	fin >> s;
	cout << "start read from Lexicon_full_words.txt" << endl;
	cout << "reading terms: -> " << endl;

	while (fin >> s) {
//		int len = s.length();
		set<string> tempSet;
		//one character will cost 3 bytes in linux, 2 in windows....maybe not right.you need to test it
		temp.clear();
		temp = s.substr(0, ChineseLength);
		lexiconIterator = content.find(temp);
		if (lexiconIterator == content.end()) {
			tempSet.clear();
			tempSet.insert(s);
		} else {
			tempSet = content[temp];
			tempSet.insert(s);
		}
		content[temp] = tempSet;

	}
	cout << "finish read the lexicon." << endl;
	cout << "finish read the lexicon .lexicon size:" << content.size() << endl;

	fin.close();
}
void show_Lexicon() {
	map<string, set<string> >::iterator lexiconIterator;
	int count = 0;
	for (lexiconIterator = content.begin(); lexiconIterator != content.end();
			lexiconIterator++) {
		string first = lexiconIterator->first;
		set<string> second = lexiconIterator->second;
		cout << first << ":";
		set<string>::iterator setIt;
		for (setIt = second.begin(); setIt != second.end(); setIt++) {
			cout << *setIt << " ";
		}
		cout << endl;
		count++;
		if (count == 10) {
			break;
		}
	}
	cout << "lexicon size:" << content.size() << endl;
}
void write_index() {
	fstream outputFile("lexicon.index", fstream::out);
	map<string, set<string> >::iterator lexiconIterator;
	for (lexiconIterator = content.begin(); lexiconIterator != content.end();
			lexiconIterator++) {
		string first = lexiconIterator->first;
		set<string> second = lexiconIterator->second;
		outputFile << "#:" << first << endl;
		set<string>::iterator setIt;
		for (setIt = second.begin(); setIt != second.end(); setIt++) {
			outputFile << *setIt << " ";
		}
		outputFile << endl;
	}
	cout << "write index: lexicon size:" << content.size() << endl;
	outputFile.close();
}
void write_freq() {
	fstream outputFile("freq.index", fstream::out);
	map<string, int>::iterator freqIterator;
	for (freqIterator = freq.begin(); freqIterator != freq.end();
			freqIterator++) {
		string first = freqIterator->first;
		int second = freqIterator->second;
		outputFile << first << " " << second;

		outputFile << endl;
	}
	cout << "write index: freq size:" << freq.size() << endl;
	outputFile.close();

}
void build_freq() {
	ifstream fin("freq.index");
	string tempStr;
	int tempFreq;
	while (fin >> tempStr) {
		fin >> tempFreq;
		if (tempFreq > 1 || tempStr.length() == ChineseLength) {
			freq[tempStr] = tempFreq;
		}
	}
	fin.close();
//	write_freq();
	cout << "index: freq size:" << freq.size() << endl;
}

void build_index() {
	ifstream fin("lexicon.index");
//	cout<<fin.good()<<endl;
	if (fin.good() == 0) {
		cout << "build index , need some time , please wait for a moment! \n";
		read_terms_from_Lexicon();
		write_index();
		fin.open("lexicon.index", ifstream::in);
	}
	cout << "hello ,begin load index \n";
	string tempStr;
	set<string> tempSet;
	string key = "";
	while (fin >> tempStr) {
		if (tempStr.find("#:", 0) == 0) {
			if (key != "")
				content[key] = tempSet;
			tempSet.clear();
			key = tempStr.substr(2);
		} else {
			tempSet.insert(tempStr);
		}
//		if(content.size()>10){
//			break;
//		}
	}
	content[key] = tempSet;
	cout << "lexicon size:" << content.size() << endl;
	fin.close();
	build_freq();
}

vector<Chunk> chunklist;
int minChunkWordNumber;
void mmseg_recursion(string src, Chunk tempChunk) {
	set<string> termslist;
	int i = 0;
	int len = src.length();
	//get the single character.
	string singleWordStr;
	Word tempWord;
	singleWordStr = src.substr(0, ChineseLength);
	tempWord.setValue(singleWordStr);

	//if the character is the end character , return
	if ((i + ChineseLength) >= len) {
		tempChunk.addWord(tempWord);
		chunklist.push_back(tempChunk);
		if (minChunkWordNumber > tempChunk.getWordNumber()) {
			minChunkWordNumber = tempChunk.getWordNumber();
		}
		return;
	} else {
		string tempStr;
		map<string, set<string> >::iterator lexIt;
		lexIt = content.find(singleWordStr);
		//if the lexicon has no this word
		//them let it be a single term
		if (lexIt == content.end()) {
			tempChunk.addWord(tempWord);
			string remain = src.substr(ChineseLength);
			mmseg_recursion(remain, tempChunk);
			tempChunk.list.pop_back();
		} else {
			termslist = content[singleWordStr];
			set<string>::iterator setIt;
			vector<string> termsVector;
//			for (setIt = termslist.begin(); setIt != termslist.end(); setIt++) {
//				termsVector.push_back(*setIt);
//			}
//			sort(termsVector.begin(), termsVector.end());
//			int sizeVec = termsVector.size();
			for (setIt = termslist.begin(); setIt != termslist.end(); setIt++) {
				tempStr = *setIt;
				size_t foundit = src.find(tempStr, 0);
				if (foundit == 0 && tempStr != singleWordStr) {
					tempWord.setValue(tempStr);
					tempChunk.addWord(tempWord);
					if (tempChunk.getWordNumber() > (minChunkWordNumber)) {
						tempChunk.list.pop_back();
						return;
					}

					//if the term has all remain character of string
					//return
					if (tempStr.length() == src.length()) {
						chunklist.push_back(tempChunk);
						if (minChunkWordNumber > tempChunk.getWordNumber()) {
							minChunkWordNumber = tempChunk.getWordNumber();
						}
						return;
					}
					string remain = src.substr(tempStr.length());
					mmseg_recursion(remain, tempChunk);
					tempChunk.list.pop_back();
				}
			}
			//process the single character situation
			tempStr = singleWordStr;
			tempWord.setValue(tempStr);
			tempChunk.addWord(tempWord);
			if (tempChunk.getWordNumber() > (minChunkWordNumber)) {
				tempChunk.list.pop_back();
				return;
			}
			string remain = src.substr(tempStr.length());
			mmseg_recursion(remain, tempChunk);
			tempChunk.list.pop_back();

		}
	}
}

vector<string> mmseg(string src) {
	vector<string> res;
	chunklist.clear();
	minChunkWordNumber = 0x7ffffff0;
	Chunk tempChunk;
	vector<int> indexInChunkList;
	int min = 0x7fffffff;
//	cout << min;
	mmseg_recursion(src, tempChunk);
	int chunkListSize = chunklist.size();
	if (chunkListSize == 1) {
		return chunklist.at(0).getVectorString();
	} else {
		for (int i = 0; i < chunkListSize; i++) {
			if (chunklist.at(i).getWordNumber() < min) {
				min = chunklist.at(i).getWordNumber();
				indexInChunkList.clear();
				indexInChunkList.push_back(i);
			} else if (chunklist.at(i).getWordNumber() == min) {
				indexInChunkList.push_back(i);
			}
		}
		//rule 1 to find the max average length chunk
		if (indexInChunkList.size() == 1) {
			return chunklist.at(indexInChunkList.at(0)).getVectorString();
		} else {
			//rule 2 find the least variance of chunk
			double minVariance = min * src.length() * src.length();
			vector<int> tempIndex = indexInChunkList;
			indexInChunkList.clear();
			for (size_t index = 0; index < tempIndex.size(); index++) {
				int i = tempIndex.at(index);
				if (chunklist.at(i).getVariance() < minVariance) {
					minVariance = chunklist.at(i).getVariance();
					indexInChunkList.clear();
					indexInChunkList.push_back(i);
				} else if (chunklist.at(i).getVariance() == minVariance) {
					indexInChunkList.push_back(i);
				}
			}

			if (indexInChunkList.size() == 1) {
				return chunklist.at(indexInChunkList.at(0)).getVectorString();
			} else {
				//rule 3 have most frequency terms
				vector<int> tempIndex = indexInChunkList;
				indexInChunkList.clear();
				long max = 0;
				int tempIndexSize = tempIndex.size();
				for (int index = 0; index < tempIndexSize; index++) {
					int i = tempIndex.at(index);
					if (chunklist.at(i).getFreq() > max) {
						max = chunklist.at(i).getFreq();
						indexInChunkList.clear();
						indexInChunkList.push_back(i);
					} else if (chunklist.at(i).getFreq() == max) {
						indexInChunkList.push_back(i);
					}
				}
				return chunklist.at(indexInChunkList.at(0)).getVectorString();
			}
		}
	}
}
void showTermsSegment(vector<string> src) {
	cout << "segment like this:";
	int size = src.size();
	for (int i = 0; i < size; i++) {
		cout << src.at(i) << " ";
	}
	cout << endl;
}
int main() {
	initial();
//	read_terms_from_Lexicon();
//	write_index();
	build_index();
//	show_Lexicon();
	string test = "中华人民共和国在1949年建立";
	test = "从此开始了新中国的伟大篇章";
	test = "研究生命起源";
	test = "北京天安门";
//	从此开始了新中国的伟大篇章中华人民共和国在一九四九年建立
	test = "主要是因为研究生命起源北京天安门";
	test = "从此开始了新中国的伟大篇章中华人民共和国在一九五五年建立主要是因为研究生命起源北京天安门";

//	test ="国际化企业中华人民共和国";
//	size_t found;
//	found = test.find("开始", 10);
//	bool flag = (found != string::npos);
//	cout << test.substr(test.length(), 3) << endl;
//	cout << test.substr(24, 4) << endl;
//	test = "，";
//	cout << test.length();

//	vector<string> res = mmseg(test);
//	int min = 0x7fffffff;
//	cout << min;
	vector<string> seg = mmseg(test);
//	seg.push_back("从");
//	sort(seg.begin(), seg.end());
	cout << endl;
	cout << "test string :" << test << endl;
	showTermsSegment(seg);

	test = "主要是因为研究生死";
	seg = mmseg(test);
	cout << endl;
	cout << "test string :" << test << endl;
	showTermsSegment(seg);

	test = "曾经在深圳交过公积金，后来离开深圳时注销了公积金账户，这个情况是按“未在深圳市缴纳过公积金”的情况处理吗？";
	seg = mmseg(test);
	cout << endl;
	cout << "test string :" << test << endl;
	showTermsSegment(seg);

	return 0;
}

