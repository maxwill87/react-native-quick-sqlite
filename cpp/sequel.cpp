/*
 * sequel.cpp
 *
 * Created by Oscar Franco on 2021/03/07
 * Copyright (c) 2021 Oscar Franco
 *
 * This code is licensed under the SSPL license
 * https://www.mongodb.com/licensing/server-side-public-license
 */

#include "sequel.h"
#include <iostream>
#include <sstream>
#include <sqlite3.h>
#include <ctime>
#include <unistd.h>
#include <sys/stat.h>
#include <map>

using namespace std;
using namespace facebook;

std::map<string, sqlite3*> dbMap = std::map<string, sqlite3*>();

inline bool file_exists (const string &path) {
  struct stat buffer;
  return (stat (path.c_str(), &buffer) == 0);
}

string get_db_path(string const &dbName)
{
    char *home = getenv("HOME");
    char const *subdir = "/Documents/";

    stringstream ss;
    ss << home << subdir << dbName;
    string dbPath = ss.str();

    return dbPath;
}

bool sequel_open(string const &dbName)
{
    cout << "[react-native-sequel] Opening DB" << endl;

    string dbPath = get_db_path(dbName);
    int sqlOpenFlags = SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE;

    sqlite3 *db;
    int exit = 0;
    exit = sqlite3_open_v2(dbPath.c_str(), &db, sqlOpenFlags, nullptr);

    if (exit != SQLITE_OK)
    {
        cout << "react-native-sequel: Error opening database [" << dbName << "]: " << sqlite3_errmsg(db) << endl;
        return false;
    }
    else
    {
        cout << "Opened database successfully!" << endl;
        dbMap[dbName] = db;
    }

    return true;
}

bool sequel_close(string const &dbName)
{
    cout << "[react-native-sequel] Closing DB" << endl;

    if(dbMap.count(dbName) == 0){
        cout << "[react-native-sequel]: No DB open" << endl;
        return true;
    }

    sqlite3 *db = dbMap[dbName];

    sqlite3_close(db);

    dbMap.erase(dbName);

    return true;
}

bool sequel_remove(string const &dbName)
{
    cout << "[react-native-sequel] Deleting DB" << endl;

    if(dbMap.count(dbName) == 1){
        sequel_close(dbName);
    }

    string dbPath = get_db_path(dbName);

    if(!file_exists(dbPath)) {
        cout << "[react-native-sequel] File not found" << endl;
        return false;
    }

    remove(dbPath.c_str());
    cout << "[react-native-sequel] DB at " << dbPath << "has been deleted." << endl;

    return true;
}

vector<jsi::Object> sequel_execute(jsi::Runtime &rt,string const &dbName, string const &query)
{
    vector<jsi::Object> results;

    sqlite3 *db = dbMap[dbName];

    if(dbMap.count(dbName) == 0){
        cout << "[react-native-sequel]: "<< dbName << " database is not open" << endl;
        return results;
    }

    // this time we will first compile the SQL
    sqlite3_stmt *statement;

    if (sqlite3_prepare_v2(db, query.c_str(), -1, &statement, NULL) != SQLITE_OK)
    {
        cout << "INVALID SQL QUERY" << endl;
    }

    bool isConsuming = true;
    int result, i, count, column_type;
    string column_name;



    while (isConsuming)
    {
        result = sqlite3_step(statement);
        jsi::Object entry = jsi::Object(rt);

        switch (result)
        {
        case SQLITE_ROW:
            i = 0;
            count = sqlite3_column_count(statement);

            while (i < count)
            {
                column_type = sqlite3_column_type(statement, i);
                column_name = sqlite3_column_name(statement, i);

                switch (column_type)
                {

                case SQLITE_INTEGER:
                {
                    int column_value = sqlite3_column_int(statement, i);
                    entry.setProperty(rt, column_name.c_str(), jsi::Value(column_value));
                    break;
                }

                case SQLITE_FLOAT:
                {
                    double column_value = sqlite3_column_double(statement, i);
                    entry.setProperty(rt, column_name.c_str(), jsi::Value(column_value));
                    break;
                }

                case SQLITE_TEXT:
                {
                    // TODO: not all the stored text is ASCII, replace this for UTF 8
                    const char *column_value = reinterpret_cast<const char *>(sqlite3_column_text(statement, i));
                    entry.setProperty(rt, column_name.c_str(), jsi::String::createFromAscii(rt, column_value));
                    break;
                }

                case SQLITE_NULL:
                // Intentionally left blank to switch to default case
                default:
                    entry.setProperty(rt, column_name.c_str(), jsi::Value(nullptr));
                    break;
                }

                i++;
            }

            results.push_back(move(entry));
            break;

        case SQLITE_DONE:
            isConsuming = false;
            break;

        default:
            cout << "react-native-sequel: Error executing query" << endl;
            isConsuming = false;
        }
    }

    sqlite3_finalize(statement);

    return results;
}
