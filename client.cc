#include "client.h"

Client::Client() {
    // setup variables
    buflen_ = 1024;
    buf_ = new char[buflen_+1];
}

Client::~Client() {
}

void Client::run() {//When Server  
    // connect to the server and run echo program
    cout << "Client Running" << endl;//
    create();
    echo();
}

void
Client::create() 
{
    //cout << "create()" << endl;
    
}

void
Client::close_socket() 
{
    cout << "close_socket()" << endl;
    //cout << "% " ;
}

void
Client::echo() {
    string line;
    
    // loop to handle user interface
    //cout << "% " ;
    while (getline(cin,line)) {
        cout << "% " ;
        // append a newline
        line = pare_request(line);
        line += "\n";
        // send request
        bool success = send_request(line);
        
        // break if an error occurred
        if (not success)
            break;
        // get a response
        success = get_response();
        // break if an error occurred
        if (not success)
            break;

    }
    close_socket();
}

bool
Client::send_request(string request) {
    // prepare to send request
    //cout << "send_request()" << endl;
    const char* ptr = request.c_str();
    int nleft = request.length();
    int nwritten;
    // loop to be sure it is all sent
    while (nleft) {
        if ((nwritten = send(server_, ptr, nleft, 0)) < 0) {
            if (errno == EINTR) {
                // the socket call was interrupted -- try again
                continue;
            } else {
                // an error occurred, so break out
                perror("write");
                return false;
            }
        } else if (nwritten == 0) {
            // the socket is closed
            return false;
        }
        nleft -= nwritten;
        ptr += nwritten;
    }
    return true;
}

bool
Client::get_response() {
    //cout << "get_response()" << endl;
    string response = "";
    // read until we get a newline
    while (response.find("\n") == string::npos) {
        int nread = recv(server_,buf_,1024,0);
        if (nread < 0) {
            if (errno == EINTR)
                // the socket call was interrupted -- try again
                continue;
            else
                // an error occurred, so break out
                return "";
        } else if (nread == 0) {
            // the socket is closed
            return "";
        }
        // be sure to use append in case we have binary data
        response.append(buf_,nread);
    }
    // a better client would cut off anything after the newline and
    // save it in a cache
    cout << response;
    return true;
}
Client::translate_request(string &request)
{
    if(request == "send")
    {
        return "put";
    }
    if (request == "read")
    {
        return "get";
    }
    if(request == "quit")
    {
        return "reset";
    }
}

string
Client::parse_request(string &request)
{
    string token = token_getter(request);
    string rest_request = string(request);
    string command = "";
    string request_to_server = "";
    if(token == "send")
    {
        command.append(transform_reuqest(token));
        request_to_server.append(command);
        request_to_server.append(" ");
        request_to_server.append(parse_send(rest_request));
        return request_to_server;
    }
    /*
    else if(token == "list")
    {
        command.append(transform_reuqest(token));
        request_to_server.append(string(command));
        request_to_server.append(" ");
        request_to_server.append(parse_read(rest_request));
        return request_to_server;
    }
    else if (token = "read")
    {
        command.append(transform_reuqest(token));
        request_to_server.append(string(command));
        request_to_server.append(" ");
        request_to_server.appen(parse_read(read_request));
        return request_to_server;
    }
    else if (token == "quit")
    {
        command.append(transform_reuqest(token));
        return parse_quit(command);
    }
    */
    else
    {
        string err = string(token);
        err.append(": command not found");
        return err;
    }
}
string
Client::token_getter(string &request)
{
    string token;
    for(int i = 0; i < request.size(); i++)
    {
        if(request[i] != ' ')
        {
            if (request[i] != '\n')
            {
                token.push_back(request[i]);
            }
        }
        else
        {
            request.erase(request.begin(), request.end()-(request.size()-token.size()-1));
            break;
        }
    }
    return token;
}

string
Client::parse_send(string &request)
{
    username = parse_username(request);
    subject = parse_subject(request);
    cout << "-Type your message. End with a blank line-" << endl;

    string incoming_message;
    string whole_message;
    while(true)
    {
        getline(cin,incoming_message);
        whole_message.append(incoming_message);
        if(input_message.empty())
        {
            break;
        }
        whole_message,append("\n");
    }
    message = parse_message(whole_message);
}
