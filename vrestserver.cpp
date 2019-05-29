#include <cpprest/http_client.h>
#include <cpprest/filestream.h>
#include <cpprest/json.h>
#include <boost/algorithm/string/replace.hpp>
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
        int json_get(string data_choice);
        int json_get(string data_choice,string username, string password);
        void json_post(json::value json_v, string username, string password);
        void my_print_results(json::value const & value);
        void user_login();
        void http_status(int status_code);

};

void Author::http_status(int status_code){
    switch (status_code){
    case 200:
        cout<<"Status OK"<<endl;
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
    cout<<"myprint():value size="<<value.size()<<endl;
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
}

int Author::json_get(string data_choice)
{
    cout << "json_get() without login"<<endl;
    int status=0;
    http_client client(U("http://127.0.0.1:8000/api/"));
    uri_builder builder(U(data_choice));
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
            cout<<data_choice<<" size="<<value.size()<<endl;
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

void Author::user_login()
{
    cout<<"Please input your username and password\n";
    cin>>this->username>>this->password;
}

int Author::json_get(string data_choice,string username, string password)
{
    cout << "json_get() by "<<username<<endl;
    int status=0;
    // http_client_config config;
    // config.set_timeout(utility::seconds(15));
    http_client client(U("http://127.0.0.1:8000/api/"));
    string redirect = data_choice + "?username="+this->username+"&password="+this->password;
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
            cout<<data_choice<<" size="<<value.size()<<endl;
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

void Author::json_post(json::value json_v, string username, string password)
{
    cout << "json_post() by "<<username<<endl;
    http_client client(U("http://127.0.0.1:8000/api/"));
    string redirect ="author/?username="+this->username+"&password="+this->password;
    //cout<<"redirect:"<<redirect<<endl;
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
    string data_choice;
    cout<<"Get or Post?\n";
    cin>>choice;
    if (choice=="Get"){
        cout<<"choose requirement："<<endl
            <<"1. publisher  2. author "<<endl;
        cin>>data_choice;
        data_choice+='/';
        
        if(author.json_get(data_choice)){
           cout<<"author.json_get() succeed"<<endl;
        }
        else{
           cout<<"author.json_get() failed, login and try"<<endl;
           author.user_login();
           while(!author.json_get(data_choice,author.username,author.password)){
               author.user_login();
           }
        }
    }
    else if (choice=="Post"){
        author.user_login();
        string name;
        string email;
        cout<<"Pleas input your name and email\n";
        cin>>name>>email;
        json::value json_v ;
        json_v["name"] = json::value::string(name);
        json_v["email"] = json::value::string(email);
        author.json_post(json_v,author.username,author.password);
    }
}