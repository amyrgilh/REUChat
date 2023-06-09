#include <iostream>
#include <vector>
#include <string>


class ChatController {

public:
    ChatController(std::string URL) {
        _URL = URL;
    }

    //display all chats in database
    void view(std::string URL) {
        std::cout << "ALL CHATS:" << std::endl;

        std::string view = GET(URL);
        std::string delimiter = ",";

        size_t pos = 0;
        std::string token;
        while ((pos = view.find(delimiter)) != std::string::npos) {
            token = view.substr(0, pos);
            std::cout << token << std::endl;
            view.erase(0, pos + delimiter.length());
        }
        std::cout << view << std::endl;
        std::cout << "" << std::endl;
    }

    //send a user-entered message
    void send(std::string URL) {
        std::cout << "What message would you like to send?" << std::endl;
        std::string message;
        std::getline(std::cin, message);
        std::string check = POST(URL, message);

        //check for error
        if (check == "200") {
            std::cout << "Sent." << std::endl;
        }
        else {
            std::cout << "Not sent--something went wrong.";
        }
    }


    //starts program
    void startup(std::string URL) {
        std::cout << "Hello! Welcome to the REU Chat.  Here you can send chats to the database and see what other interns have sent." << std::endl;
        std::cout << " " << std::endl;
        run(URL);
    }



private:
    /** METHODS */

    // GET returns a string representation of a list of ALL chat messages in the database
    std::string GET(std::string url) {
        std::string cmd = "curl -s " + url;
        FILE* p = _popen(cmd.c_str(), "r");
        char buffer[2];
        std::string body;
        if (!p) {
            return "Error";
        }
        while (fgets(buffer, sizeof(buffer), p)) {
            body += buffer;
        }
        return body;
    }

    // POST sends a message to be stored in the database. If successful, it returns the string "200"
    std::string POST(std::string url, std::string message) {
        std::string cmd = "curl -s POST -d \"chat=" + message + "\" " + url;
        FILE* p = _popen(cmd.c_str(), "r");
        char buffer[2];
        std::string body;
        if (!p) {
            return "Error";
        }
        while (fgets(buffer, sizeof(buffer), p)) {
            body += buffer;
        }
        return body;
    }

    void run(std::string URL) {
        int x = 0;
        while (x == 0) {
            std::cout << "If you would like to view chats sent by you and other interns, type view." << std::endl;
            std::cout << "If you would like to send a chat to the database, type send." << std::endl;
            std::cout << "To exit, type exit." << std::endl;
            std::string instruction;
            getline(std::cin, instruction);

            //choose what to do based on instruction
            if (instruction == "view") {
                view(URL);
            }
            else if (instruction == "send") {
                send(URL);
            }
            else if (instruction == "exit") {
                std::cout << "Goodbye." << std::endl;
                x = 1;
            }
            else {
                std::cout << "Not an option." << std::endl;
            }

        }
    }


    /** VARS */
    std::string _URL;

};



int main()
{
    std::string URL = "10.24.221.149:5001/api/v1/chats";
    ChatController c = ChatController(URL);

    c.startup(URL);


    return 0;
}