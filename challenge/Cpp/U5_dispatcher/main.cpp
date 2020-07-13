#include <iostream>
#include <map>
#include <string>
#include <iostream>
#include <functional>
#include <ctime>

//
// supporting tools and software
//
// Validate and test your json commands
// https://jsonlint.com/

// RapidJSON : lots and lots of examples to help you use it properly
// https://github.com/Tencent/rapidjson
//

// std::function
// std::bind
// std::placeholders
// std::map
// std::make_pair

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

using namespace rapidjson;
using namespace std;

bool g_done = false;

auto exit_command = R"({"command":"exit", "payload":{"reason":"User requested exit."}})";
auto help_command = R"({"command":"help", "payload":{"usage":"Enter json command in 'command':'<command>','payload': { // json payload of arguments }"}})";
auto info_command = R"({"command":"info", "payload":{"name":"Roman","age":"24"}})";
auto cpp_command =  R"({"command":"cpp", "payload":{"what":"Prints C++ symbol"}})";
auto time_command = R"({"command":"time", "payload":{"usage":"Prints time in specified format: (choose 12/24) ","format":24}})";

class Controller {
public:
    bool help( rapidjson::Value &payload )
    {
        for( auto i = payload.MemberBegin(); i != payload.MemberEnd(); i++ )
        {
            cout << i->name.GetString() << ": " << i->value.GetString() << endl;
        }
        cout << "Copy/paste one of the following commands.";

        return true;
    }

    bool exit( rapidjson::Value &payload )
    {
        for( auto i = payload.MemberBegin(); i != payload.MemberEnd(); i++ )
        {
            cout << i->name.GetString() << ": " << i->value.GetString() << endl;
        }
        g_done = true;

        return true;
    }

    bool info( rapidjson::Value &payload )
    {
        cout << "Printing author information:" << endl;

        for( auto i = payload.MemberBegin(); i != payload.MemberEnd(); i++ )
        {
            cout << i->name.GetString() << ": " << i->value.GetString() << endl;
        }
        return true;
    }

    bool cpp( rapidjson::Value &payload )
    {
        for(auto i = payload.MemberBegin(); i != payload.MemberEnd(); i++ )
        {
            cout << i->name.GetString() << ": " << i->value.GetString() << endl;
        }
        cout << "Printing C++ symbol:" << endl << endl;

        cout << R"(   ______            ___               ___         )" << endl;
        cout << R"( /        \         |   |             |   |        )" << endl;
        cout << R"(|   _____ /      ___|   |___       ___|   |___     )" << endl;
        cout << R"(|  |            |           |     |           |    )" << endl;
        cout << R"(|  |_____       |___     ___|     |___     ___|    )" << endl;
        cout << R"(|         \         |   |             |   |        )" << endl;
        cout << R"( \ ______ /         |___|             |___|        )" << endl << endl;

        return true;
    }

    bool time( rapidjson::Value &payload )
    {
        if( !( payload.HasMember( "format" ) ) )
        {
            cout << "Payload must have \"format\" member" << endl;
            return false;
        }

        if( !( payload["format"].IsInt() ) )
        {
            cout << "Format must be an integer value, valid formats: 12, 24" << endl;
            return false;
        }

        int format =  payload["format"].GetInt();

        std::time_t t = std::time( 0 );
        std::tm* now = std::localtime( &t );

        if( format == 24 )
        {
            cout << now->tm_hour << ":" << now->tm_min << ":" << now->tm_sec << endl;
        }
        else if( format == 12 )
        {
            if( now->tm_hour > 12 )
            {
                cout << now->tm_hour-12 << ":" << now->tm_min << ":" << now->tm_sec << " PM" << endl;
            }
            else
            {
                cout << now->tm_hour << ":" << now->tm_min << ":" << now->tm_sec << " AM" << endl;
            }
        }
        else
        {
            cout << "Wrong format, valid formats: 12, 24" << endl;
            return false;
        }
        return true;
    }

};

// gimme ... this is actually tricky
// Bonus Question: why did I type cast this?
typedef std::function< bool( rapidjson::Value& ) > CommandHandler;
/**************************************************************************************************************************/
/** "type cast" je konverzia nie ? Cize zmena na iny datovy typ. Ja si myslim ze tam ziadna konverzia nieje.             **/
/** Ak ste mysleli "typedef", tak to je preto aby sme stale nemuseli pisat tie dlhe formuly, ale jednoducho si vytvorime **/
/** novy datovy typ.                                                                                                     **/
/** Ak ste mysleli "<bool>(rapidjson::Value&)", tak bool je navratovy typ, a rapidjson::Value& je typ parametra funkcie  **/
/**************************************************************************************************************************/


class CommandDispatcher {
public:
    // ctor - need impl
    CommandDispatcher()
    {

    }

    // dtor - need impl
    virtual ~CommandDispatcher()
    {
        // question why is it virtual? Is it needed in this case?
        /******************************************************************************************************************/
        /** Nie je to potrebne, virtualne konstruktory/destruktory sa pouzivaju pri triedach, ktore dedia z inych tried, **/
        /** aby sme ich potom v dediacej triede implementovali.                                                          **/
        /******************************************************************************************************************/
    }

    bool addCommandHandler( std::string command, CommandHandler handler )
    {
        command_handlers_.insert( std::make_pair( command, handler ) );

        return true;
    }

    bool dispatchCommand( std::string command_json )
    {
        if( command_json.empty() )
        {
            cout << "Empty command !" << endl;
            return false;
        }

        Document doc;

        if( doc.Parse( command_json.c_str() ).HasParseError() )
        {
            cout << "Invalid json format !!!" << endl;
            return false;
        }

        if( !( doc.HasMember( "payload" ) ) )
        {
            cout << "Command must have \"payload\" member" << endl;
            return false;
        }

        Value& val = doc["payload"];
        cout << endl;
        auto found = command_handlers_.end();
        for( auto i = command_handlers_.begin(); i != command_handlers_.end(); i++ )
        {
            /***************************************************************************************/
            /** Finding commands in map only by "command":"XXX", other parameters can be modified **/
            /***************************************************************************************/
            if( !( i->first.substr( 1, i->first.find( "," ) - 1 ).compare( command_json.substr( 1, i->first.find( "," ) - 1 ) ) ) )
            {
                found = i;
            }
        }

        if( ( found == command_handlers_.end() ) || !( found->second( val ) ) )
        {
            cout << "ERROR: command has invalid content !" << endl;
            return false;
        }

        cout << endl;

        return true;
    }

private:
    std::map<std::string, CommandHandler> command_handlers_;

    // another gimme ...
    // Question: why delete these?

    // delete unused constructors
    CommandDispatcher (const CommandDispatcher&) = delete;
    CommandDispatcher& operator= (const CommandDispatcher&) = delete;
    /*****************************************************************************************************/
    /** Oba sluzia na kopirovanie objektu, to nam vsak v tomto pripade nie je treba, preto ich vymazeme **/
    /*****************************************************************************************************/
};

int main()
{
    std::cout << "COMMAND DISPATCHER: STARTED" << std::endl;

    CommandDispatcher command_dispatcher;
    Controller controller;                 // controller class of functions to "dispatch" from Command Dispatcher

    // Implement
    // add command handlers in Controller class to CommandDispatcher using addCommandHandler

    command_dispatcher.addCommandHandler( help_command, std::bind( &Controller::help, controller, std::placeholders::_1 ) );
    command_dispatcher.addCommandHandler( exit_command, std::bind( &Controller::exit, controller, std::placeholders::_1 ) );
    command_dispatcher.addCommandHandler( info_command, std::bind( &Controller::info, controller, std::placeholders::_1 ) );
    command_dispatcher.addCommandHandler( cpp_command, std::bind( &Controller::cpp, controller, std::placeholders::_1 ) );
    command_dispatcher.addCommandHandler( time_command, std::bind( &Controller::time, controller, std::placeholders::_1 ) );

    string command;

    while( ! g_done ) {

        cout << endl << "COMMANDS:" << endl;
        cout << help_command <<endl;
        cout << exit_command <<endl;
        cout << info_command <<endl;
        cout << cpp_command <<endl;
        cout << time_command <<endl;

        cout << endl << "\tenter command : ";

        getline( cin, command );

        if ( !( command_dispatcher.dispatchCommand( command ) ) )
        {
            cout << "ERROR: command could not be executed !" << endl;
        }
    }

    std::cout << "COMMAND DISPATCHER: ENDED" << std::endl;

    return 0;
}
