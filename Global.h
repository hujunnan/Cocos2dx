#ifndef GLOBAL_H
#define GLOBAL_H

#include <string>
#include <vector>
using std::string;
using std::vector;

#include "json/rapidjson.h"
#include "json/document.h"

class Global
{
public:
    Global();
    ~Global();
    static string gameSessionId;
    static long score;
	static int round;
	static int heart;
	static int dtime;
    static string toString(vector<char> *buffer);

    static string getSessionIdFromHeader(string header);

	static rapidjson::Document getDocFromString(string str);
};

#endif
