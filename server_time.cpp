#include <cpprest/http_client.h>
#include <cpprest/filestream.h>
#include <cpprest/json.h>
#include <boost/algorithm/string/replace.hpp>
#include <sys/time.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
using namespace utility;                    // Common utilities like string conversions
using namespace web;                        // Common features like URIs.
using namespace web::http;                  // Common HTTP functionality
using namespace web::http::client;          // HTTP client features
using namespace concurrency::streams;       // Asynchronous streams
using namespace std;

class Author
{
    public:
        string username;
        string password;   
        //http_client client(U("http://127.0.0.1:8000/api/"));
        void user_login();
        int  check_login();
        int  json_get();
        void json_post(json::value json_v);
        void my_print_results(json::value const & value);
        void http_status(int status_code);

};

void Author::http_status(int status_code){
    switch (status_code){
    case 200:
        cout<<"Status OK"<<endl;
        break;
    case 403:
        cout<<"Forbidden: You do not have the permission for the resource.";
        break;
    case 301: 
        //Moved Permanently
        cout<<"Wrong Username or Password:";
        break;
    }
}
void Author::my_print_results(json::value const & value)
{
    cout<<"--------------------Print--------------------"<<endl;
    if(value.size()){
        if(value.is_array()){
            auto array=value.as_array();
            for(auto iter = array.begin();iter!=array.end();++iter ){
                auto data = *iter;
                auto dataobj=data.as_object();
                 for (auto iterInner = dataobj.cbegin(); iterInner != dataobj.cend(); ++iterInner) {
                    auto &propertyName = iterInner->first;
                    auto &propertyValue = iterInner->second;
                    cout << propertyName << ", Value: " << 
                    propertyValue << std::endl;
                 }
            }
        } 
        // else if (is_string)              
    }
    cout<<"---------------------End---------------------"<<endl;
}


void Author::user_login()
{
    cout<<"Please input your username and password\n";
    cin>>this->username>>this->password;
}

int Author::check_login()
{
    int st;
    http_client client(U("http://127.0.0.1:8000/api/"));
    string redirect ="author/?username="+this->username+"&password="+this->password;
    uri_builder builder(U(redirect));
    client
    .request(methods::GET, builder.to_string())
    .then([&](http_response response) -> pplx::task<json::value> {
        cout<<"in Check: status code="<<response.status_code()<<" ";
        this->http_status(response.status_code());
        if(response.status_code() == status_codes::OK) {
            st=response.status_code();
            return response.extract_json();
        }
        return pplx::task_from_result(json::value());
    })    
    .wait();
    return st;
}

int Author::json_get()
{
    cout << "json_get by "<<this->username<<endl;
    int status=0;
    http_client client(U("http://127.0.0.1:8000/api/"));
    string redirect ="author/?username="+this->username+"&password="+this->password;
    cout<<"redirect:"<<redirect<<endl;
    uri_builder builder(U(redirect));
    client
    .request(methods::GET, builder.to_string())
    .then([&](http_response response) -> pplx::task<json::value> {
        cout<<"status code="<<response.status_code()<<" ";
        this->http_status(response.status_code());
        if(response.status_code() == status_codes::OK) {
            status=1;
            return response.extract_json();
        }
        else { //print status code
            cout<<"Error: "<<response.status_code()<<" ";
            this->http_status(response.status_code());
        }
        return pplx::task_from_result(json::value());
    })
    .then([&](pplx::task<json::value> previousTask) {  //[&]
        try{
            json::value const & value = previousTask.get();
            cout<<"author"<<" size="<<value.size()<<endl;
            if(value.size()){
                this->my_print_results(value);
            }
        } catch (http_exception const & e) {
            printf("Error exception:%s\n", e.what());
        }
    })
    .wait();
    return status;
}

void Author::json_post(json::value json_v)
{
    cout << "json_post() by "<<this->username<<endl;
    http_client client(U("http://127.0.0.1:8000/api/"));
    string redirect ="author/?username="+this->username+"&password="+this->password;
    uri_builder builder(U(redirect));
    client
    .request(methods::POST, U(""), json_v)
    .then([](http_response response) -> pplx::task<string_t> {
        if(response.status_code() == status_codes::Created) {
            return response.extract_string();
        }
        return pplx::task_from_result(string_t());
    })
    .then([](pplx::task<string_t> previousTask) {
        try {
            string_t const & v = previousTask.get();
            cout << v << endl;
        } catch (http_exception const & e) {
            printf("Error exception:%s\n", e.what());
        }
    })
    .wait();
}

int main()
{
    Author author;
    string choice;
    bool login=false;
    time_t t1,t2;
    while(1){
        author.user_login(); 
        if(author.check_login()==200){  //sucessful login set time
            time(&t1);  
            cout<<"Get or Post?\n";
            cin>>choice;
            do{
                if (choice=="Get"){
                    author.json_get();
                }
                else if (choice=="Post"){
                    string name;
                    string email;
                    cout<<"Pleas input your name and email\n";
                    cin>>name>>email;
                    json::value post_data ;
                    post_data["name"] = json::value::string(name);
                    post_data["email"] = json::value::string(email);
                    author.json_post(post_data);
                }
                time(&t2);
                cout<<"Get or Post?\n";
            }while((t2-t1)<15 && (cin>>choice));
            author.username="";
            author.password="";
         }
        else cout<<"Time out, login again."<<endl;  //login in failed
    }
}