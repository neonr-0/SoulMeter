#pragma once
#include "pch.h"

#include <vector>
#include <unordered_map>
#include ".\FlatBuffers\include\STRUCT_.h"

#define FLATBUFFERUTIL FlatBufferUtil::getInstance()

using namespace SoulMeterFBS::History;

class FlatBufferUtil : public Singleton<FlatBufferUtil> {

public:

	VOID tIntDoubleListConverter(flatbuffers::FlatBufferBuilder& fbb, std::vector<flatbuffers::Offset<_tIntDoubleList>>& vIntDouble, unordered_map<UINT32, vector<double>>& m)
	{
		for (auto itr = m.begin(); itr != m.end(); itr++) 
		{
			auto fcvS = fbb.CreateVector(itr->second);

			_tIntDoubleListBuilder tidlb(fbb);

			tidlb.add__f(itr->first);
			tidlb.add__s(fcvS);
			
			vIntDouble.push_back(tidlb.Finish());
		}
	}

	VOID tIntDoubleListReverser(const _tIntDoubleList* tIntDoubleList, unordered_map<UINT32, vector<double>>& m)
	{
		vector<double> vTmp;

		for (auto itr = tIntDoubleList->_s()->begin(); itr != tIntDoubleList->_s()->end(); itr++)
			vTmp.push_back(*itr);

		m[tIntDoubleList->_f()] = vTmp;
	}

	VOID fbvDoubleReverser(const flatbuffers::Vector<double>* fbvDouble, vector<double>& vDouble)
	{
		for (auto itr = fbvDouble->begin(); itr != fbvDouble->end(); itr++)
			vDouble.push_back(*itr);
	}
};