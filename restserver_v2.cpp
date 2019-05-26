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
        void json_get();
        void json_post(json::value json_v, string username, string password);
        void my_print_results(json::value const & value);
};

void my_print_results(json::value const & value)
{
    if(!value.is_null())
     {
         auto array = value.as_array();
         for(int i=0;i<array.size();i++)
         {
            auto name = array[i].at(U("id")).as_string();
            auto email = array[i].at(U("title")).as_string();
            cout
            << "{name = " << name
            << ", email = \"" << email
            << "\"}" << endl;
         }
    }
}

//get请求获得author的相关信息不需要验证
void Author::json_get()
{
    http_client client(U("http://127.0.0.1:8000/api"));
    uri_builder builder(U("/author/"));
    client
    .request(methods::GET, builder.to_string())
    .then([](http_response response) -> pplx::task<json::value> {
        if(response.status_code() == status_codes::OK) {
    	printf("status ok\n");
    	return response.extract_json();
        }
        return pplx::task_from_result(json::value());
    })
    .then([](pplx::task<json::value> previousTask) {
        try {
            json::value const & v = previousTask.get();
            auto array = v.as_array();
	    for(int i=0;i<array.size();i++)
             {
                auto name = array[i].at(U("name")).as_string();
                auto email = array[i].at(U("email")).as_string();
                cout
                << "name = " << name
                << ", email = \"" << email
                 << endl;
             }
        } catch (http_exception const & e) {
            printf("Error exception:%s\n", e.what());
        }
    })
    .wait();
}

void Author::json_post(json::value json_v, string username, string password)
{
    http_client client(U("http://127.0.0.1:8000/api/author/?username="+username+"&password="+password));

   // json::value json_v ;
    //json_v["name"] = json::value::string("cpp");
    //json_v["email"] = json::value::string("cpp@qq.com");

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
    cout<<"Get or Post?\n";
    cin>>choice;
    if (choice=="Get")
    author.json_get();
    else if (choice=="Post")
    {
        string username, password,name,email ;
        cout<<"Please input your username and password\n";
        cin>>username>>password;
        cout<<"Pleas input your name and email\n";
        cin>>name>>email;
        json::value json_v ;
        json_v["name"] = json::value::string(name);
        json_v["email"] = json::value::string(email);
        author.json_post(json_v,username,password);
    }
    else
    cout<<"Invalid input\n";
}

